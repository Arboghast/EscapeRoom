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

#ifndef ACUTIL_INPUT_H
#define ACUTIL_INPUT_H

#include <vector>

struct HWND__;
typedef HWND__ *HWND;
struct IDirectInput8;
struct IDirectInputDevice8;
typedef unsigned int UINT;
typedef unsigned char BYTE;

namespace acUtility
{

class CInput
{
public:
	CInput();
	~CInput();

	int Initialize();
	int Uninitialize();

	int PollDevices();

	int CreateKeyboardDevice(HWND appWnd);
	int ReleaseKeyboardDevice();

	int AcquireKeyboard();
	int UnacquireKeyboard();

	int GetKeyState(int key);

	int KeyToAscii(int key, char *ch);

	int CreateMouseDevice(HWND appWnd);
	int ReleaseMouseDevice();

	int AcquireMouse();
	int UnacquireMouse();
	int GetMouseButtonState(int button);

	int GetMouseDelta(int *dx, int *dy, int *dz);

	bool GetActionState(UINT action);
	int  MapKeyToAction(UINT action, int key);

protected:
	int PollMouse();
	int PollKeyboard();
	int DetermineActionStates();

	IDirectInput8 *dinput;

	IDirectInputDevice8 *keyboard;
	bool isKeyboardAcquired;
	BYTE keyboardState[256];

	IDirectInputDevice8 *mouse;
	bool isMouseAcquired;
	int relMouseX;
	int relMouseY;
	int relMouseZ;	
	BYTE mouseButtons[8];

	bool isCapsLockOn;

	struct keyMapping
	{
		UINT action;
		int key;
	};

	std::vector<bool>       actionStates;
	std::vector<keyMapping> keyMappings;
};

#define acIS_PRESSED(x)   (((x)&0x80) == 0x80)
#define acIS_RELEASED(x)  (((x)&0x80) == 0x00)
#define acWAS_PRESSED(x)  ((x) == 0x80)
#define acWAS_RELEASED(x) ((x) == 0x01)

// Keys
#define acKEY_ESCAPE          0x01
#define acKEY_1               0x02
#define acKEY_2               0x03
#define acKEY_3               0x04
#define acKEY_4               0x05
#define acKEY_5               0x06
#define acKEY_6               0x07
#define acKEY_7               0x08
#define acKEY_8               0x09
#define acKEY_9               0x0A
#define acKEY_0               0x0B
#define acKEY_MINUS           0x0C    /* - on main keyboard */
#define acKEY_EQUALS          0x0D
#define acKEY_BACK            0x0E    /* backspace */
#define acKEY_TAB             0x0F
#define acKEY_Q               0x10
#define acKEY_W               0x11
#define acKEY_E               0x12
#define acKEY_R               0x13
#define acKEY_T               0x14
#define acKEY_Y               0x15
#define acKEY_U               0x16
#define acKEY_I               0x17
#define acKEY_O               0x18
#define acKEY_P               0x19
#define acKEY_LBRACKET        0x1A
#define acKEY_RBRACKET        0x1B
#define acKEY_RETURN          0x1C    /* Enter on main keyboard */
#define acKEY_LCONTROL        0x1D
#define acKEY_A               0x1E
#define acKEY_S               0x1F
#define acKEY_D               0x20
#define acKEY_F               0x21
#define acKEY_G               0x22
#define acKEY_H               0x23
#define acKEY_J               0x24
#define acKEY_K               0x25
#define acKEY_L               0x26
#define acKEY_SEMICOLON       0x27
#define acKEY_APOSTROPHE      0x28
#define acKEY_GRAVE           0x29    /* accent grave */
#define acKEY_LSHIFT          0x2A
#define acKEY_BACKSLASH       0x2B
#define acKEY_Z               0x2C
#define acKEY_X               0x2D
#define acKEY_C               0x2E
#define acKEY_V               0x2F
#define acKEY_B               0x30
#define acKEY_N               0x31
#define acKEY_M               0x32
#define acKEY_COMMA           0x33
#define acKEY_PERIOD          0x34    /* . on main keyboard */
#define acKEY_SLASH           0x35    /* / on main keyboard */
#define acKEY_RSHIFT          0x36
#define acKEY_MULTIPLY        0x37    /* * on numeric keypad */
#define acKEY_LALT            0x38    /* left Alt */
#define acKEY_SPACE           0x39
#define acKEY_CAPSLOCK        0x3A
#define acKEY_F1              0x3B
#define acKEY_F2              0x3C
#define acKEY_F3              0x3D
#define acKEY_F4              0x3E
#define acKEY_F5              0x3F
#define acKEY_F6              0x40
#define acKEY_F7              0x41
#define acKEY_F8              0x42
#define acKEY_F9              0x43
#define acKEY_F10             0x44
#define acKEY_NUMLOCK         0x45
#define acKEY_SCROLLLOCK      0x46    /* Scroll Lock */
#define acKEY_NUMPAD7         0x47
#define acKEY_NUMPAD8         0x48
#define acKEY_NUMPAD9         0x49
#define acKEY_SUBTRACT        0x4A    /* - on numeric keypad */
#define acKEY_NUMPAD4         0x4B
#define acKEY_NUMPAD5         0x4C
#define acKEY_NUMPAD6         0x4D
#define acKEY_ADD             0x4E    /* + on numeric keypad */
#define acKEY_NUMPAD1         0x4F
#define acKEY_NUMPAD2         0x50
#define acKEY_NUMPAD3         0x51
#define acKEY_NUMPAD0         0x52
#define acKEY_DECIMAL         0x53    /* . on numeric keypad */
#define acKEY_OEM_102         0x56    /* <> or \| on RT 102-key keyboard (Non-U.S.) */
#define acKEY_F11             0x57
#define acKEY_F12             0x58
#define acKEY_F13             0x64    /*                     (NEC PC98) */
#define acKEY_F14             0x65    /*                     (NEC PC98) */
#define acKEY_F15             0x66    /*                     (NEC PC98) */
#define acKEY_KANA            0x70    /* (Japanese keyboard)            */
#define acKEY_ABNT_C1         0x73    /* /? on Brazilian keyboard */
#define acKEY_CONVERT         0x79    /* (Japanese keyboard)            */
#define acKEY_NOCONVERT       0x7B    /* (Japanese keyboard)            */
#define acKEY_YEN             0x7D    /* (Japanese keyboard)            */
#define acKEY_ABNT_C2         0x7E    /* Numpad . on Brazilian keyboard */
#define acKEY_NUMPADEQUALS    0x8D    /* = on numeric keypad (NEC PC98) */
#define acKEY_PREVTRACK       0x90    /* Previous Track (DIK_CIRCUMFLEX on Japanese keyboard) */
#define acKEY_AT              0x91    /*                     (NEC PC98) */
#define acKEY_COLON           0x92    /*                     (NEC PC98) */
#define acKEY_UNDERLINE       0x93    /*                     (NEC PC98) */
#define acKEY_KANJI           0x94    /* (Japanese keyboard)            */
#define acKEY_STOP            0x95    /*                     (NEC PC98) */
#define acKEY_AX              0x96    /*                     (Japan AX) */
#define acKEY_UNLABELED       0x97    /*                        (J3100) */
#define acKEY_NEXTTRACK       0x99    /* Next Track */
#define acKEY_NUMPADENTER     0x9C    /* Enter on numeric keypad */
#define acKEY_RCONTROL        0x9D
#define acKEY_MUTE            0xA0    /* Mute */
#define acKEY_CALCULATOR      0xA1    /* Calculator */
#define acKEY_PLAYPAUSE       0xA2    /* Play / Pause */
#define acKEY_MEDIASTOP       0xA4    /* Media Stop */
#define acKEY_VOLUMEDOWN      0xAE    /* Volume - */
#define acKEY_VOLUMEUP        0xB0    /* Volume + */
#define acKEY_WEBHOME         0xB2    /* Web home */
#define acKEY_NUMPADCOMMA     0xB3    /* , on numeric keypad (NEC PC98) */
#define acKEY_DIVIDE          0xB5    /* / on numeric keypad */
#define acKEY_SYSRQ           0xB7
#define acKEY_RALT            0xB8    /* right Alt */
#define acKEY_PAUSE           0xC5    /* Pause */
#define acKEY_HOME            0xC7    /* Home on arrow keypad */
#define acKEY_UP              0xC8    /* UpArrow on arrow keypad */
#define acKEY_PRIOR           0xC9    /* PgUp on arrow keypad */
#define acKEY_LEFT            0xCB    /* LeftArrow on arrow keypad */
#define acKEY_RIGHT           0xCD    /* RightArrow on arrow keypad */
#define acKEY_END             0xCF    /* End on arrow keypad */
#define acKEY_DOWN            0xD0    /* DownArrow on arrow keypad */
#define acKEY_NEXT            0xD1    /* PgDn on arrow keypad */
#define acKEY_INSERT          0xD2    /* Insert on arrow keypad */
#define acKEY_DELETE          0xD3    /* Delete on arrow keypad */
#define acKEY_LWIN            0xDB    /* Left Windows key */
#define acKEY_RWIN            0xDC    /* Right Windows key */
#define acKEY_APPS            0xDD    /* AppMenu key */
#define acKEY_POWER           0xDE    /* System Power */
#define acKEY_SLEEP           0xDF    /* System Sleep */
#define acKEY_WAKE            0xE3    /* System Wake */
#define acKEY_WEBSEARCH       0xE5    /* Web Search */
#define acKEY_WEBFAVORITES    0xE6    /* Web Favorites */
#define acKEY_WEBREFRESH      0xE7    /* Web Refresh */
#define acKEY_WEBSTOP         0xE8    /* Web Stop */
#define acKEY_WEBFORWARD      0xE9    /* Web Forward */
#define acKEY_WEBBACK         0xEA    /* Web Back */
#define acKEY_MYCOMPUTER      0xEB    /* My Computer */
#define acKEY_MAIL            0xEC    /* Mail */
#define acKEY_MEDIASELECT     0xED    /* Media Select */

}

#endif