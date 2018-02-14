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

#ifndef ACGFX_DYNRENDER_H
#define ACGFX_DYNRENDER_H

#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "acgfx_graphics.h"

namespace acGraphics
{

enum eRenderTypes
{
	RENDER_TRIANGLE_LIST,
	RENDER_QUAD_LIST,
	RENDER_LINE_LIST,
};


class CDynRender : public IHasDeviceObjects
{
public:
	CDynRender();
	CDynRender(CGraphics *gfx);
	~CDynRender();

	int  InitDeviceObjects(CGraphics *gfx);
	void ReinitializeDeviceObjects(CGraphics *gfx);
	void ReleaseDeviceObjects();

	IDirect3DDevice9 *GetDevice();
	CGraphics *GetGraphics();

	int  Begin(eRenderTypes type);
	void VtxColor(DWORD argb);
	void VtxNormal(float x, float y, float z);
	void VtxNormal(D3DXVECTOR3 &normal);
	void VtxTexCoord(float x, float y);
	void VtxTexCoord(D3DXVECTOR2 &tc);
	void VtxPos(float x, float y, float z);
	void VtxPos(D3DXVECTOR3 &pos);
	void VtxData(DWORD data);
	int  End();

protected:
	CGraphics *gfx;
	IDirect3DDevice9 *device;
	UINT maxDynamicVertices;
	IDirect3DVertexBuffer9 *dynamicVertexBuffer;
	UINT vertexBase;
	UINT vertexLimit;
	std::vector<unsigned char> tempVertexBuffer;
	unsigned char *dynamicVertices;
	eRenderTypes renderType;
	UINT vertexCount;
	UINT subVertexCount;
	IDirect3DVertexDeclaration9 *declaration;

	DWORD color;
	D3DXVECTOR2 texCoord;
	D3DXVECTOR3 normal;
	DWORD data;
};

}

#endif