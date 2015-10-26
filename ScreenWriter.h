#ifndef SCREENWRITER_H
#define SCREENWRITER_H


#include "FontWriter.h"



extern FontWriter3d *testFont[10];	// test font writer

bool WriterInit();	// initialize the writer scene code
int DrawFontStrings(float t);   // render the different fonts onto the screen 


#endif