// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: ApplicationClass.cpp
// --------------------------------------------------------------------------------------------
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2023
// --------------------------------------------------------------------------------------------
// code by : Pedro Borges - pmborg@yahoo.com
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
//
// PURPOSE: 25+
//
// --------------------------------------------------------------------------------------------
#include "main.h"
#include "ApplicationClass.h"
#include "mem_leak.h"

#include "winSystemClass.h"

#if defined USE_SCENE_MANAGER
	#include "SceneManager.h"

	namespace WOMA
	{
		SceneManager* sceneManager;
	}
#endif

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

#if defined SCENE_TERRAIN_QUAD_TREE //67
	TerrainQuadtree = NULL;
#endif

	// TERRAIN

//#if defined USE_TITLE_BANNER //26
//	m_titleModel = NULL;
//#endif

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
