// --------------------------------------------------------------------------------------------
// Filename: 035TextureBump.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/

//case SHADER_NORMAL_BUMP:

#define PS_USE_LIGHT		//23
#define PS_USE_ALFA_TEXTURE	//33
#define PS_USE_ALFACOLOR 	//33
#define PS_USE_SPECULAR		//34
#define PS_USE_BUMP		 	//35		NEW!!

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn						
{
	float3 position : POSITION;	//21
	float2 texCoords: TEXCOORD; //22
	float3 normal	: NORMAL;	//23
	float3 tangent	: TANGENT;	//35		NEW:BUMPv2!!
    float3 binormal : BINORMAL; //35		NEW:BUMPv2!!
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
    float3 tangent				: TANGENT;				//35		NEW!!
    float3 binormal				: BINORMAL;				//35		NEW!!
    float4 color				: COLOR0;
};


/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
//#if DXAPI11 == 1
Texture2D shaderTexture;		// 22: Texture
Texture2D AlfaMapTexture;		// 33: AlfaMap
Texture2D TangentMapTexture;	// 35: TangentMap NEW!! DXMODELCLASS:1959
//#endif
#if DXAPI12 == 1
Texture2D AlfaMapTexture:		register(t0); // 33: AlfaMap	//DX12: Descriptor: 2
Texture2D shaderTexture:		register(t1); // 22: Texture	//DX12: Descriptor: 3
Texture2D TangentMapTexture:	register(t1); // 35: TangentMap //DX12: Descriptor: 4
#endif

//#if DXAPI11 == 1
SamplerState SampleType;
//#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"
#include "light.hlsl"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
	PSIn output;
	float4 cameraPosition;

	//21: POSITION: Calculate the position of the vertex against the world, view, and projection matrices
if (VS_USE_WVP) {
	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
} else {
	float4 position = float4(input.position, 1);
	position = mul(position, worldMatrix);
	position = mul(position, view);			//viewMatrix
	position = mul(position, projection);	//projectionMatrix
	output.position = position;
}
    float power = 2;
	
    float4 tangent = mul(float4(input.tangent.xyz, 0.0), worldMatrix);
    float4 normal = mul(float4(input.normal.xyz, 0.0), worldMatrix);
    float3 binormal = input.binormal; //cross(normal.xyz, tangent.xyz);
	
    float4 posWorld = mul(input.position, worldMatrix);
	
    float4 light = normalize(posWorld - lightDirection);
    float4 eye = normalize(vEye - light);
    float4 vhalf = normalize(eye - lightDirection);
	
    float3 L = float3(dot(tangent, light), dot(binormal, light.xyz), dot(normal, light));
    float3 H = float3(dot(tangent, vhalf), dot(binormal, vhalf.xyz), dot(normal, vhalf));
	
    float diffuse = dot(normal, L);
    float specular = dot(normal, H);
    specular = pow(specular, power);
	
    output.color = 2.0 * (diffuse * diffuseColor + specular /** vSpecularMaterial*/) + ambientColor;
	
	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	//23: LIGHT: NORMAL
	//if (VShasLight || VShasSpecular) 
    //output.normal = mul(float4(input.normal, 1), transpose(worldMatrix)); // Calculate the normal vector against the world matrix only
	
	//34: SPECULAR
#if defined PS_USE_SPECULAR
	//Used by PS_USE_SPECULAR and PS_USE_FOG
	cameraPosition = mul(float4(input.position, 1), WV);	
	output.cameraPosition = cameraPosition;

	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);	// L = Lp - p (L = lightDirection)
	}
#endif

	//35: BUMP NEW!!
#if defined PS_USE_BUMP
    //if (VShasNormMap)
    {
		// Calculate the tangent vector against the world matrix only and then normalize the final value.
        output.tangent = mul(input.tangent, worldMatrix);
        output.tangent = normalize(output.tangent);
		
		// Calculate the binormal vector against the world matrix only and then normalize the final value.
        output.binormal = mul(input.binormal, worldMatrix);
        output.binormal = normalize(output.binormal);
    }
#endif

	return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
	float  lightIntensity = 0;

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location

    float4 Color	= shaderTexture.Sample(SampleType, input.texCoords);
    float4 Normal	= TangentMapTexture.Sample(SampleType, input.texCoords);
	
    float4 textureColor = Color * dot(2.0 * (Normal - 0.5), 2.0 * (input.color - 0.5));
	
	//===================================================================================
	/*
    float3 normal = input.normal;

    float4 color = shaderTexture.Sample(SampleType, input.texCoords) * pin.Diffuse;

    float3 eyeVector = normalize(EyePosition - pin.PositionWS.xyz);
    float3 worldNormal = normalize(pin.NormalWS);

    // Before lighting, peturb the surface's normal by the one given in normal map.
    float3 localNormal = TwoChannelNormalX2(NormalTexture.Sample(Sampler, pin.TexCoord).xy);
    float3 normal = PeturbNormal(localNormal, pin.PositionWS.xyz, worldNormal, pin.TexCoord);

    // Do lighting
    ColorPair lightResult = ComputeLights(eyeVector, normal, 3);

    color.rgb *= lightResult.Diffuse;

    AddSpecular(color, lightResult.Specular);
    ApplyFog(color, pin.PositionWS.w);
	*/
	//===================================================================================	
	/*
	//35: BUMP NEW!!
	//if (hasNormMap)
    {
        float3 normalMap = TangentMapTexture.Sample(SampleType, input.texCoords);						// Load normal from normal map
		normalMap = (2.0f * normalMap) - 1.0f; // Change normal map range from [0, 1] to [-1, 1]
		float3 tangent = normalize(input.tangent - dot(input.tangent, input.normal) * input.normal);	//Make sure tangent is completely orthogonal to normal
		float3 biTangent = cross(input.normal, input.tangent);											// Create the biTangent
		float3x3 texSpace = float3x3(tangent, biTangent, input.normal);									// Create the "Texture Space"
		normal = normalize(mul(normalMap, texSpace));													// BUMP: Convert normal from normal map to texture space and store in input.normal
    }
	
	// 23: LIGHT
    if (isSky)
        lightIntensity = PSlightFunc1(normal);
    else
        lightIntensity = PSlightFunc2(normal);

    if (hasTexture) {
        textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);
    } else {
        textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
    }
*/

/*
    // Sample the pixel from the normal map.
    float4 bumpMap = TangentMapTexture.Sample(SampleType, input.texCoords);

    // Expand the range of the normal value from (0, +1) to (-1, +1).
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    // Calculate the normal from the data in the normal map.
    float3 bumpNormal = (bumpMap.x * input.tangent) + (bumpMap.y * input.binormal) + (bumpMap.z * input.normal);
    //float3 bumpNormal = (bumpMap.z * input.tangent);

    // Normalize the resulting bump normal.
    bumpNormal = normalize(bumpNormal);
    float3 l = (float3)lightDirection;
	
    // Calculate the amount of light on this pixel based on the normal map value.
    lightIntensity = saturate(dot(bumpNormal, l));
    //lightIntensity = dot(bumpNormal, l);

    // Determine the final amount of diffuse color based on the diffuse color combined with the light intensity.
    float4 color = saturate(diffuseColor * lightIntensity);

    // Combine the final light color with the texture color.
    textureColor = color * textureColor;
*/
	
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
			float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
			float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));	// (R.V)
			float4 specular = pow(fPhoneValue, nShininess);							// Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

			color = color * textureColor;
			textureColor = saturate(textureColor + specular);		// specular = Ls (contribution of the light source) * Ks (specular component of the material)
		}
	}
#endif

	return textureColor;
}
