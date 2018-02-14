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

#define DIRECTINPUT_VERSION 0x0800
#define INITGUID // Defining this allows us to not link with dxguid.lib
#include <dinput.h>

#include "acutil_input.h"
#include "acutil_log.h"

namespace acUtility
{

typedef HRESULT (WINAPI *pfDIRECTINPUT8CREATE)(HINSTANCE, DWORD, REFIID, LPVOID*, LPUNKNOWN);

#define RELEASE_COM(x) if(x) {x->Release(); x = 0;}

// Global variables for the DLL

HMODULE dinput8Lib = 0;
pfDIRECTINPUT8CREATE pfDirectInput8Create = 0;

CInput::CInput()
{
	dinput = 0;
	mouse = 0;
	keyboard = 0;
}

CInput::~CInput()
{
	Uninitialize();
}

int CInput::Initialize()
{
	// Load the D3D8.dll
	if( !dinput8Lib )
	{
		dinput8Lib = LoadLibrary("dinput8.dll");
		if( dinput8Lib == 0 ) 
		{
			LOG(("Failed to load 'dinput8.dll'"));
			return -1;
		}

		pfDirectInput8Create = (pfDIRECTINPUT8CREATE)GetProcAddress(dinput8Lib, "DirectInput8Create");
		if( pfDirectInput8Create == NULL )
		{
			LOG(("Failed to obtain address of 'DirectInput8Create()'"));
			return -1;
		}
	}

	// Create Direct3D
	HRESULT hr = pfDirectInput8Create(GetModuleHandle(0), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dinput, 0);
	if( FAILED(hr) )
	{
		LOG(("DirectInput8Create() failed (%X)", hr));
		return -1;
	}

	return 0;
}

int CInput::Uninitialize()
{
	ReleaseKeyboardDevice();
	ReleaseMouseDevice();

	RELEASE_COM(dinput);

	return 0;
}


int CInput::CreateKeyboardDevice(HWND hWnd)
{
	if( keyboard ) return -1;

	HRESULT hr;
	hr = dinput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create keyboard device (%X)", hr));
		return -1;
	}

	// It seems that setting the high order bit makes the object optional
	DIOBJECTDATAFORMAT objs[256];
	for( int n = 0; n < 256; n++ )
	{
		objs[n].pguid = &GUID_Key;
		objs[n].dwOfs = n;
		objs[n].dwType = DIDFT_BUTTON | DIDFT_MAKEINSTANCE(n) | 0x80000000l;
		objs[n].dwFlags = 0;
	}

	DIDATAFORMAT format;
	format.dwSize = sizeof(DIDATAFORMAT);
	format.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
	format.dwFlags = 2;
	format.dwDataSize = 256;
    format.dwNumObjs = 256;
	format.rgodf = objs;

	hr = keyboard->SetDataFormat(&format);
	if( FAILED(hr) )
	{
		LOG(("Failed to set keyboard format (%X)", hr));
		return -1;
	}

	hr = keyboard->SetCooperativeLevel((HWND)hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if( FAILED(hr) )
	{
		LOG(("Failed to set keyboard cooperative level (%X)", hr));
		return -1;
	}

	isKeyboardAcquired = false;

	memset(keyboardState, 0, 256);

	return 0;
}

int CInput::ReleaseKeyboardDevice()
{
	RELEASE_COM(keyboard);

	return 0;
}

int CInput::AcquireKeyboard()
{
	if( !keyboard ) 
		return -1;

	// Check state of caps lock
	isCapsLockOn = GetKeyState(VK_CAPITAL) & 0x01 ? true : false;

	HRESULT hr = keyboard->Acquire();
	if( FAILED(hr) )
	{
		LOG(("Failed to acquire keyboard (%X)", hr));
		return -1;
	}

	isKeyboardAcquired = true;

	return 0;
}

int CInput::UnacquireKeyboard()
{
	if( !keyboard )
		return -1;

	isKeyboardAcquired = false;

	keyboard->Unacquire();

	return 0;
}


int CInput::CreateMouseDevice(HWND hWnd)
{
	if( mouse ) return -1;

	HRESULT hr;
	hr = dinput->CreateDevice(GUID_SysMouse, &mouse, NULL);
	if( FAILED(hr) )
	{
		LOG(("Failed to create mouse device (%X)", hr));
		return -1;
	}

	// It seems that setting the high order bit makes the object optional

	DIOBJECTDATAFORMAT objs[] = 
	{
	  { &GUID_XAxis, 0, DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 }, 
	  { &GUID_YAxis, 4, DIDFT_AXIS | DIDFT_ANYINSTANCE, 0 }, 
	  { &GUID_ZAxis, 8, DIDFT_AXIS | DIDFT_ANYINSTANCE | 0x80000000l, 0 }, 
	  { &GUID_Button, 12, DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	  { &GUID_Button, 13, DIDFT_BUTTON | DIDFT_ANYINSTANCE, 0 },
	  { &GUID_Button, 14, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	  { &GUID_Button, 15, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	  { &GUID_Button, 16, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	  { &GUID_Button, 17, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	  { &GUID_Button, 18, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	  { &GUID_Button, 12, DIDFT_BUTTON | DIDFT_ANYINSTANCE | 0x80000000l, 0 },
	};

	DIDATAFORMAT format;
	format.dwSize = sizeof(DIDATAFORMAT);
	format.dwObjSize = sizeof(DIOBJECTDATAFORMAT);
	format.dwFlags = DIDF_RELAXIS;
	format.dwDataSize = 20;
    format.dwNumObjs = 11;
	format.rgodf = objs;

	hr = mouse->SetDataFormat(&format);
	if( FAILED(hr) )
	{
		LOG(("Failed to set mouse format (%X)", hr));
		return -1;
	}

	hr = mouse->SetCooperativeLevel((HWND)hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if( FAILED(hr) )
	{
		LOG(("Failed to set mouse cooperative level (%X)", hr));
		return -1;
	}

	isMouseAcquired = false;

	return 0;
}

int CInput::ReleaseMouseDevice()
{
	RELEASE_COM(mouse);

	return 0;
}

int CInput::AcquireMouse()
{
	if( !mouse ) 
		return -1;

	HRESULT hr = mouse->Acquire();
	if( FAILED(hr) )
	{
		LOG(("Failed to acquire mouse (%X)", hr));
		return -1;
	}

	isMouseAcquired = true;

	return 0;
}

int CInput::UnacquireMouse()
{
	if( !mouse )
		return -1;

	isMouseAcquired = false;

	mouse->Unacquire();

	return 0;
}

int CInput::PollDevices()
{
	PollMouse();
	PollKeyboard();
	DetermineActionStates();

	return 0;
}

int CInput::KeyToAscii(int key, char *result)
{
	// Get current windows keyboard layout
	static HKL layout = GetKeyboardLayout(0);

	static BYTE vkState[256];
	memset(vkState, 0, 256);

	// Determine current keyboard layout (for SHIFT, CTRL, ALT etc)
	// GetKeyboardLayout() doesn't work since we are using Exclusive access to keyboard

	if( acIS_PRESSED(keyboardState[acKEY_LSHIFT]) || 
		acIS_PRESSED(keyboardState[acKEY_RSHIFT]) )
		vkState[VK_SHIFT] = 0x80;
	if( acIS_PRESSED(keyboardState[acKEY_LCONTROL]) || 
		acIS_PRESSED(keyboardState[acKEY_RCONTROL]) ||
		acIS_PRESSED(keyboardState[acKEY_RALT]) )
		vkState[VK_CONTROL] = 0x80;
	if( acIS_PRESSED(keyboardState[acKEY_LALT]) ||
		acIS_PRESSED(keyboardState[acKEY_RALT]) )
		vkState[VK_MENU] = 0x80;
	if( isCapsLockOn )
		vkState[VK_CAPITAL] = 0x01;

	// Map scancode to virtual key
	UINT vk = MapVirtualKeyEx(key, 1, layout);

	// Translate virtual key to ascii characters
	// TODO: Use ToUnicodeEx to transform to a unicode character
	return ToAsciiEx(vk, key, vkState, (WORD*)result, 0, layout);
}

int CInput::PollKeyboard()
{
	HRESULT hr;
	if( keyboard && isKeyboardAcquired )
	{
		static BYTE buffer[256];

		hr = keyboard->GetDeviceState(sizeof(buffer), buffer);
		if( FAILED(hr) )
		{
			if( hr == DIERR_INPUTLOST )
			{
				hr = keyboard->Acquire();
				if( FAILED(hr) )
					return -1;
				else
					return PollKeyboard();
			}
			else
				return -1;
		}

		// Update keyboard state
		for( int n = 0; n < 256; n++ )
			keyboardState[n] = (keyboardState[n]>>7) | (buffer[n] ? 0x80 : 0x00);

		if( acWAS_PRESSED(keyboardState[acKEY_CAPSLOCK]) )
			isCapsLockOn = !isCapsLockOn;
	}
	else
	{
		for( int n = 0; n < 256; n++ )
			keyboardState[n] = (keyboardState[n]>>7);
	}

	return 0;
}


int CInput::PollMouse()
{
	HRESULT hr;
	if( mouse && isMouseAcquired )
	{
		DIMOUSESTATE2 mouseState;

		hr = mouse->GetDeviceState(sizeof(mouseState), &mouseState);
		if( FAILED(hr) )
		{
			if( hr == DIERR_INPUTLOST )
			{
				hr = mouse->Acquire();
				if( FAILED(hr) )
					return -1;
				else
					return PollMouse();
			}
			else
				return -1;
		}

		relMouseX = mouseState.lX;
		relMouseY = mouseState.lY;
		relMouseZ = mouseState.lZ;

		// Update mouse buttons
		for( int n = 0; n < 8; n++ )
			mouseButtons[n] = (mouseButtons[n]>>7) | (mouseState.rgbButtons[n] ? 0x80 : 0x00);
	}
	else
	{
		relMouseX = 0;
		relMouseY = 0;
		relMouseZ = 0;

		for( int n = 0; n < 8; n++ )
			mouseButtons[n] = (mouseButtons[n]>>7);
	}

	return 0;
}

int CInput::GetMouseDelta(int *dx, int *dy, int *dz)
{
	if( dx ) *dx = relMouseX;
	if( dy ) *dy = relMouseY;
	if( dz ) *dz = relMouseZ;

	return -1;
}

int CInput::GetKeyState(int key)
{
	return keyboardState[key];
}

int CInput::GetMouseButtonState(int button)
{
	return mouseButtons[button & 0x7];
}

bool CInput::GetActionState(UINT action)
{
	if( action >= actionStates.size() ) 
		return false;

	return actionStates[action];
}

int CInput::MapKeyToAction(UINT action, int key)
{
	keyMapping km;
	km.action = action;
	km.key    = key;

	keyMappings.push_back(km);

	if( action >= actionStates.size() )
		actionStates.resize(action+1);

	return 0;
}

int CInput::DetermineActionStates()
{
	unsigned int n;
	for( n = 0; n < actionStates.size(); n++ )
		actionStates[n] = false;

	for( n = 0; n < keyMappings.size(); n++ )
	{
		if( acIS_PRESSED(keyboardState[keyMappings[n].key]) )
			actionStates[keyMappings[n].action] = true;
	}

	return 0;
}

} // End namespace

