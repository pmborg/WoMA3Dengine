// --------------------------------------------------------------------------------------------
// Filename: 055Slope_Texture_Terrain.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2024
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
/*********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#define PS_USE_LIGHT			//23
//#define PS_USE_FADE			//Night/Day
//#define PS_USE_ALFA_TEXTURE	//33
//#define PS_USE_ALFACOLOR 		//33
//#define PS_USE_SPECULAR			//34
//#define PS_USE_FOG				//51
//#define PS_USE_NORMAL_MAP
//#define PS_USE_SHADOW

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3 position : POSITION;
    float2 texCoords : TEXCOORD;
    float3 normal : NORMAL;
    float4 color : COLOR;
#if defined PS_USE_MAP
	float3 tangent: TANGENT;
#endif
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;
    float4 Wposition : POSITION;
    float2 texCoords : TEXCOORD0;
    float3 normal : NORMAL;
    float4 color : COLOR;
    float4 cameraPosition : WS;
#if defined PS_USE_FOG
    float fogFactor			: FOG; // 51 FOG
#endif
#if defined PS_USE_SPECULAR
	float3 viewDirection	: TEXCOORD1;			
#endif
#if defined PS_USE_SHADOWMAP_TEXTURE//45
	float4 lightViewPosition: LIGHT_VIEW_POSITION;	
#endif
#if defined PS_USE_MAP
	float3 tangent		: TANGENT;				
#endif
};

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh

Texture2D grassTexture : register(t0); //017grass
Texture2D slopeTexture : register(t1); //017slope
Texture2D rockTexture : register(t2); //017rock
Texture2D blackrockTexture : register(t3);

//Texture2D AlfaMapTexture			: register(t1);	// 43: AlfaMap
//Texture2D ShadowMapTextureTexture	: register(t2);	// 45: ShadowMap
//Texture2D TangentMapTexture;		: register(t3);	// 47: TangentMap

SamplerState SampleType : register(s0); // 3D (default) WRAP
//SamplerState SampleTypeClamp		: register(s1);

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////

PSIn MyVertexShader055Slope_Texture_Terrain(VSIn input)
{
    PSIn output;
    float4 cameraPosition;
	
	//21: POSITION: Calculate the position of the vertex against the world, view, and projection matrices
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
	
    output.Wposition = mul(float4(input.position, 1), worldMatrix); // Calculate the position of the vertex against the world, view, and projection matrices
	
	//22: TEXTURE: Store the texture coordinates for the pixel shader.
    output.texCoords = input.texCoords; // FOG: Calculate the camera position.

	//51: FOG: Calculate the camera position.
    cameraPosition = mul(float4(input.position, 1), WV); // FOG: Calculate the camera position.
    output.cameraPosition = cameraPosition;
#if defined PS_USE_FOG
    if (VShasFog) 
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif
	
	//23: LIGHT: NORMAL
	//if (VShasLight || VShasSpecular)
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); //23: LIGHT: Calculate the normal vector against the world matrix only.
	
	//34: SPECULAR
#if defined PS_USE_SPECULAR
    if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
    {
        float4 worldPosition = mul(float4(input.position, 1), worldMatrix); // P
        output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz); // L = Lp - p (L = lightDirection)
    }
#endif

#if defined PS_USE_SHADOW
	// ShadowMap
	if (VShasShadowMap)
		output.lightViewPosition = mul(mul(float4(input.position, 1), worldMatrix), ViewToLightProj);
#endif
#if defined PS_USE_MAP
	// BUMP
	if (VShasNormMap)
		output.tangent = mul(input.tangent, worldMatrix);
#endif
	
	//Send the color map color into the pixel shader.	
    output.color = input.color;

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader055Slope_Texture_Terrain(PSIn input) : SV_TARGET
{
    float4 textureColor;
    float lightIntensity = 0;
    float4 color;

#if defined PS_USE_FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
	
    if (input.fogFactor < 0)
        input.fogFactor = 0;
	// Need to be after sky (No fog on Sky) ;)
	//if (hasFog && input.fogFactor == 0)
	//	return fogColor * fade;
#endif

    float blendAmount;
    float4 grassColor;
    float4 slopeColor;
    float4 rockColor;


    //0: grassTexture 
    //1: slopeTexture 
    //2: rockTexture 
    //3: blackrockTexture
    
    grassColor = grassTexture.Sample(SampleType, input.texCoords);
    slopeColor = slopeTexture.Sample(SampleType, input.texCoords);
    rockColor = rockTexture.Sample(SampleType, input.texCoords);
    textureColor = blackrockTexture.Sample(SampleType, input.texCoords);
	
    if (input.Wposition.y < -1.95)
    {
        //
    }
    else
    {
        float slope;
        if (input.normal.y > 0)
            slope = (1.0f - input.normal.y);
        else
            slope = (1.0f + input.normal.y);
		
        if (slope < 0.2)
        {
            blendAmount = slope / 0.2f;
            textureColor = lerp(grassColor, slopeColor, blendAmount);
        }
        else if ((slope < 0.75) && (slope >= 0.2f))
        {
            blendAmount = (slope - 0.2f) * (1.0f / (0.75f - 0.2f));
            textureColor = lerp(slopeColor, rockColor, blendAmount);
        }
        else if (slope >= 0.75)
        {
            textureColor = rockTexture.Sample(SampleType, input.texCoords);
        }
    }
	
	// 23: LIGHT
    if (hasLight)
    {
        float3 normal = input.normal;
#if defined PS_USE_NORMAL_MAP
		if (hasNormMap)
		{
			float4 normalMap = TangentMapTexture.Sample(SampleType, input.texCoords);	// Load normal from normal map
			normalMap = (2.0f*normalMap) - 1.0f;							// Change normal map range from [0, 1] to [-1, 1]
			float3 tangent = normalize(input.tangent - dot(input.tangent, input.normal)*input.normal);	//Make sure tangent is completely orthogonal to normal
			float3 biTangent = cross(input.normal, input.tangent);				// Create the biTangent
			float3x3 texSpace = float3x3(tangent, biTangent, input.normal);		// Create the "Texture Space"
			normal = normalize(mul(normalMap, texSpace));				// BUMP: Convert normal from normal map to texture space and store in input.normal
		}
#endif
        lightIntensity = PSlightFunc1(normal);
        if (hasTexture)
            textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);
    }

	//44:
#if defined PS_USE_SPECULAR
	// http://3dgep.com/texturing-lighting-directx-11/#Specular
	if (hasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		if (lightIntensity > 0.0f)
		{
			float4 color = ambientColor;
			color += (VSdiffuseColor * lightIntensity);
			color = saturate(color);

			float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
			float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));	// (R.V)
			float4 specular = pow(fPhoneValue, nShininess);							// Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

			color = color * textureColor;
			textureColor = saturate(textureColor + specular);		// specular = Ls (contribution of the light source) * Ks (specular component of the material)
		}
	}
#endif
	
#if defined PS_USE_FOG				//31
	if (hasFog)
		textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;	// FOG: Calculate the final color using the fog effect equation.
#endif

#if defined PS_USE_ALFA_TEXTURE		//43
// 43: Alfa Map: (Optional AlfaMap for blending textutres)
	if (hasAlfaMap)
		textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_SHADOWMAP_TEXTURE//45
	if (castShadow)
		textureColor.rgb = textureColor.rgb * HasShadows(input.lightViewPosition);
#endif 

#if defined PS_USE_FADE
	// FADE (Night):
	if (fade < 1)
		textureColor.rgb = textureColor.rgb * fade; // Used by Sun or All (at night)
#endif
	
#if defined PS_USE_ALFACOLOR		//26
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

    //color = color * textureColor;
	
	//return float4(1,1,1,1);
	return textureColor;
}