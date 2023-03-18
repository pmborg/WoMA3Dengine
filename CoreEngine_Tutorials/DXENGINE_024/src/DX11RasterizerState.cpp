// NOTE!: This code was automatically generated/extracted by WOMA3DENGINE
#include "platform.h"

#include "dx11Class.h"

namespace DirectX {
//Init Step: 4
// ----------------------------------------------------------------------------------------------
bool DX11Class::createRasterizerStates(bool lineAntialiasing)
// ----------------------------------------------------------------------------------------------
{
	//USE:
	//SetRasterizerState(UINT cullMode, UINT fillMode)

    HRESULT result;
	D3D11_RASTERIZER_DESC rasterDesc;	// Note: = {0} don't work here
    ZeroMemory(&rasterDesc, sizeof(rasterDesc));

    // Create and Set the Rasterizer State:
	//
    // This will give us control over how polygons are rendered. 
    // We can do things like make our scenes render in wireframe mode or have DirectX draw both the front and back faces of polygons. 

    // (Setup the raster description which will determine how and what polygons will be drawn)
    // --------------------------------------------------------------------------------------------
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.DepthBias = 0;					//to be used shadows
	rasterDesc.DepthBiasClamp = 0;				//to be used shadows
	rasterDesc.SlopeScaledDepthBias = 0;		//to be used shadows

	rasterDesc.DepthClipEnable = true; // Enable clipping based on distance: http://technet.microsoft.com/de-de/subscriptions/ff476198%28v=vs.85%29.aspx
	rasterDesc.ScissorEnable = ScissorEnable;
	
    rasterDesc.MultisampleEnable = (ReqMSAAmultiSampleCount > 1) ? true : false;	// Enable/Disable multisample antialiasing.
	rasterDesc.AntialiasedLineEnable = (ReqMSAAmultiSampleCount > 1) ? false : true;
	//rasterDesc.AntialiasedLineEnable = lineAntialiasing;							// Enable line antialiasing; only applies if doing line drawing and MultisampleEnable is false.

    // --------------------------------------------------------------------------------------------
    // FRONT - Create the rasterizer state from the description we just filled out:
    // --------------------------------------------------------------------------------------------
    rasterDesc.CullMode = D3D11_CULL_FRONT;
    rasterDesc.FillMode = D3D11_FILL_SOLID;		//SOLID
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_FRONT][FILL_SOLID]);
    IF_FAILED_RETURN_FALSE(result);

    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;	//WIREFRAME
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_FRONT][FILL_WIRE]);
    IF_FAILED_RETURN_FALSE(result);

	// --------------------------------------------------------------------------------------------
    // BACK - Create the rasterizer state from the description we just filled out:
    // --------------------------------------------------------------------------------------------
    rasterDesc.CullMode = D3D11_CULL_BACK;	//Do not draw triangles that are back-facing.
    rasterDesc.FillMode = D3D11_FILL_SOLID;		 //SOLID
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_BACK][FILL_SOLID]);
    IF_FAILED_RETURN_FALSE(result);

    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;	//WIREFRAME
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_BACK][FILL_WIRE]);
    IF_FAILED_RETURN_FALSE(result);

	// --------------------------------------------------------------------------------------------
    // NONE - Create the rasterizer state from the description we just filled out:
    // --------------------------------------------------------------------------------------------
	rasterDesc.CullMode = D3D11_CULL_NONE;
    rasterDesc.FillMode = D3D11_FILL_SOLID;
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_NONE][FILL_SOLID]);
    IF_FAILED_RETURN_FALSE(result);

    rasterDesc.FillMode = D3D11_FILL_WIREFRAME;
    result = m_device->CreateRasterizerState(&rasterDesc, &m_rasterState[CULL_NONE][FILL_WIRE]);
    IF_FAILED_RETURN_FALSE(result);

    return true;
}


// ----------------------------------------------------------------------------------------------
void DX11Class::SetRasterizerState(UINT CullMode, UINT fillMode) 
// ----------------------------------------------------------------------------------------------
{
	UINT rasterState = CullMode * 10 + fillMode;
	if (rasterState != mCurRasterState)
	{
		g_deviceContext->RSSetState(m_rasterState[CullMode][fillMode]);
		mCurRasterState = rasterState;
	}
}

}
