// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass_ext.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2023 Pedro Miguel Borges [pmborg@yahoo.com]
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
// PURPOSE: 25+
// --------------------------------------------------------------------------------------------
#include "main.h"
#include "ApplicationClass.h"
#include "mem_leak.h"

#include "winSystemClass.h"

ApplicationClass::ApplicationClass()
{
	CLASSLOADER();

	// ---------------------------------------------------------------------
	// private:

		RENDER_PAGE = CORE_ENGINE_LEVEL;
  #if _DEBUG
	WOMA_LOGManager_DebugMSG("RENDER_PAGE: %d\n", RENDER_PAGE);
  #endif

	// ---------------------------------------------------------------------
	// public:
	dt = 0.0f;	// Delta time



#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN //24
	autoGenUnderWaterTerrain		= NULL;
#endif

	// TERRAIN

#if defined USE_LIGHT_RAY // LightModel
	m_lightRayModel = NULL;
#endif

	Start();
}

ApplicationClass::~ApplicationClass() {CLASSDELETE();}

//	-------------------------------------------------------------------------------------------
void ApplicationClass::Shutdown()
{
	WOMA_LOGManager_DebugMSG ("ApplicationClass::Shutdown()\n");

#if defined SCENE_GENERATEDUNDERWATER || defined SCENE_UNDERWATER_BATH_TERRAIN
	SAFE_DELETE (autoGenUnderWaterTerrain);
#endif
#if defined SCENE_MAIN_TOPO_TERRAIN
	SAFE_DELETE (mainTopoTerrain);
#endif
#if defined SCENE_MAIN_TERRAIN
	//SAFE_DELETE(mainTerrain);
#endif


}



//-------------------------------------------------------------------------------------------
bool ApplicationClass::Start()
//-------------------------------------------------------------------------------------------
{

#if defined USE_RASTERTEK_TEXT_FONT
	AppTextClass = NULL;
#endif

	return true;
}



//-----------------------------------------------------------------------------------------
void ApplicationClass::WOMA_APPLICATION_Shutdown()
//-----------------------------------------------------------------------------------------
{
	WOMA_LOGManager_DebugMSG("WOMA_APPLICATION_Shutdown()\n");

#if defined USE_LIGHT_RAY

#endif
}
