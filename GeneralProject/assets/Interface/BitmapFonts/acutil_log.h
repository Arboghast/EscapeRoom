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

#ifndef ACUTIL_LOG_H
#define ACUTIL_LOG_H

struct _iobuf;
typedef _iobuf FILE;
typedef unsigned int UINT;

namespace acUtility
{

class CLog
{
public:
	CLog();
	~CLog();

	void Location(const char *file, UINT line);
	void Write(const char *str, ...);

	static const char *GetFilePath();

protected:
	FILE *f;

	const char *locationFile;
	UINT        locationLine;
};

extern CLog *g_log;

// usage: LOG(("%s", value)); note the double parenthesis
#define LOG(args) {if( acUtility::g_log ) { acUtility::g_log->Location(__FILE__, __LINE__); acUtility::g_log->Write args; }}

}

#endif