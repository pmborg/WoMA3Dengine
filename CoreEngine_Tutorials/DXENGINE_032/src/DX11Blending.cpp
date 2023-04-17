// --------------------------------------------------------------------------------------------
// Filename: DX11Blending.cpp
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
// PURPOSE: 
// --------------------------------------------------------------------------------------------
//WomaIntegrityCheck = 1234567831;

#include "platform.h"

#include "dx11Class.h"

namespace DirectX {

#if defined INTRO_DEMO || defined USE_ALPHA_BLENDING // 26
//10 The Initialize function is what does the entire setup of Direct3D for DirectX 11.
// ----------------------------------------------------------------------------------------------
bool DX11Class::CreateBlendState()
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//DX9Class::TurnOnAlphaBlending()
	//DX9Class::TurnOffAlphaBlending()

	HRESULT result;

	//We have a new description variable for setting up the two new blend states.
	D3D11_BLEND_DESC blendStateDescription;

	//First initialize the blend state description.

	// Clear the blend state description.
	ZeroMemory(&blendStateDescription, sizeof(D3D11_BLEND_DESC));

/*
	To create an alpha enabled blend state description change BlendEnable to TRUE and DestBlend to D3D11_BLEND_INV_SRC_ALPHA. 
	The other settings are set to their default values which can be looked up in the Windows DirectX Graphics Documentation.
	
	Final color =
					(src pixel color * src pixel blend factor)
						some blending operator (+/ -/*)
					(  dest pixel color *  dest pixel blend factor )

	https://takinginitiative.wordpress.com/2010/04/09/directx-10-tutorial-6-transparency-and-alpha-blending/
*/

	// Create an alpha enabled blend state description.
    blendStateDescription.RenderTarget[0].BlendEnable = TRUE;

    blendStateDescription.RenderTarget[0].SrcBlend =	D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_SRC_COLOR; // 
    blendStateDescription.RenderTarget[0].DestBlend =	D3D11_BLEND_INV_SRC_ALPHA; //
    blendStateDescription.RenderTarget[0].BlendOp =		D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;// D3D11_BLEND_ONE;
    blendStateDescription.RenderTarget[0].DestBlendAlpha =	D3D11_BLEND_ZERO;
    blendStateDescription.RenderTarget[0].BlendOpAlpha =	D3D11_BLEND_OP_ADD;
    blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D10_COLOR_WRITE_ENABLE_ALL;

	/*
	blendStateDescription.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; //D3D11_BLEND_SRC_COLOR; // 
	blendStateDescription.RenderTarget[0].DestBlend =  D3D11_BLEND_BLEND_FACTOR; // D3D11_BLEND_INV_SRC_ALPHA;
	blendStateDescription.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blendStateDescription.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDescription.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = 0x0f; //D3D10_COLOR_WRITE_ENABLE_ALL; // 
	*/
	//We then create an alpha enabled blending state using the description we just setup.

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaEnableBlendingState);
	if(FAILED(result)){WomaFatalExceptionW(TEXT("Error: CreateBlendState"));return false;}

	//Now to create an alpha disabled state we change the description we just made to set BlendEnable to FALSE. 
	//The rest of the settings can be left as they are.

	// Modify the description to create an alpha disabled blend state description.
	blendStateDescription.RenderTarget[0].BlendEnable = FALSE;
	blendStateDescription.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	blendStateDescription.AlphaToCoverageEnable = false;////NEW!!!

	//We then create an alpha disabled blending state using the modified blend state description. 
	//We now have two blending states we can switch between to turn on and off alpha blending.

	// Create the blend state using the description.
	result = m_device->CreateBlendState(&blendStateDescription, &m_alphaDisableBlendingState);
	if(FAILED(result)){WomaFatalExceptionW(TEXT("Error: CreateBlendState"));return false;}

	return true;
}
#endif


//The first new function TurnOnAlphaBlending allows us to turn on alpha blending by using the OMSetBlendState function with our 
//m_alphaEnableBlendingState blending state.

// ----------------------------------------------------------------------------------------------
void DX11Class::TurnOnAlphaBlending()
// ----------------------------------------------------------------------------------------------
{
	if (g_AlphaBlend) return;
	// Turn on the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaEnableBlendingState, blendFactor, 0xffffffff);
	g_AlphaBlend = true;
}

//The second new function TurnOffAlphaBlending allows us to turn off alpha blending by using the OMSetBlendState function with 
//our m_alphaDisableBlendingState blending state.
// ----------------------------------------------------------------------------------------------
void DX11Class::TurnOffAlphaBlending()
// ----------------------------------------------------------------------------------------------
{
	if (!g_AlphaBlend) return;
	// Turn off the alpha blending.
	m_deviceContext->OMSetBlendState(m_alphaDisableBlendingState, blendFactor, 0xffffffff);
	g_AlphaBlend = false;
}

}
