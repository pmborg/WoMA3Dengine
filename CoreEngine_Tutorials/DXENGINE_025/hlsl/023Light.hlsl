// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

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
};

/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;
#endif
#if DXAPI12 == 1
Texture2D shaderTexture:	register(t0);
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
////////////////
// CBUFFERS
////////////////
// SYNC: DXshaderClass.h -- DX12: CBV
#if DXAPI11 == 1
cbuffer VSShaderParametersBuffer	//DX11
#endif
#if DXAPI12 == 1
cbuffer VSShaderParametersBuffer : register(b0) //Register is needed for DX12: Descriptor: 0
#endif
{
    // VERTEX: need to match: 
    // [DXshaderClass.h] VSconstantBufferType

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
float3 VSlightDirection; // LIGHT
float VSlightPAD; // 3+1=XMFLOAT4
float4 VSambientColor; // LIGHT
float4 VSdiffuseColor; // LIGHT
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
float4 pixelColor;

    // BLOCK2:
bool hasTexture; // No? Use pixelColor, then.
bool hasLight; // Future Load Obj. Engine Level
bool hasSpecular; // Future Load Obj. Engine Level
bool isFont; // Future Load Obj. Engine Level

    // BLOCK3:
float4 ambientColor; // LIGHT: Ka
float4 diffuseColor; // LIGHT: Kd
float4 emissiveColor; // LIGHT: Ke 
float4 lightDirection; // LIGHT

    // BLOCK4:
bool hasColorMap; // 66
float lightType; // Future
float shaderType; // Future
float shaderTypeParameter; // Future

    // BLOCK5:
bool hasAlfaColor;
float alfaColor;
float fade; // Fade from 0 to 1
float frameTime; // For animations

    // BLOCK6:
bool hasFog;
bool isSky;
bool hasAlfaMap;
bool hasNormMap;

    // BLOCK7:
float3 cameraPosition; // Future
bool castShadow;
float3 specularColor;
float nShininess;
};


////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc2(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, -lightDirection));							// Calculate the amount of light on this pixel
}

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader023Light(VSIn input)
{
	PSIn output;

if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	//23: LIGHT: NORMAL
	if (VShasLight || VShasSpecular) 
		output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only

	return output;
}


////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader023Light(PSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
	if (hasTexture) 
	{
		textureColor = shaderTexture.Sample(SampleType, input.texCoords);
	}

	// 23 & 43: LIGHT
	if (hasLight) 
	{
		lightIntensity = PSlightFunc2(input.normal);

		if (hasTexture) {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);	
		} else {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
		}
	}

	return textureColor;
	//return lightIntensity; //FOR DEBUG
}
