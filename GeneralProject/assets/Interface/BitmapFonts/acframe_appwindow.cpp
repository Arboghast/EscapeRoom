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


#include "acframe_appwindow.h"
#include "acframe_application.h"
#include "acutil_log.h"

namespace acFramework
{
#define IDI_ICON 101  // This is the default resouce id for the application icon

CAppWindow::CAppWindow() : acWindow::CWindow()
{
	isResizing = false;
	renderWhileResizing = true;
}

int CAppWindow::Create(const char *title, int width, int height)
{
	RECT rc;
	rc.bottom = height;
	rc.right = width;
	rc.left = 0;
	rc.top = 0;
	AdjustWindowRectEx(&rc, WS_OVERLAPPEDWINDOW, FALSE, 0);

	HICON hIcon = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, LR_DEFAULTSIZE, LR_DEFAULTSIZE, LR_SHARED);
	HICON hIconSmall = (HICON)LoadImage(GetModuleHandle(0), MAKEINTRESOURCE(IDI_ICON), IMAGE_ICON, 16, 16, LR_SHARED);

	int r = RegisterClass("GameWnd", 0, 0, hIcon, hIconSmall, AC_REGDEFCURSOR);
	if( r < 0 ) return r;

	// Call the base class' Create method
	r = acWindow::CWindow::Create(title, rc.right-rc.left, rc.bottom-rc.top, WS_OVERLAPPEDWINDOW, 0, 0, "GameWnd");
	if( r < 0 ) return r;

	return 0;
}

LRESULT CAppWindow::MsgProc(UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( msg )
	{
	case WM_PAINT:
		OnPaint();
		return 0;
	
	case WM_ERASEBKGND:
		return 1;

	case WM_SIZE:
		if( wParam == SIZE_RESTORED || wParam == SIZE_MAXIMIZED )
		{
			g_app->Pause(true);
			OnResized();
			g_app->Pause(false);
		}
		else if( wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED )
		{
			// The deactivate message will pause the app
		}
		return 0;

	case WM_ACTIVATE:
		// Don't unpause if the app is still minimized
		if( ( LOWORD(wParam) == WA_ACTIVE ||
			  LOWORD(wParam) == WA_CLICKACTIVE ) && 
			HIWORD(wParam) == 0 )
		{
			g_app->Pause(false);
		}
		else if( LOWORD(wParam) == WA_INACTIVE )
		{
			g_app->Pause(true);
		}
		return 0;


	case WM_SIZING:
		if( renderWhileResizing )
			Invalidate(FALSE);
		return 0;

	case WM_ENTERSIZEMOVE:
		g_app->Pause(true);
		isResizing = true;
		if( !renderWhileResizing )
			Invalidate(FALSE);
		return 0;

	case WM_EXITSIZEMOVE:
		isResizing = false;
		OnResized();
		g_app->Pause(false);
		return 0;

	case WM_CLOSE:
		OnClose();
		return 0;
		
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWndProc(msg, wParam, lParam);
}

void CAppWindow::OnResized()
{
	RECT rect;
	GetClientRect(GetHandle(), &rect);

	int w = rect.right - rect.left;
	int h = rect.bottom - rect.top;

	g_app->ResizeMainView(w, h);
	
	Invalidate(FALSE);
}

void CAppWindow::OnPaint()
{
	RECT rect;
	PAINTSTRUCT ps;
	if( !renderWhileResizing && isResizing )
	{
		BeginPaint(hWnd, &ps);

		HDC dc = GetDC(hWnd);
		GetClientRect(hWnd, &rect);
		FillRect(dc, &rect, (HBRUSH)GetStockObject(GRAY_BRUSH));

		EndPaint(hWnd, &ps);
	}
	else if( GetUpdateRect(hWnd, &rect, FALSE) )
	{
		HDC dc = BeginPaint(hWnd, &ps);

		g_app->BaseRender();

		EndPaint(hWnd, &ps);
	}
}

void CAppWindow::OnClose()
{
	DestroyWindow(hWnd);
}

void CAppWindow::GetCursorPos(int *x, int *y)
{
	POINT pt;
	::GetCursorPos(&pt);

	ScreenToClient(hWnd, &pt);

	RECT rc;
	GetClientRect(hWnd, &rc);
	pt.y = rc.bottom - pt.y;

	if( x ) *x = pt.x;
	if( y ) *y = pt.y;
}

} // end namespace
