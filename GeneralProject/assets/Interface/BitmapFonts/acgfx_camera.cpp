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

#include "acgfx_camera.h"
#include "acgfx_graphics.h"

namespace acGraphics
{

CCamera::CCamera(CGraphics *gfx)
{
	this->gfx = gfx;
	position  = D3DXVECTOR3(0,0,0);
	lookAt    = D3DXVECTOR3(0,0,1);
	upVector  = D3DXVECTOR3(0,1,0);

	fov         = 60;
	viewWidth   = 10;
	znear       = 0.1f;
	zfar        = 100.0f;
	ortho       = false;
	aspectRatio = 3.0f/4;
}

CGraphics *CCamera::GetGraphics()
{
	return gfx;
}

void CCamera::SetOrthogonal(float vw)
{
	viewWidth = vw;
	ortho = true;
}

void CCamera::SetPerspective(float va)
{
	fov = va;
	ortho = false;
}

void CCamera::SetAspectRatio(float aspect)
{
	aspectRatio = aspect;
}

void CCamera::SetNearFarPlanes(float n, float f)
{
	znear = n;
	zfar  = f;
}

void CCamera::SetPosition(const D3DXVECTOR3 &pos)
{
	position = pos;
}

void CCamera::SetLookAt(const D3DXVECTOR3 &at)
{
	lookAt = at;
}

void CCamera::SetUpVector(const D3DXVECTOR3 &up)
{
	upVector = up;
}

void CCamera::SetupViewProjMtx()
{
	// Compute the view matrix
	D3DXMATRIX mtx = GetViewMatrix();
	gfx->SetMatrix(D3DTS_VIEW, &mtx);

	// Compute the projection matrix
	mtx = GetProjMatrix();
	gfx->SetMatrix(D3DTS_PROJECTION, &mtx);
}

D3DXMATRIX CCamera::GetViewMatrix()
{
	D3DXMATRIX mtx;

	// Make sure the view vector isn't parallel with the upVector
	D3DXVECTOR3 v = lookAt - position;
	D3DXVECTOR3 c;
	D3DXVec3Cross(&c, &v, &upVector);
	if( D3DXVec3LengthSq(&c) > 0.00001f )
	{
		// Compute the view matrix
		D3DXMatrixLookAtLH(&mtx, &position, &lookAt, &upVector);
	}
	else
	{
		// The camera is looking in the direction of the upVector, let's 
		// swizzle the upVector to produce a vector in another direction
		D3DXVECTOR3 u(upVector.y, upVector.z, upVector.x);

		D3DXMatrixLookAtLH(&mtx, &position, &lookAt, &u);
	}

	return mtx;
}

D3DXMATRIX CCamera::GetProjMatrix()
{
	D3DXMATRIX mtx;

	// Compute the projection matrix
	if( !ortho )
	{
		float w = 2*znear*cosf(fov*3.141592f/180);
		float h = aspectRatio*w;
		D3DXMatrixPerspectiveLH(&mtx, w, h, znear, zfar);
	}
	else
	{
		D3DXMatrixOrthoLH(&mtx, viewWidth, aspectRatio*viewWidth, znear, zfar);
	}

	return mtx;
}

D3DXVECTOR3 CCamera::GetViewVector()
{
	// Determine the orientation of the camera
	D3DXMATRIX mtx;
	D3DXMatrixLookAtLH(&mtx, &position, &lookAt, &upVector);

	D3DXVECTOR3 view  = D3DXVECTOR3(mtx._13, mtx._23, mtx._33);

	return view;
}

D3DXVECTOR3 CCamera::GetViewVectorThroughScreen(int mx, int my)
{
	// Map the x and y to the near plane
	int w, h;
	gfx->GetScreenDimensions(&w, &h);

	float nearWidth = 2*znear*cosf(fov*3.141592f/180);
	float nearHeight = 3*nearWidth/4;

	float x = mx*(float)nearWidth/w;
	float y = my*(float)nearHeight/h;

	// Determine the orientation of the camera
	D3DXMATRIX mtx;
	D3DXMatrixLookAtLH(&mtx, &position, &lookAt, &upVector);

	D3DXVECTOR3 view  = D3DXVECTOR3(mtx._13, mtx._23, mtx._33);
	D3DXVECTOR3 right = D3DXVECTOR3(mtx._11, mtx._21, mtx._31);
	D3DXVECTOR3 up    = D3DXVECTOR3(mtx._12, mtx._22, mtx._32);

	// Compute the point on the near plane in front of the camera
	D3DXVECTOR3 p = position + znear*view - nearWidth/2*right - nearHeight/2*up;
	p += x*right + y*up;

	p = p - position;
	D3DXVec3Normalize(&p, &p);

	return p;
}

D3DXVECTOR3 CCamera::GetPosition()
{
	return position;
}

} // end namespace




