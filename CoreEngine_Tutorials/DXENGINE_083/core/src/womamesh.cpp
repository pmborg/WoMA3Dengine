// --------------------------------------------------------------------------------------------
// Filename: womamesh.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "standard_platform.h"
#include "ApplicationClass.h"
#include "womamesh.h"
#include "MyDemo.h"
#include "mem_leak.h"
#include "OSengine.h"
#include "fileLoader.h"

#include "stdafx.h"
#include "PPG.h"

struct WomaMesh {
    Scene scene;
    SceneModel* assimpSceneModel = NULL;
} WOMAMESH;

WomaMesh womamesh1;

std::ofstream os_file("log.txt", std::ios::out);
void log(char* msg)
{
    LOG_FILE << msg << std::endl;
}

void InitMeshDemo(ApplicationClass* app, MeshApplication* demoapp, MyDemo* demo)
{
    // Lighting
    auto lightColour = XMFLOAT4(5.0f, 5.0f, 5.0f, 1.0f);

    Light pointLight;
    pointLight.m_Color = XMFLOAT4(1, 1, 0, 0);
    pointLight.m_Position = XMFLOAT4(4, 3, 0, 0);
    pointLight.m_LightType = LightType::PointLight;

    Light dirLight;
    dirLight.m_Color = lightColour;
    dirLight.m_Direction = XMFLOAT4(-1, -1, 1, 0);
    dirLight.m_LightType = LightType::DirectionalLight;

    Light spotLight;
    spotLight.m_Color = XMFLOAT4(Colors::Magenta);
    spotLight.m_Direction = XMFLOAT4(0, -1, 0, 0);
    spotLight.m_Position = XMFLOAT4(3, 2, 0, 0);
    spotLight.m_LightType = LightType::SpotLight;
    spotLight.m_SpotAngle = 3.142f / 4.0f;

    //82:
    womamesh1.scene.lightManager
        .AddLight(dirLight)
        .AddLight(pointLight)
        .AddLight(spotLight)
        .SetGlobalAmbient(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));

    womamesh1.scene.Start(demoapp->m_Graphics);
}
void LoadAllMeshModels(UINT this_level, ApplicationClass* app, MeshApplication* demoapp, MyDemo* demo)
{
#if DX_ENGINE_LEVEL >= 79 && defined USE_MODEL1
    womamesh1.assimpSceneModel = SceneModel::LoadModelToScene(DX_ENGINE_LEVEL, true, 0, WOMA::LoadFile((TCHAR*)ASSIMP_MODEL_BOBLAMPCLEAN), "", womamesh1.scene, demoapp->m_Graphics);
#endif

#ifdef DEBUG_MESH
    log("STARTING...");
#endif
}
void UpdateAllMeshAnimations(MeshApplication* demoapp, MyDemo* demo, float deltaTime)
{
    //Update animation/bone matrix's and RENDER all MESHs:
#if DX_ENGINE_LEVEL >= 79 && defined USE_MODEL1
    if (womamesh1.assimpSceneModel && womamesh1.assimpSceneModel->loaded)
        demo->animJob.UpdateTimeElapsed(womamesh1.scene, deltaTime);
#endif
}
void RenderAllMeshModels(MeshApplication* demoapp, MyDemo* demo)
{
    demoapp->m_Graphics.m_DeviceContext->RSSetState(demoapp->m_Graphics.m_RasterizerState);
        // Model 1 -----------------a-------------------------------------------------------------------
#if DX_ENGINE_LEVEL >= 79 && defined USE_MODEL1
    if (womamesh1.assimpSceneModel && womamesh1.assimpSceneModel->loaded)
    {
        XMMATRIX world = XMMatrixIdentity();
        //Scale:
        //_11
        //_22
        //_33

        world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.02f;
        XMMATRIX rotX = XMMatrixRotationX(-PI / 2);
        world *= rotX;
        XMMATRIX rotZ = XMMatrixRotationZ(PI / 2);
        world *= rotZ;
        //Translate:
        const float X = 39, Z = 20;
        world.r[3].m128_f32[0] = X; //_41: X
        world.r[3].m128_f32[2] = Z; //_43: Z
        world.r[3].m128_f32[1] = mainTerrain->getTerrainHeight(TERRAIN_ID, world.r[3].m128_f32[0], world.r[3].m128_f32[2]);

        womamesh1.scene.UpdateWorldMatrixModel(demoapp->m_Graphics, world);
        demo->gBufferPass->Render(demoapp->m_Graphics, womamesh1.scene);
    }
#endif

    // Model 2 ------------------------------------------------------------------------------------------

}

bool threadLoadMeshAlive=false;
HANDLE threadLoadMeshHandle=NULL;
unsigned long threadLoadMeshId=NULL;

static MeshApplication *demoapp_;
static MyDemo *demo_;

DWORD StartMeshLibs(LPVOID lpParam)
{
    SetUnhandledExceptionFilter(TopLevelFilter);
    ApplicationClass* app = static_cast<ApplicationClass*>(lpParam);

    // INIT: Model 1,2,3,4...
    InitMeshDemo(app, demoapp_, demo_);
    LoadAllMeshModels(DX_ENGINE_LEVEL, app, demoapp_, demo_);

    return 0;
};

void ApplicationClass::RenderMeshAnimations()
{
    static MeshApplication demoapp; demoapp_ = &demoapp;
    static MyDemo demo; demo_ = &demo;

#if defined USE_ASSIMP_LATEST && defined MAIN_RENDER_ASSIMP // ASSIMP: Skin-MESH (0.15ms)
    if (m_Driver->RenderfirstTime)
    {
        demo_->Start(demoapp.m_Graphics);
        threadLoadMeshHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartMeshLibs, (void*)this, 0, &threadLoadMeshId);
    }

    static UINT filmeIdx = 0; // 1st line of filme file
    static DWORD m_startTime = timeGetTime();
    static DWORD previousTime = timeGetTime();

    if (total_deltaTime > 70000) {
        m_startTime = timeGetTime();
        previousTime = m_startTime;
        filmeIdx = 0;
    }

    DWORD currentTime = timeGetTime();
    float deltaTime = (currentTime - previousTime) / 1000.0f;
    previousTime = currentTime;
    total_deltaTime = (timeGetTime() - m_startTime);

    UpdateAllMeshAnimations(&demoapp, &demo, deltaTime);
    RenderAllMeshModels(&demoapp, &demo);
#endif
}


