// --------------------------------------------------------------------------------------------
// Filename: 099Terrain.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2023
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#if (!defined DXAPI11 && !defined DXAPI12)
    #define DXAPI11 1
#endif

#define FOGGED
#define PS_USE_SHADOWMAP_TEXTURE

//////////////
// TYPEDEFS //
//////////////

//VERTEX:
struct VSIn
{
    float4 position : POSITION;     // 
    float4 tex : TEXCOORD0;         // TEXTURE
    float3 normal : NORMAL;         // LIGHT	
    float4 color : COLOR;           // MIX COLOR
    float4 texMapping : TEXCOORD1;  // Big Mapping
    float3 tangent : TANGENT;       // 
    float3 binormal : BINORMAL;     // 
};

//PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;                  // 
    float4 tex : TEXCOORD0;                         // TEXTURE
    float3 normal : NORMAL;                         // LIGHT	
    float4 color : COLOR;                           // MIX COLOR
    float fogFactor : FOG;                          // FOG
    float4 depthPosition : TEXCOORD2;               // 
    float4 texMapping : TEXCOORD1;                  // 
    float3 tangent : TANGENT;                       // Normals
    float3 binormal : BINORMAL;                     // Normals
    float4 verticePosition : TEXCOORD3;             // (below water)...
    float4 lightViewPosition : LIGHT_VIEW_POSITION; // 99 SHADOWS
};

/////////////
// GLOBALS //
/////////////
//Texture2D shaderTexture;  //CH05 - TEXTURE

//CH17&CH18 - There are three textures we will be using for the three different degrees of slope that we want to handle.
Texture2D grassTexture : register(t0);          //CH17 - T 017grass             (Dirty Grass)         N normal001*
Texture2D slopeTexture : register(t1);          //CH17 - T 017slope             (Mixture Slop)        N normal001*
Texture2D rockTexture : register(t2);           //CH17 - T rock02_savdf         (Black Rock)          N normal001*

Texture2D detailTexture : register(t3);         //CH18 - TN grassTexture        (Grass with BUMP)     t11*
Texture2D textureMappingTexture : register(t4); //CH19 - MAP t_019TerrainMapping(General Mapping)     -
Texture2D stonePathTexture : register(t5);      //CH19 - T stone001             (Stone walk path)     t9*
Texture2D sandTexture : register(t6);           //CH19 - T 019sandV2            (Sand to beach)       N normal001*
Texture2D mudTexture : register(t7);            //CH19 - T CrackedMudV2         (Mud for "Estabulo")  N normal001*
Texture2D detailMappingTexture : register(t8);  //CH20 - MAP t_019TerrainMappingV2
Texture2D normalTexture : register(t9);         //CH21 - N stone001normal       t9*

Texture2D generalNormalTexture : register(t10); //CH22 - N normal001            *

Texture2D bigPathMappingTexture: register(t11); //CH22 - N grassNormal          t11*
Texture2D smallstonePathTexture: register(t12); //CH22 - T 056B_castle.jpg
Texture2D shaderTexture        : register(t13); //CH23 - MAP colorLightMapTexture

Texture2D LightMapTexture      : register(t14); //LIGHTMAP LightMapTexture

Texture2D ShadowMapTexture     : register(t15); //next free slot after terrain textures

// Normal sampler for all normal textures
SamplerState SampleType : register(s0);
// Comparison sampler ONLY for shadow-map
SamplerComparisonState ShadowSampler : register(s2);


////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"
#include "shadows.hlsli"

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
PSIn VS_Main(VSIn input)
{
    PSIn output;
    float4 cameraPosition; //FOG
    input.position.w = 1.0f; //CH4 Change the position vector to be 4 units for proper matrix calculations.

    // 1. Store original (for shadows)
    float4 originalLocal = input.position;
    
    // 2. Apply underwater deformation for rendering only
    float4 renderLocal = input.position;
    if (renderLocal.y < 0)
        renderLocal.y *= 3;
    
    // 3. World positions
    float4 worldOriginal = mul(originalLocal, worldMatrix); // correct for shadow map
    float4 worldRender = mul(renderLocal, worldMatrix); // correct for final rendering

    // 4. Compute world positions
    output.position = mul(renderLocal, WVP); //Calculate the position of the vertex against the world, view, and projection matrices.

    // 5. NORMALS, tangent, binormal use deformation too
    output.normal = normalize(mul(input.normal, (float3x3) worldMatrix)); //Calculate the normal vector against the world matrix only.
    output.tangent = normalize(mul(input.tangent, (float3x3) worldMatrix)); //Calculate the tangent vector against the world matrix only and then normalize.
    output.binormal = normalize(mul(input.binormal, (float3x3) worldMatrix)); //Calculate the binormal vector against the world matrix only and then normalize.

    // 6. Fog depth must use *rendered* world position
    cameraPosition = mul(input.position, WV); //FOG: Calculate the camera position.
#if defined (FOGGED)
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart)); // Calculate linear fog.  
#endif    
    
    // 7. Texture mapping (unchanged)
    output.tex = input.tex; //CH05 - TEXTURE- Store the texture coordinates for the pixel shader.
    output.texMapping = input.texMapping; //CH19 Texture Mapping
    output.color = input.color; //CH13 - MIX COLOR - Send the color map color, into the pixel shader.
    output.depthPosition = cameraPosition; //CH18 - Store the position value in a second input value for depth value calculations.
    

    // 8. This is used by: if(input.verticePosition.y < 0)
    //    must use the *rendered* position or fog/underwater breaks
    output.verticePosition = worldRender;

    #if defined PS_USE_SHADOWMAP_TEXTURE
        //output.lightViewPosition = mul(mul(input.position, worldMatrix), ViewToLightProj);
        output.lightViewPosition = mul(worldOriginal, ViewToLightProj);
    #endif
    
    return output;
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
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
                    textureColor = lerp(textureColor, color, alphaValue.g * 2.5); //GREEN for stone...

                    color = sandTexture.Sample(SampleType, input.tex.xy);
                    textureColor = lerp(textureColor, color, alphaValue.b); //BLUE for sand...

                    color = mudTexture.Sample(SampleType, input.tex.xy);
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
                float4 fog4 = 0;
                fog4.r = (1.0 - input.fogFactor);
                fog4.g = fog4.r;
                fog4.b = fog4.r;
                textureColor.rgb = lerp(textureColor.rgb, fogColor.rgb, fog4.rgb);
                textureColor.a = 1;
            }
#endif

#if defined PS_USE_SHADOWMAP_TEXTURE//36
        //if (castShadow)
        {
            float shadow = HasShadows(input.lightViewPosition);
            textureColor.rgb = textureColor.rgb * shadow;
        }
#endif
        
        if (shaderTypeParameter > 0)
        {
            textureColor.a = 1;
            textureColor *= 1.5;
        }
        
        if (fade < 1)
        {
            textureColor.rgb *= fade*2;                                                                   //98
            float4 m_LightMapTexture = LightMapTexture.Sample(SampleType, input.texMapping.xy); //x1    //98
            return textureColor * m_LightMapTexture;                                                    //98
        }
        
        return textureColor;
	}
#if defined (FOGGED)
    else
    {
        clip(-1.0);
        return (float4) 0; // this pixel is too far away (on fog), SKIP it! CLIP IF: (x is less than zero)
    }
#endif
}


// VS input/out already defined by you
PSIn VS_Main__(VSIn input)
{
    PSIn o;
    input.position.w = 1.0f;

    float4 localOriginal = input.position; // used for shadow projection
    float4 localRender = input.position; // used for final rendering
    if (localRender.y < 0)
        localRender.y *= 3; // underwater deformation only for rendered world

    // Final clip pos (rendered)
    o.position = mul(localRender, WVP); // keep this — it fixed your screen.

    // world positions
    float4 worldOriginal = mul(localOriginal, worldMatrix);
    float4 worldRender = mul(localRender, worldMatrix);

    // normals/tangent/binormal
    o.normal = normalize(mul(input.normal, (float3x3) worldMatrix));
    o.tangent = normalize(mul(input.tangent, (float3x3) worldMatrix));
    o.binormal = normalize(mul(input.binormal, (float3x3) worldMatrix));

    // fog usage (use worldRender for fog)
    float4 camPos = mul(worldRender, view);
    camPos = mul(camPos, projection);
    o.fogFactor = saturate((VSfogEnd - camPos.z) / (VSfogEnd - VSfogStart));
    o.depthPosition = camPos;

    // UVs, mapping, color, etc
    o.tex = input.tex;
    o.texMapping = input.texMapping;
    o.color = input.color;
    o.verticePosition = worldRender;

    // Important: compute light-projection using the SAME world positions that were used when writing the shadow map
#if defined(PS_USE_SHADOWMAP_TEXTURE)
    o.lightViewPosition = mul(worldOriginal, ViewToLightProj); // worldOriginal if your shadow-map was made without deformation
#endif

    return o;
}
