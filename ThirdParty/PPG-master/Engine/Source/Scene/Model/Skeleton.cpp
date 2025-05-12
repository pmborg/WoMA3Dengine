#include "stdafx.h"

#include "Skeleton.h"

Skeleton::~Skeleton()
{
    for (Bone* bone : m_Bones)
    {
        delete bone;
    }
    m_Bones.clear();
}

