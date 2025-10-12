// --------------------------------------------------------------------------------------------
// Filename: Dx11DriverFonts.cpp
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
//WomaIntegrityCheck = 1234525217;

#include "OSengine.h"
#if defined DX11 //&& ENGINE_LEVEL >= 20		// Initializing Engine && defined DX11
#include "mem_leak.h"
#include "dx11Class.h"
#include "winsystemclass.h"	// SystemHandle

#if defined USE_DX_DRIVER_FONT
extern std::vector<DXTextLine> allTextArray;
#endif

namespace DirectX {
#if defined USE_DX_DRIVER_FONT // FONT v2

// invoked@: DX11Class::Resize
#if _NOT
// https://learn.microsoft.com/en-us/windows/win32/directwrite/getting-started-with-directwrite
#endif
bool DX11Class::InitD2D_D3D101_DWrite(IDXGIAdapter1 *Adapter, int screenWidth, int screenHeight, WCHAR* fontStyle, float FontSizeIndex, float R, float G, float B)
{
	//Create our Direc3D 10.1 Device///////////////////////////////////////////////////////////////////////////////////////
	HRESULT hr;

	D3D10_FEATURE_LEVEL1 featureLevelsDX10 [] =	// NOTE: For this device, just to render the fonts, any level is possible:
												{	
													D3D10_FEATURE_LEVEL_10_1, D3D10_FEATURE_LEVEL_10_0,
												#if defined DX11_ALLOW_DX9
													D3D10_FEATURE_LEVEL_9_3, D3D10_FEATURE_LEVEL_9_2, D3D10_FEATURE_LEVEL_9_1
												#endif
												};

	unsigned int totalFeature10Levels = ARRAYSIZE( featureLevelsDX10 );

	for (UINT i = 0; i < totalFeature10Levels; i++)
	{
		hr = D3D10CreateDevice1(Adapter, 
								D3D10_DRIVER_TYPE_HARDWARE,
								NULL, 
								D3D10_CREATE_DEVICE_BGRA_SUPPORT,
								featureLevelsDX10[i], 
								D3D10_1_SDK_VERSION, 
								&d3d101Device);
		if (hr == S_OK)
			break;
	}

	IF_FAILED_RETURN_FALSE (hr);

	//Create Shared Texture that Direct3D 10.1 will render on//////////////////////////////////////////////////////////////
	D3D11_TEXTURE2D_DESC sharedTexDesc = {0};					//ZeroMemory(&sharedTexDesc, sizeof(sharedTexDesc));

	sharedTexDesc.Width = screenWidth;
	sharedTexDesc.Height = screenHeight;	
	sharedTexDesc.Format = BUFFER_COLOR_FORMAT;					//DXGI_FORMAT_B8G8R8A8_UNORM;
	sharedTexDesc.MipLevels = 1;	
	sharedTexDesc.ArraySize = 1;
	// Don't Use MSAA Here!
	sharedTexDesc.SampleDesc.Count = 1;		// MSAA_COUNT;		//Anti-Aliasing: MultiSample tech.
	sharedTexDesc.SampleDesc.Quality = 0;	// MSAA_QUALITY;	//Anti-Aliasing: Texture Filtering tech. (MSAA > 0)
	sharedTexDesc.Usage = D3D11_USAGE_DEFAULT;
	sharedTexDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;	
	sharedTexDesc.MiscFlags = D3D11_RESOURCE_MISC_SHARED_KEYEDMUTEX;	

	IF_FAILED_RETURN_FALSE (hr = m_device11->CreateTexture2D(&sharedTexDesc, NULL, &sharedTex11));

	// Get the keyed mutex for the shared texture (for D3D11)///////////////////////////////////////////////////////////////
	IF_FAILED_RETURN_FALSE (hr = sharedTex11->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&SystemHandle->m_Application->keyedMutex11));

	// Get the "Shared Handle" needed to open the shared texture in D3D10.1///////////////////////////////////////////////////
	HANDLE sharedHandle10;

	IDXGIResource *sharedResource10;
	IF_FAILED_RETURN_FALSE (hr = sharedTex11->QueryInterface(__uuidof(IDXGIResource), (void**)&sharedResource10));
	IF_FAILED_RETURN_FALSE (hr = sharedResource10->GetSharedHandle(&sharedHandle10));
	sharedResource10->Release();

	// "Open the Shared Surface" for the shared texture in D3D10.1///////////////////////////////////////////////////////////////////
	IDXGISurface1 *sharedSurface10;	

	IF_FAILED_RETURN_FALSE (hr = d3d101Device->OpenSharedResource(sharedHandle10, __uuidof(IDXGISurface1), (void**)(&sharedSurface10)));
	IF_FAILED_RETURN_FALSE (hr = sharedSurface10->QueryInterface(__uuidof(IDXGIKeyedMutex), (void**)&SystemHandle->m_Application->keyedMutex10));

	// Create D2D factory///////////////////////////////////////////////////////////////////////////////////////////////////
	ID2D1Factory *D2DFactory;	
	IF_FAILED_RETURN_FALSE (hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory), (void**)&D2DFactory));	

	D2D1_RENDER_TARGET_PROPERTIES renderTargetProperties;
	ZeroMemory(&renderTargetProperties, sizeof(renderTargetProperties));

	renderTargetProperties.type = D2D1_RENDER_TARGET_TYPE_HARDWARE;
	renderTargetProperties.pixelFormat = D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED);	

	IF_FAILED_RETURN_FALSE (hr = D2DFactory->CreateDxgiSurfaceRenderTarget(sharedSurface10, &renderTargetProperties, &D2DRenderTarget));

	sharedSurface10->Release();
	D2DFactory->Release();	

	// Create a solid color brush to draw something with:
	IF_FAILED_RETURN_FALSE (hr = D2DRenderTarget->CreateSolidColorBrush(D2D1::ColorF(R, G, B, 1.0f), &Brush));

	 // MAX(22, 22.0f * MIN(1, 2 * SystemHandle->m_scaleX)); // At least 8, Max: 24 (with this formula Text Scale Better)

	// Create new font: (DirectWrite)
	IF_FAILED_RETURN_FALSE(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), reinterpret_cast<IUnknown**>(&DWriteFactory)));

	// Use: Text Font: "Consolas"
	IF_FAILED_RETURN_FALSE(hr = DWriteFactory->CreateTextFormat(fontStyle, NULL, DWRITE_FONT_WEIGHT_REGULAR, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, FontSizeIndex, L"en-us", &TextFormat));
	IF_FAILED_RETURN_FALSE(hr = TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_LEADING));
	IF_FAILED_RETURN_FALSE(hr = TextFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR));

	d3d101Device->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_POINTLIST);

	return InitD2DScreenTexture();
}

bool DX11Class::InitD2DScreenTexture()
{
	HRESULT hr;

	// DEFINE: vertex buffer -------------------------------------------------------------
	DXtextureVertexType v[] =
	{
		// Front Face
#if DX_ENGINE_LEVEL >= 99 && defined USE_WOMA_ENGINE_ONE_CBUFFER
		DXtextureVertexType(-1.0f, -1.0f, -1.0f, 0, 0.0f, 1.0f),
		DXtextureVertexType(-1.0f,  1.0f, -1.0f, 0, 0.0f, 0.0f),
		DXtextureVertexType( 1.0f,  1.0f, -1.0f, 0, 1.0f, 0.0f),
		DXtextureVertexType( 1.0f, -1.0f, -1.0f, 0, 1.0f, 1.0f),
#else
		DXtextureVertexType(-1.0f, -1.0f, -1.0f, 0.0f, 1.0f),
		DXtextureVertexType(-1.0f,  1.0f, -1.0f, 0.0f, 0.0f),
		DXtextureVertexType( 1.0f,  1.0f, -1.0f, 1.0f, 0.0f),
		DXtextureVertexType( 1.0f, -1.0f, -1.0f, 1.0f, 1.0f),
#endif
	};

	// DEFINE: index buffer -------------------------------------------------------------
	DWORD indices[] = {
		// Front Face
		0,  1,  2, 0,  2,  3,
	};

	// CREATE: vertex buffer -------------------------------------------------------------
	D3D11_BUFFER_DESC indexBufferDesc = {0}; //ZeroMemory( &indexBufferDesc, sizeof(indexBufferDesc) );

	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(DWORD) * _countof(indices);
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	//indexBufferDesc.CPUAccessFlags = 0;
	//indexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;

	iinitData.pSysMem = indices;
	IF_FAILED_RETURN_FALSE (hr = m_device11->CreateBuffer(&indexBufferDesc, &iinitData, &d2dIndexBuffer));

	// CREATE: index buffer -------------------------------------------------------------
	D3D11_BUFFER_DESC vertexBufferDesc = {0}; //ZeroMemory( &vertexBufferDesc, sizeof(vertexBufferDesc) );

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof( DXtextureLightVertexType ) * _countof(v);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	//vertexBufferDesc.CPUAccessFlags = 0;
	//vertexBufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData; 

	ZeroMemory( &vertexBufferData, sizeof(vertexBufferData) );
	vertexBufferData.pSysMem = v;
	IF_FAILED_RETURN_FALSE (hr = m_device11->CreateBuffer( &vertexBufferDesc, &vertexBufferData, &d2dVertBuffer));

	// CREATE Texture -------------------------------------------------------------
	//Create A shader resource view from the texture D2D will render to,
	//So we can use it to texture a square which overlays our scene

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = 1;
	IF_FAILED_RETURN_FALSE (hr = m_device11->CreateShaderResourceView(sharedTex11, &srvd, &d2dTexture));

	// CREATE BlendState: "Color Blending" Transparency -------------------------------------------------------------
	D3D11_BLEND_DESC blendDesc = {0};			//ZeroMemory( &blendDesc, sizeof(blendDesc) );
	D3D11_RENDER_TARGET_BLEND_DESC rtbd = {0};	//ZeroMemory( &rtbd, sizeof(rtbd) );

	/*
	//v1:
	rtbd.BlendEnable			 = true;
	rtbd.SrcBlend				 = D3D11_BLEND_SRC_COLOR;
	rtbd.DestBlend				 = D3D11_BLEND_INV_SRC_ALPHA;
	rtbd.BlendOp				 = D3D11_BLEND_OP_ADD;
	rtbd.SrcBlendAlpha			 = D3D11_BLEND_ONE;
	rtbd.DestBlendAlpha			 = D3D11_BLEND_ZERO;
	rtbd.BlendOpAlpha			 = D3D11_BLEND_OP_ADD;
	rtbd.RenderTargetWriteMask	 = D3D10_COLOR_WRITE_ENABLE_ALL;

	blendDesc.AlphaToCoverageEnable = false;
	blendDesc.RenderTarget[0] = rtbd;
	*/

	//v2:
	blendDesc.AlphaToCoverageEnable = FALSE;
	auto& rt = blendDesc.RenderTarget[0];
	rt.BlendEnable = TRUE;
	rt.RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	// Premultiplied: Src = ONE, Dest = 1 - SrcA
	rt.SrcBlend = D3D11_BLEND_ONE;
	rt.DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	rt.BlendOp = D3D11_BLEND_OP_ADD;
	rt.SrcBlendAlpha = D3D11_BLEND_ONE;
	rt.DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	rt.BlendOpAlpha = D3D11_BLEND_OP_ADD;
	
	IF_FAILED_RETURN_FALSE(hr = m_device11->CreateBlendState(&blendDesc, &Transparency));

	// CREATE -------------------------------------------------------------
	D3D11_RASTERIZER_DESC cmdesc;

	ZeroMemory(&cmdesc, sizeof(D3D11_RASTERIZER_DESC));
	cmdesc.FillMode = D3D11_FILL_SOLID;
	cmdesc.CullMode = D3D11_CULL_NONE;   // <- safer for a fullscreen quad
	cmdesc.FrontCounterClockwise = false;
	hr = m_device11->CreateRasterizerState(&cmdesc, &CWcullMode);

	// CREATE the color SHADER -------------------------------------------------------------
	SystemHandle->m_Application->m_FontV2Shader = NEW DXshaderClass (ShaderVersionH, ShaderVersionL, false/*2D*/);
	IF_NOT_THROW_EXCEPTION (SystemHandle->m_Application->m_FontV2Shader);

#if DX_ENGINE_LEVEL >= 99 && defined USE_WOMA_ENGINE_ONE_CBUFFER
	SHADER_TYPE shadertype = SHADER_TYPE_TEXTUREFONT25; //99
#else
	SHADER_TYPE shadertype = SHADER_TEXTURE;
#endif
	bool result = SystemHandle->m_Application->m_FontV2Shader->Initialize(NULL, TEXT("m_FontV2Shader"), shadertype, m_device11, SystemHandle->m_hWnd, TRIANGLELIST);

	if(!result)
		{ WomaFatalExceptionW(TEXT("Could not initialize the m_FontV2Shader")); /*return false;*/ }

	return true;
}

void DX11Class::addText(int Xpos, int Ypos, TCHAR* printText, float R, float G, float B)
{
	DXTextLine lineText;
	lineText.Xpos = Xpos;
	lineText.Ypos = Ypos;
	lineText.printText = printText;
	lineText.R = R;
	lineText.G = G;
	lineText.B = B;

	allTextArray.push_back(lineText);
}

// 50,100
void DirectX::DX11Class::RenderDriverText(void* ctx)
{
	ID3D11DeviceContext* pContext = (ID3D11DeviceContext*)ctx;
	//if (!keyedMutex11)	//main window might resize
	//	return;

	if (SystemHandle->m_Application->keyedMutex11)
		SystemHandle->m_Application->keyedMutex11->ReleaseSync(0);		//Release the D3D 11 Device
	else
		return;
	if (SystemHandle->m_Application->keyedMutex10)
		SystemHandle->m_Application->keyedMutex10->AcquireSync(0, 5);	//Use D3D10.1 device
	else
		return;
	D2DRenderTarget->BeginDraw();		//Draw D2D content

	D2DRenderTarget->Clear(D2D1::ColorF(0.0f, 0.0f, 0.0f, 0.0f));					//Clear D2D Background
	D2D1_COLOR_F FontColor = D2D1::ColorF(1.0f, 1.0f, 1.0f, 1.0f);					//Set the Font Color
	Brush->SetColor(FontColor);														//Set the brush color D2D will use to draw with

	for (size_t i = 0; i < allTextArray.size(); i++)
	{
		D2D1_RECT_F layoutRect = D2D1::RectF((float)allTextArray[i].Xpos, (float)allTextArray[i].Ypos, (float)SystemHandle->AppSettings->WINDOW_WIDTH, (float)SystemHandle->AppSettings->WINDOW_HEIGHT);	//Create the D2D Render Area
		#ifndef UNICODE
		WCHAR Wbuffer[MAX_STR_LEN] = { 0 }; MultiByteToWideChar(CP_ACP, 0, allTextArray[i].printText.c_str(), -1, Wbuffer, MAX_STR_LEN);
		D2DRenderTarget->DrawText(Wbuffer, (UINT)allTextArray[i].printText.length(), TextFormat, layoutRect, Brush);	// Draw the Text (Use Pre-Created Text Size)
		#else
		D2DRenderTarget->DrawText(allTextArray[i].printText.c_str(), (UINT)allTextArray[i].printText.length(), TextFormat, layoutRect, Brush);	// Draw the Text (Use Pre-Created Text Size)
		#endif
	}

	D2DRenderTarget->EndDraw();	
	SystemHandle->m_Application->keyedMutex10->ReleaseSync(1);		//Release the D3D10.1 Device
	SystemHandle->m_Application->keyedMutex11->AcquireSync(1, 5);	//Use the D3D11 Device

	// -------------------------------------------------------------
	//Set the blend state for D2D render target texture objects
	//m_deviceContext->OMSetBlendState(Transparency, NULL, 0xffffffff);		
	TurnOnAlphaBlending(pContext);

	//Set the d2d vertex buffer
	static UINT stride = sizeof( DXtextureVertexType );
	static UINT offset = 0;
	pContext->IASetVertexBuffers( 0, 1, &d2dVertBuffer, &stride, &offset );

	//Set the d2d Index buffer
	pContext->IASetIndexBuffer( d2dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	pContext->PSSetShaderResources( 0, 1, &d2dTexture );
	pContext->RSSetState(CWcullMode);

	//Draw TEXT:
	static XMMATRIX World = XMMatrixIdentity();
	SystemHandle->m_Application->m_FontV2Shader->Render(NULL, pContext, 6, &World, &World, &World);

	allTextArray.clear();
	TurnOffAlphaBlending(pContext);
}

#endif
}

#endif