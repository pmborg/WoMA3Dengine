// --------------------------------------------------------------------------------------------
// Filename: DXfrustumClass.cpp
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
//WomaIntegrityCheck = 1234567831;

#include "platform.h"
#if DX_ENGINE_LEVEL >= 30 && (defined DX_ENGINE)

#pragma warning( disable : 4005 ) // Disable warning C4005: '' : macro redefinition
#pragma warning( disable : 4238 ) // Disable warning C4238: nonstandard extension used : class rvalue used as lvalue

#include "DXfrustumclass.h"
#include "OSengine.h"		// Get [SystemHandle] Pointer to System Class: WINDOWS, LINUX & ANDROID
#include "DXmodelClass.h"

DXfrustumClass::DXfrustumClass()
{
	CLASSLOADER();
}

DXfrustumClass::~DXfrustumClass() {CLASSDELETE();}

//ConstructFrustum is called every frame by the GraphicsClass. It passes in the the depth of the screen, the projection matrix, 
//and the view matrix. We then use these input variables to calculate the matrix of the view frustum at that frame. 
//With the new frustum matrix we then calculate the six planes that form the view frustum.

void DXfrustumClass::ConstructFrustum(float screenDepth, XMMATRIX *ProjectionMatrix, XMMATRIX* g_viewMatrix)
{
    XMMATRIX projectionMatrix = *ProjectionMatrix; // Copy Matrix
	XMMATRIX matrix;

    ////m_screenDepth = screenDepth; // Note: Maybe shorter than: g_SCREEN_DEPTH 
	float r = screenDepth / (screenDepth - SystemHandle->AppSettings->SCREEN_NEAR);
	projectionMatrix._33 = r;
	projectionMatrix._43 = -r * SystemHandle->AppSettings->SCREEN_NEAR;

	// Create the frustum matrix from the view matrix and updated projection matrix.
    matrix = XMMatrixMultiply(*g_viewMatrix, projectionMatrix);

	// Calculate left plane of frustum.
	m_planes[0].m128_f32[0] /*.a*/ = matrix._14 + matrix._11; 
	m_planes[0].m128_f32[1] /*.b*/ = matrix._24 + matrix._21;
	m_planes[0].m128_f32[2] /*.c*/ = matrix._34 + matrix._31;
	m_planes[0].m128_f32[3] /*.d*/ = matrix._44 + matrix._41;

	// Calculate right plane of frustum.
	m_planes[1].m128_f32[0] /*.a*/ = matrix._14 - matrix._11; 
	m_planes[1].m128_f32[1] /*.b*/ = matrix._24 - matrix._21;
	m_planes[1].m128_f32[2] /*.c*/ = matrix._34 - matrix._31;
	m_planes[1].m128_f32[3] /*.d*/ = matrix._44 - matrix._41;

	// Calculate near plane of frustum.
	m_planes[2].m128_f32[0] /*.a*/ = matrix._14 + matrix._13;        // a = The X value of the plane's normal
	m_planes[2].m128_f32[1] /*.b*/ = matrix._24 + matrix._23;        // b = The Y value of the plane's normal
	m_planes[2].m128_f32[2] /*.c*/ = matrix._34 + matrix._33;        // c = The Z value of the plane's normal
	m_planes[2].m128_f32[3] /*.b*/ = matrix._44 + matrix._43;        // d = The distance the plane is from the origin

	// Calculate far plane of frustum.
	m_planes[3].m128_f32[0] /*.a*/ = matrix._14 - matrix._13;        // a = The X value of the plane's normal
	m_planes[3].m128_f32[1] /*.b*/ = matrix._24 - matrix._23;        // b = The Y value of the plane's normal
	m_planes[3].m128_f32[2] /*.c*/ = matrix._34 - matrix._33;        // c = The Z value of the plane's normal
	m_planes[3].m128_f32[3] /*.d*/ = matrix._44 - matrix._43;        // d = The distance the plane is from the origin

    for(int i = 0; i < 4; i++) {
		m_planes[i] = XMPlaneNormalize (m_planes[i]);
	}
}

//CheckPoint checks if a single point is inside the viewing frustum. This is the most general of the four checking algorithms but can be very efficient if 
//used correctly in the right situation over the other checking methods. It takes the point and checks to see if it is inside all six planes. 
//If the point is inside all six then it returns true, otherwise it returns false if not.

bool DXfrustumClass::CheckPoint(float x, float y, float z)
{
	// Check if the point is inside all six planes of the view frustum.
	for(int i=0; i<4; i++) 
	{
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet(x, y, z, 0) )) < 0.0f )
			return false;
	}

	return true;
}

//CheckCube checks if any of the eight corner points of the cube are inside the viewing frustum. 
//It only requires as input the center point of the cube and the radius, it uses those to calculate the 8 corner points of the cube. 
//It then checks if any one of the corner points are inside all 4 planes of the viewing frustum. 
//If it does find a point inside all six planes of the viewing frustum it returns true, otherwise it returns false.

bool DXfrustumClass::CheckCube(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if any one point of the cube is in the view frustum.
	for(int i=0; i<4; i++) 
	{
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;

		return false;
	}

	return true;
}

//CheckSphere checks if the radius of the sphere from the center point is inside all six planes of the viewing frustum. 
//If it is outside any of them then the sphere cannot be seen and the function will return false. 
//If it is inside all six the function returns true that the sphere can be seen.

bool DXfrustumClass::CheckSphere(float xCenter, float yCenter, float zCenter, float radius)
{
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<4; i++) 
	{
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet(xCenter, yCenter, zCenter, 0) )) < -radius )
			return false;
	}

	return true;
}

#pragma warning( disable : 4701 ) // warning C4701: potentially uninitialized local variable 'd' used
float DXfrustumClass::SphereDistance(float xCenter, float yCenter, float zCenter, float radius)
{   float d;
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<4; i++) 
	{
		d = XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet(xCenter, yCenter, zCenter, 0) ));
		if( d <= -radius)
			return 0;
	}

	return d + radius;
}

//CheckRectangle works the same as CheckCube except that that it takes as input the x radius, y radius, and z radius of the rectangle 
//instead of just a single radius of a cube. It can then calculate the 8 corner points of the rectangle and do the 
//frustum checks similar to the CheckCube function.

bool DXfrustumClass::CheckRectangle(float xCenter, float yCenter, float zCenter, float xSize, float ySize, float zSize)
{
	// Check if any of the 4 planes of the rectangle are inside the view frustum.
	for(int i=0; i<4; i++)
	{
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_planes[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;

		return false;
	}

	return true;
}

#endif
