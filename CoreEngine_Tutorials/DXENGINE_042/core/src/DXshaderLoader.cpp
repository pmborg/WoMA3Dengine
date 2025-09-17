// --------------------------------------------------------------------------------------------
// Filename: DXshaderLoader.cpp
// --------------------------------------------------------------------------------------------
// World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
// --------------------------------------------------------------------------------------------
// Copyright(C) 2013 - 2025 Pedro Miguel Borges [pmborg@yahoo.com]
//
// This file is part of the WorldOfMiddleAge project.
//
// The WorldOfMiddleAge project files can not be copied or distributed for commercial use 
// without the express written permission of Pedro Miguel Borges [pmborg@yahoo.com]
// You may not alter or remove any copyright or other notice from copies of the content.
// The content contained in this file is provided only for educational and informational purposes.
// 
// Downloaded from : https://github.com/pmborg/WoMA3Dengine
// --------------------------------------------------------------------------------------------
// PURPOSE: Inject HLSL code on main EXE file
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234525217;

#include "platform.h"

#if defined DX11 || defined DX9 || defined DX12
#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
#pragma warning( disable : 4838 )
#include "DXshaderClass.h"

#ifdef USE_PRECOMPILED_SHADERS

#ifdef more_info
// ALL SHADERS:
// https://en.wikipedia.org/wiki/High-Level_Shading_Language

shader 2.0 winxp - DX9
shader 3.0 winxp - DX9.0C
shader 4.0 vista - DX10
shader 4.1 vista - DX10.1
shader 5.0 win7 - DX11
shader 5.0 win8 - DX11.1
shader 5.0 win8.1 - DX11.2
shader 5.1 win8.1 - DX11.3
shader 5.1 win10 - DX12
#endif

// DX9				shader 2.0
// https://msdn.microsoft.com/en-us/library/bb509655%28v=vs.85%29.aspx
// DX9.0c			shader 3.0
// https://msdn.microsoft.com/en-us/library/dn933277%28v=vs.85%29.aspx

#include "../../SAMPLES/DXengine/021ColorVs_40.h"	// DX 10			shader 4.0
#include "../../SAMPLES/DXengine/021ColorPs_40.h"	// https://msdn.microsoft.com/en-us/library/bb509657%28v=vs.85%29.aspx

#include "../../SAMPLES/DXengine/021ColorVs_41.h"	// DX 10.1			shader 4.1
#include "../../SAMPLES/DXengine/021ColorPs_41.h"

#include "../../SAMPLES/DXengine/021ColorVs_50.h"	// DX 11			shader 5.0
#include "../../SAMPLES/DXengine/021ColorPs_50.h"	// https://msdn.microsoft.com/en-us/library/ff471356%28v=vs.85%29.aspx

#include "../../SAMPLES/DXengine/021ColorVs_51.h"	// DX 11.3 & DX12	shader 5.1
#include "../../SAMPLES/DXengine/021ColorPs_51.h"	// https://msdn.microsoft.com/en-us/library/dn933277%28v=vs.85%29.aspx

#include "../../SAMPLES/DXengine/022TextureVs_40.h" 
//#include "../../SAMPLES/DXengine/022TextureGs_40.h" 
#include "../../SAMPLES/DXengine/022TexturePs_40.h"

#include "../../SAMPLES/DXengine/022TextureVs_41.h" 
//#include "../../SAMPLES/DXengine/022TextureGs_41.h"
#include "../../SAMPLES/DXengine/022TexturePs_41.h"

#include "../../SAMPLES/DXengine/022TextureVs_50.h" 
//#include "../../SAMPLES/DXengine/022TextureGs_50.h" 
#include "../../SAMPLES/DXengine/022TexturePs_50.h"

#include "../../SAMPLES/DXengine/022TextureVs_51.h" 
//#include "../../SAMPLES/DXengine/022TextureGs_51.h" 
#include "../../SAMPLES/DXengine/022TexturePs_51.h"

#include "../../SAMPLES/DXengine/023LightVs_40.h" 
#include "../../SAMPLES/DXengine/023LightPs_40.h"
#include "../../SAMPLES/DXengine/023LightVs_41.h" 
#include "../../SAMPLES/DXengine/023LightPs_41.h"
#include "../../SAMPLES/DXengine/023LightVs_50.h" 
#include "../../SAMPLES/DXengine/023LightPs_50.h"
#include "../../SAMPLES/DXengine/023LightVs_51.h" 
#include "../../SAMPLES/DXengine/023LightPs_51.h"

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER
// CastShadowMap (rendered by objects that will cast a shadow)
#include "../../SAMPLES/DXengine/036ShadowMapVs_40.h" 
#include "../../SAMPLES/DXengine/036ShadowMapPs_40.h"
#include "../../SAMPLES/DXengine/036ShadowMapVs_41.h" 
#include "../../SAMPLES/DXengine/036ShadowMapPs_41.h"
#include "../../SAMPLES/DXengine/036ShadowMapVs_50.h" 
#include "../../SAMPLES/DXengine/036ShadowMapPs_50.h"
#include "../../SAMPLES/DXengine/036ShadowMapVs_51.h" 
#include "../../SAMPLES/DXengine/036ShadowMapPs_51.h"

// RenderWithShadows (render a object, like flor, that will render with shadows)
#include "../../SAMPLES/DXengine/036LightRenderShadowVs_40.h" 
#include "../../SAMPLES/DXengine/036LightRenderShadowPs_40.h"
#include "../../SAMPLES/DXengine/036LightRenderShadowVs_41.h" 
#include "../../SAMPLES/DXengine/036LightRenderShadowPs_41.h"
#include "../../SAMPLES/DXengine/036LightRenderShadowVs_50.h" 
#include "../../SAMPLES/DXengine/036LightRenderShadowPs_50.h"
#include "../../SAMPLES/DXengine/036LightRenderShadowVs_51.h" 
#include "../../SAMPLES/DXengine/036LightRenderShadowPs_51.h"
#endif

#include "../../SAMPLES/DXengine/035TextureLightNormalVs_40.h" 
#include "../../SAMPLES/DXengine/035TextureLightNormalPs_40.h"
#include "../../SAMPLES/DXengine/035TextureLightNormalVs_41.h" 
#include "../../SAMPLES/DXengine/035TextureLightNormalPs_41.h"
#include "../../SAMPLES/DXengine/035TextureLightNormalVs_50.h" 
#include "../../SAMPLES/DXengine/035TextureLightNormalPs_50.h"
#include "../../SAMPLES/DXengine/035TextureLightNormalVs_51.h" 
#include "../../SAMPLES/DXengine/035TextureLightNormalPs_51.h"

#include "../../SAMPLES/DXengine/051LightInstanceVs_40.h" 
#include "../../SAMPLES/DXengine/051LightInstancePs_40.h"
#include "../../SAMPLES/DXengine/051LightInstanceVs_41.h" 
#include "../../SAMPLES/DXengine/051LightInstancePs_41.h"
#include "../../SAMPLES/DXengine/051LightInstanceVs_50.h" 
#include "../../SAMPLES/DXengine/051LightInstancePs_50.h"
#include "../../SAMPLES/DXengine/051LightInstanceVs_51.h" 
#include "../../SAMPLES/DXengine/051LightInstancePs_51.h"

#include "../../SAMPLES/DXengine/051ShadowMapInstanceVs_40.h" 
#include "../../SAMPLES/DXengine/051ShadowMapInstancePs_40.h"
#include "../../SAMPLES/DXengine/051ShadowMapInstanceVs_41.h" 
#include "../../SAMPLES/DXengine/051ShadowMapInstancePs_41.h"
#include "../../SAMPLES/DXengine/051ShadowMapInstanceVs_50.h" 
#include "../../SAMPLES/DXengine/051ShadowMapInstancePs_50.h"
#include "../../SAMPLES/DXengine/051ShadowMapInstanceVs_51.h" 
#include "../../SAMPLES/DXengine/051ShadowMapInstancePs_51.h"

#include "../../SAMPLES/DXengine/051TextureLightNormalInstanceVs_40.h" 
#include "../../SAMPLES/DXengine/051TextureLightNormalInstancePs_40.h"
#include "../../SAMPLES/DXengine/051TextureLightNormalInstanceVs_41.h" 
#include "../../SAMPLES/DXengine/051TextureLightNormalInstancePs_41.h"
#include "../../SAMPLES/DXengine/051TextureLightNormalInstanceVs_50.h" 
#include "../../SAMPLES/DXengine/051TextureLightNormalInstancePs_50.h"
#include "../../SAMPLES/DXengine/051TextureLightNormalInstanceVs_51.h" 
#include "../../SAMPLES/DXengine/051TextureLightNormalInstancePs_51.h"

shaderTree shaderManager_40[] =
{
//#if DX_ENGINE_LEVEL >= 21 // SHADER_COLOR
	/* 00  */{ (void*)VS_Main_40, NULL, (void*)PS_Main_40, sizeof(VS_Main_40), 0, sizeof(PS_Main_40) }
//#endif

	/* 01 */,{ (void*)VS_Main_40, NULL/*(void*)MyGeometryShader022Texture_40*/, (void*)PS_Main_40, sizeof(VS_Main_40), 0 /*sizeof (MyGeometryShader022Texture_40)*/, sizeof(PS_Main_40) }

	/* 02  */,{ (void*)VS_Main_40, NULL,(void*)PS_Main_40, sizeof(VS_Main_40),0,sizeof(PS_Main_40) }

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER// SHADER_TEXTURE_LIGHT_RENDERSHADOW
	/* 03  */,{ (void*)VS_Main_40,NULL,(void*)MyPixelShader045LightRenderShadow_40, sizeof(VS_Main_40),0,sizeof(MyPixelShader045LightRenderShadow_40) }
#endif
#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER // SHADER_TEXTURE_LIGHT_SAVESHADOW
	/* 04  */,{ (void*)VS_Main_40,NULL,(void*)MyPixelShader045ShadowMap_40, sizeof(VS_Main_40),0,sizeof(MyPixelShader045ShadowMap_40) }
#endif

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_40,NULL,(void*)MyPixelShader047TextureLightNormal_40, sizeof(MyVertexShader047TextureLightNormal_40),0,sizeof(MyPixelShader047TextureLightNormal_40) }

	/* 06 */,{ (void*)MyVertexShader051LightInstance_40,NULL,(void*)MyPixelShader051LightInstance_40, sizeof(MyVertexShader051LightInstance_40),0,sizeof(MyPixelShader051LightInstance_40) }

	/* 07 */,{ (void*)MyVertexShader051ShadowMapInstance_40,NULL,(void*)MyPixelShader051ShadowMapInstance_40, sizeof(MyVertexShader051ShadowMapInstance_40),0,sizeof(MyPixelShader051ShadowMapInstance_40) }

};

shaderTree shaderManager_41[] =
{
	/* 00 */{ (void*)VS_Main_41, NULL, (void*)PS_Main_41, sizeof(VS_Main_41), 0, sizeof(PS_Main_41) }

	/* 01 */,{ (void*)VS_Main_41, NULL /*(void*)MyGeometryShader022Texture_41*/, (void*)PS_Main_41, sizeof(VS_Main_41), 0 /*sizeof (MyGeometryShader022Texture_41)*/, sizeof(PS_Main_41) }

	/* 02 */,{ (void*)VS_Main_41,NULL,(void*)PS_Main_41, sizeof(VS_Main_41),0,sizeof(PS_Main_41) }

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER // ShadowMap
	/* 03  */,{ (void*)VS_Main_41,NULL,(void*)MyPixelShader045LightRenderShadow_41, sizeof(VS_Main_41),0,sizeof(MyPixelShader045LightRenderShadow_41) }
	/* 04 */,{ (void*)VS_Main_41,NULL,(void*)MyPixelShader045ShadowMap_41, sizeof(VS_Main_41),0,sizeof(MyPixelShader045ShadowMap_41) }
#endif

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_41,NULL,(void*)MyPixelShader047TextureLightNormal_41, sizeof(MyVertexShader047TextureLightNormal_41),0,sizeof(MyPixelShader047TextureLightNormal_41) }

	/* 06 */,{ (void*)MyVertexShader051LightInstance_41,NULL,(void*)MyPixelShader051LightInstance_41, sizeof(MyVertexShader051LightInstance_41),0,sizeof(MyPixelShader051LightInstance_41) }

#if DX_ENGINE_LEVEL >= 40  ShadowMap + Instancing 
	/* 07 */,{ (void*)MyVertexShader051ShadowMapInstance_41,NULL,(void*)MyPixelShader051ShadowMapInstance_41, sizeof(MyVertexShader051ShadowMapInstance_41),0,sizeof(MyPixelShader051ShadowMapInstance_41) }
#endif

	/* 08 */,{ (void*)MyVertexShader051TextureLightNormalInstance_41,NULL,(void*)MyPixelShader051TextureLightNormalInstance_41, sizeof(MyVertexShader051TextureLightNormalInstance_41),0,sizeof(MyPixelShader051TextureLightNormalInstance_41) }

};

shaderTree shaderManager_50[] =
{
	/* 00 */{ (void*)VS_Main_50, NULL, (void*)PS_Main_50, sizeof(VS_Main_50), 0, sizeof(PS_Main_50) }

	/* 01 */,{ (void*)VS_Main_50, NULL /*	(void*)MyGeometryShader022Texture_50*/, (void*)PS_Main_50, sizeof(VS_Main_50), 0/*sizeof (MyGeometryShader022Texture_50)*/, sizeof(PS_Main_50) }

	/* 02 */,{ (void*)VS_Main_50,NULL,(void*)PS_Main_50, sizeof(VS_Main_50),0,sizeof(PS_Main_50) }

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER // ShadowMap
	/* 03  */,{ (void*)VS_Main_50,NULL,(void*)MyPixelShader045LightRenderShadow_50, sizeof(VS_Main_50),0,sizeof(MyPixelShader045LightRenderShadow_50) }
	/* 04 */,{ (void*)VS_Main_50,NULL,(void*)MyPixelShader045ShadowMap_50, sizeof(VS_Main_50),0,sizeof(MyPixelShader045ShadowMap_50) }
#endif

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_50,NULL,(void*)MyPixelShader047TextureLightNormal_50, sizeof(MyVertexShader047TextureLightNormal_50),0,sizeof(MyPixelShader047TextureLightNormal_50) }

	/* 06 */,{ (void*)MyVertexShader051LightInstance_50,NULL,(void*)MyPixelShader051LightInstance_50, sizeof(MyVertexShader051LightInstance_50),0,sizeof(MyPixelShader051LightInstance_50) }

	/* 07 */,{ (void*)MyVertexShader051ShadowMapInstance_50,NULL,(void*)MyPixelShader051ShadowMapInstance_50, sizeof(MyVertexShader051ShadowMapInstance_50),0,sizeof(MyPixelShader051ShadowMapInstance_50) }

	/* 08 */,{ (void*)MyVertexShader051TextureLightNormalInstance_50,NULL,(void*)MyPixelShader051TextureLightNormalInstance_50, sizeof(MyVertexShader051TextureLightNormalInstance_50),0,sizeof(MyPixelShader051TextureLightNormalInstance_50) }

};


shaderTree shaderManager_51[] =
{
	/* 00 */{ (void*)VS_Main_51, NULL, (void*)PS_Main_51, sizeof(VS_Main_51), 0, sizeof(PS_Main_51) }

	/* 01 */,{ (void*)VS_Main_51,	NULL /*(void*)MyGeometryShader022Texture_51*/, (void*)PS_Main_51, sizeof(VS_Main_51), 0 /*sizeof(MyGeometryShader022Texture_51)*/, sizeof(PS_Main_51) }

	/* 02 */,{ (void*)VS_Main_51,NULL,(void*)PS_Main_51, sizeof(VS_Main_51),0,sizeof(PS_Main_51) }

#if DX_ENGINE_LEVEL >= 36 && defined USE_SHADOW_MAP && defined USE_SCENE_MANAGER // ShadowMap
	/* 03 */,{ (void*)VS_Main_51,NULL,(void*)MyPixelShader045LightRenderShadow_51, sizeof(VS_Main_51),0,sizeof(MyPixelShader045LightRenderShadow_51) }
	/* 04 */,{ (void*)VS_Main_51,NULL,(void*)MyPixelShader045ShadowMap_51, sizeof(VS_Main_51),0,sizeof(MyPixelShader045ShadowMap_51) }
#endif

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_51,NULL,(void*)MyPixelShader047TextureLightNormal_51, sizeof(MyVertexShader047TextureLightNormal_51),0,sizeof(MyPixelShader047TextureLightNormal_51) }

	/* 06 */,{ (void*)MyVertexShader051LightInstance_51,NULL,(void*)MyPixelShader051LightInstance_51, sizeof(MyVertexShader051LightInstance_51),0,sizeof(MyPixelShader051LightInstance_51) }

	/* 07 */,{ (void*)MyVertexShader051ShadowMapInstance_51,NULL,(void*)MyPixelShader051ShadowMapInstance_51, sizeof(MyVertexShader051ShadowMapInstance_51),0,sizeof(MyPixelShader051ShadowMapInstance_51) }

	/* 08 */,{ (void*)MyVertexShader051TextureLightNormalInstance_51,NULL,(void*)MyPixelShader051TextureLightNormalInstance_51, sizeof(MyVertexShader051TextureLightNormalInstance_51),0,sizeof(MyPixelShader051TextureLightNormalInstance_51) }

};


#endif

#endif
