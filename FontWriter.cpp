#include "FontWriter.h"


FontWriter::FontWriter()
{
	fontlist = 0;
	strcpy(name, "Arial");
	mFont = NULL;
	range = 96;	// set the default character range set to 96
	fontheight = 12;
	position[0] = position[1] = position[2] = 0.0;
	rasterPos[0] = rasterPos[1] = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}

FontWriter::FontWriter(const char *fontname)
{
	fontlist = 0;
	strcpy(name, fontname);
	mFont = NULL;
	range = 96;	// set the default character range set to 96
	fontheight = 12;
	position[0] = position[1] = position[2] = 0.0;
	rasterPos[0] = rasterPos[1] = 0.0;
	color[0] = color[1] = color[2] = 1.0;
}

void FontWriter::DeleteFont()
{
	glDeleteLists(fontlist, range);
}

void FontWriter::setRange(int rnge)
{
	range = rnge;
}

void FontWriter::setHeight(int hgt)
{
	fontheight = hgt;
}

void FontWriter::fontRaster(int x, int y)
{
	glRasterPos2i(x, y);
}

void FontWriter::setPos(float xp, float yp, float zp)
{
	position[0] = xp;
	position[1] = yp;
	position[2] = zp;
}

void FontWriter::setPos(float p[3])
{
	position[0] = p[0];
	position[1] = p[1];
	position[2] = p[2];
}

float *FontWriter::getRaster()
{
	return rasterPos;
}


float *FontWriter::getPos()
{
	return position;
}

void FontWriter::setRaster(float xp, float yp)
{
	rasterPos[0] = xp;
	rasterPos[1] = yp;
}

void FontWriter::setColor(float r, float g, float b)
{
	color[0] = r;
	color[1] = g;
	color[2] = b;
}

void FontWriter::setColor(float p[3])
{
	color[0] = p[0];
	color[1] = p[1];
	color[2] = p[2];
}

bool FontWriter::SetupFont()
{
	HFONT tmpfont;
	
	fontlist = glGenLists(range);

	tmpfont = CreateFont(-fontheight,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
		FF_DONTCARE|DEFAULT_PITCH,name);
	if(tmpfont == NULL)
		return false;

	mFont = (HFONT)SelectObject(hDC, tmpfont);
	wglUseFontBitmaps(hDC, 32,range,fontlist);
	SelectObject(hDC, mFont);
	DeleteObject(tmpfont);

	return true;
}


void FontWriter::PrintBitmapString(char *str,...)
{
	char stuff[256];
	va_list va;

	if(!str)
		return;

	va_start(va, str);
		vsprintf(stuff,str,va);
	va_end(va);
	
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(position[0],position[1],position[2]);
	glColor3f(color[0], color[1], color[2]);
	
	glRasterPos2f(rasterPos[0], rasterPos[1]);

	glPushAttrib(GL_LIST_BIT);
	glListBase(fontlist - 32);
	glCallLists(strlen(stuff), GL_UNSIGNED_BYTE, stuff);
	glPopAttrib();

	glPopMatrix();
}


// 3D font writer
void FontWriter3d::setDepth(float d)
{
	depth = d;
}

bool FontWriter3d::SetupFont()
{
	HFONT tmpfont;
	
	fontlist = glGenLists(range);

	tmpfont = CreateFont(-fontheight,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,ANSI_CHARSET,OUT_TT_PRECIS,CLIP_DEFAULT_PRECIS,ANTIALIASED_QUALITY,
		                 FF_DONTCARE|DEFAULT_PITCH,name);
	if(tmpfont == NULL)
		return false;

	mFont = (HFONT)SelectObject(hDC, tmpfont);

	wglUseFontOutlines(hDC, 0, range-1, fontlist, 0.0, depth, WGL_FONT_POLYGONS, data);

	return true;
}

GLYPHMETRICSFLOAT* FontWriter3d::getData()
{
	return data;
}

void FontWriter3d::PrintBitmapString(char *str,...)
{
	if(!str)
		return;

	char stuff[256];
	va_list va;

	va_start(va, str);
		vsprintf(stuff,str,va);
	va_end(va);

	float length = 0.0;

	for(int i = 0; i < strlen((const char*)stuff); i++)
	{
		length+=data[stuff[i]].gmfCellIncX;
	}

	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef((-length/2)-position[0],position[1],position[2]);
	glColor3f(color[0], color[1], color[2]);

	glPushAttrib(GL_LIST_BIT);
	glListBase(fontlist);
	glCallLists(strlen(stuff), GL_UNSIGNED_BYTE, stuff);
	glPopAttrib();

	glPopMatrix();
}


void FontWriterTexture::DeleteFont()
{
	glDeleteLists(fontlist,256);
}

bool FontWriterTexture::SetupFont()
{
	textureImage = corona::OpenImage(name);
	if(!textureImage)
		return false;

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexEnvi(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, textureImage->getWidth(), textureImage->getHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, textureImage->getPixels());

	delete textureImage;

	float cx;
	float cy;

	fontlist = glGenLists(256);
	glBindTexture(GL_TEXTURE_2D, textureId);

	for(int loop = 0; loop < 256; loop++)
	{
		cx = (loop%16)/16;
		cy = (loop/16)/16;
		
		glNewList(fontlist+loop, GL_COMPILE);

		glBegin(GL_QUADS);

		glTexCoord2f(cx,1-cy-0.0625f);
		glVertex2i(0, 0);

		glTexCoord2f(cx+0.0625, 1-cy-0.0625);
		glVertex2i(16, 0);

		glTexCoord2f(cx+0.0625, 1-cy);
		glVertex2i(16, 16);

		glTexCoord2f(cx, 1-cy);
		glVertex2i(0, 16);

		glEnd();

		glTranslated(10, 0, 0);

		glEndList();
	}

	return true;
}

void FontWriterTexture::PrintBitmapString(char *str, ...)
{
	if(str == NULL)
		return;

	char stuff[256];
	va_list va;

	va_start(va, str);
		vsprintf(stuff,str,va);
	va_end(va);

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureId);

	glDisable(GL_DEPTH_TEST);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	
	glTranslatef(position[0],position[1],position[2]);
	glColor3f(color[0], color[1], color[2]);
	
	glRasterPos2f(rasterPos[0], rasterPos[1]);

	glPushAttrib(GL_LIST_BIT);
	glListBase(fontlist - 32);
	glCallLists(strlen(stuff), GL_UNSIGNED_BYTE, stuff);
	glPopAttrib();

	glPopMatrix();

	glEnable(GL_DEPTH_TEST);
}