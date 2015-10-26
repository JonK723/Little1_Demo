// stdafx.cpp : source file that includes just the standard includes
// GraphicsDemo2.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// TODO: reference any additional headers you need in STDAFX.H
// and not in this file


// checks the list of opengl extensions for a given extension
// if the extension is found then  true is returned else fale
bool checkExtensions(char *extension)
{

	char *extstring = NULL;
	char *token;

	extstring = (char *)glGetString(GL_EXTENSIONS);

	if(!extstring)	// if the string list is NULL
		return false;

	if(strstr(extstring, extension) != NULL)
		return true;

	return false;
}


// this function generates points between P0 and P1 using 't' increments
// the result is then applied to P0
void interpolatePos(float *P0, float *P1, float t)
{
	float nt = 1-t;
	float point[3];

	point[0] = (1-t)*P0[0] + t*P1[0];
	point[1] = (1-t)*P0[1] + t*P1[1];
	point[2] = (1-t)*P0[2] + t*P1[2];

	P0[0] = point[0];
	P0[1] = point[1];
	P0[2] = point[2];
}


void beginOrtho(int w, int h)	// functions to begin and end orthographic projections 
{
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0,w,0,h);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void endOrtho()
{
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);	
	glPopMatrix();
}

