// --------------------------------------------------------------------------------------------
// Filename: stdafx.h
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
#pragma warning( disable : 4099 )

#define NOMINMAX //https://stackoverflow.com/questions/5004858/stdmin-gives-error
#include <windows.h>
#include <fstream> 

extern std::ofstream os_file;
#define LOG_FILE os_file

// DirectX includes
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <Directxpackedvector.h>
#include <DirectXColors.h>

// DirectXTex

// STL includes
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <filesystem>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")
#include "woma_macros.h"

inline DirectX::XMFLOAT3 XMFLOAT3Subtract(DirectX::XMFLOAT3 a, DirectX::XMFLOAT3 b)
{
    return { a.x - b.x,
             a.y - b.y,
             a.z - b.z };
}

inline DirectX::XMFLOAT2 XMFLOAT2Subtract(DirectX::XMFLOAT2 a, DirectX::XMFLOAT2 b)
{
    return { a.x - b.x,
             a.y - b.y };
}

inline void SetDebugName(ID3D11DeviceChild* child, const std::string& name)
{
    if (child != nullptr)
        child->SetPrivateData(WKPDID_D3DDebugObjectName, (UINT)name.size(), name.c_str());
}

inline float GetFloatAtIndex(DirectX::XMFLOAT4& vec, int i)
{
    switch (i)
    {
        case 0:
            return vec.x;
        case 1:
            return vec.y;
        case 2:
            return vec.z;
        case 3:
            return vec.w;
    }
    assert(0);
    return 0.0;
};

inline void SetFloatAtIndex(DirectX::XMFLOAT4& vec, int i, float val)
{
    switch (i)
    {
        case 0:
            vec.x = val;
            return;
        case 1:
            vec.y = val;
            return;
        case 2:
            vec.z = val;
            return;
        case 3:
            vec.w = val;
            return;
    }
};
