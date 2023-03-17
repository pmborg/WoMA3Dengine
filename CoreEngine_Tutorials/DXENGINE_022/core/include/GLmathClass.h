// --------------------------------------------------------------------------------------------
// Filename: GLmathClass.h
// --------------------------------------------------------------------------------------------
// ********************************************************************************************
// World of Middle Age  - 3D Multi-Platform ENGINE 2017
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : http://woma.servegame.com
//
// PURPOSE:
//
// ********************************************************************************************
#pragma once

#include "platform.h"

struct vec4 {
	float x, y, z, w;

	vec4(){}
	vec4(const float ix, const float iy, const float iz, const float iw){
		x = ix;
		y = iy;
		z = iz;
		w = iw;
	}
/*
	void operator += (const float s);
	void operator += (const vec4 &v);
	void operator -= (const float s);
	void operator -= (const vec4 &v);
	void operator *= (const float s);
	void operator *= (const vec4 &v);
	void operator /= (const float s);
	void operator /= (const vec4 &v);
*/
};


#define MATRIX4X4 float m_projectionMatrix[16];

struct mat4 {

	//vec4	rows[4];
	float	m[16];

	mat4(){}

	mat4(const float m00, const float m01, const float m02, const float m03,
		 const float m10, const float m11, const float m12, const float m13,
		 const float m20, const float m21, const float m22, const float m23,
		 const float m30, const float m31, const float m32, const float m33)
	{
		m[0]=m00;	m[1]=m01;	m[2]=m02;	m[3]=m03;
		m[4]=m10;	m[5]=m11;	m[6]=m12;	m[7]=m13;
		m[8]=m20;	m[9]=m21;	m[10]=m22;	m[11]=m23;
		m[12]=m30;	m[13]=m31;	m[14]=m32;  m[15]=m33;
	}

	mat4 mat4identity()
		{ return  mat4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1); }

};

mat4 operator * (const mat4 &mat1, const mat4 &mat2);

////////////////////////////////////////////////////////////////////////////////
// Class name: GLmathClass
////////////////////////////////////////////////////////////////////////////////
class GLmathClass
{
public:
	GLmathClass();
	~GLmathClass();

	mat4 rotateX(float angle);
	mat4 rotateY(float angle);
	mat4 rotateZ(float angle);
	//mat4 translation(float x, float y, float z);

	mat4 BuildPerspectiveFovLHMatrix(float fieldOfView, float screenAspect, float zNear, float zFar);
};


extern void MatrixTranspose(mat4* result, mat4* matrix);