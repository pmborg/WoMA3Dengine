// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
// --------------------------------------------------------------------------------------------
// Filename: DX11ViewPort-and-Scissor.cpp
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
//WomaIntegrityCheck = 1234567891;

#include "platform.h"
#include <d3d11.h>

#include "dx11Class.h"

namespace DirectX {

// REVIEW - Init Step: 5 - Rasterizer State: Set the Viewport for rendering
// MORE INFO: https://msdn.microsoft.com/en-us/library/windows/desktop/bb205126%28v=vs.85%29.aspx
// ----------------------------------------------------------------------------------------------
void DX11Class::setViewportDevice(UINT monitorWindow, int screenWidth, int screenHeight)
// ----------------------------------------------------------------------------------------------
{
	// The viewport also needs to be setup so that Direct3D can map clip space coordinates to the render target space. 
	// Set this to be the entire size of the window. 
	DX11windowsArray[monitorWindow].viewport.TopLeftX = 0.0f;	// MinX
	DX11windowsArray[monitorWindow].viewport.TopLeftY = 0.0f;	// MinY
	DX11windowsArray[monitorWindow].viewport.Width = (float)screenWidth;
	DX11windowsArray[monitorWindow].viewport.Height = (float)screenHeight;

	DX11windowsArray[monitorWindow].viewport.MinDepth = 0.0f;	// MinZ
	DX11windowsArray[monitorWindow].viewport.MaxDepth = 1.0f;	// MaxZ

	// Create the viewport:
	ASSERT (DX11windowsArray[monitorWindow].viewport.Width > 0 && 
			DX11windowsArray[monitorWindow].viewport.Height > 0);
	m_deviceContext->RSSetViewports(1, &DX11windowsArray[monitorWindow].viewport);

	// Just to Test it: on a fullScreen
	//setScissorRectangle(0, 600, 0, 500, true);
}

void DX11Class::setScissorRectangle(UINT left, UINT right, UINT top, UINT bottom, bool enabled)
{
	ScissorEnable = enabled;

	D3D11_RECT ScissorRect[1];
	ScissorRect[0].left = left;
	ScissorRect[0].right = right;
	ScissorRect[0].top = top;
	ScissorRect[0].bottom = bottom;
	m_deviceContext->RSSetScissorRects(1, ScissorRect);
}
}

