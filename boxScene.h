#ifndef BOX_SCENE_H
#define BOX_SCENE_H

#include "Particles.h"


extern BoxParticle *boxptl;
extern float fogcolor[3];


int DrawBoxScene(float t);
bool BoxSceneInit();

#endif