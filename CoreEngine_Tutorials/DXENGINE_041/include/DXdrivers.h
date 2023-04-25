// ----------------------------------------------------------------------------------------------
// Filename: DXdrivers.h
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
// PURPOSE: MAIN PURPOSE: Work as a DirectX Driver API/Interface
// ----------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#pragma once

#if (defined DX_ENGINE)

		//#include "DX11class.h"
		// -------------------------------------------------------------------------------------------
		// Use OLD xnamath from DirectX SDK June2010 or Windows Kit 8?
		// -------------------------------------------------------------------------------------------
		
			#pragma warning( disable : 4005 )		// Disable warning C4005: '' : macro redefinition
			#include <d3d11.h>

			#if D3D11_SPEC_DATE_YEAR == 2009
			#include <d3dx11async.h>
			#endif

		#if D3D11_SPEC_DATE_YEAR == 2009		// Use the OLD DirectX_SDK_June2010 ?
			#pragma warning( disable : 4324 )	// 4324: '': structure was padded due to __declspec(align())
			#include <DirectXMath.h> //#include <xnamath.h>				// #include <d3dx10math.h>
		#else
			#include <DirectXMath.h>			// Use the NEW DirectX11
			using namespace DirectX;
		#endif
		
	#if defined DX9sdk
		#include "DX9Class.h"
	#endif

#endif

