#include "application.h"
#include "acframe_appwindow.h"
#include "acgfx_graphics.h"
#include "acutil_log.h"
#include "acgfx_font.h"
#include "acgfx_camera.h"
#include "acgfx_dynrender.h"
#include "acutil_timer.h"
#include "acutil_unicode.h"
#include <stdio.h>
#include <d3dx9.h>


using namespace acGraphics;

CApplication::CApplication() : CBaseApplication()
{
	fontArial = 0;
	fontComic = 0;
	fontTiny    = 0;
	fontChinese = 0;
}

CApplication::~CApplication()
{
}

int CApplication::Init()
{
	int r;

	SetWindowText(wnd->GetHandle(), "Bitmap fonts");

	fontArial = new CFont();
	r = fontArial->Init("data/arial24.fnt", "data/font.fx", render);
	if( r < 0 )
	{
		LOG(("Failed to initialize font 'data/arial24.fnt' (%d)", r));
		return -1;
	}

	fontComic = new CFont();
	r = fontComic->Init("data/comic24.fnt", "data/font.fx", render);
	if( r < 0 )
	{
		LOG(("Failed to initialize font 'data/comic24.fnt' (%d)", r));
		return -1;
	}

	fontTiny = new CFont();
	r = fontTiny->Init("data/comic10.fnt", "data/font.fx", render);
	if( r < 0 )
	{
		LOG(("Failed to initialize font 'data/comic10.fnt' (%d)", r));
		return -1;
	}

	fontChinese = new CFont();
	r = fontChinese->Init("data/chinese.fnt", "data/font.fx", render);
	if( r < 0 )
	{
		LOG(("Failed to initialize font 'data/chinese.fnt' (%d)", r));
		return -1;
	}

	return 0;
}

void CApplication::Uninit()
{
	if( fontArial )
	{
		delete fontArial;
		fontArial = 0;
	}

	if( fontComic )
	{
		delete fontComic;
		fontComic = 0;
	}

	if( fontTiny )
	{
		delete fontTiny;
		fontTiny = 0;
	}

	if( fontChinese )
	{
		delete fontChinese;
		fontChinese = 0;
	}
}

void CApplication::Frame()
{
}

void CApplication::Render()
{
	IDirect3DDevice9 *dev = gfx->GetDevice();

	DWORD color = D3DCOLOR_ARGB(0,123,145,145);
	dev->Clear(0, 0, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, color, 1.0f, 0);
	dev->BeginScene();

	//------------------------------
	fontArial->PrepareEffect();
	fontArial->PreparePixelPerfectOutput();

	// Write the FPS
	char buf[256];
	sprintf(buf, "fps: %.2f", timer->GetFps());
	fontArial->Write(5, 5, 0, buf, 0, 0);

	// Write the info
	int w, h;
	gfx->GetScreenDimensions(&w, &h);
	fontArial->Write(200, h-1*fontArial->GetHeight(), 0, "Left", 0, FONT_ALIGN_LEFT);
	fontArial->Write(200, h-2*fontArial->GetHeight(), 0, "Center", 0, FONT_ALIGN_CENTER);
	fontArial->Write(200, h-3*fontArial->GetHeight(), 0, "Right", 0, FONT_ALIGN_RIGHT);

	// Write in a box
	fontArial->WriteBox((float)350, (float)h-100, 0, (float)w-360, 
		"Writing in a box with automatic line breaks is easy. "
	    "It looks good as well when the text is justified to give straight edges.\n"
		"\n"
		"Paragraphs are also possible.\n"
		"\n"
		"By the way, the text in the upper right corner says 'welcome' in traditional chinese.\n", 0, FONT_ALIGN_JUSTIFY);

	// Write with the comic font
	fontComic->PrepareEffect();
	fontComic->PreparePixelPerfectOutput();
	fontComic->WriteBox(10, (float)h-150, 0, 250, 
		"Here's an example of a font without a baked-in border. This font is generated from Comic Sans MS. The text is right aligned.", 0, FONT_ALIGN_RIGHT);

	// Write with the tiny font
	fontTiny->PrepareEffect();
	fontTiny->PreparePixelPerfectOutput();
	fontTiny->WriteML(50, 50, 0, "This is a tiny font (Comic Sans MS, 10pt)", 0, 0);

	// Write 'welcome' in traditional chinese
	fontChinese->PrepareEffect();
	fontChinese->PreparePixelPerfectOutput();
	unsigned int cl, len = 0;
	len += acUtility::EncodeUTF8(27489, buf, &cl); 
	len += acUtility::EncodeUTF8(36814, &buf[len], &cl);
	buf[len] = 0;

	// Set this font to use UTF8 encoding
	fontChinese->SetTextEncoding(UTF8);
	fontChinese->Write((float)w-110, (float)h-50, 0, buf, 0, 0);

	dev->EndScene();
}

