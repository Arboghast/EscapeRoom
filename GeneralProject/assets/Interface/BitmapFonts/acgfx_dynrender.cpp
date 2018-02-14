/*
   AngelCode Tool Box Library
   Copyright (c) 2007 Andreas Jönsson
  
   This software is provided 'as-is', without any express or implied 
   warranty. In no event will the authors be held liable for any 
   damages arising from the use of this software.

   Permission is granted to anyone to use this software for any 
   purpose, including commercial applications, and to alter it and 
   redistribute it freely, subject to the following restrictions:

   1. The origin of this software must not be misrepresented; you 
      must not claim that you wrote the original software. If you use
      this software in a product, an acknowledgment in the product 
      documentation would be appreciated but is not required.

   2. Altered source versions must be plainly marked as such, and 
      must not be misrepresented as being the original software.

   3. This notice may not be removed or altered from any source 
      distribution.
  
   Andreas Jönsson
   andreas@angelcode.com
*/

#include <assert.h>

#include "acgfx_dynrender.h"
#include "acgfx_graphics.h"
#include "acutil_log.h"

namespace acGraphics
{

struct SVertex
{
	D3DXVECTOR3 pos;       // 12
	D3DXVECTOR3 normal;    // 12
	D3DCOLOR    diffuse;   //  4
	DWORD       data;      //  4
	D3DXVECTOR2 tex;       //  8
};

static const D3DVERTEXELEMENT9 elements[] =
{
	{0,  0, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_POSITION    , 0},
	{0, 12, D3DDECLTYPE_FLOAT3  , 0, D3DDECLUSAGE_NORMAL      , 0},
	{0, 24, D3DDECLTYPE_D3DCOLOR, 0, D3DDECLUSAGE_COLOR       , 0},
	{0, 28, D3DDECLTYPE_UBYTE4  , 0, D3DDECLUSAGE_BLENDINDICES, 0},
	{0, 32, D3DDECLTYPE_FLOAT2  , 0, D3DDECLUSAGE_TEXCOORD    , 0},
	D3DDECL_END()
};


CDynRender::CDynRender()
{
	gfx = 0;
	device = 0;
	dynamicVertexBuffer = 0;
	dynamicVertices = 0;
	color = 0xFFFFFFFF;
	declaration = 0;
}

CDynRender::CDynRender(CGraphics *gfx)
{
	this->gfx = 0;
	device = 0;
	dynamicVertexBuffer = 0;
	dynamicVertices = 0;
	declaration = 0;

	InitDeviceObjects(gfx);
}

CDynRender::~CDynRender()
{
	ReleaseDeviceObjects();
}

IDirect3DDevice9 *CDynRender::GetDevice()
{
	return device;
}

CGraphics *CDynRender::GetGraphics()
{
	return gfx;
}

int CDynRender::InitDeviceObjects(CGraphics *gfx)
{
	if( this->gfx != 0 )
		return -1;

	if( this->device != 0 )
		return -1;

	if( gfx == 0 )
		return -1;

	this->gfx = gfx;
	device = gfx->GetDevice();
	if( device == 0 )
		return -1;

	device->AddRef();

	// Create dynamic vertex buffer

	// Note that since this buffer is placed in the default pool it must be released and recreated when resetting the device
	maxDynamicVertices = 1000;
	HRESULT hr = device->CreateVertexBuffer(maxDynamicVertices*sizeof(SVertex), D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY, 0, D3DPOOL_DEFAULT, &dynamicVertexBuffer, NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create vertex buffer (%X)", hr));
		return -1;
	}

	hr = device->CreateVertexDeclaration(elements, &declaration);
	if( FAILED(hr) )
	{
		LOG(("Failed to create vertex declaration (%X)", hr));
		return -1;
	}

	vertexBase = 0;

	// Allocate memory for a temporary storage for dynamic vertices
	vertexLimit    = 300;
	tempVertexBuffer.resize(vertexLimit*sizeof(SVertex));

	return 0;
}

void CDynRender::ReinitializeDeviceObjects(CGraphics *gfx)
{
	InitDeviceObjects(gfx);
}

void CDynRender::ReleaseDeviceObjects()
{
	if( gfx )
		gfx = 0;

	if( device )
	{
		device->Release();
		device = 0;
	}

	if( declaration )
	{
		declaration->Release();
		declaration = 0;
	}

	if( dynamicVertexBuffer )
	{
		dynamicVertexBuffer->Release();
		dynamicVertexBuffer = 0;
	}
}

int CDynRender::Begin(eRenderTypes type)
{
	assert(dynamicVertices == 0);

	renderType      = type;
	vertexCount     = 0;
	subVertexCount  = 0;
	dynamicVertices = &tempVertexBuffer[0];

	return 0;
}

int CDynRender::End()
{	
	HRESULT hr;

	if( device == 0 ) return -1;

	// Lock the vertex buffer to update it
	if( vertexBase + vertexCount > maxDynamicVertices )
		vertexBase = 0;
	hr = dynamicVertexBuffer->Lock(vertexBase * sizeof(SVertex), vertexCount*sizeof(SVertex), (void**)&dynamicVertices, vertexBase ? D3DLOCK_NOOVERWRITE : D3DLOCK_DISCARD);
	if( FAILED(hr) ) 
	{
		LOG(("Failed to lock vertex buffer (%X)", hr));
		return -1;
	}

	// Copy data
	memcpy(dynamicVertices, &tempVertexBuffer[0], vertexCount*sizeof(SVertex));

	// Unlock
	hr = dynamicVertexBuffer->Unlock();
	dynamicVertices = 0;

	// Set streams
	hr = device->SetStreamSource(0, dynamicVertexBuffer, 0, sizeof(SVertex));

	// Set the vertex declaration that we're using
	hr = device->SetVertexDeclaration(declaration);

	// Draw the vertices
	if( renderType != RENDER_LINE_LIST )
		gfx->DrawPrimitive(D3DPT_TRIANGLELIST, vertexBase, vertexCount/3);
	else
		gfx->DrawPrimitive(D3DPT_LINELIST, vertexBase, vertexCount/2);
	vertexBase += vertexCount;

	return 0;
}

void CDynRender::VtxColor(DWORD argb)
{
	color = argb;
}

void CDynRender::VtxNormal(D3DXVECTOR3 &n)
{
	normal = n;
}

void CDynRender::VtxNormal(float x, float y, float z)
{
	normal.x = x;
	normal.y = y;
	normal.z = z;
}

void CDynRender::VtxTexCoord(D3DXVECTOR2 &tc)
{
	texCoord = tc;
}

void CDynRender::VtxTexCoord(float u, float v)
{
	texCoord.x = u;
	texCoord.y = v;
}

void CDynRender::VtxData(DWORD data)
{
	this->data = data;
}

void CDynRender::VtxPos(D3DXVECTOR3 &pos)
{
	VtxPos(pos.x, pos.y, pos.z);
}

void CDynRender::VtxPos(float x, float y, float z)
{
	assert(dynamicVertices);
	if( !dynamicVertices ) return;

	// If too many vertices are used, render the current ones and then start over
	if( vertexCount < vertexLimit )
	{
		// Increase the number of vertices used
		vertexCount++;

		((SVertex*)dynamicVertices)->pos.x = x;
		((SVertex*)dynamicVertices)->pos.y = y;
		((SVertex*)dynamicVertices)->pos.z = z;

		((SVertex*)dynamicVertices)->normal = normal;
		((SVertex*)dynamicVertices)->diffuse = color;
		((SVertex*)dynamicVertices)->tex = texCoord;
		((SVertex*)dynamicVertices)->data = data;

		dynamicVertices += sizeof(SVertex);

		if( renderType == RENDER_QUAD_LIST )
		{
			subVertexCount = (++subVertexCount) % 4;
			if( subVertexCount == 0 )
			{
				// Duplicate vertex 0 in the quad
				memcpy(dynamicVertices, dynamicVertices - sizeof(SVertex)*4, sizeof(SVertex));
				dynamicVertices += sizeof(SVertex);

				// Duplicate vertex 2 in the quad
				memcpy(dynamicVertices, dynamicVertices - sizeof(SVertex)*3, sizeof(SVertex));
				dynamicVertices += sizeof(SVertex);

				vertexCount += 2;
			}
		}
	}
	else
	{
		End();
		Begin(renderType);
		VtxPos(x,y,z);
	}
}

} // end namespace




