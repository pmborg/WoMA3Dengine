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
