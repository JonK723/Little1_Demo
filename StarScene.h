#ifndef STARSCENE_H
#define STARSCENE_H

#include "Particles.h"

extern float angle[3];
extern float lpos[3];
extern unsigned int testpic;
extern unsigned int pics[4];
extern corona::Image *pmyimage;
extern LorenzParticle *ptl1;
extern LorenzParticle *ptl2;
extern LorenzParticle *ptl3;
extern LogarithmicParticle *logPart;

int DrawStarField(float t);
bool SceneInit();

#endif
