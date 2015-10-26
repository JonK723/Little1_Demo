#ifndef LOGOSCENE_H
#define LOGOSCENE_H

#include "FontWriter.h"
#include "stdafx.h"

extern unsigned int logopic;
extern unsigned int names;
extern corona::Image *logoImg;

bool InitLogoScene();
int DrawLogoScene(float t);

#endif