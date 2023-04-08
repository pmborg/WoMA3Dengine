// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: Math3D.cpp
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
//WomaIntegrityCheck = 1234567829;

#include "platform.h"
#include "Math3D.h"
#include "TrigonometryMathClass.h"

#ifndef INFINITY
#define INFINITY	1e+8f
#endif

vec3 operator + (const vec3 &u, const vec3 &v){
	return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

vec3 operator + (const vec3 &v, const float s){
	return vec3(v.x + s, v.y + s, v.z + s);
}

vec3 operator + (const float s, const vec3 &v){
	return vec3(v.x + s, v.y + s, v.z + s);
}


// THERORY: Math behind:
// ---------------------------------------------------------------------------
// If you have 3 vertices, V1, V2 and V3, ordered in counterclockwise order:
//   Vec1    X   Vec2

vec3 vector3dCrossProduct (const vec3 &vector1, const vec3 &vector2)	//Simulate: D3DXVec3Cross
// ---------------------------------------------------------------------------
{
	vec3 result;

	result.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);
	result.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);
	result.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);

	return result;
}

float vector3dLength(const vec3 &v) {
	return FAST_sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

vec3 vector3dNormalize(const vec3 &vec)	//D3DXVec3Normalize
// ---------------------------------------------------------------------------
{
	vec3 result;
    float len = 1 / FAST_sqrt/*sqrt*/(vec.x*vec.x + vec.y*vec.y + vec.z*vec.z);
            
    result.x = vec.x * len;
    result.y = vec.y * len;
    result.z = vec.z * len;
            
    return result;
}

	#include "VirtualModelClass.h" // dont move this ! (before V1 V2 V3)

// (V2 - V1) X (V3 - V1), where X is the cross product of the 2 vectors
#define V1 0
#define V2 1
#define V3 2

//#include "VirtualModelClass.h"

vec3 CalcNormals(ModelColorVertexType* model) 
// ---------------------------------------------------------------------------
{
	/* vector3dCrossProduct: Vec1   X   Vec2:*/
	/* normal.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);*/
	/* normal.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);*/
	/* normal.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);*/

	vec3 normal, vector1, vector2;

	/* Define Vec1*/
	vector1.x = model[V1].x - model[V3].x;
	vector1.y = model[V1].y - model[V3].y;
	vector1.z = model[V1].z - model[V3].z;
	/* Define Vec2*/
	vector2.x = model[V3].x - model[V2].x;
	vector2.y = model[V3].y - model[V2].y;
	vector2.z = model[V3].z - model[V2].z;

	normal = vector3dCrossProduct (vector1, vector2);

	return vector3dNormalize (normal);
}

vec3 CalcNormals(ModelTextureVertexType* model) 
// ---------------------------------------------------------------------------
{
	/* vector3dCrossProduct: Vec1   X   Vec2:*/
	/* normal.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);*/
	/* normal.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);*/
	/* normal.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);*/

	vec3 normal, vector1, vector2;

	/* Define Vec1*/
	vector1.x = model[V1].x - model[V3].x;
	vector1.y = model[V1].y - model[V3].y;
	vector1.z = model[V1].z - model[V3].z;
	/* Define Vec2*/
	vector2.x = model[V3].x - model[V2].x;
	vector2.y = model[V3].y - model[V2].y;
	vector2.z = model[V3].z - model[V2].z;

	normal = vector3dCrossProduct (vector1, vector2);

	return vector3dNormalize (normal);
}

vec3 CalcNormals(ModelTextureLightVertexType* model) 
// ---------------------------------------------------------------------------
{
	/* vector3dCrossProduct: Vec1   X   Vec2:*/
	/* normal.x = (vector1.y * vector2.z) - (vector1.z * vector2.y);*/
	/* normal.y = (vector1.z * vector2.x) - (vector1.x * vector2.z);*/
	/* normal.z = (vector1.x * vector2.y) - (vector1.y * vector2.x);*/

	vec3 normal, vector1, vector2;

	/* Define Vec1*/
	vector1.x = model[V1].x - model[V3].x;
	vector1.y = model[V1].y - model[V3].y;
	vector1.z = model[V1].z - model[V3].z;
	/* Define Vec2*/
	vector2.x = model[V3].x - model[V2].x;
	vector2.y = model[V3].y - model[V2].y;
	vector2.z = model[V3].z - model[V2].z;

	normal = vector3dCrossProduct (vector1, vector2);

	return vector3dNormalize (normal);
}

