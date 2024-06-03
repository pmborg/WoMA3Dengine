// --------------------------------------------------------------------------------------------
// Filename: 036LightRenderShadow.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*   DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*   -------------------------------------------------------------------------------------------
*   code by : Pedro Borges - pmborg@yahoo.com
*   Downloaded from : http://woma.servegame.com
*
**********************************************************************************************/

#define PS_USE_LIGHT            //23
#define PS_USE_ALFA_TEXTURE     //33
#define PS_USE_ALFACOLOR        //33
#define PS_USE_SPECULAR         //34
#define PS_USE_SHADOWMAP_TEXTURE//36

//#define PS_USE_BUMP             
//#define PS_USE_FADE
//#define PS_USE_SKY

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3 position : POSITION; //21
    float2 texCoords: TEXCOORD; //22
    float3 normal   : NORMAL;   //23
};

// PIXEL:
struct PSIn
{
    float4 position         : SV_POSITION;          // 21 COLOR
    float2 texCoords        : TEXCOORD;             // 22 TEXTURE
    float3 normal           : NORMAL;               // 23 LIGHT
    float3 viewDirection    : TEXCOORD1;            // 34 SPECULAR
    float4 cameraPosition   : WS;                   // 34 SPECULAR
    //float3 tangent        : TANGENT;          	// 35 BUMP
    float4 lightViewPosition: LIGHT_VIEW_POSITION;  // 36 SHADOWS
};

/////////////
// GLOBALS //
/////////////

//Set on: DXmodelClass::RenderSubMesh
#if DXAPI11 == 1
Texture2D shaderTexture;            // 21:
Texture2D AlfaMapTexture;           // 33: AlfaMap
Texture2D ShadowMapTextureTexture;  // 36: ShadowMap
//Texture2D TangentMapTexture;      // 35: TangentMap
#endif
#if DXAPI12 == 1
Texture2D shaderTexture             : register(t0); // 21:
Texture2D AlfaMapTexture            : register(t1); // 33: AlfaMap
Texture2D ShadowMapTextureTexture   : register(t2); // 36: ShadowMap
//Texture2D TangentMapTexture       : register(t3); // 35: TangentMap
#endif

//NOTE: If add more Textures... UPDATE: void DX11Class::EndScene(UINT monitorWindow)

#if DXAPI11 == 1
SamplerState SampleType;
#endif
#if DXAPI12 == 1
SamplerState SampleType: register(s0);
#endif

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsl"
#include "light.hlsl"
#include "shadows.hlsl"


////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader036LightRenderShadow(VSIn input)
{
    PSIn output;
    float4 cameraPosition;
    //output.originalPosition = input.position;

    //21: COMMON
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

    cameraPosition = mul(float4(input.position, 1), WV);                                // FOG: Calculate the camera position.
    //output.cameraPosition = cameraPosition;

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

    #if defined PS_USE_SPECULAR         //44
    //44: Specular
    // MORE INFO: http://3dgep.com/texturing-lighting-directx-11/#Specular
    if (VShasSpecular)  // If enabled on material, calculate the Specular LIGHT
    {
        float4 worldPosition = mul(float4(input.position, 1), worldMatrix);         // P
        output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);   // L = Lp - p (L = lightDirection)
    }
    #endif

    //35: BUMP
#if defined PS_USE_BUMP
    if (VShasNormMap)
        output.tangent = mul(input.tangent, worldMatrix);
#endif

    #if defined PS_USE_SHADOWMAP_TEXTURE//36
    //36:
    if (VShasShadowMap)
        output.lightViewPosition = mul(mul(float4(input.position, 1), worldMatrix), ViewToLightProj);
    #endif

    return output;
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader036LightRenderShadow(PSIn input) : SV_TARGET
{
    float4  textureColor = pixelColor;    // SET PIXEL COLOR
    float   lightIntensity = 0;
#if defined PS_USE_FOG
    //float4    fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
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
    // 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
    if (hasTexture)
        textureColor = shaderTexture.Sample(SampleType, input.texCoords);

    // 23: LIGHT
    if (hasLight) 
    {
    //35: BUMP
    #if defined PS_USE_BUMP
        float3 normal = input.normal;
        
        if (hasNormMap) 
        {
            float4 normalMap    = TangentMapTexture.Sample( SampleType, input.texCoords);   // Load normal from normal map
            normalMap           = (2.0f*normalMap) - 1.0f;                          // Change normal map range from [0, 1] to [-1, 1]
            float3 tangent      = normalize(input.tangent - dot(input.tangent, input.normal)*input.normal); //Make sure tangent is completely orthogonal to normal
            float3 biTangent    = cross(input.normal, input.tangent);               // Create the biTangent
            float3x3 texSpace   = float3x3(tangent, biTangent, input.normal);       // Create the "Texture Space"
            normal              = normalize(mul(normalMap, texSpace));              // BUMP: Convert normal from normal map to texture space and store in input.normal
        }
    #endif
    
	#if defined PS_USE_SHADOWMAP_TEXTURE//36
        if (lightType == 1) 
            lightIntensity = PSlightFunc1(input.normal);
        else
            lightIntensity = PSlightFunc2(input.normal);
	#else
        if (lightType == 1) 
            lightIntensity = PSlightFunc2(input.normal);
        else
            lightIntensity = PSlightFunc1(input.normal);
	#endif


        if (hasTexture) {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + lightIntensity);  
        } else {
            textureColor = textureColor * saturate(emissiveColor + ambientColor + (lightIntensity * diffuseColor));
        }
    }
    
    #if defined PS_USE_SPECULAR         //44
    // http://3dgep.com/texturing-lighting-directx-11/#Specular
    if (hasSpecular)    // If enabled on material, calculate the Specular LIGHT
    {
        if (lightIntensity > 0.0f)
        {
            float4 color = ambientColor;
            color += (VSdiffuseColor * lightIntensity);
            color = saturate(color);

            float3 Reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
            float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));    // (R.V)
            float4 specular = pow(fPhoneValue, nShininess);                         // Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

            color = color * textureColor;
            textureColor = saturate(textureColor + specular);       // specular = Ls (contribution of the light source) * Ks (specular component of the material)
        }
    } 
    #endif

#if defined PS_USE_FOG
    // 31: FOG: Calculate the final color using the fog effect equation.
    if (hasFog)
        textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;
#endif

#if defined PS_USE_ALFA_TEXTURE     //43
    // 43: Alfa Map: (Optional AlfaMap for blending textutres)
    if (hasAlfaMap)
        textureColor.a = AlfaMapTexture.Sample(SampleType, input.texCoords).r;
#endif

#if defined PS_USE_ALFACOLOR
    if (hasAlfaColor)
        textureColor.a = alfaColor;
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
            float  fPhoneValue = saturate(dot(Reflection, input.viewDirection));    // (R.V)
            float4 specular = pow(fPhoneValue, nShininess);                         // Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

            color = color * textureColor;
            textureColor = saturate(textureColor + specular);       // specular = Ls (contribution of the light source) * Ks (specular component of the material)
        }
    }
#endif

    return textureColor;
}
