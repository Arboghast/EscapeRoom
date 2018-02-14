#ifndef APPLICATION_H
#define APPLICATION_H

#include "acframe_application.h"
#include "acgfx_graphics.h"
#include <vector>

namespace acGraphics
{
	class CFont;
	class CCamera;
}

struct ID3DXMesh;
struct IDirect3DTexture9;

class CApplication : public acFramework::CBaseApplication
{
public:
	CApplication();
	~CApplication();

	int  Init();
	void Uninit();
	void Frame();
	void Render();

protected:
	acGraphics::CFont *fontArial;
	acGraphics::CFont *fontComic;
	acGraphics::CFont *fontTiny;
	acGraphics::CFont *fontChinese;
};

#endif