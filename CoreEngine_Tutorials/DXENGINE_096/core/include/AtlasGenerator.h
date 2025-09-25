// --------------------------------------------------------------------------------------------
// Filename: AtlasGenerator.h
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
// PURPOSE:
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#pragma once
#include "platform.h"

#if defined GENERATE_ATLAS_INTEGRATION_DDS

#define MAX_ATLAS_REGIONS 128
// replace the current AtlasRegion
struct AtlasRegion {
	float offsetU, offsetV;   // top-left in atlas UV space
	float scaleU, scaleV;    // width/height in atlas UV space
};

HRESULT GenerateBillboardAtlasDDS(
	ID3D11Device* device,
	const std::vector<STRING>& billboardTextures,
	ID3D11ShaderResourceView** outAtlasSRV,
	std::vector<AtlasRegion>& outRegions);
#endif
