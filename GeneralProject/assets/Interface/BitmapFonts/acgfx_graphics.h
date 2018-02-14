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

#ifndef ACGFX_GRAPHICS_H
#define ACGFX_GRAPHICS_H

#include <d3d9.h>
#include <d3dx9.h>
#include <vector>
#include <string>

namespace acGraphics
{

class CGraphics;

class IHasDeviceObjects
{
public:
	virtual void ReinitializeDeviceObjects(CGraphics *gfx) = 0;
	virtual void ReleaseDeviceObjects() = 0;
};

struct SEffect
{
	SEffect(const char *name, ID3DXEffect *fx) {effectName = name; effect = fx;}
	~SEffect() {if( effect ) effect->Release();}
	std::string effectName;
	ID3DXEffect *effect;
};

class CGraphics
{
public:
	CGraphics();
	~CGraphics();

	int Initialize();
	int Uninitialize();

	int ReleaseDevice();

	void RegisterDeviceUser(IHasDeviceObjects *user);
	void UnregisterDeviceUser(IHasDeviceObjects *user);

	int CreateWindowedDevice(HWND wnd, int w, int h);
	int ResizeDevice(int w, int h);

	IDirect3DDevice9 *GetDevice() {return device;}
	
	void GetMatrixPalette(D3DXMATRIX **palette, UINT *paletteSize) {if( palette ) *palette = mtxPalette; if( paletteSize ) *paletteSize = mtxPaletteSize;}

	void PrepareRenderSolid();

	int  LoadEffect(const char *filename);
	void SetEffect(const char *name);
	int  SetEffectTechnique(const char *name);
	int  BeginEffect(UINT *numPasses);
	void BeginEffectPass(UINT pass);
	void EndEffectPass();
	void EndEffect();
	void CommitEffectChanges();

	void DrawPrimitive(D3DPRIMITIVETYPE type, UINT startVertex, UINT primitiveCount);
	void SetMatrix(D3DTRANSFORMSTATETYPE type, const D3DMATRIX *matrix);
	void SetTexture(IDirect3DTexture9 *texture);
	void SetEffectParam(const char *param, D3DXVECTOR3 &vec);
	void SetEffectParam(const char *param, D3DXMATRIX &mtx);

	// Render targets
	int  CreateRenderTarget(int width, int height, D3DFORMAT format, IDirect3DTexture9 **texture);
	int  CreateRenderTargetForShadowMap(int width, int height, IDirect3DTexture9 **texture);
	void SetRenderTarget(IDirect3DTexture9 *texture);

	// 
	void GetScreenDimensions(int *w, int *h);

protected:
	int CheckCapabilities(UINT adapter);

	int OneTimeInit();
	int OnLost();
	int OnReset();

	IDirect3D9 *d3d9;
	IDirect3DDevice9 *device;
	D3DPRESENT_PARAMETERS pparam;

	UINT         mtxPaletteSize;
	D3DXMATRIX  *mtxPalette;

	std::vector<SEffect*> effects; 
	ID3DXEffect *currentEffect;

	std::vector<IHasDeviceObjects*> deviceUsers;

	// Default texture
	IDirect3DTexture9 *defaultTexture;

	// Depth Stencil surface for rendertargets
	IDirect3DSurface9 *rtDepthStencilSurface;
	UINT rtWidth;
	UINT rtHeight;

	// Original back buffer surface and depth stencil surface
	IDirect3DSurface9 *screenRenderSurface;
	IDirect3DSurface9 *screenDepthStencilSurface;
};

}

#endif