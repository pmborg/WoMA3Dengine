// --------------------------------------------------------------------------------------------
// Filename: realSkyPlaneClass.h
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Adopt the base idea of https://rastertek.com/tertut12.html
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#if defined USE_CURVED_REAL_SKY_PLANE

class RealSkyPlaneClass
{
private:
	int skyPlaneResolution = 0, textureRepeat = 0;
	float skyPlaneWidth = 0, skyPlaneTop = 0, skyPlaneBottom=0;
	std::vector<ModelTextureVertexType> woma_skyPlane;

public:
	float m_translation = 0;

	RealSkyPlaneClass();
	~RealSkyPlaneClass();
	void Render(void* pContext);

	bool Initialize(void* ctx, void* Driver);

private:
	void Shutdown();

	bool InitializeSkyPlane(void* ctx, void* Driver, int skyPlaneResolution, float skyPlaneWidth, float skyPlaneTop, float skyPlaneBottom, int textureRepeat);
};
#endif
