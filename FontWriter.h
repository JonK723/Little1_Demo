#ifndef FONT_WRITER_H
#define FONT_WRITER_H

#include "stdafx.h"

// Simple bitmap font renderer
class FontWriter {
public:
	FontWriter();
	FontWriter(const char *fontname); // initialize the font writer with the specified font
	
	~FontWriter()
	{
		DeleteFont();
	}

	virtual bool SetupFont();
	virtual void DeleteFont();
	virtual void PrintBitmapString(char* str,...);	// prints a string and other arguments if necessary
	void setRange(int rnge);
	void setHeight(int hgt);
	void fontRaster(int x, int y);  // set the raster position for the text to be rendered
	void setRaster(float xp, float yp);
	void setPos(float xp, float yp, float zp);
	void setPos(float p[3]);
	void setColor(float r, float g, float b);
	void setColor(float p[3]);
	float *getRaster();
	float *getPos();

protected:
	char name[256];	// the name of the font
	int fontlist;	// the display list for the font
	HFONT mFont;	// handle to the font
	int range;
	int fontheight;
	float position[3];	// the position to translate to to draw the font
	float rasterPos[2];	// the raster position for this font
	float color[3];
};


class FontWriter3d : public FontWriter {
public:
	FontWriter3d()
	{
		depth = 0.5;
		fontlist = 0;
		name[0] = '\0';
		mFont = NULL;
		range = 256;	// set the default character range set to 96
		fontheight = 12;
		color[0] = color[1] = color[2] = 1.0;
	}

	FontWriter3d(const char *fname)
	{
		depth = 0.5;
		strcpy(name, fname);
		fontlist = 0;
		mFont = NULL;
		range = 256;	// set the default character range set to 96
		fontheight = 12;
		color[0] = color[1] = color[2] = 1.0;
	}

	~FontWriter3d()
	{
		DeleteFont();
	}

	virtual bool SetupFont();
	void setDepth(float d);
	GLYPHMETRICSFLOAT *getData();
	virtual void PrintBitmapString(char* str,...);
protected:
	float depth;  // how thick are the fonts
	GLYPHMETRICSFLOAT data[256];
};


class FontWriterTexture : public FontWriter
{
public:
	FontWriterTexture()
	{
		name[0] = '\0';
		textureId = -1;
		textureImage = NULL;
	}

	FontWriterTexture(const char* texture)
	{
		strcpy(name, texture);
		textureId = -1;
		textureImage = NULL;
	}

	~FontWriterTexture()
	{
		DeleteFont();
	}

	virtual bool SetupFont();
	virtual void DeleteFont();
	virtual void PrintBitmapString(char* str,...);	// prints a string and other arguments if necessary

protected:
	unsigned int textureId;
	corona::Image *textureImage;
};


#endif