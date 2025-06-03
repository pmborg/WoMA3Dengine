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
#include "OSengine.h"
#include "AnimationJob.h"
#include "Scene/Scene.h"
#include "Scene/Model/Skeleton.h"

using namespace DirectX;

inline float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

template <typename T>
int FindTranslation(double time, std::vector<T> keys)
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

void AnimationModelLoader::UpdateTimeElapsed(Scene& scene, float deltaTime)
{

	//for (size_t i = 1; i < scene.m_Node.size(); ++i)  //AQUI
    for (size_t i = 0; i < scene.m_Node.size(); ++i)
	{
		auto sceneObj = scene.m_Node[i];

        Animator& animator = sceneObj->m_Animator;
        if (animator.m_Skeleton == nullptr) continue;

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
		//if (i==1) //AQUI!
        ReadNodeHierarchy(animator.m_TimeElapsed, skeleton->m_Root, animator, skeleton->m_RootTransform);
		//break;    //AQUI!
//#if _NOT
		//AI VERSION:
		//for (size_t i = 0; i < skeleton->m_Bones.size(); ++i)
		//{
		//	CalculateBoneTransforms(animator.m_TimeElapsed, skeleton->m_Bones[i], animator, skeleton->m_RootTransform);
		//}
//#endif
    }

}

void AnimationModelLoader::ReadNodeHierarchy2(float AnimationTime, Bone* bone, Animator& animator, const XMMATRIX& parentTransform)
{
    //animator.m_FinalTransforms[bone->m_Index] = bone->m_Offset * globalTransform * skeleton->m_GlobalInverse;

#ifdef DEBUG_MESH
    if (m_Driver->RenderfirstTime)
    {
#define i bone->m_Index
        LOG_FILE << "Transform[" << i << "]:" << std::endl;

        LOG_FILE << " " << animator.m_FinalTransforms[i].r[0].m128_f32[0];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[0].m128_f32[1];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[0].m128_f32[2];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[0].m128_f32[3] << std::endl;

        LOG_FILE << " " << animator.m_FinalTransforms[i].r[1].m128_f32[0];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[1].m128_f32[1];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[1].m128_f32[2];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[1].m128_f32[3] << std::endl;

        LOG_FILE << " " << animator.m_FinalTransforms[i].r[2].m128_f32[0];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[2].m128_f32[1];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[2].m128_f32[2];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[2].m128_f32[3] << std::endl;

        LOG_FILE << " " << animator.m_FinalTransforms[i].r[3].m128_f32[0];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[3].m128_f32[1];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[3].m128_f32[2];
        LOG_FILE << " " << animator.m_FinalTransforms[i].r[3].m128_f32[3] << std::endl;
#undef i
    }
#endif
}

void AnimationModelLoader::ReadNodeHierarchy (float AnimationTime, Bone* bone, Animator& animator, const XMMATRIX& parentTransform)
{
	std::string& boneName = bone->m_Name;

    float factor = 0;
    XMMATRIX globalTransform;
    Skeleton* skeleton = animator.m_Skeleton;
    Animation& animation = skeleton->m_Animations[animator.m_AnimIndexChosen];
    NodeAnimation& nodeAnim = animation.mChannels[boneName];

	if (nodeAnim.mPositionKeys.size() > 1) //Female dont have: nodeAnim.m_Positions
	{
    // Translation:
    XMVECTOR interpPos = nodeAnim.mPositionKeys[0].m_Position;
    if (nodeAnim.mPositionKeys.size() > 1)
    {
        //ori: int posKeyIdx = CurrentKeyIndex<NodeAnimation::PositionKey>(nodeAnim.m_Positions, time);
		int position_index = FindTranslation(AnimationTime, nodeAnim.mPositionKeys);
        int next_position_index = position_index + 1;

        NodeAnimation::PositionKey posKey = nodeAnim.mPositionKeys[position_index];
        NodeAnimation::PositionKey nPosKey = nodeAnim.mPositionKeys[next_position_index];

		float delta_time = (nPosKey.m_Time - posKey.m_Time);
        factor = (AnimationTime - posKey.m_Time) / delta_time;

        interpPos = XMVectorLerp(posKey.m_Position, nPosKey.m_Position, factor);
    }
    XMMATRIX translate_matr = XMMatrixTranslationFromVector(interpPos);

    // Rotation:
    XMVECTOR interpQuat = nodeAnim.m_Rotations[0].m_Quaternion;
    if (nodeAnim.m_Rotations.size() > 1)
    {
        int RotationIndex = FindTranslation<NodeAnimation::RotationKey>(AnimationTime, nodeAnim.m_Rotations);
        int NextRotationIndex = RotationIndex + 1;

        NodeAnimation::RotationKey rotKey = nodeAnim.m_Rotations[RotationIndex];
        NodeAnimation::RotationKey nRotKey = nodeAnim.m_Rotations[NextRotationIndex];

		float DeltaTime = (nRotKey.m_Time - rotKey.m_Time);
        factor = (AnimationTime - rotKey.m_Time) / DeltaTime;

        interpQuat = XMQuaternionSlerp(rotKey.m_Quaternion, nRotKey.m_Quaternion, factor);

    }
    XMMATRIX rotation_matr = XMMatrixRotationQuaternion(interpQuat);

    // Scaling:
    float interpScale = 1;
//#define doscale
#if defined doscale
    if (nodeAnim.m_Scalings.size() > 1)
    {
		interpScale = nodeAnim.m_Scalings[0].m_Scaling.x;

        int scalKeyIdx = CurrentKeyIndex<NodeAnimation::ScalingKey>(nodeAnim.m_Scalings, time);
        int nScalKeyIdx = scalKeyIdx + 1;

        NodeAnimation::ScalingKey scalKey = nodeAnim.m_Scalings[scalKeyIdx];
        NodeAnimation::ScalingKey nScalKey = nodeAnim.m_Scalings[nScalKeyIdx];

        auto t = (time - scalKey.m_Time) / (nScalKey.m_Time - scalKey.m_Time);
        interpScale = lerp(scalKey.m_Scaling.x, nScalKey.m_Scaling.x, t);
    }

    XMMATRIX scale = XMMatrixScaling(interpScale, interpScale, interpScale);
	XMMATRIX nodeTransform = scale * rotation_matr * translate_matr;
#else
    //NEW version:
    XMMATRIX nodeTransform = rotation_matr * translate_matr;

#endif

		globalTransform = nodeTransform * parentTransform;
    	}
	else
	{
		globalTransform = parentTransform;
	}

    if (bone->m_Index < animator.m_Skeleton->m_Bones.size() - 1)
    {
        animator.m_FinalTransforms[bone->m_Index] = bone->m_Offset * globalTransform;// *skeleton->m_GlobalInverse;

#ifdef DEBUG_MESH
    if (m_Driver->RenderfirstTime) 
    {
        #define i bone->m_Index
        LOG_FILE << "Transform[" << i << "]:" << std::endl;

        XMMATRIX t_FinalTransforms = animator.m_FinalTransforms[i];
        //t_FinalTransforms = XMMatrixTranspose(t_FinalTransforms);

        LOG_FILE << " " << t_FinalTransforms.r[0].m128_f32[0];
        LOG_FILE << " " << t_FinalTransforms.r[0].m128_f32[1];
        LOG_FILE << " " << t_FinalTransforms.r[0].m128_f32[2];
        LOG_FILE << " " << t_FinalTransforms.r[0].m128_f32[3] << std::endl;
                          
        LOG_FILE << " " << t_FinalTransforms.r[1].m128_f32[0];
        LOG_FILE << " " << t_FinalTransforms.r[1].m128_f32[1];
        LOG_FILE << " " << t_FinalTransforms.r[1].m128_f32[2];
        LOG_FILE << " " << t_FinalTransforms.r[1].m128_f32[3] << std::endl;
                          
        LOG_FILE << " " << t_FinalTransforms.r[2].m128_f32[0];
        LOG_FILE << " " << t_FinalTransforms.r[2].m128_f32[1];
        LOG_FILE << " " << t_FinalTransforms.r[2].m128_f32[2];
        LOG_FILE << " " << t_FinalTransforms.r[2].m128_f32[3] << std::endl;
                          
        LOG_FILE << " " << t_FinalTransforms.r[3].m128_f32[0]; 
        LOG_FILE << " " << t_FinalTransforms.r[3].m128_f32[1];
        LOG_FILE << " " << t_FinalTransforms.r[3].m128_f32[2];
        LOG_FILE << " " << t_FinalTransforms.r[3].m128_f32[3] << std::endl;
        #undef i
    }
#endif
    }

	for (size_t i = 0; i < bone->m_Children.size(); ++i) {
		ReadNodeHierarchy(AnimationTime, bone->m_Children[i], animator, globalTransform);
	}
}

