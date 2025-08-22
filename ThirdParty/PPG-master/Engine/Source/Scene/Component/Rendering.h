// --------------------------------------------------------------------------------------------
// Filename: Rendering.h
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
#include <vector>

class Mesh;
class PBRMaterial;
class Texture;
class Skeleton;
class Animator;

struct Component
{
    bool m_IsEnabled = false;
};

struct MeshRenderer : public Component
{
    PBRMaterial* m_Material;
    Mesh* m_Mesh;
    Animator* m_Animator = nullptr;
};

struct SpriteRenderer : public Component
{
    Texture* m_Sprite = nullptr;
};

struct Animator : public Component
{
    Skeleton* m_Skeleton = nullptr;
    float m_TimeElapsed = 0;
    UINT m_AnimIndexChosen = 0;
	XMMATRIX m_FinalTransforms[128] = {};
};
