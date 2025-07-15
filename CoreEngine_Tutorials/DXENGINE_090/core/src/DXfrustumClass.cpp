// --------------------------------------------------------------------------------------------
// Filename: DXfrustumClass.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
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
//WomaIntegrityCheck = 1234525256;

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

#define _11 r[0].m128_f32[0]
#define _12 r[0].m128_f32[1]
#define _13 r[0].m128_f32[2]
#define _14 r[0].m128_f32[3]

#define _21 r[1].m128_f32[0]
#define _22 r[1].m128_f32[1]
#define _23 r[1].m128_f32[2]
#define _24 r[1].m128_f32[3]

#define _31 r[2].m128_f32[0]
#define _32 r[2].m128_f32[1]
#define _33 r[2].m128_f32[2]
#define _34 r[2].m128_f32[3]

#define _41 r[3].m128_f32[0]
#define _42 r[3].m128_f32[1]
#define _43 r[3].m128_f32[2]
#define _44 r[3].m128_f32[3]

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
	m_frustumPlane[0].m128_f32[0] /*.a*/ = matrix._14 + matrix._11; 
	m_frustumPlane[0].m128_f32[1] /*.b*/ = matrix._24 + matrix._21;
	m_frustumPlane[0].m128_f32[2] /*.c*/ = matrix._34 + matrix._31;
	m_frustumPlane[0].m128_f32[3] /*.d*/ = matrix._44 + matrix._41;

	// Calculate right plane of frustum.
	m_frustumPlane[1].m128_f32[0] /*.a*/ = matrix._14 - matrix._11; 
	m_frustumPlane[1].m128_f32[1] /*.b*/ = matrix._24 - matrix._21;
	m_frustumPlane[1].m128_f32[2] /*.c*/ = matrix._34 - matrix._31;
	m_frustumPlane[1].m128_f32[3] /*.d*/ = matrix._44 - matrix._41;

	// Calculate near plane of frustum.
	m_frustumPlane[2].m128_f32[0] /*.a*/ = matrix._14 + matrix._13;        // a = The X value of the plane's normal
	m_frustumPlane[2].m128_f32[1] /*.b*/ = matrix._24 + matrix._23;        // b = The Y value of the plane's normal
	m_frustumPlane[2].m128_f32[2] /*.c*/ = matrix._34 + matrix._33;        // c = The Z value of the plane's normal
	m_frustumPlane[2].m128_f32[3] /*.b*/ = matrix._44 + matrix._43;        // d = The distance the plane is from the origin

	// Calculate far plane of frustum.
	m_frustumPlane[3].m128_f32[0] /*.a*/ = matrix._14 - matrix._13;        // a = The X value of the plane's normal
	m_frustumPlane[3].m128_f32[1] /*.b*/ = matrix._24 - matrix._23;        // b = The Y value of the plane's normal
	m_frustumPlane[3].m128_f32[2] /*.c*/ = matrix._34 - matrix._33;        // c = The Z value of the plane's normal
	m_frustumPlane[3].m128_f32[3] /*.d*/ = matrix._44 - matrix._43;        // d = The distance the plane is from the origin

    for(int i = 0; i < 4; i++) {
		m_frustumPlane[i] = XMPlaneNormalize (m_frustumPlane[i]);
	}
}

#undef _11
#undef _12
#undef _13
#undef _14
	   
#undef _21
#undef _22
#undef _23
#undef _24
	   
#undef _31
#undef _32
#undef _33
#undef _34
	   
#undef _41
#undef _42
#undef _43
#undef _44


//CheckPoint checks if a single point is inside the viewing frustum. This is the most general of the four checking algorithms but can be very efficient if 
//used correctly in the right situation over the other checking methods. It takes the point and checks to see if it is inside all six planes. 
//If the point is inside all six then it returns true, otherwise it returns false if not.

bool DXfrustumClass::CheckPoint(float x, float y, float z)
{
	// Check if the point is inside all six planes of the view frustum.
	for(int i=0; i<4; i++) 
	{
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet(x, y, z, 0) )) < 0.0f )
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
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter - radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - radius), (yCenter - radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + radius), (yCenter - radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - radius), (yCenter + radius), (zCenter + radius), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + radius), (yCenter + radius), (zCenter + radius), 0) )) >= 0.0f )
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
        float f = XMVectorGetX(XMPlaneDotCoord(m_frustumPlane[i], XMVectorSet(xCenter, yCenter, zCenter, 0)));
		if (f < -radius )
			return false;
	}

	return true;
}

#pragma warning( disable : 4701 ) // warning C4701: potentially uninitialized local variable 'd' used
float DXfrustumClass::SphereDistance(float xCenter, float yCenter, float zCenter, float radius)
{   
    float d;
	// Check if the radius of the sphere is inside the view frustum.
	for(int i=0; i<4; i++) 
	{
        d = XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet(xCenter, yCenter, zCenter, 0) ));
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
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - xSize), (yCenter - ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter - zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + xSize), (yCenter - ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter - xSize), (yCenter + ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;
		if (XMVectorGetX(XMPlaneDotCoord( m_frustumPlane[i], XMVectorSet((xCenter + xSize), (yCenter + ySize), (zCenter + zSize), 0) )) >= 0.0f )
			continue;

		return false;
	}

	return true;
}

#if defined USE_AABB_COLISION_CHECK
bool DXfrustumClass::CheckAABB(float minX, float minY, float minZ, float maxX, float maxY, float maxZ)
{
    for (int i = 0; i < 4; i++) // If you add far/near planes, use 6
    {
        // Get plane normal components
        XMFLOAT4 plane;
        XMStoreFloat4(&plane, m_frustumPlane[i]);

        // Positive vertex (furthest in the direction of the normal)
        float x = (plane.x >= 0.0f) ? maxX : minX;
        float y = (plane.y >= 0.0f) ? maxY : minY;
        float z = (plane.z >= 0.0f) ? maxZ : minZ;

        // Plane equation: dot(normal, vertex) + d
        float distance = plane.x * x + plane.y * y + plane.z * z + plane.w;

        // If the vertex is outside, the whole box is outside
        if (distance < 0.0f)
            return false;
    }

    return true;
}

/*
bool DXfrustumClass::CheckAABB(float xMin, float yMin, float zMin, float xMax, float yMax, float zMax)
{
    XMFLOAT3 corners[8] = {
        {xMin, yMin, zMin},
        {xMax, yMin, zMin},
        {xMin, yMax, zMin},
        {xMax, yMax, zMin},
        {xMin, yMin, zMax},
        {xMax, yMin, zMax},
        {xMin, yMax, zMax},
        {xMax, yMax, zMax}
    };

    for (int i = 0; i < 6; i++)
    {
        XMVECTOR plane = m_frustumPlane[i];
        int out = 0;

        for (int j = 0; j < 8; j++)
        {
            XMVECTOR corner = XMLoadFloat3(&corners[j]);
            if (XMVectorGetX(XMPlaneDotCoord(plane, corner)) < 0.0f)
                out++;
        }

        // All 8 points are outside this plane = culled
        if (out == 8)
            return false;
    }

    return true;
}
*/
#endif

#endif
