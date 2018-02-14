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

#ifndef ACGFX_CAMERA_H
#define ACGFX_CAMERA_H

#include <d3dx9.h>
#include <d3d9.h>

namespace acGraphics
{

class CGraphics;

class CCamera
{
public:
	CCamera(CGraphics *gfx);

	CGraphics *GetGraphics();
	void SetupViewProjMtx();

	void SetNearFarPlanes(float near, float far);
	void SetPosition(const D3DXVECTOR3 &pos);
	void SetLookAt(const D3DXVECTOR3 &at);
	void SetUpVector(const D3DXVECTOR3 &up);

	void SetPerspective(float viewAngle);
	void SetOrthogonal(float viewWidth);
	void SetAspectRatio(float aspect);

	D3DXVECTOR3 GetViewVector();
	D3DXVECTOR3 GetViewVectorThroughScreen(int x, int y);
	D3DXVECTOR3 GetPosition();

	D3DXMATRIX GetViewMatrix();
	D3DXMATRIX GetProjMatrix();

protected:
	CGraphics *gfx;

	float       fov;
	float       viewWidth;
	float       aspectRatio;
	float		znear;
	float       zfar;
	bool        ortho;

	D3DXVECTOR3 position;
	D3DXVECTOR3 lookAt;
	D3DXVECTOR3 upVector;
};

}

#endif