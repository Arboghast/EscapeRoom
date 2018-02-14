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

#include <stdio.h>      // _vsnprintf()
#include <stdarg.h>     // va_list, va_start(), etc
#include <string>
#include <windows.h>    // GetModuleFileName()

#include "acutil_log.h"

using namespace std;

namespace acUtility
{

CLog *g_log = 0;
static const int FILENAMELENGTH = 15;

static char buffer[260];
const char *CLog::GetFilePath()
{
	GetModuleFileName(0, buffer, 260);
	char *c = strrchr(buffer, '\\');
	if( c ) *c = '\0';

	strcat(buffer, "\\log.txt");

	return buffer;
}

CLog::CLog()
{
	f = 0;

	// wtc - Write Text Commit
	f = fopen(GetFilePath(), "wtc"); 

	locationFile = 0;
	locationLine = 0;
	
	g_log = this;
}

CLog::~CLog()
{
	g_log = 0;
	if( f ) fclose(f);
}

void CLog::Location(const char *file, UINT line)
{
	locationFile = file;
	locationLine = line;
}

void CLog::Write(const char *format, ...)
{
	if( !f ) return;

	if( locationFile )
	{
		const char *filename = 0;
		filename = strrchr(locationFile, '\\');
		if( !filename )
			filename = locationFile;
		else
			filename++;
		static char buf[FILENAMELENGTH+1];
		strncpy(buf,filename,FILENAMELENGTH);
		buf[FILENAMELENGTH] = '\0';

		fprintf(f, "%-*s|%5d|", FILENAMELENGTH, buf, locationLine);

		locationFile = 0;
	}

	va_list args;
	va_start(args, format);

	vfprintf(f, format, args);
	fprintf(f, "\n");
	fflush(f);

	va_end(args);
}

}
