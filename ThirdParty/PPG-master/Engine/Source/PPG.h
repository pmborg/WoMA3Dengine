// --------------------------------------------------------------------------------------------
// Filename: PPG.h
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

#include "Demo.h"
#include "LowLevel/Application.h"

#include "Scene/Scene.h"
#include "Scene/Camera.h"
#include "Scene/Light.h"
#include "Scene/Material/Material.h"
#include "Scene/Model/SceneModel.h"

#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Resources/Sampler.h"
#include "Resources/Shader.h"

#include "Passes/BlitPass.h"
#include "Passes/GBufferPass.h"
#include "Passes/DeferredPass.h"
#include "Passes/SSAOPass.h"
#include "Passes/ToneMapPass.h"

#include "Jobs/AnimationJob.h"

