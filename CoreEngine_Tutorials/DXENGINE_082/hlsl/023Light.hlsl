// --------------------------------------------------------------------------------------------
// Filename: 023Light.hlsl
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
//#define PS_USE_ALFA_TEXTURE	//33
#define PS_USE_ALFACOLOR 	    //33
//#define PS_USE_SPECULAR		//34
#define PS_USE_FOG              //51


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
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
#endif
#if defined PS_USE_FOG
    float fogFactor				: FOG;		// 51 FOG
#endif	
};


////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"

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


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader023Light(VSIn input)
{
	PSIn output;
	float4 cameraPosition;
    float4 position;
    
    if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
    } else {
	    position = float4(input.position, 1);
	    position = mul(position, worldMatrix);
	    position = mul(position, view);			//viewMatrix
	    position = mul(position, projection);	//projectionMatrix
	    output.position = position;
    }

    //if (isAnimatedBill)
    //    output.position.x += sin(vsframeTime * 100) * (1 - input.texCoords.y) / 200;
    if (isAnimatedBill)
        output.position.x += sin(vsframeTime) * (1 - input.texCoords.y) / 100;
    
	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	cameraPosition = mul(float4(input.position, 1), WV);

#if defined PS_USE_FOG
	//51:
    if (VShasFog)
    {
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
    }
    else if (vsIsSky)
    {
        float _VSfogStart;
        float _VSfogEnd;
       //if (position.y < 0)
       //{
       //    _VSfogStart = 1;
       //    _VSfogEnd = 5;
       //}
       //else
        {
            _VSfogStart = 0;
            _VSfogEnd = 1524;
        }
        output.fogFactor = saturate((_VSfogEnd - cameraPosition.z) / (_VSfogEnd - _VSfogStart)); // Calculate linear fog.  
    }
#endif
	
	//23: LIGHT: NORMAL
    if (VShasLight || VShasSpecular) 
        output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); // Calculate the normal vector against the world matrix only
	
	//34: SPECULAR
#if defined PS_USE_SPECULAR
	output.cameraPosition = cameraPosition;

	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);	// L = Lp - p (L = lightDirection)
	}
#endif
    
	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader023Light(PSIn input) : SV_TARGET
{
    float4 textureColor = pixelColor; // SET PIXEL COLOR
    float lightIntensity = 0;
#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
	
	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
    if (hasTexture) 
        textureColor = shaderTexture.Sample(SampleType, input.texCoords);

	// 23: LIGHT
	//if (hasLight) 
	{
        if (isSky)
            lightIntensity = PSlightFunc2(input.normal);
        else
            lightIntensity = PSlightFunc1(input.normal);

        if (hasTexture)
        {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);
        }
        else
        {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
        }
    }

#if defined PS_USE_ALFA_TEXTURE // 33: Alfa Map: (Optional AlfaMap for blending textutres)
    if (hasAlfaMap)
        textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
    if (hasAlfaColor)
        textureColor.a = alfaColor;
#endif

#if defined PS_USE_SPECULAR //34: If enabled on material, calculate the Specular LIGHT
    if (hasSpecular)
    {
        if (lightIntensity > 0.0f)
        {
            float4 color = ambientColor;
			
            color += (diffuseColor * lightIntensity);
		
            color = saturate(color);
			//return color;
            float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
            float fPhoneValue = saturate(dot(Reflection, input.viewDirection)); // (R.V)
            float4 specular = pow(fPhoneValue, nShininess); // Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

            color = color * textureColor;
            textureColor = saturate(textureColor + specular); // specular = Ls (contribution of the light source) * Ks (specular component of the material)
			//return specular;
        }
    }
#endif

#if defined PS_USE_FOG
    if (hasFog || isSky)
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
