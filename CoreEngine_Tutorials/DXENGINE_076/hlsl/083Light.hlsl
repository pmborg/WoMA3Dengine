// --------------------------------------------------------------------------------------------
// Filename: 083Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525256;

#define DXAPI11             1   //force
#define PS_USE_LIGHT		    //23
//#define PS_USE_FOG              //51

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn						
{
	float3 position : POSITION;	//21
	float2 texCoords: TEXCOORD; //22
	float3 normal	: NORMAL;	//23
};

// PIXEL:
struct PSIn						
{
	float4 position				: SV_POSITION;			// 21
	float2 texCoords			: TEXCOORD;				// 22
	float3 normal				: NORMAL;				// 23 LIGHT
#if defined PS_USE_FOG
    float fogFactor				: FOG;		// 51 FOG
#endif	
};


/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;	// 22: Texture
Texture2D AlfaMapTexture;	// 33: AlfaMap
#endif
#if DXAPI12 == 1
Texture2D AlfaMapTexture:	register(t0); // 33: AlfaMap	//DX12: Descriptor: 2
Texture2D shaderTexture:	register(t1); // 22: Texture	//DX12: Descriptor: 3
#endif

#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

////////////////
// CBUFFERS
////////////////
// --------------------------------------------------------------------------------------------
// Filename: cbuffer.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{
	// BLOCK: VS1
    matrix worldMatrix; //worldMatrix
    matrix view; //view
    matrix projection; //projection
    matrix WV; //worldMatrix+viewMatrix
    matrix WVP; //worldMatrix+viewMatrix+projectionMatrix

	// 23 BLOCK: VS2
    bool VShasLight;
    bool VShasSpecular;
    bool VShasNormMap;
    bool VShasFog;

	// 23 BLOCK: VS3
    float3 VSlightDirection; // LIGHT (XMVECTOR = XMFLOAT4)
    float VSlightPAD; // 3+1=XMFLOAT4
    float4 VSambientColor; // LIGHT: Ka
    float4 VSdiffuseColor; // LIGHT: Kd
    float4 VSemissiveColor; // LIGHT: Ke

	// 31 BLOCK: VS4
    float VSfogStart;
    float VSfogEnd;
    bool VShasShadowMap;
    bool VS_USE_WVP;

	// 45 BLOCK: VS5
    matrix ViewToLightProj;
    matrix WorldInverseTranspose; // WorldInverseTranspose
    float4 vEye; // camera position													   

	// 42 BLOCK: VS6
    float VSrotX;
    float VSrotY;
    float VSrotZ;
    float time;

	// 42 BLOCK: VS7
    float VSshaderType;
    float vsPAD2;
    float vsPAD3;
    float vsPAD4;

    // FIRE:
    float vsframeTime;
    float3 scrollSpeeds;
    float3 scales;
    bool isAnimatedBill;
};

///////////////
// PIXEL BUFFER
///////////////
//Note: on DX11 dont use: register()

// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer PSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer PSShaderParametersBuffer : register(b1)	//Register is needed for DX12: Descriptor: 1
#endif
{
	// BLOCK1:
    //float4 pixelColor;
    //
	//// BLOCK2:
    //bool hasTexture; // No? Use pixelColor, then.
    //bool hasLight; // Future Load Obj. Engine Level
    //bool hasSpecular; // Future Load Obj. Engine Level
    //bool isFont; // Future Load Obj. Engine Level

	// BLOCK3:
    //float4 ambientColor; // LIGHT: Ka
    //float4 diffuseColor; // LIGHT: Kd
    //float4 emissiveColor; // LIGHT: Ke 
    //float4 lightDirection; // LIGHT

	//// BLOCK4:
    //bool hasColorMap; // 66
    //float lightType; // Future
    //float PSshaderType; // Future
    //float shaderTypeParameter; // Future
    //
	//// BLOCK5:
    //bool hasAlfaColor;
    //float alfaColor;
    //float fade; // Fade from 0 to 1
    //float frameTime; // For animations
    //
	//// BLOCK6:
    //bool hasFog;
    //bool isSky;
    //bool hasAlfaMap;
    //bool hasNormMap;
    //
	//// BLOCK7:
    //float3 cameraPosition; // Future
    //bool castShadow;
    //float3 specularColor;
    //float nShininess;
    //
    //// FIRE:
    //float2 distortion1;
    //float2 distortion2;
    //float2 distortion3;
    //float distortionScale;
    //float distortionBias;
};

//#include "light.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader083Light(VSIn input)
{
	PSIn output;
	float4 cameraPosition;
   
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices

    if (isAnimatedBill)
        output.position.x += sin(vsframeTime) * (1 - input.texCoords.y) / 100;
    
	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

#if defined PS_USE_FOG
    cameraPosition = mul(float4(input.position, 1), WV);
	//51:
    if (VShasFog) 
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	
	//23: LIGHT: NORMAL
	output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader083Light(PSIn input) : SV_TARGET
{
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
	
	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
    float4 textureColor = shaderTexture.Sample(SampleType, input.texCoords);
    //float lightIntensity = PSlightFunc1(input.normal);
    float4 ambientColor = float4(0.55f, 0.55f, 0.55f, 1);
    textureColor = textureColor * saturate(ambientColor /*+ lightIntensity*/);

#if defined PS_USE_FOG
    if (hasFog)
    {
        float4 fog4 = 0;

        fog4.r = (1.0 - input.fogFactor);
        fog4.g = fog4.r;
        fog4.b = fog4.r;
        textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
    }
#endif
    
    //return float4(0, 1, 1, 1);
	return textureColor;
}
