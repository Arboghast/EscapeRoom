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


// To use this framework you'll need to implement the CApplication class.
// The CApplication class should derive from the CBaseApplication class.
// The following virtual methods are called by the CBaseApplication and 
// should be implemented in the CApplication class:
//
// - int  Init()   
//   Initialize any application specific resources here. 
//   Return a negative value in case of failure.
//
// - void Uninit()
//   Unitialize the application specific resources here.
//
// - void Frame()
//   Called every frame. Can be used to do time based updates.
//
// - void Render()
//   Called to render the view.




#ifndef GAMEAPPLICATION_H
#define GAMEAPPLICATION_H

// Forward declare the classes instead of including header to reduce compile time
namespace acUtility
{
	class CTimer;
	class CLog;
	class CInput;
}
namespace acGraphics
{
	class CGraphics;
	class CDynRender;
}
namespace acFramework
{
	class CAppWindow;
}

namespace acFramework
{

class CBaseApplication
{
public:
	CBaseApplication();
	virtual ~CBaseApplication();

	int Main();

	acFramework::CAppWindow *wnd;
	acUtility::CTimer       *timer;
	acUtility::CInput       *input;
	acGraphics::CGraphics   *gfx;
	acGraphics::CDynRender  *render;

protected:
	friend class acFramework::CAppWindow;

	virtual int  Init();
	virtual void Uninit();
	virtual void Frame();
	virtual void Render();

	void Pause(bool doPause);
	void ResizeMainView(int width, int height);

	int pauseCount;

private:
	int  BaseInit();
	void BaseUninit();
	void BaseFrame();
	void BaseRender();

	acUtility::CLog        *log;
};

extern CBaseApplication *g_app;

}

#endif