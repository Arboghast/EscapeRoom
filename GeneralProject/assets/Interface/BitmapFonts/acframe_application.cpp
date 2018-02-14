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


#include <string>

#include "acframe_application.h"
#include "acframe_appwindow.h"
#include "acutil_timer.h"
#include "acutil_log.h"
#include "acutil_input.h"
#include "acgfx_graphics.h"
#include "acgfx_dynrender.h"
#include "acgfx_font.h"

using namespace acUtility;
using namespace acGraphics;
using namespace std;

namespace acFramework
{

// Global instance of the application
CBaseApplication *g_app = 0;


CBaseApplication::CBaseApplication()
{
	log        = 0;
	wnd        = 0;
	timer      = 0;
	gfx        = 0;
	render     = 0;
	pauseCount = 1;
}

CBaseApplication::~CBaseApplication()
{
	BaseUninit();
}

int CBaseApplication::Main()
{
	int r = BaseInit();
	if( r < 0 )
	{
		// TODO: Localize
		string msg = "The application failed to initialize. View the log file for more information.\n\n";
		msg += g_log->GetFilePath();
		msg += "\n\nDo you wish to view the log now?";

		int id = MessageBox(0, msg.c_str(), "Fatal Error", MB_ICONERROR|MB_YESNO);
		if( id == IDYES )
			ShellExecute(NULL, "open", g_log->GetFilePath(), 0, 0, SW_SHOWNORMAL);

		BaseUninit();

		return r;
	}

	while( !wnd->CheckMessages(pauseCount > 0) )
	{
		if( pauseCount == 0 )
			BaseFrame();
	}

	LOG(("Application closed normally"));

	BaseUninit();

	return 0;
}

int CBaseApplication::BaseInit()
{
	log = new CLog();

	wnd = new CAppWindow();
	int r = wnd->Create("Application", 600, 450);
	if( r < 0 )
	{
		LOG(("Failed to create game window (%d)", r));
		return -1;
	}

	gfx = new CGraphics();
	r = gfx->Initialize();
	if( r < 0 )
	{
		LOG(("Failed to initialize graphics object (%d)", r));
		return -1;
	}

	r = gfx->CreateWindowedDevice(wnd->GetHandle(), 600, 450);
	if( r < 0 )
	{
		LOG(("Failed to create graphics device (%d)", r));
		return -1;
	}

	render = new CDynRender();
	r = render->InitDeviceObjects(gfx);
	if( r < 0 )
	{
		LOG(("Failed to initialize the dynamic renderer (%d)", r));
		return -1;
	}
	gfx->RegisterDeviceUser(render);

	input = new CInput();
	r = input->Initialize();
	if( r < 0 )
	{
		LOG(("Failed to initialize the input object (%d)", r));
		return -1;
	}
	r = input->CreateKeyboardDevice(wnd->GetHandle());
	if( r < 0 )
	{
		LOG(("Failed to create keyboard device (%d)", r));
		return -1;
	}
	r = input->CreateMouseDevice(wnd->GetHandle());
	if( r < 0 )
	{
		LOG(("Failed to create mouse device (%d)", r));
		return -1;
	}

	timer = new CTimer();

	// Call the application specific initialization routine
	r = Init();
	if( r < 0 )
	{
		LOG(("Application specific init failed (%d)", r));
		return -1;
	}

	timer->Start();

	ShowWindow(wnd->GetHandle(), SW_SHOW);

	LOG(("Initialization completed"));

	return 0;
}

int CBaseApplication::Init()
{
	return 0;
}

void CBaseApplication::BaseUninit()
{
	// Call the application specific uninitialization routine
	Uninit();

	if( input )
	{
		delete input;
		input = 0;
	}

	if( render )
	{
		delete render;
		render = 0;
	}

	if( gfx )
	{
		delete gfx;
		gfx = 0;
	}

	if( wnd )
	{
		delete wnd;
		wnd = 0;
	}

	if( timer )
	{
		delete timer;
		timer = 0;
	}

	if( log )
	{
		delete log;
		log = 0;
	}
}

void CBaseApplication::Uninit()
{
}

void CBaseApplication::Pause(bool doPause)
{
	if( doPause ) 
	{
		pauseCount++; 
		if( pauseCount == 1 )
		{
			if( timer )
				timer->Stop();

			// Unacquire input devices
			if( input )
			{
				input->UnacquireKeyboard();
				input->UnacquireMouse();
			}
		}
	}
	else 
	{
		pauseCount--;
		if( pauseCount == 0 )
		{
			if( timer )
				timer->Continue();

			// Re-acquire input devices
			if( input )
			{
				input->AcquireKeyboard();
				input->AcquireMouse();
			}
		}
	}
}

void CBaseApplication::BaseRender()
{
	if( !gfx ) return;

	IDirect3DDevice9 *dev = gfx->GetDevice();
	if( !dev ) return;

	// Call the application specific render routine
	Render();

	dev->Present(0,0,0,0);
}

void CBaseApplication::Render()
{
}

void CBaseApplication::ResizeMainView(int width, int height)
{
	gfx->ResizeDevice(width, height);

	D3DVIEWPORT9 vp;
	vp.X      = 0;
	vp.Y      = 0;
	vp.Height = height;
	vp.Width  = width;
	vp.MinZ   = 0;
	vp.MaxZ   = 1;
	gfx->GetDevice()->SetViewport(&vp);
}

void CBaseApplication::BaseFrame()
{
	timer->NewFrame();

	// Poll input devices
	input->PollDevices();

	Frame();

	BaseRender();
}

void CBaseApplication::Frame()
{
}

} // end namespace