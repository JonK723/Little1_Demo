#ifndef FOUNTAINSCENE_H
#define FOUNTAINSCENE_H

#include "Particles.h"

extern FountainParticle *fountain;
extern FountainParticle *fountain2;
extern FountainParticle *fountain3;

int DrawFountain(float t);
bool FountainSceneInit();

#endif