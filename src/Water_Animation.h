#pragma once
#include "OpenGLRenderer.h"

//Initialize the Water Animation Project. This function is called once on startup.
void Water_Animation_Initialize(OpenGLRenderer& renderer);

//This function is called every frame.
void Water_Animation_Iterate(OpenGLRenderer& renderer, double time);