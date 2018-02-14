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


#include "acutil_timer.h"

namespace acUtility
{

//=============================================================================

CTimer::CTimer()
{
	// We need to know how often the clock is updated
	UINT64 tps;
    if( !QueryPerformanceFrequency((LARGE_INTEGER *)&tps) )
	{
		usePerformance = false;
        ticksPerSecond = 1000.0f;
	}
	else
	{
		usePerformance = true;
		ticksPerSecond = (float)(__int64)tps;
	}

	fps = 0;
	isRunning = false;
}


//=============================================================================

// Reset counter and start the timer
void CTimer::Start()
{
	// Get the current time so we know when we started
	if( usePerformance )
	{
		UINT64 ticks;
		QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
		baseTime = (float)(__int64)ticks/ticksPerSecond;
	}
	else
        baseTime = (float)timeGetTime()/ticksPerSecond;

	isRunning  = true;

	lastUpdate = 0;
	numFrames  = 0;

	frameTime  = 0;
	deltaTime  = 0;
}


//=============================================================================

// Stop the timer
void CTimer::Stop()
{
	if( isRunning )
	{
		// Remember when we stopped so we can know how long we have been paused
		if( usePerformance )
		{
			UINT64 ticks;
			QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
			stopTime = (float)(__int64)ticks/ticksPerSecond;
		}
		else
			stopTime = (float)timeGetTime()/ticksPerSecond;

		isRunning = false;
	}
}



//=============================================================================

// Start the timer without reseting
void CTimer::Continue()
{
	if( !isRunning )
	{
		float time;

		// Get the current time
		if( usePerformance )
		{
			UINT64 ticks;
			QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
			time = (float)(__int64)ticks/ticksPerSecond;
		}
		else
			time = (float)timeGetTime()/ticksPerSecond;

		// Increase baseticks to reflect the time we were paused
		baseTime += time - stopTime;

		isRunning = true;
	}
}




//=============================================================================

// Get the current time
float CTimer::GetTime()
{
	float time;

	if( isRunning )
	{
		if( usePerformance )
		{
			UINT64 ticks;
			QueryPerformanceCounter((LARGE_INTEGER *)&ticks);
			time = (float)(__int64)ticks/ticksPerSecond;
		}
		else
			time = (float)timeGetTime()/ticksPerSecond;
	}
	else
		time = stopTime;

	// Subtract the time when we started to get 
	// the time our timer has been running
	time -= baseTime;

	return time;
}



//=============================================================================

// Call this once per frame
void CTimer::NewFrame()
{
	deltaTime = GetTime() - frameTime;
	frameTime += deltaTime;

	// Update frames per second counter
	numFrames++;
	if( frameTime - lastUpdate > FPS_INTERVAL )
	{
		fps = numFrames / (frameTime - lastUpdate);
		numFrames = 0;
		lastUpdate = frameTime;
	}
}


//=============================================================================
 
float CTimer::GetFps()
{
	return fps;
}


//=============================================================================

// This is the time when Frame() was called last
float CTimer::GetFrameTime()
{
	return frameTime;
}

//=============================================================================

// This is the time that passed between the last two calls to Frame()
float CTimer::GetDeltaTime()
{
	return deltaTime;
}

//=============================================================================

bool CTimer::IsRunning()
{
	return isRunning;
}

} // end namespace acUtility
