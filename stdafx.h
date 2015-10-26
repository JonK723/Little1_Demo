// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma warning(disable: 4305)
#pragma warning(disable: 4244)


#define pi 3.14159265
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#define _CRT_SECURE_NO_DEPRECATE



#include <windows.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <iostream>
#include <fstream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include "corona.h"
#include "fmod.h"
using namespace std;


extern HGLRC hRC;
extern HDC hDC;
extern HWND hWND;
extern HINSTANCE hInstance;

extern double tStart, tFinish, tFTime, fps;
extern int fullW, fullH;

extern string errorString;


bool checkExtensions(char *extension);	// check for a specific extension

// this function generates points between P0 and P1 using 't' increments
// the result is then applied to P0
void interpolatePos(float *P0, float *P1, float t);

void beginOrtho(int w, int h);	// functions to begin and end orthographic projections 
void endOrtho();