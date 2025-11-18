// --------------------------------------------------------------------------------------------
// Filename: 040LightInstance.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*   DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*   -------------------------------------------------------------------------------------------
*   code by : Pedro Borges - pmborg@yahoo.com
*   Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#if (!defined DXAPI11 && !defined DXAPI12)
    #define DXAPI11 1
#endif

#define PS_USE_LIGHT            //23
#define PS_USE_ALFA_TEXTURE     //33
#define PS_USE_ALFACOLOR        //33
#define PS_USE_SPECULAR         //34
//#define PS_USE_BUMP			//35 not available on this version/level
//#define PS_USE_SHADOWMAP_TEXTURE//36

//#define PS_USE_FOG	//not available on this version/level
//#define PS_USE_FADE	//not available on this version/level
//#define PS_USE_SKY	//not available on this version/level

#define TEXTURE_MODE -1 //Default is -1

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
	float3 position : POSITION;				//21
	float2 texCoords: TEXCOORD; 			//22
	float3 normal	: NORMAL;				//23
	float3 instancePosition : INSTANCEPOS;	//40: D3D11_INPUT_PER_INSTANCE_DATA
};

// PIXEL:
struct PSIn
{
    float4 position			: SV_POSITION;			// 21 COLOR
    float2 texCoords		: TEXCOORD0;			// 22 TEXTURE
	float3 normal			: NORMAL;				// 23 LIGHT
	float3 viewDirection	: TEXCOORD1;			// 34 SPECULAR
	float4 cameraPosition	: WS;					// 34 SPECULAR
};

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture             : register(t0); 	// 21:
Texture2D AlfaMapTexture 	        : register(t1);    	// alpha map (optional)
Texture2D ShadowMapTextureTexture   : register(t2);     // shadow map
Texture2D normalMapTexture 	        : register(t3);  	// NORMAL MAP
#endif
#if DXAPI12 == 1
Texture2D shaderTexture             : register(t0); 	// 21:
Texture2D AlfaMapTexture 	        : register(t1);    	// alpha map (optional)
Texture2D ShadowMapTextureTexture 	: register(t2);    	// shadow map
Texture2D normalMapTexture      	: register(t3);  	// NORMAL MAP
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
#include "cbuffer.hlsli"
#include "light.hlsli"
#include "TextureSampling.hlsli"
//#include "shadows.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input, uint instanceID : SV_InstanceID)
{
	PSIn output;
	float4 cameraPosition;

	/*
	_m00, _m01,_m02, _m03
	_m10, _m11,_m12, _m13
	_m20, _m21,_m22, _m23
	_m30, _m31,_m32, _m33
	*/

	matrix <float, 4, 4> rotationAroundY =
	{
		{ 1, 0, 0, 0 },		//cos(VSrotY), 0.0f, -sin(VSrotY), 0.0f,
		{ 0, 1, 0, 0 },		//0.0f, 1.0f, 0.0f, 0.0f,
		{ 0, 0, 1, 0 },		//sin(VSrotY), 0.0f, cos(VSrotY), 0.0f,
		{ 0, 0, 0, 1 }		//0.0f, 0.0f, 0.0f, 1.0f
	};
	rotationAroundY[0].x = cos(VSrotY);
	rotationAroundY[0].z = -sin(VSrotY);
	rotationAroundY[2].x = sin(VSrotY);
	rotationAroundY[2].z = cos(VSrotY);
	//[1st] ROTATE the instance
	float4 position = float4(input.position, 1);
	position = mul(position, rotationAroundY);

	//[2nd] TRANSLATE the instance
	// 40 - Update the position of the vertices based on the data for this particular instance.
	//input.position = RotateAroundYInDegrees(input.position, VSrotY);
	position.x += input.instancePosition.x * (1 / worldMatrix._m00);
	position.y += input.instancePosition.y * (1 / worldMatrix._m11);
	position.z += input.instancePosition.z * (1 / worldMatrix._m22);

	//[3th] PRESPECTIVE the instance
	//21: POSITION: Calculate the position of the vertex against the world, view, and projection matrices
	if (VS_USE_WVP) {
		output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
	}
	else {
		position = mul(position, worldMatrix);
		position = mul(position, view);			//viewMatrix
		position = mul(position, projection);	//projectionMatrix
		output.position = position;
	}

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	// FOG: Calculate the camera position.
	cameraPosition = mul(float4(input.position, 1), WV);								

    //23: LIGHT: NORMAL
    if (VShasLight || VShasSpecular) 
    {
        //----- 2 Lines in 1 -----
        //normal = mul(input.normal, (float3x3)worldMatrix);
        //normal = normalize(output.normal);
        output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only
    }
	
#if defined PS_USE_FOG
    //31: // FOG: Calculate the camera position.
    if (VShasFog)
    {
        //----- 2 Lines in 1 -----
        //cameraViewPosition = mul(input.position, worldMatrix);
        //cameraViewPosition = mul(cameraPosition, viewMatrix);
        float4 cameraViewPosition = mul(float4(input.position, 1), WV);
        output.fogFactor = saturate((VSfogEnd - cameraViewPosition.z) / (VSfogEnd - VSfogStart));   // Calculate linear fog.  
    }
#endif

#if defined PS_USE_SPECULAR
	output.cameraPosition = cameraPosition;
	if (VShasSpecular)
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);
	}
#endif

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Helpers - reuse your sampling function for consistency
////////////////////////////////////////////////////////////////////////////////
float4 GetShaderTexture(Texture2D tex, float2 texCoords, uint mipLevel, int mode)
{
    switch (mode)
    {
        case 0: return NearestInterpolation(SampleType, tex, texCoords);
        case 1: return BilinearInterpolation(SampleType, tex, texCoords);
        case 2: return TrilinearInterpolation(SampleType, tex, texCoords, mipLevel);
        case 3: return CubicInterpolation(SampleType, tex, texCoords);
        default: return tex.Sample(SampleType, texCoords);
    }
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;
#if defined PS_USE_FOG
    float4    fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif

#if defined PS_USE_SKY
    // 30: SKY
    if (isSky)
        if (hasFog)
            return fogColor;
        else
            return Sky(input.cameraPosition.y, input.originalPosition.y, input.texCoords);
#endif

#if defined PS_USE_FOG
    // 100% FOG: Need to be after sky (No fog on Sky) ;)
    if (hasFog && input.fogFactor == 0)
        return fogColor * fade;
#endif

    //-----------------------------------------------------------------------------------
	// Sample base albedo
	if (hasTexture)
	{
		//textureColor = shaderTexture.Sample(SampleType, input.texCoords);
		textureColor = GetShaderTexture(shaderTexture, input.texCoords, 0, TEXTURE_MODE);
	}

	// === BUMP / NORMAL MAPPING ===
	// start with the interpolated (world-space) normal from VS
	float3 normal = normalize(input.normal);
	
#if defined PS_USE_BUMP
	if (hasNormMap)
	{
		// Sample the normal map using your texture sampling helper (consistent filtering)
		float3 nmap = GetShaderTexture(normalMapTexture, input.texCoords, 0, TEXTURE_MODE).xyz * 2.0f - 1.0f;
        // Fix for OpenGL-style normal maps in a DirectX engine:
        nmap.g = -nmap.g;
        
		// Optional: normal strength parameter for tuning/debugging
		float normalStrength = 1.0f; // <---- tune this or expose in cbuffer
		nmap = saturate(nmap * normalStrength);

		// Ensure tangent is orthogonal to the (transformed) normal
		float3 T = normalize(input.tangent - dot(input.tangent, normal) * normal);

		// Compute bitangent consistently (handedness depends on exporter - this is standard)
		float3 B = normalize(cross(normal, T));

		// Build TBN (columns = T, B, N)
		float3x3 TBN = float3x3(T, B, normal);

		// Transform normal from tangent space to world space
		normal = normalize(mul(nmap, TBN));
	}
#endif

	// 23: LIGHT
	if (hasLight) 
	{
		if (lightType == 1)	
			lightIntensity = PSlightFunc1(input.normal);
		else
			lightIntensity = PSlightFunc2(input.normal);

		if (hasTexture) {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);	
		} else {
			textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
		}
	}

#if defined PS_USE_FOG
    // 31: FOG: Calculate the final color using the fog effect equation.
    if (hasFog)
        textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
#endif

#if defined PS_USE_SHADOWMAP_TEXTURE//36
    //36: SHADOWS [NEW PART!]
    if (castShadow)
        textureColor.rgb = textureColor.rgb * HasShadows(input.lightViewPosition);
#endif

#if defined PS_USE_FADE
    // FADE: Used by Sun or All (at night)
    if (fade < 1)
        textureColor.rgb = textureColor.rgb * fade;
#endif

#if defined PS_USE_SPECULAR //34: If enabled on material, calculate the Specular LIGHT

	if (hasSpecular)	
	{
		if (lightIntensity > 0.0f)
		{
			float4 color = ambientColor;
			
			color += (diffuseColor * lightIntensity);
			color = saturate(color);
			float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
			float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));	// (R.V)
			float4 specular = pow(fPhoneValue, nShininess);							// Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

			color = color * textureColor;
			textureColor = saturate(textureColor + specular);		// specular = Ls (contribution of the light source) * Ks (specular component of the material)
		}
	}
#endif

#if defined PS_USE_ALFA_TEXTURE // 33: Alfa Map: (Optional AlfaMap for blending textutres)
	if (hasAlfaMap)
		textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_ALFACOLOR	// 33: Alfa Color
	if (hasAlfaColor)
		textureColor.a = alfaColor;
#endif

    //return float4(1,1,1,1);
    return textureColor;
}