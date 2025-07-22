// --------------------------------------------------------------------------------------------
// Filename: SceneObject.cpp
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
// WomaIntegrityCheck = 1234525217;

#include "stdafx.h"
#include "Scene/SceneObject.h"
#include "Scene/Material/Material.h"
#include "Resources/Mesh.h"

SceneObject::SceneObject(const std::string& name, SceneObject::Index index, SceneObject::Index parentIndex) :
    m_Name(name),
    m_Index(index),
    m_ParentIndex(parentIndex),
    m_ChildrenIndices()
{
}
