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

// CTimer
//
// This class can be used to keep track of time in your applications. It also
// measures the framerate if Frame() is called once per frame.
//
// Requires winmm.lib when compiled

#ifndef ACUTIL_TIMER_H
#define ACUTIL_TIMER_H

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <mmsystem.h>

namespace acUtility
{

const float FPS_INTERVAL = 0.25f;

class CTimer
{
public:
	CTimer();

	void  Start();
	void  Stop();
	void  Continue();

 	float GetTime();
	float GetFrameTime();
	float GetDeltaTime();

	void  NewFrame();
	float GetFps();

	bool  IsRunning();

protected:
	bool   isRunning;

	float  baseTime;
	float  stopTime;

	float  frameTime;
	float  deltaTime;

	DWORD  numFrames;
	float  lastUpdate;
	float  fps;

	bool   usePerformance;
	float  ticksPerSecond;
};

}

#endif