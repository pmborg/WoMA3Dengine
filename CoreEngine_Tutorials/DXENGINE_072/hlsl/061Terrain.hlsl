// --------------------------------------------------------------------------------------------
// Filename: 061Terrain.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525256;

#define FOGGED

//////////////
// TYPEDEFS //
//////////////

//VERTEX:
struct VSIn
{
    float4 position : POSITION; //CH04 - 
    float4 tex : TEXCOORD0; //CH05 - TEXTURE
    float3 normal : NORMAL; //CH12 - LIGHT	
    float4 color : COLOR; //CH13 - MIX COLOR
    float4 texMapping : TEXCOORD1; //CH19 - Big Mapping
    float3 tangent : TANGENT; //CH21
    float3 binormal : BINORMAL; //CH21
};

//PIXEL:
struct PSIn
{
    float4 position : SV_POSITION; //CH04
    float4 tex : TEXCOORD0; //CH05 - TEXTURE
    float3 normal : NORMAL; //CH12 - LIGHT	
    float4 color : COLOR; //CH13 - MIX COLOR
    float fogFactor : FOG; //CH16 - FOG
    float4 depthPosition : TEXCOORD2; //CH18
    float4 texMapping : TEXCOORD1; //CH19
    float3 tangent : TANGENT; //CH21: Normals
    float3 binormal : BINORMAL; //CH21: Normals
    float4 verticePosition : TEXCOORD3; //CH22: (below water)...
};

/////////////
// GLOBALS //
/////////////
//Texture2D shaderTexture;  //CH05 - TEXTURE

//CH17&CH18 - There are three textures we will be using for the three different degrees of slope that we want to handle.
Texture2D grassTexture : register(t0); //CH17 - T 017grass             (Dirty Grass)         N normal001*
Texture2D slopeTexture : register(t1); //CH17 - T 017slope             (Mixture Slop)        N normal001*
Texture2D rockTexture : register(t2); //CH17 - T rock02_savdf         (Black Rock)          N normal001*

Texture2D detailTexture : register(t3); //CH18 - TN grassTexture        (Grass with BUMP)     t11*
Texture2D textureMappingTexture : register(t4); //CH19 - MAP t_019TerrainMapping(General Mapping)     -
Texture2D stonePathTexture : register(t5); //CH19 - T stone001             (Stone walk path)     t9*
Texture2D sandTexture : register(t6); //CH19 - T 019sandV2            (Sand to beach)       N normal001*
Texture2D mudTexture : register(t7); //CH19 - T CrackedMudV2         (Mud for "Estabulo")  N normal001*
Texture2D detailMappingTexture : register(t8); //CH20 - MAP t_019TerrainMappingV2
Texture2D normalTexture : register(t9); //CH21 - N stone001normal       t9*

Texture2D generalNormalTexture : register(t10); //CH22 - N normal001            *

Texture2D bigPathMappingTexture : register(t11); //CH22 - N grassNormal          t11*
Texture2D smallstonePathTexture : register(t12); //CH22 - T 056B_castle.jpg
Texture2D shaderTexture         : register(t13); //CH23 - MAP colorLightMapTexture

SamplerState SampleType; //CH05 - TEXTURE

////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader061terrain_fog_slope_detail_mapping(VSIn input)
{
    PSIn output;
    float4 cameraPosition; //FOG
    input.position.w = 1.0f; //CH4 Change the position vector to be 4 units for proper matrix calculations.

    if (input.position.y < 0)
        input.position.y *= 3;
    
    output.position = mul(input.position, WVP); //Calculate the position of the vertex against the world, view, and projection matrices.
    output.tex = input.tex; //CH05 - TEXTURE- Store the texture coordinates for the pixel shader.

	//CH 12 LIGHT:
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); //Calculate the normal vector against the world matrix only.

    //CH13 COLOR:Extra Coloration
    output.color = input.color; //CH13 - MIX COLOR - Send the color map color, into the pixel shader.

    cameraPosition = mul(input.position, WV); //FOG: Calculate the camera position.

    //CH16 FOG:
#if defined (FOGGED)
    if (true /*VShasFog*/)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
    else
        output.fogFactor = 0;
#endif    
    //CH18 - Store the position value in a second input value for depth value calculations.
    output.depthPosition = cameraPosition;

    //CH19 Texture Mapping
    output.texMapping = input.texMapping;

	//CH21: Both the input tangent and binormal are calculated against the world matrix and then normalized the same as the input normal vector.
    output.tangent = normalize(mul(input.tangent, (float3x3) worldMatrix)); //Calculate the tangent vector against the world matrix only and then normalize.
    output.binormal = normalize(mul(input.binormal, (float3x3) worldMatrix)); //Calculate the binormal vector against the world matrix only and then normalize.

	//CH22
    output.verticePosition = input.position;

    return output;
}

/*
////////////////////////////////////////////////////////////////////////////////
float3 normapMapFunc(float4 bumpMap, PSIn input) 
////////////////////////////////////////////////////////////////////////////////
{
    float3 bumpNormal; //CH21

    if (input.depthPosition.z < 50) // Bump only on closer textures (Faster version for the rest)
    {
        bumpMap = (bumpMap * 2.0f) - 1.0f; //Expand the range of the normal value from (0, +1) to (-1, +1).

        bumpNormal = input.normal + bumpMap.x * input.tangent +
                     bumpMap.y * input.binormal; //Calculate the normal from the data in the bump map.

        bumpNormal = normalize(bumpNormal); //Normalize the resulting bump normal.
    }
    else
    {
        bumpNormal = input.normal;
    }

    return bumpNormal;
}

////////////////////////////////////////////////////////////////////////////////
inline float4 lightFunc(float3 bumpNormal, float4 textureColor) 
////////////////////////////////////////////////////////////////////////////////
{
    float4 color = 0; //ambientColor;

    float lightIntensity = saturate(dot(bumpNormal, -lightDirection)); // Calculate the amount of light on this pixel.
    color += (diffuseColor * lightIntensity);
    return color * textureColor; // The texture pixel is combined with the light color to create the final color result.
}
*/

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader061terrain_fog_slope_detail_mapping(PSIn input) : SV_TARGET
{
    float4 color = ambientColor;

    float4 textureColor; //CH05 - TEXTURE
    float lightIntensity = PSlightFunc2(input.normal); //CH12 - LIGHT
    
#if defined FOGGED
    //float4 fogColor = (input.verticePosition.y > 0.1f) ? float4(0.5f, 0.5f, 0.5f, 1.0f) : float4(0.2f, 0.2f, 0.6f, 1.0f); //FOG
    float4 fogColor = float4(87.0f / 256.0f, 87 / 256.0f, 87.0f / 256.0f, 1.0f);
#endif
    
    float4 grassColor; //CH17 - SLOP
    float4 slopeColor; //CH17
    float4 rockColor; //CH17
    float slope; //CH17
    float blendAmount; //CH17
    float4 detailColor; //CH18
    float detailBrightness; //CH18
    float4 mappingColor; //CH19
    float4 alphaValue; //CH20
    float4 blendColor; //CH20
    float depthValue; //CH21
    float4 alphamappingColor; //CH22

#if defined (FOGGED)
        if (true /*input.fogFactor > 0*/) 
#endif
        {
  		//CH19: This wll be our mapping texture:
        mappingColor = textureMappingTexture.Sample(SampleType, input.texMapping.xy); //x1

		// TERRAIN:
		{	
			    //CH17 - Start by sampling all three main textures.
			    //------------------------------------------------------------------------------------------------------
			    // Sample the grass color from the texture using the sampler at this texture coordinate location.
                grassColor = grassTexture.Sample(SampleType, input.tex.xy);

			    // Sample the slope color from the texture using the sampler at this texture coordinate location.
                slopeColor = slopeTexture.Sample(SampleType, input.tex.xy);

			    // Sample the rock color from the texture using the sampler at this texture coordinate location.
                rockColor = rockTexture.Sample(SampleType, input.tex.xy);

			    // ALPHA MIXTURE Flat Terrain Big Areas:
                //------------------------------------------------------------------------------------------------------
			    // Sample the pixel color from the detail map texture using the sampler at this texture coordinate location.
                detailColor = detailTexture.Sample(SampleType, input.tex.xy);
                grassColor = lerp(grassColor, detailColor, (mappingColor.r + mappingColor.g + mappingColor.b) / 3);
            
			    // ALPHA MIXTURE Flat Terrain Big walk Path:
                //------------------------------------------------------------------------------------------------------
                alphamappingColor = bigPathMappingTexture.Sample(SampleType, input.texMapping.xy); //x1
                color = smallstonePathTexture.Sample(SampleType, input.tex.xy);
                grassColor = lerp(grassColor, color, alphamappingColor.g);

			    //CH17 - Now determine the slope for this pixel, which is just one subtracted from the Y normal.
			    //------------------------------------------------------------------------------------------------------
                slope = 1.0f - input.normal.y; // Calculate the slope of this point.

			    // Determine which texture to use based on height:
                if (slope < 0.2f)
                {
                    blendAmount = slope / 0.2f;
                    textureColor = lerp(grassColor, slopeColor, blendAmount);
                }
                else if (slope < 0.7f)
                {
                    blendAmount = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
                    textureColor = lerp(slopeColor, rockColor, blendAmount);
                }
                else
                {
                    textureColor = rockColor;
                }
            }

        //if( input.depthPosition.z  < 128)
        {
                textureColor = lerp(textureColor, stonePathTexture.Sample(SampleType, input.tex.zw), mappingColor.g / 3); //GREEN for sidewalk...
                textureColor = lerp(textureColor, sandTexture.Sample(SampleType, input.tex.zw), mappingColor.b / 3); //BLUE for sand...
                textureColor = lerp(textureColor, mudTexture.Sample(SampleType, input.tex.zw), mappingColor.r / 3); //RED for mud...

                //CH22: Apply General BUMP
                //------------------------------------------------------------------------------------------------------
                float4 generalBumpMap = generalNormalTexture.Sample(SampleType, input.tex.zw); // BUMP for "stonePathTexture"
                //textureColor = lightFunc(normapMapFunc(generalBumpMap, input), textureColor);
                textureColor += (diffuseColor / 3 * lightIntensity);
            
                //CH24: Add transparency on Cyan
                alphaValue = detailMappingTexture.Sample(SampleType, input.texMapping.xy); //x1	// This wll be our mapping texture:
                if (alphaValue.g >= 0.1f && alphaValue.b >= 0.25f)
                { // TRANSPARENT Color
                    clip(-1.0);
                    return (float4) 0;
                }

               // CH20 Flat Part:
               //------------------------------------------------------------------------------------------------------
                if (slope < 0.2f)
                {
                    float4 bumpMap = normalTexture.Sample(SampleType, input.tex.xy); //BUMP for "stonePathTexture"

                    color = stonePathTexture.Sample(SampleType, input.tex.xy);
                    //color = lightFunc(normapMapFunc(bumpMap, input), color); //Add BUMP
                    textureColor = lerp(textureColor, color, alphaValue.g * 2.5); //GREEN for stone...

                    color = sandTexture.Sample(SampleType, input.tex.xy);
                    //color = lightFunc(normapMapFunc(generalBumpMap, input), color); //Add BUMP
                    textureColor = lerp(textureColor, color, alphaValue.b); //BLUE for sand...

                    color = mudTexture.Sample(SampleType, input.tex.xy);
                    //color = lightFunc(normapMapFunc(generalBumpMap, input), color); //Add BUMP
                    textureColor = lerp(textureColor, color, alphaValue.r / 1.5f); //RED for mud...
                }
        }

        if (input.verticePosition.y < 0)
            textureColor.rgb -= 0.2f;

		//CH23:
            if (false /*!isDay*/)
            {
#if defined (FOGGED)
                fogColor.rgb -= 0.1f; // Darker Night
                textureColor = input.fogFactor * textureColor - (1.0 - input.fogFactor) * fogColor; // FOG: Calculate the final color using the fog effect equation.
#endif
                textureColor /= 2;
                color = shaderTexture.Sample(SampleType, input.texMapping.xy); //Light Map Only at night!
                textureColor = textureColor * color;
            }
#if defined (FOGGED)
            else
            {
                // The fog color equation does a linear interpolation between the texture color and the fog color based on the fog factor input value.
              
                //version 1:
                //textureColor /= 1.5;
		        //textureColor = input.fogFactor * textureColor + (1.0 - input.fogFactor) * fogColor;	// FOG: Calculate the final color using the fog effect equation.
              
                //version 2:
                float4 fog4 = 0;
                fog4.r = (1.0 - input.fogFactor);
        
                fog4.g = fog4.r;
                fog4.b = fog4.r;
                textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
                textureColor.a = 1;
            }
#endif

        if (shaderTypeParameter > 0)
        {
            textureColor.a = 1;
            textureColor *= 1.5;
        }
        
        return textureColor;
        //return float4(1,1,1,1);
    }
#if defined (FOGGED)
        else
        {
            clip(-1.0);
            return (float4) 0; // this pixel is too far away (on fog), SKIP it! CLIP IF: (x is less than zero)
        }
#endif
}
