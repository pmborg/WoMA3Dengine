// --------------------------------------------------------------------------------------------
// Filename: Sampler.h
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
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG

#pragma once
#include "stdafx.h"

class Graphics;

class Sampler
{
public:
    Sampler(Graphics& graphics, D3D11_FILTER filter, D3D11_TEXTURE_ADDRESS_MODE addressMode);
    ~Sampler();
    void Use(ID3D11DeviceContext* deviceContext, UINT startSlot);
    ID3D11SamplerState* m_Sampler;
};
