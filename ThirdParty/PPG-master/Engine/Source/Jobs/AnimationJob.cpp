// --------------------------------------------------------------------------------------------
// Filename: AnimationJob.cpp
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

#include "stdafx.h"

#include "AnimationJob.h"
#include "Scene/Scene.h"
#include "Scene/Model/Skeleton.h"

using namespace DirectX;

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

template <typename T>
int FindTranslation(std::vector<T> keys, double time)
{
    float duration = time;
    for (UINT i = 0; i < keys.size() - 1; ++i)
    {
        if (duration < keys[i + 1].m_Time)
        {
            return i;
        }
    }
	assert(0);
	return 0;
}

void AnimationJob::UpdateTimeElapsed(Scene& scene, float deltaTime)
{

	for (size_t i = 1; i < scene.m_Node.size(); ++i)
	{
		auto sceneObj = scene.m_Node[i];
		if (sceneObj->m_Animator.m_Skeleton == nullptr) 
			continue;

        Animator& animator = sceneObj->m_Animator;
        Skeleton* skeleton = animator.m_Skeleton;
		if (skeleton->m_Animations.size() == 0) 
		{
			animator.m_IsEnabled = false;
			return;
		}
        Animation& animation = skeleton->m_Animations[animator.m_AnimIndexChosen];

        animator.m_TimeElapsed += deltaTime * animation.m_TicksPerSecond;
        animator.m_TimeElapsed = fmod(animator.m_TimeElapsed, animation.m_Duration);

		// ORGINAL:
		if (i==1)
        CalculateBoneTransforms(skeleton->m_Root, animator, skeleton->m_RootTransform, animator.m_TimeElapsed);
		break;
    }

}

void AnimationJob::CalculateBoneTransforms(Bone* bone, Animator& animator, const XMMATRIX& parentTransform, float time)
{
	std::string& boneName = bone->m_Name;

    Skeleton* skeleton = animator.m_Skeleton;
    Animation& animation = skeleton->m_Animations[animator.m_AnimIndexChosen];
    NodeAnimation& nodeAnim = animation.mChannels[boneName];
    float factor = 0;
	XMMATRIX globalTransform;

	if (nodeAnim.mPositionKeys.size() > 1) //Female dont have: nodeAnim.m_Positions
	{
    // Translation:
    XMVECTOR interpPos = nodeAnim.mPositionKeys[0].m_Position;
    if (nodeAnim.mPositionKeys.size() > 1)
    {
        //ori: int posKeyIdx = CurrentKeyIndex<NodeAnimation::PositionKey>(nodeAnim.m_Positions, time);
		int position_index = FindTranslation(nodeAnim.mPositionKeys, time);
        int next_position_index = position_index + 1;

        NodeAnimation::PositionKey posKey = nodeAnim.mPositionKeys[position_index];
        NodeAnimation::PositionKey nPosKey = nodeAnim.mPositionKeys[next_position_index];

		float delta_time = (nPosKey.m_Time - posKey.m_Time);
        factor = (time - posKey.m_Time) / delta_time;

        interpPos = XMVectorLerp(posKey.m_Position, nPosKey.m_Position, factor);
    }
    XMMATRIX translate_matr = XMMatrixTranslationFromVector(interpPos);

    // Rotation:
    XMVECTOR interpQuat = nodeAnim.m_Rotations[0].m_Quaternion;
    if (nodeAnim.m_Rotations.size() > 1)
    {
        int RotationIndex = FindTranslation<NodeAnimation::RotationKey>(nodeAnim.m_Rotations, time);
        int NextRotationIndex = RotationIndex + 1;

        NodeAnimation::RotationKey rotKey = nodeAnim.m_Rotations[RotationIndex];
        NodeAnimation::RotationKey nRotKey = nodeAnim.m_Rotations[NextRotationIndex];

		float DeltaTime = (nRotKey.m_Time - rotKey.m_Time);
        factor = (time - rotKey.m_Time) / DeltaTime;

        interpQuat = XMQuaternionSlerp(rotKey.m_Quaternion, nRotKey.m_Quaternion, factor);

    }
    XMMATRIX rotation_matr = XMMatrixRotationQuaternion(interpQuat);

    // Scaling:
    float interpScale = 1;
#if scale
    if (nodeAnim.m_Scalings.size() > 1)
    {
		interpScale = nodeAnim.m_Scalings[0].m_Scaling.x;

        int scalKeyIdx = CurrentKeyIndex<NodeAnimation::ScalingKey>(nodeAnim.m_Scalings, time);
        int nScalKeyIdx = scalKeyIdx + 1;

        NodeAnimation::ScalingKey scalKey = nodeAnim.m_Scalings[scalKeyIdx];
        NodeAnimation::ScalingKey nScalKey = nodeAnim.m_Scalings[nScalKeyIdx];

        t = (time - scalKey.m_Time) / (nScalKey.m_Time - scalKey.m_Time);
        interpScale = lerp(scalKey.m_Scaling.x, nScalKey.m_Scaling.x, t);
    }

    XMMATRIX scale = XMMatrixScaling(interpScale, interpScale, interpScale);
	XMMATRIX nodeTransform = scale * rotation * translation;
#endif
	    //NEW version:
		XMMATRIX nodeTransform = rotation_matr * translate_matr;
		globalTransform = nodeTransform * parentTransform;
	}
	else
	{
		globalTransform = parentTransform;
	}


    animator.m_FinalTransforms[bone->m_Index] = bone->m_Offset * globalTransform * skeleton->m_GlobalInverse;

	for (size_t i = 0; i < bone->m_Children.size(); ++i) {
		CalculateBoneTransforms(bone->m_Children[i], animator, globalTransform, time);
	}
}

