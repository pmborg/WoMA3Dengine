// --------------------------------------------------------------------------------------------
// Filename: 022Texture.hlsl [LATEST]
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2017
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525256;

//////////////
// TYPEDEFS //
//////////////
#define DXAPI11             1   //force
#define PS_USE_FOG           	//51 
#define PS_USE_ALFACOLOR		//50 water
//#if defined PS_USE_FADE

// VERTEX:
struct VSIn
{
    float3 position : POSITION;
    float2 texCoords : TEXCOORD0; //22
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float2 texCoords : TEXCOORD0;
#if defined PS_USE_FOG && DXAPI11 == 1
    float fogFactor : FOG; // 51 FOG
#endif	
};

//Set on: DXmodelClass::RenderSubMesh
Texture2D shaderTexture : register(t0); //DX12: SRV
SamplerState SampleType; //3D (default) WRAP

SamplerState samAnisotropic
{
    Filter = ANISOTROPIC;
    MaxAnisotropy = 16;

    AddressU = WRAP;
    AddressV = WRAP;
};


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
    float  VSlightPAD; // 3+1=XMFLOAT4
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
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader022Texture(VSIn input)
{
    PSIn output;
#if defined PS_USE_FOG
    float4 cameraPosition;
#endif
    
    if (VS_USE_WVP)
    {
        output.position = mul(float4(input.position, 1), WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    }
    else
    {
        float4 position = float4(input.position, 1);
        position = mul(position, worldMatrix);
        position = mul(position, view); //viewMatrix
        position = mul(position, projection); //projectionMatrix
        output.position = position;
    }

    
    if (isAnimatedBill)
    {
        output.position.x += sin(vsframeTime * 100) * (1 - input.texCoords.y) / 10;
    }
    
    output.texCoords = input.texCoords; // TEXTURE: Store the texture coordinates for the pixel shader:

    //51:
#if defined PS_USE_FOG
    cameraPosition = mul(float4(input.position, 1), WV);
#endif
#if defined PS_USE_FOG && DXAPI11 == 1
    if (VShasFog)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	
    return output;
}

float4 MyPixelShader022Texture(PSIn input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location:
    float4 textureColor = shaderTexture.Sample(SampleType, input.texCoords);
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
    
    textureColor.rgb *= fade;

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
    if (hasAlfaColor)
        textureColor.a = alfaColor;
#endif

#if defined PS_USE_FOG && DXAPI11 == 1
    if (hasFog)
    {
        float4 fog4 = 0;
        if (isSky)
        {
            fog4.r = 0.9;
        }
        else
        {
            fog4.r = (1.0 - input.fogFactor);
        }
        fog4.g = fog4.r;
        fog4.b = fog4.r;
        textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
        //textureColor = fog4;

    }
#endif
    
    //return float4(1,1,1,1);
    return textureColor;
}
