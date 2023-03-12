// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
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


