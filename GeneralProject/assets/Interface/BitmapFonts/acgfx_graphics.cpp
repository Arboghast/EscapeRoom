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

#include <sstream>
#include <d3dx9.h>

#include "acgfx_graphics.h"
#include "acutil_log.h"

using namespace std;

namespace acGraphics
{

#define RELEASE_COM(x) if(x) {(x)->Release(); (x) = 0;}

// Global variables for the DLL
typedef IDirect3D9* (WINAPI *pfDIRECT3DCREATE9)(UINT);
HMODULE d3d9Lib = 0;
pfDIRECT3DCREATE9 pfDirect3DCreate9 = 0;

CGraphics::CGraphics()
{
	d3d9             = 0;
	device           = 0;
	mtxPalette       = 0;
	currentEffect    = 0;
	defaultTexture   = 0;

	rtDepthStencilSurface     = 0;
	rtWidth                   = 0;
	rtHeight                  = 0;

	screenRenderSurface       = 0;
	screenDepthStencilSurface = 0;
}

CGraphics::~CGraphics()
{
	Uninitialize();
}

int CGraphics::Initialize()
{
	// Load the D3D9.dll
	if( !d3d9Lib )
	{
		d3d9Lib = LoadLibrary("d3d9.dll");
		if( d3d9Lib == 0 ) 
		{
			LOG(("Failed to load d3d9.dll"));
			return -1;
		}

		pfDirect3DCreate9 = (pfDIRECT3DCREATE9)GetProcAddress(d3d9Lib, "Direct3DCreate9");
		if( pfDirect3DCreate9 == NULL )
		{
			LOG(("Failed to obtain address of Direct3DCreate9()"));
			return -1;
		}
	}

	// Create Direct3D
	d3d9 = pfDirect3DCreate9(D3D_SDK_VERSION);
	if( d3d9 == 0 )
	{
		LOG(("Failed to create Direct3D9"));
		return -1;
	}

	return 0;
}

int CGraphics::Uninitialize()
{
	ReleaseDevice();
	
	RELEASE_COM(d3d9);

	return 0;
}

int CGraphics::ReleaseDevice()
{
	RELEASE_COM(screenRenderSurface);
	RELEASE_COM(screenDepthStencilSurface);
	RELEASE_COM(rtDepthStencilSurface);
	RELEASE_COM(defaultTexture);

	if( mtxPalette ) delete[] mtxPalette;

	for( unsigned int n = 0; n < effects.size(); n++ )
	{
		delete effects[n];
	}
	effects.resize(0);

	RELEASE_COM(device);

	return 0;
}

int CGraphics::CreateWindowedDevice(HWND wnd, int w, int h)
{
	if( d3d9 == 0 ) return -1;
	ReleaseDevice();

	HRESULT hr;

	// Assume it's the first adapter that is displaying the windows
	UINT adapter = 0;

	// Check the capabilities of this adapter
	if( CheckCapabilities(adapter) < 0 )
		 return -1;
	
	// Check if the current display format is acceptable
	D3DDISPLAYMODE displayMode;
	hr = d3d9->GetAdapterDisplayMode(adapter, &displayMode);
	if( FAILED(hr) ) 
	{
		LOG(("GetAdapterDisplayMode() failed (%X)", hr));
		return -1;
	}

	if( !(displayMode.Format == D3DFMT_R8G8B8   || 
		  displayMode.Format == D3DFMT_X8R8G8B8 || 
		  displayMode.Format == D3DFMT_R5G6B5   || 
		  displayMode.Format == D3DFMT_X1R5G5B5) )
	{
		LOG(("Unsupported current display pixel format (%X)", displayMode.Format));
		return -1;
	}

	// Choose the best pixel format for the backbuffer
	D3DFORMAT format ;
	D3DFORMAT formats[] = {D3DFMT_R8G8B8, D3DFMT_X8R8G8B8, D3DFMT_R5G6B5, D3DFMT_X1R5G5B5};
	int numFormats = sizeof(formats)/sizeof(D3DFORMAT);
	int n;
	for( n = 0; n < numFormats; n++ )
		if( SUCCEEDED(d3d9->CheckDeviceType(adapter, D3DDEVTYPE_HAL, displayMode.Format, formats[n], TRUE)) )
		{
			format = formats[n];
			break;
		}

	if( n == numFormats ) 
	{
		LOG(("Unable to find supported back buffer pixel format"));
		return -1;
	}

	// Choose the best depth stencil format available
	D3DFORMAT depthFormat;
	D3DFORMAT depthFormats[] = {D3DFMT_D32, D3DFMT_D24X8, D3DFMT_D16};
	numFormats = sizeof(depthFormats)/sizeof(D3DFORMAT);
	for( n = 0; n < numFormats; n++ )
		if( SUCCEEDED(d3d9->CheckDepthStencilMatch(adapter, D3DDEVTYPE_HAL, displayMode.Format, format, depthFormats[n])) )
		{
			depthFormat = depthFormats[n];
			break;
		}

	if( n == numFormats )
	{
		LOG(("Unable to find supported depth/stencil format"));
		return -1;
	}

	// Setup the presentation parameters
    ZeroMemory( &pparam, sizeof(pparam) );
	pparam.BackBufferWidth            = w;
	pparam.BackBufferHeight           = h;
	pparam.BackBufferFormat           = format;
	pparam.BackBufferCount            = 1;
	pparam.MultiSampleType            = D3DMULTISAMPLE_NONE;
	pparam.SwapEffect                 = D3DSWAPEFFECT_DISCARD;
	pparam.hDeviceWindow              = (HWND)wnd;
	pparam.Windowed                   = TRUE;
	pparam.EnableAutoDepthStencil     = TRUE; 
	pparam.AutoDepthStencilFormat     = depthFormat;
	pparam.Flags                      = D3DPRESENTFLAG_LOCKABLE_BACKBUFFER; // TODO: Make optional as it may incurr a performance penalty on some cards
	pparam.FullScreen_RefreshRateInHz = 0;
	pparam.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;

	// Try with hardware vertex processing first
	hr = d3d9->CreateDevice(adapter, D3DDEVTYPE_HAL, (HWND)wnd,
							D3DCREATE_HARDWARE_VERTEXPROCESSING,
							&pparam, &device);
	if( FAILED(hr) )
	{
		LOG(("Failed to create Direct3D device with hardware vertex processing (%X), trying software processing", hr));

		// Try again with software vertex processing
		hr = d3d9->CreateDevice(adapter, D3DDEVTYPE_HAL, (HWND)wnd, 
			                    D3DCREATE_SOFTWARE_VERTEXPROCESSING, 
								&pparam, &device);
		if( FAILED(hr) )
		{
			LOG(("Failed to create Direct3D device with software processing (%X)", hr));

			return -1;
		}
	}

	return OneTimeInit();
}

int CGraphics::ResizeDevice(int w, int h)
{
	if( device == 0 ) return -1;

	if( w == 0 ) w = 1;
	if( h == 0 ) h = 1;

	if( unsigned(w) != pparam.BackBufferWidth ||
		unsigned(h) != pparam.BackBufferHeight )
	{
		pparam.BackBufferWidth = w;
		pparam.BackBufferHeight = h;

		OnLost();

		HRESULT hr = device->Reset(&pparam);
		if( FAILED(hr) )
		{
			LOG(("Failed to reset Direct3D device (%X)", hr));
			return -1;
		}
		else
			OnReset();
	}

	return 0;
}

int CGraphics::CheckCapabilities(UINT adapter)
{
	HRESULT hr;

	// Get adapter identifier
	D3DADAPTER_IDENTIFIER9 id;
	hr = d3d9->GetAdapterIdentifier(0, 0, &id);
	if( FAILED(hr) )
	{
		LOG(("Failed to get adapter identifier (%X)", hr));
	}
	else
	{
		LOG(("Adapter description: %s", id.Description));
		LOG(("Driver: %s", id.Driver));
		LOG(("Version: %d.%d.%d.%d", HIWORD(id.DriverVersion.HighPart), LOWORD(id.DriverVersion.HighPart), HIWORD(id.DriverVersion.LowPart), LOWORD(id.DriverVersion.LowPart)));
	}

	// Get the capabilities
	D3DCAPS9 caps;
	hr = d3d9->GetDeviceCaps(adapter, D3DDEVTYPE_HAL, &caps);
	if( FAILED(hr) )
	{
		LOG(("Failed to get D3D device caps (%X)", hr));
		return -1;
	}

	LOG(("Checking graphics capabilities..."));

	// alpha blending textures : alpha*src + (1-alpha)*dst
	LOG(("%-40s: %s", "Blending with source alpha", (caps.SrcBlendCaps & D3DPBLENDCAPS_SRCALPHA) ? "Yes" : "No"));
	LOG(("%-40s: %s", "Blending with inverse source alpha", (caps.DestBlendCaps & D3DPBLENDCAPS_INVSRCALPHA) ? "Yes" : "No"));
	LOG(("%-40s: %s", "Textures with alpha channel", (caps.TextureCaps & D3DPTEXTURECAPS_ALPHA) ? "Yes" : "No"));

	// Check texture sizes (for loading)
	LOG(("%-40s: %d", "Max texture width", caps.MaxTextureWidth));
	LOG(("%-40s: %d", "Max texture height", caps.MaxTextureHeight));

	// Check guardband clipping (for optimization)

	// Check max sizes for vertex and index buffers

	// Dynamic textures
	LOG(("%-40s: %s", "Dynamic textures", (caps.Caps2 & D3DCAPS2_DYNAMICTEXTURES) ? "Yes" : "No"));

	// Check shader version
	LOG(("%-40s: %hX", "Vertex shader version", caps.VertexShaderVersion));
	LOG(("%-40s: %hX", "Pixel shader version", caps.PixelShaderVersion));

	// Is render targets supported? How many?
	LOG(("%-40s: %d", "Number of simultaneous RTs", caps.NumSimultaneousRTs));

	// Enumerate render target formats
	D3DDISPLAYMODE displayMode;
	hr = d3d9->GetAdapterDisplayMode(adapter, &displayMode);
	struct fmt_t {D3DFORMAT format; const char *desc;};
	fmt_t formats[18] = {
		{D3DFMT_A16B16G16R16,  "A16B16G16R16"},
		{D3DFMT_A2R10G10B10,   "A2R10G10B10"},
		{D3DFMT_A2B10G10R10,   "A2B10G10R10"},
		{D3DFMT_A8R8G8B8,      "A8R8G8B8"},
		{D3DFMT_A8B8G8R8,      "A8B8G8R8"},
		{D3DFMT_X8R8G8B8,      "X8R8G8B8"},
		{D3DFMT_X8B8G8R8,      "X8B8G8R8"},
		{D3DFMT_R8G8B8,        "R8G8B8"},
		{D3DFMT_G16R16,        "G16R16"},
		{D3DFMT_R32F,          "R32F"},
		{D3DFMT_G32R32F,       "G32R32F"},
		{D3DFMT_A32B32G32R32F, "A32B32G32R32F"},
		{D3DFMT_R16F,          "R16F"},
		{D3DFMT_G16R16F,       "G16R16F"},
		{D3DFMT_A16B16G16R16F, "A16B16G16R16F"},
		{D3DFMT_A8,            "A8"},
		{D3DFMT_L8,            "L8"},
		{D3DFMT_L16,           "L16"}};

	for( int n = 0; n < 18; n++ )
	{
		hr = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, displayMode.Format, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, formats[n].format);
		if( SUCCEEDED(hr) )
		{
			LOG(("Available render target format: %s", formats[n].desc));
		}
	}

	return 0;
}

void CGraphics::PrepareRenderSolid()
{
	// These defaults are defined to always produce output for rendering
	// They are not meant for best performance, nor best visual quality

	device->SetRenderState(D3DRS_ALPHABLENDENABLE,       FALSE);
	device->SetRenderState(D3DRS_SRCBLEND,               D3DBLEND_SRCALPHA);
	device->SetRenderState(D3DRS_DESTBLEND,              D3DBLEND_INVSRCALPHA);

	device->SetRenderState(D3DRS_LIGHTING,               FALSE);
	device->SetRenderState(D3DRS_COLORVERTEX,            TRUE);
	device->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE,  D3DMCS_COLOR1);
	device->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE,  D3DMCS_COLOR1);
	device->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	device->SetRenderState(D3DRS_NORMALIZENORMALS,       TRUE);

	device->SetTextureStageState(0, D3DTSS_COLOROP,   D3DTOP_MODULATE);
	device->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_CURRENT);

	device->SetTextureStageState(0, D3DTSS_ALPHAOP,   D3DTOP_SELECTARG1);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
	device->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_CURRENT);

	D3DMATERIAL9 material;
	material.Diffuse  = D3DXCOLOR(1,1,1,1);
	material.Specular = D3DXCOLOR(1,1,1,1);
	material.Ambient  = D3DXCOLOR(1,1,1,1);
	material.Emissive = D3DXCOLOR(0,0,0,1);
	material.Power    = 0;
	device->SetMaterial(&material);
}

int CGraphics::OnLost()
{
	RELEASE_COM(screenRenderSurface);
	RELEASE_COM(screenDepthStencilSurface);
	RELEASE_COM(defaultTexture);
	RELEASE_COM(rtDepthStencilSurface);

	for( UINT n = 0; n < deviceUsers.size(); n++ )
		deviceUsers[n]->ReleaseDeviceObjects();

	for( unsigned int n = 0; n < effects.size(); n++ )
		if( effects[n] ) 
			effects[n]->effect->OnLostDevice();

	return 0;
}

int CGraphics::OnReset()
{
	HRESULT hr = 0;

	// Create the default texture that will be used when no other texture is set
	hr = device->CreateTexture(1, 1, 1, D3DUSAGE_DYNAMIC, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &defaultTexture, NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create default texture (0x%X)", hr));
	}
	else
	{
		D3DLOCKED_RECT lr;
		hr = defaultTexture->LockRect(0, &lr, NULL, 0);
		if( FAILED(hr) )
		{
			LOG(("Failed to lock surface of default texture (0x%X)", hr));
		}
		else
		{
			*(DWORD*)lr.pBits = D3DCOLOR_RGBA(255, 255, 255, 255);
			defaultTexture->UnlockRect( 0 );
		}
	}

	// Get the reference to the render surface and depth buffer
    hr = device->GetRenderTarget( 0, &screenRenderSurface );
	if( FAILED(hr) )
	{
		LOG(("Failed to obtain reference to screen render surface (0x%X)", hr));
	}
	hr = device->GetDepthStencilSurface( &screenDepthStencilSurface );
	if( FAILED(hr) )
	{
		LOG(("Failed to obtain reference to screen depth stencil surface (0x%X)", hr));
	}

	// Recreate render target's depth stencil buffer
	if( rtWidth && rtHeight )
	{
		// Create the accompanying depth stencil buffer, which must have at least the same width and height
		hr = device->CreateDepthStencilSurface(rtWidth,
											   rtHeight,
											   pparam.AutoDepthStencilFormat,
										       D3DMULTISAMPLE_NONE,
											   0,
											   TRUE,
											   &rtDepthStencilSurface,
											   NULL);
		if( FAILED(hr) )
		{
			LOG(("Failed to create depth stencil surface (0x%X)", hr));
		}
	}

	PrepareRenderSolid();

    // Reset effects
	for( unsigned int n = 0; n < effects.size(); n++ )
		if( effects[n] )
			effects[n]->effect->OnResetDevice();

	for( UINT n = 0; n < deviceUsers.size(); n++ )
		deviceUsers[n]->ReinitializeDeviceObjects(this);

	return 0;
}

int CGraphics::CreateRenderTargetForShadowMap(int width, int height, IDirect3DTexture9 **texture)
{
	HRESULT hr;

	// Try different formats until an acceptable one is found
	struct fmt_t {D3DFORMAT format; const char *desc;};
	fmt_t formats[18] = {
		{D3DFMT_R32F,          "R32F"},
		{D3DFMT_R16F,          "R16F"},
		{D3DFMT_L16,           "L16"},
		{D3DFMT_G16R16,        "G16R16"},
		{D3DFMT_A16B16G16R16,  "A16B16G16R16"},
		{D3DFMT_A2R10G10B10,   "A2R10G10B10"},
		{D3DFMT_A2B10G10R10,   "A2B10G10R10"},
		{D3DFMT_A8,            "A8"},
		{D3DFMT_L8,            "L8"},
		{D3DFMT_X8R8G8B8,      "X8R8G8B8"},
		{D3DFMT_X8B8G8R8,      "X8B8G8R8"},
		{D3DFMT_A8R8G8B8,      "A8R8G8B8"},
		{D3DFMT_A8B8G8R8,      "A8B8G8R8"},
		{D3DFMT_R8G8B8,        "R8G8B8"},
		{D3DFMT_G32R32F,       "G32R32F"},
		{D3DFMT_A32B32G32R32F, "A32B32G32R32F"},
		{D3DFMT_G16R16F,       "G16R16F"},
		{D3DFMT_A16B16G16R16F, "A16B16G16R16F"}};

	D3DFORMAT displayFormat;
	if( pparam.Windowed )
	{
		D3DDISPLAYMODE displayMode;
		hr = d3d9->GetAdapterDisplayMode(0, &displayMode);
		displayFormat = displayMode.Format;
	}
	else
		displayFormat = pparam.BackBufferFormat;

	D3DFORMAT shadowFormat = D3DFMT_UNKNOWN;
	for( int n = 0; n < 18; n++ )
	{
		hr = d3d9->CheckDeviceFormat(0, D3DDEVTYPE_HAL, displayFormat, D3DUSAGE_RENDERTARGET, D3DRTYPE_TEXTURE, formats[n].format);
		if( SUCCEEDED(hr) )
		{
			LOG(("Format chosen for shadow map: %s", formats[n].desc));
			shadowFormat = formats[n].format;
			break;
		}
	}

	int r;
	if( shadowFormat == D3DFMT_UNKNOWN )
	{
		LOG(("No suitable format found for shadow map"));
	}
	else
	{
		r = CreateRenderTarget(width, height, shadowFormat, texture);
		if( r < 0 )
			LOG(("Failed to create shadow map render target"));
	}

	return r;
}

// formats: D3DFMT_R32F
int CGraphics::CreateRenderTarget(int width, int height, D3DFORMAT format, IDirect3DTexture9 **texture)
{
	*texture = 0;

	HRESULT hr;
	hr = device->CreateTexture(width, height,
                               1, D3DUSAGE_RENDERTARGET,
                               format,
                               D3DPOOL_DEFAULT,
                               texture,
                               NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create render target (0x%X)", hr));
		return -1;
	}

	// Create the accompanying depth stencil buffer, which must have at least the same width and height
	// TODO: If the depth stencil surface has already been created, then we only need to create a new one if the new render target is larger
	RELEASE_COM(rtDepthStencilSurface);
	rtWidth  = width;
	rtHeight = height;
    hr = device->CreateDepthStencilSurface(width,
                                           height,
                                           pparam.AutoDepthStencilFormat,
                                           D3DMULTISAMPLE_NONE,
                                           0,
                                           TRUE,
                                           &rtDepthStencilSurface,
                                           NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create depth stencil surface (0x%X)", hr));
		return -1;
	}

	return 0;
}

void CGraphics::SetRenderTarget(IDirect3DTexture9 *texture)
{
	HRESULT hr;
	if( texture == 0 )
	{
		// Restore screen surfaces
		hr = device->SetRenderTarget(0, screenRenderSurface);
		if( FAILED(hr) )
		{
			LOG(("Failed to restore screen render surface (0x%X)", hr));
		}

		hr = device->SetDepthStencilSurface(screenDepthStencilSurface);
		if( FAILED(hr) )
		{
			LOG(("Failed to restore screen depth stencil surface (0x%X)", hr));
		}
	}
	else
	{
		// Set the render target
		IDirect3DSurface9 *surface;
	    hr = texture->GetSurfaceLevel(0, &surface);
		if( FAILED(hr) )
		{
			LOG(("Failed to get surface from texture (0x%X)", hr));
		}
		else
		{
			hr = device->SetRenderTarget(0, surface);
			if( FAILED(hr) )
			{
				LOG(("Failed to set custom render surface (0x%X)", hr));
			}
			RELEASE_COM(surface);

			// Set the custom depth stencil surface
			hr = device->SetDepthStencilSurface(rtDepthStencilSurface);
			if( FAILED(hr) )
			{
				LOG(("Failed to set custom depth stencil surface (0x%X)", hr));
			}
		}
	}
}

int CGraphics::LoadEffect(const char *filename)
{
	HRESULT hr;
	DWORD shaderFlags = D3DXFX_NOT_CLONEABLE;
	ID3DXEffect *effect;
	ID3DXBuffer *compilerErrors;
	hr = D3DXCreateEffectFromFile(device, 
		                          filename,
								  0,
								  0,
								  shaderFlags,
								  0,
								  &effect,
								  &compilerErrors);
	if( FAILED(hr) )
	{
		LOG(("Effect file '%s' failed to compile (0x%X)", filename, hr));
		if( compilerErrors )
			LOG(("%s", (const char*)compilerErrors->GetBufferPointer()));
		return -1;
	}

	effects.push_back(new SEffect(filename,effect));

/*
    D3DCAPS9 caps;
    device->GetDeviceCaps(&caps);
	
    if( caps.VertexShaderVersion >= D3DVS_VERSION(2,0) )
        skinMeshEffect->SetTechnique("Skinning20");
    else
        skinMeshEffect->SetTechnique("Skinning11");
*/

	effect->OnResetDevice();

	currentEffect = effect;

	return 0;
}

void CGraphics::SetEffect(const char *name)
{
	for( int n = 0; n < (signed)effects.size(); n++ )
	{
		if( effects[n]->effectName == name )
		{
			currentEffect = effects[n]->effect;
			return;
		}
	}
	currentEffect = 0;
}

int CGraphics::SetEffectTechnique(const char *name)
{
	if( currentEffect == 0 )
		return -1;

	HRESULT hr = currentEffect->SetTechnique(name);
	if( FAILED(hr) )
	{
		LOG(("Couldn't set technique '%s' on effect", name));
		return -1;
	}

	return 0;
}

int CGraphics::OneTimeInit()
{
	// Allocate the matrix palette
	mtxPaletteSize = 32;
	mtxPalette = new D3DXMATRIX[mtxPaletteSize];

	OnReset();

	return 0;
}

void CGraphics::DrawPrimitive(D3DPRIMITIVETYPE type, UINT startVertex, UINT primitiveCount)
{
	if( currentEffect == 0 )
		return;

	UINT numPasses;
	currentEffect->Begin(&numPasses, 0);
	for( UINT n = 0; n < numPasses; n++ )
	{
		currentEffect->BeginPass(n);
		device->DrawPrimitive(type, startVertex, primitiveCount);
		currentEffect->EndPass();
	}
	currentEffect->End();
}

int CGraphics::BeginEffect(UINT *numPasses)
{
	if( currentEffect == 0 )
		return -1;

	currentEffect->Begin(numPasses, 0);
	return 0;
}

void CGraphics::EndEffect()
{
	if( currentEffect )
		currentEffect->End();
}

void CGraphics::BeginEffectPass(UINT pass)
{
	if( currentEffect )
		currentEffect->BeginPass(pass);
}

void CGraphics::EndEffectPass()
{
	if( currentEffect )
		currentEffect->EndPass();
}

void CGraphics::CommitEffectChanges()
{
	if( currentEffect )
		currentEffect->CommitChanges();
}

void CGraphics::SetTexture(IDirect3DTexture9 *texture)
{
	if( currentEffect == 0 ) return;

	D3DXHANDLE handle = 0;
	handle = currentEffect->GetParameterBySemantic(NULL, "TEXTURE0");

	if( handle )
	{
		// If no texture is given, then use the default texture instead
		currentEffect->SetTexture(handle, texture == 0 ? defaultTexture : texture);
	}
}

void CGraphics::SetEffectParam(const char *param, D3DXVECTOR3 &vec)
{
	if( currentEffect == 0 ) return;

	D3DXHANDLE handle = 0;
	handle = currentEffect->GetParameterBySemantic(NULL, param);

	if( handle )
	{
		D3DXVECTOR4 v4(vec.x, vec.y, vec.z, 1);
		currentEffect->SetVector(handle, &v4);
	}
}

void CGraphics::SetEffectParam(const char *param, D3DXMATRIX &mtx)
{
	if( currentEffect == 0 ) return;

	D3DXHANDLE handle = 0;
	handle = currentEffect->GetParameterBySemantic(NULL, param);

	if( handle )
	{
		currentEffect->SetMatrix(handle, &mtx);
	}
}

void CGraphics::SetMatrix(D3DTRANSFORMSTATETYPE type, const D3DMATRIX *matrix)
{
	if( currentEffect == 0 ) return;

	// TODO: Get these handles when loading the effect.
	// Store them in a structure together with the effect object.
	
	// TODO: The graphics object must store the values of the matrices
	// so that when switching to another effect the variables can be updated 
	// in the new effect as well.

	D3DXHANDLE handle = 0;
	if( type == D3DTS_WORLD )
		handle = currentEffect->GetParameterBySemantic(NULL, "WORLD");
	else if( type == D3DTS_VIEW )
		handle = currentEffect->GetParameterBySemantic(NULL, "VIEW");
	else if( type == D3DTS_PROJECTION )
		handle = currentEffect->GetParameterBySemantic(NULL, "PROJECTION");

	if( handle )
		currentEffect->SetMatrix(handle, (const D3DXMATRIX*)matrix);
}

void CGraphics::RegisterDeviceUser(IHasDeviceObjects *user)
{
	deviceUsers.push_back(user);
}

void CGraphics::UnregisterDeviceUser(IHasDeviceObjects *user)
{
	for( UINT n = 0; n < deviceUsers.size(); n++ )
	{
		if( deviceUsers[n] == user )
		{
			if( n < deviceUsers.size() - 1 )
				deviceUsers[n] = deviceUsers.back();
				
			deviceUsers.pop_back();

			break;
		}
	}
}

void CGraphics::GetScreenDimensions(int *w, int *h)
{
	if( w ) *w = pparam.BackBufferWidth;
	if( h ) *h = pparam.BackBufferHeight;
}

} // end namespace
