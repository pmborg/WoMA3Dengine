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
//WomaIntegrityCheck = 1234525256;

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

WomaMesh womamesh2;

WomaMesh womamesh3;
WomaMesh womamesh4[1];

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
    womamesh2.scene.lightManager
        .AddLight(dirLight)
        .AddLight(pointLight)
        .AddLight(spotLight)
        .SetGlobalAmbient(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));
    womamesh4[MAIN_CHAR_MODEL1].scene.lightManager
        .AddLight(dirLight)
        .AddLight(pointLight)
        .AddLight(spotLight)
        .SetGlobalAmbient(XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f));

    womamesh1.scene.Start(demoapp->m_Graphics);
    womamesh2.scene.Start(demoapp->m_Graphics);
    womamesh3.scene.Start(demoapp->m_Graphics);
    womamesh4[MAIN_CHAR_MODEL1].scene.Start(demoapp->m_Graphics);
}
void LoadAllMeshModels(UINT this_level, ApplicationClass* app, MeshApplication* demoapp, MyDemo* demo)
{
#if DX_ENGINE_LEVEL >= 79 && defined USE_MODEL1
    womamesh1.assimpSceneModel = SceneModel::LoadModelToScene(DX_ENGINE_LEVEL, true, 0, WOMA::LoadFile((TCHAR*)ASSIMP_MODEL_BOBLAMPCLEAN), "", womamesh1.scene, demoapp->m_Graphics);
#endif

#if DX_ENGINE_LEVEL >= 84 && defined USE_MODEL2
    womamesh2.assimpSceneModel = SceneModel::LoadModelToScene(DX_ENGINE_LEVEL, true, 0, WOMA::LoadFile((TCHAR*)ASSIMP_MODEL_FEMALE), "", womamesh2.scene, demoapp->m_Graphics);
#if defined SCENE_SKIN
    {
#ifdef LOAD_WALK
        std::ifstream fileIn(WOMA::LoadFile((TCHAR*)MODEL_FEMALE_PATH));
        std::string filename;
        while (fileIn)								    // Loop until the end of the file is reached
        {
            fileIn >> app->filmeKey.timeFrame;			// Get next string from file
            if (fileIn) {
                fileIn >> app->filmeKey.X; fileIn >> app->filmeKey.Y; fileIn >> app->filmeKey.Z;
                fileIn >> app->filmeKey.rotY;
                app->loadFilme.push_back(app->filmeKey);
            }
        }

        fileIn.close();
#endif
    }
    if (!app->m_characterPos)
        app->m_characterPos = NEW PositionClass(0);
    if (!app->m_character)
        app->m_character = NEW PlayerClass(0);
#endif
#endif

#if DX_ENGINE_LEVEL >= 86 && defined USE_MODEL3
    womamesh3.assimpSceneModel = SceneModel::LoadModelToScene(DX_ENGINE_LEVEL, true, 0, WOMA::LoadFile(FOREST_HUNTRESS), "", womamesh3.scene, demoapp->m_Graphics);
#endif
#if (DX_ENGINE_LEVEL == 86 || DX_ENGINE_LEVEL == 87) && defined USE_MODEL4
    womamesh4[MAIN_CHAR_MODEL1].assimpSceneModel = SceneModel::LoadModelToScene(DX_ENGINE_LEVEL, false, 1, WOMA::LoadFile((TCHAR*)TEXT("engine/data/scene86ForestHuntress/")), "", womamesh4[MAIN_CHAR_MODEL1].scene, demoapp->m_Graphics);
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
#if DX_ENGINE_LEVEL >= 84 && defined (SCENE_SKIN)
    if (womamesh2.assimpSceneModel && womamesh2.assimpSceneModel->loaded)
        demo->animJob.UpdateTimeElapsed(womamesh2.scene, deltaTime);
#endif
#if DX_ENGINE_LEVEL >= 86 && defined (SCENE_SKIN)
    if (womamesh3.assimpSceneModel && womamesh3.assimpSceneModel->loaded)
        demo->animJob.UpdateTimeElapsed(womamesh3.scene, deltaTime);
#endif
#if DX_ENGINE_LEVEL >= 86 && defined USE_MODEL4
    demo->animJob.UpdateTimeElapsed(womamesh4[MAIN_CHAR_MODEL1].scene, deltaTime);
#endif
}
void RenderAllMeshModels(MeshApplication* demoapp, MyDemo* demo)
{
    demoapp->m_Graphics.m_DeviceContext->RSSetState(demoapp->m_Graphics.m_RasterizerState);
        // Model 1 ------------------------------------------------------------------------------------------
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
#if DX_ENGINE_LEVEL >= 84 && defined USE_MODEL2
#if defined SCENE_SKIN
    if (womamesh2.assimpSceneModel && womamesh2.assimpSceneModel->loaded && SystemHandle->m_Application->m_characterPos)
    {
        XMMATRIX world = XMMatrixIdentity();
        //Scale:
        //_11
        //_22
        //_33
        world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.07f;

        XMMATRIX rotX = XMMatrixRotationX(PI / 2);
        world *= rotX;
        XMMATRIX rotY = XMMatrixRotationY(PI + SystemHandle->m_Application->m_characterPos->m_rotationY);
        world *= rotY;

        //Translate:
        world.r[3].m128_f32[0] = SystemHandle->m_Application->m_characterPos->m_positionX;  //_41: X
        world.r[3].m128_f32[1] = SystemHandle->m_Application->m_characterPos->m_positionY;  //_42: Y 
        world.r[3].m128_f32[2] = SystemHandle->m_Application->m_characterPos->m_positionZ;  //_43: Z

        womamesh2.scene.UpdateWorldMatrixModel(demoapp->m_Graphics, world);
        demo->gBufferPass->Render(demoapp->m_Graphics, womamesh2.scene);
    }
#endif
#endif

#if DX_ENGINE_LEVEL >= 86 && defined USE_MODEL3
    // Model 3 ------------------------------------------------------------------------------------------
    {
        XMMATRIX world = XMMatrixIdentity();
        //Scale:
        //_11
        //_22
        //_33
        world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.0075f;

        //Translate:
        const float X = 36.5f, Z = 20;
        world.r[3].m128_f32[0] = X; //_41: X
        world.r[3].m128_f32[2] = Z; //_43: Z
        world.r[3].m128_f32[1] = mainTerrain->getTerrainHeight(TERRAIN_ID, world.r[3].m128_f32[0], world.r[3].m128_f32[2]);

        womamesh3.scene.UpdateWorldMatrixModel(demoapp->m_Graphics, world);
        gBufferPass->Render(demoapp->m_Graphics, womamesh3.scene);
    }
#endif
#if DX_ENGINE_LEVEL >= 86 && defined USE_MODEL4
    // Model 4 ------------------------------------------------------------------------------------------
    {
        XMMATRIX world = XMMatrixIdentity();
        //Scale:
        //_11
        //_22
        //_33
        world.r[0].m128_f32[0] = world.r[1].m128_f32[1] = world.r[2].m128_f32[2] = 0.0075f;
        //Translate:
        const float X = 35, Z = 20;
        world.r[3].m128_f32[0] = X; //_41: X
        world.r[3].m128_f32[2] = Z; //_43: Z
        world.r[3].m128_f32[1] = mainTerrain->getTerrainHeight(TERRAIN_ID, world.r[3].m128_f32[0], world.r[3].m128_f32[2]);

        womamesh4[MAIN_CHAR_MODEL1].scene.UpdateWorldMatrixModel(demoapp->m_Graphics, world);
        demo->gBufferPass->Render(demoapp->m_Graphics, womamesh4[MAIN_CHAR_MODEL1].scene);
    }
#endif
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
    demo_->Start(demoapp_->m_Graphics);
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
        //threadLoadMeshHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)StartMeshLibs, (void*)this, 0, &threadLoadMeshId);
        StartMeshLibs((void*)this);
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

#if defined ( LOAD_WALK ) && defined (SCENE_SKIN)
    // Do the Movement Animation of the "Character"
    // ============================================
    if (m_characterPos)
    {
        if (filmeIdx < loadFilme.size()) 	// Prepare to read the next movement animation
        {
            m_characterPos->m_positionX = loadFilme[filmeIdx].X;
            m_characterPos->m_positionZ = loadFilme[filmeIdx].Z;
            m_characterPos->m_positionY = mainTerrain->getTerrainHeight(TERRAIN_ID, m_characterPos->m_positionX, m_characterPos->m_positionZ);
            m_characterPos->m_rotationY = DEG2RAD(loadFilme[filmeIdx].rotY);

            while (filmeIdx < loadFilme.size() && loadFilme[filmeIdx].timeFrame <= total_deltaTime) {
                filmeIdx++;
            }
        }
        else {
  
            m_characterPos->m_positionX = 20.0f;
            m_characterPos->m_positionZ = 20.0f;
            filmeIdx = 0;
        }
    }
#endif

    UpdateAllMeshAnimations(&demoapp, &demo, deltaTime);
    RenderAllMeshModels(&demoapp, &demo);
#endif
}


