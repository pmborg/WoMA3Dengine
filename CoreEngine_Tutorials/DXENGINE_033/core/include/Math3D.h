// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: Math3D.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for comercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

#include "platform.h"

extern float FAST_sqrt(float x);

	#include "vertexTypes.h"

#ifndef PI
	#define PI 3.14159265358979323846f
#endif

struct vec3
{
	float x, y, z;

	vec3(){}
	vec3(const float ix, const float iy, const float iz){
		x = ix;
		y = iy;
		z = iz;
	}
};

vec3 operator + (const vec3 &u, const vec3 &v);
vec3 operator + (const vec3 &v, const float s);
vec3 operator + (const float s, const vec3 &v);

	extern vec3 CalcNormals(ModelColorVertexType* model) ;
	extern vec3 CalcNormals(ModelTextureVertexType* model);
	extern vec3 CalcNormals(ModelTextureLightVertexType* model);
