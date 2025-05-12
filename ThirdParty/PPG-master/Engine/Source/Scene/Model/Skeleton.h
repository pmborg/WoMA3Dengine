// --------------------------------------------------------------------------------------------
// Filename: Skeleton.h
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
// Original Code Adapted from: https://github.com/nicholaschuayunzhi/PPG

#pragma once
#include "stdafx.h"

#include <map>
#include <queue>

using namespace DirectX;

class Bone;
class Animation;

class Skeleton
{
public:
    Bone* m_Root;
    std::vector<Animation> m_Animations;
    std::vector<Bone*> m_Bones;
    XMMATRIX m_RootTransform;
    XMMATRIX m_GlobalInverse;
    ~Skeleton();
    static const UINT NUM_BONES = 128;
};

class Bone
{
public:
    std::string m_Name;
    int m_Index;
    XMMATRIX m_GlobalTransform;
    XMMATRIX m_Offset;
    std::vector<Bone*> m_Children;

    Bone(const std::string& name, int index, const XMMATRIX& offset) :
        m_Name(name),
        m_Index(index),
        m_Offset(offset),
        m_GlobalTransform(XMMatrixIdentity())
    {
    }
};

struct NodeAnimation
{
    std::string m_Name;

    struct PositionKey
    {
        XMVECTOR m_Position;
        double m_Time;
    };
    std::vector<PositionKey> mPositionKeys;

    struct RotationKey
    {
        XMVECTOR m_Quaternion;
        double m_Time;
    };
    std::vector<RotationKey> m_Rotations;

    struct ScalingKey
    {
        XMFLOAT3 m_Scaling;
        double m_Time;
    };
    std::vector<ScalingKey> m_Scalings;
};

class Animation
{
public:
    std::string m_Name;
    std::map<std::string, NodeAnimation> mChannels;
    double m_Duration;
    double m_TicksPerSecond;
};

