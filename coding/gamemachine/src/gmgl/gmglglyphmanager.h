﻿#ifndef __GMGLGLYPHMANAGER_H__
#define __GMGLGLYPHMANAGER_H__
#include <gmcommon.h>
#include "gmdatacore/glyph/gmglyphmanager.h"
BEGIN_NS

class GMGLGlyphTexture;
GM_PRIVATE_OBJECT(GMGLGlyphManager)
{
	GMint cursor_u, cursor_v;
	GMfloat maxHeight;
	GMGLGlyphTexture* texture;
};

class GMGLGlyphManager : public GMGlyphManager
{
	DECLARE_PRIVATE(GMGLGlyphManager);

public:
	GMGLGlyphManager();
	~GMGLGlyphManager();

public:
	virtual ITexture* glyphTexture() override;

protected:
	virtual const GMGlyphInfo& createChar(GMWchar c, GMFontSizePt fontSize) override;
};

END_NS
#endif