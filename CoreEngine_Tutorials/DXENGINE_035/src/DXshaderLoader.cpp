// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DXshaderLoader.cpp
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
// PURPOSE: Inject HLSL code on main EXE file
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "platform.h"

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

#include "../../SAMPLES/DXengine/047TextureLightNormalVs_40.h" 
#include "../../SAMPLES/DXengine/047TextureLightNormalPs_40.h"
#include "../../SAMPLES/DXengine/047TextureLightNormalVs_41.h" 
#include "../../SAMPLES/DXengine/047TextureLightNormalPs_41.h"
#include "../../SAMPLES/DXengine/047TextureLightNormalVs_50.h" 
#include "../../SAMPLES/DXengine/047TextureLightNormalPs_50.h"
#include "../../SAMPLES/DXengine/047TextureLightNormalVs_51.h" 
#include "../../SAMPLES/DXengine/047TextureLightNormalPs_51.h"

shaderTree shaderManager_40[] =
{
//#if DX_ENGINE_LEVEL >= 21 // SHADER_COLOR
	/* 00  */{ (void*)MyVertexShader021Color_40, NULL, (void*)MyPixelShader021Color_40, sizeof(MyVertexShader021Color_40), 0, sizeof(MyPixelShader021Color_40) }
//#endif

	/* 01 */,{ (void*)MyVertexShader022Texture_40, NULL/*(void*)MyGeometryShader022Texture_40*/, (void*)MyPixelShader022Texture_40, sizeof(MyVertexShader022Texture_40), 0 /*sizeof (MyGeometryShader022Texture_40)*/, sizeof(MyPixelShader022Texture_40) }

	/* 02  */,{ (void*)MyVertexShader023Light_40, NULL,(void*)MyPixelShader023Light_40, sizeof(MyVertexShader023Light_40),0,sizeof(MyPixelShader023Light_40) }

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_40,NULL,(void*)MyPixelShader047TextureLightNormal_40, sizeof(MyVertexShader047TextureLightNormal_40),0,sizeof(MyPixelShader047TextureLightNormal_40) }

};

shaderTree shaderManager_41[] =
{
	/* 00 */{ (void*)MyVertexShader021Color_41, NULL, (void*)MyPixelShader021Color_41, sizeof(MyVertexShader021Color_41), 0, sizeof(MyPixelShader021Color_41) }

	/* 01 */,{ (void*)MyVertexShader022Texture_41, NULL /*(void*)MyGeometryShader022Texture_41*/, (void*)MyPixelShader022Texture_41, sizeof(MyVertexShader022Texture_41), 0 /*sizeof (MyGeometryShader022Texture_41)*/, sizeof(MyPixelShader022Texture_41) }

	/* 02 */,{ (void*)MyVertexShader023Light_41,NULL,(void*)MyPixelShader023Light_41, sizeof(MyVertexShader023Light_41),0,sizeof(MyPixelShader023Light_41) }

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_41,NULL,(void*)MyPixelShader047TextureLightNormal_41, sizeof(MyVertexShader047TextureLightNormal_41),0,sizeof(MyPixelShader047TextureLightNormal_41) }

};

shaderTree shaderManager_50[] =
{
	/* 00 */{ (void*)MyVertexShader021Color_50, NULL, (void*)MyPixelShader021Color_50, sizeof(MyVertexShader021Color_50), 0, sizeof(MyPixelShader021Color_50) }

	/* 01 */,{ (void*)MyVertexShader022Texture_50, NULL /*	(void*)MyGeometryShader022Texture_50*/, (void*)MyPixelShader022Texture_50, sizeof(MyVertexShader022Texture_50), 0/*sizeof (MyGeometryShader022Texture_50)*/, sizeof(MyPixelShader022Texture_50) }

	/* 02 */,{ (void*)MyVertexShader023Light_50,NULL,(void*)MyPixelShader023Light_50, sizeof(MyVertexShader023Light_50),0,sizeof(MyPixelShader023Light_50) }

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_50,NULL,(void*)MyPixelShader047TextureLightNormal_50, sizeof(MyVertexShader047TextureLightNormal_50),0,sizeof(MyPixelShader047TextureLightNormal_50) }

};


shaderTree shaderManager_51[] =
{
	/* 00 */{ (void*)MyVertexShader021Color_51, NULL, (void*)MyPixelShader021Color_51, sizeof(MyVertexShader021Color_51), 0, sizeof(MyPixelShader021Color_51) }

	/* 01 */,{ (void*)MyVertexShader022Texture_51,	NULL /*(void*)MyGeometryShader022Texture_51*/, (void*)MyPixelShader022Texture_51, sizeof(MyVertexShader022Texture_51), 0 /*sizeof(MyGeometryShader022Texture_51)*/, sizeof(MyPixelShader022Texture_51) }

	/* 02 */,{ (void*)MyVertexShader023Light_51,NULL,(void*)MyPixelShader023Light_51, sizeof(MyVertexShader023Light_51),0,sizeof(MyPixelShader023Light_51) }

	/* 05 */,{ (void*)MyVertexShader047TextureLightNormal_51,NULL,(void*)MyPixelShader047TextureLightNormal_51, sizeof(MyVertexShader047TextureLightNormal_51),0,sizeof(MyPixelShader047TextureLightNormal_51) }

};


#endif

