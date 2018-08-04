﻿#ifndef __IMAGEREADER_TGA_H__
#define __IMAGEREADER_TGA_H__
#include <gmcommon.h>
#include "gmimagereader.h"
BEGIN_NS

class GMImageReader_TGA : public IImageReader
{
	virtual bool load(const GMbyte* data, GMsize_t size, OUT GMImage** image) override;
	virtual bool test(const GMbyte* data) override;
};

END_NS
#endif