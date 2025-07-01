// --------------------------------------------------------------------------------------------
// Filename: 060Terrain.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	DirectX 11 Tutorial - World of Middle Age  - ENGINE 3D 2024
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : http://woma.servegame.com
*
/*********************************************************************************************/
//WomaIntegrityCheck = 1234525256;

//case SHADER_Terrain_Texture_DEMO60:

#define DXAPI11 1
#include "cbuffer.hlsli"

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float4 position : POSITION;		//CH04 - 
	float4 tex : TEXCOORD0;			//CH05 - TEXTURE
	float3 normal : NORMAL;			//CH12 - LIGHT	
	float4 color : COLOR;			//CH13 - MIX COLOR
	float4 texMapping : TEXCOORD1;	//CH19 - Big Mapping
    float3 tangent : TANGENT;		//CH21
    float3 binormal : BINORMAL;		//CH21
};


// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION;	 //CH04
	float4 tex : TEXCOORD0;			 //CH04 - TEXTURE
	float3 normal : NORMAL;			 //CH12 - LIGHT	

	float4 color : COLOR;			 //CH13 - MIX COLOR
	float4 texMapping : TEXCOORD1;	 //CH19

	float fogFactor : FOG;			 //CH16 - FOG
	float4 depthPosition : TEXCOORD2;//CH18
	float4 inputPosition : TEXCOORD3;//CH40: for below water...
    float3 tangent : TANGENT;		 //CH21
    float3 binormal : BINORMAL;		 //CH21
};

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh
																					//G:\DRIVE_MY_SOURCE_CODE\Dx11Engine3D\Dx11Engine3Dx64\src\terrain\terrainClass.cpp
Texture2D grassTexture :			register(t0);	//CH17	m_Texture				=Engine/data/004grass_texture.bmp
Texture2D slopeTexture :			register(t1);	//CH17  m_SlopeTexture			=Engine/data/017slope.jpg
Texture2D rockTexture  :			register(t2);	//CH17	m_RockTexture			=Engine/data/017rock.jpg
Texture2D detailTexture :			register(t3);	//CH18	m_DetailTexture			=Engine/data/017grass.jpg
Texture2D textureMappingTexture :	register(t4);	//CH19	m_textureMappingTexture =Engine/data/t_019TerrainMapping.png
Texture2D sidewalkTexture :			register(t5);	//CH19	m_sidewalkTexture		=Engine/data/019sidewalk.jpg
Texture2D sandTexture :				register(t6);	//CH19	m_sandTexture			=Engine/data/019sand.jpg
Texture2D mudTexture :				register(t7);	//CH19	m_mudTexture			=Engine/data/CrackedMud.jpg
Texture2D alphaMappingTexture:		register(t8);	//CH20	m_alphaMappingTexture	=Engine/data/t_019TerrainAlfa.png
Texture2D normalTexture:			register(t9);	//CH21	m_sidewalkTexture_bump	=Engine/data/dirt_bump.jpg
Texture2D shaderTexture:			register(t10);	//CH23	m_colorLightMapTexture	=Engine/data/t_023TerrainMapping.jpg

//SamplerState SampleType : register(s0); // 3D (default) WRAP
SamplerState SampleType; //CH05 - TEXTURE

////////////////////////////////////////////////////////////////////////////////
// Vertex Shader
////////////////////////////////////////////////////////////////////////////////
PSIn MyVertexShader060terrain_fog_slope_detail_mapping(VSIn input)
{
    PSIn output;
    float4 cameraPosition; //FOG

	//CH04- Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

	output.position = mul(input.position, WVP);	// Calculate the position of the vertex against the world, view, and projection matrices.
	output.tex = input.tex;	//CH05 - TEXTURE- Store the texture coordinates for the pixel shader.
	
	//LIGHT:
    output.normal = normalize(mul(input.normal, (float3x3)worldMatrix));// Calculate the normal vector against the world matrix only.
	output.color = input.color;	//CH04 - Send the color map color, into the pixel shader.	(*=only)

    //CH18 - Store the position value in a second input value for depth value calculations.
    output.depthPosition = output.position;

	//CH40
	output.inputPosition = input.position;
	if (input.position.y <= 0.0f) {
		output.fogFactor = 0.56f;	//give some fog underwater...
	} else {
		//CH16 - FOG - Calculate linear fog:
		cameraPosition = mul(input.position, WV);	
        output.fogFactor = saturate((VSfogEnd - cameraPosition.z) / (VSfogEnd - VSfogStart));
    }

	output.texMapping = input.texMapping;

	//CH21: Both the input tangent and binormal are calculated against the world matrix and then normalized the same as the input normal vector.
    // Calculate the tangent vector against the world matrix only and then normalize the final value.
    output.tangent = mul(input.tangent, (float3x3)worldMatrix);
    output.tangent = normalize(output.tangent);

    // Calculate the binormal vector against the world matrix only and then normalize the final value.
    output.binormal = mul(input.binormal, (float3x3)worldMatrix);
    output.binormal = normalize(output.binormal);

    return output;
}

////////////////////////////////////////////////////////////////////////////////
float3 normapMapFunc(PSIn input, float depthValue) 
{
float4 bumpMap;			//CH21
float3 bumpNormal;		//CH21

	if(depthValue < 0.95f)
	{    
		// Sample the pixel in the bump map.
		bumpMap = normalTexture.Sample(SampleType, input.tex.zw/*input.tex*/);

		// Expand the range of the normal value from (0, +1) to (-1, +1).
	    bumpMap = (bumpMap * 2.0f) - 1.0f;

		// Calculate the normal from the data in the bump map.
		bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;

	    // Normalize the resulting bump normal.
		bumpNormal = normalize(bumpNormal);
	} else {
		bumpNormal = input.normal;
	}

	return bumpNormal;
}



////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 MyPixelShader060terrain_fog_slope_detail_mapping(PSIn input) : SV_TARGET
{
	float4	textureColor;			//CH05 - TEXTURE
	float	lightIntensity;			//CH12 - LIGHT
	float4  color = ambientColor;
    float4	grassColor;				//CH17 - SLOP
    float4	slopeColor;				//CH17
    float4	rockColor;				//CH17
    float	slope;					//CH17
	float4	mappingColor;			//CH19
    float	blendAmount;			//CH17
	float4	detailColor;			//CH18
	float	detailBrightness;		//CH18
	float4	alphaValue;				//CH20
    float4	blendColor;				//CH20
		
	float	depthValue;				//CH21
	float4	textureLightColor;		//CH23
	float4	fogColor = float4(0.5f, 0.5f, 0.5f, 1.0f); //FOG

   
	//CH16 - FOG
	if (input.fogFactor > 0) {

		// Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
		depthValue = input.depthPosition.z / input.depthPosition.w;

		//CH17 - Start by sampling all three textures.
		//------------------------------------------------------------------------------------------------------
		grassColor = grassTexture.Sample(SampleType, input.tex.xy); //(x 8) Sample the grass color from the texture using the sampler at this texture coordinate location.

		// Sample the pixel color from the detail map texture using the sampler at this texture coordinate location.
		detailColor = detailTexture.Sample(SampleType, input.tex.zw); //Detail

		// Set the brightness of the detail texture.
		detailBrightness = 0.85f;

		// Combine the ground texture and the detail texture.  Also multiply in the detail brightness.
		grassColor = grassColor * detailColor * detailBrightness;
		grassColor.a = 1;

		//CH19: This wll be our mapping texture:
		mappingColor = textureMappingTexture.Sample(SampleType, input.texMapping.xy); //x1	// This wll be our mapping texture:

		//CH20:
		alphaValue = alphaMappingTexture.Sample(SampleType, input.texMapping.xy); //x1	// This wll be our mapping texture:

		if ( mappingColor.g >= 0.1f && mappingColor.b >= 0.25f) { // TRANSPARENT
			clip (-1.0); return (float4)0;
			//textureColor =  sandTexture.Sample(SampleType, input.tex.zw); //(Detail) Sample the rock color from the texture using the sampler at this texture coordinate location.
			//textureColor.a =  0;
			//return textureColor;
		} else
		if ( mappingColor.g >= 0.1f) { // GREEN for sidewalk...
			// Sample the rock color from the texture using the sampler at this texture coordinate location.
			textureColor =  sidewalkTexture.Sample(SampleType, input.tex.zw); //Detail
			textureColor = (alphaValue * textureColor) + ((1.0 - alphaValue) * grassColor);	// Combine the two textures based on the alpha value.
		} else
		if ( mappingColor.b >= 0.1f ) {	//BLUE for sand...
			textureColor =  sandTexture.Sample(SampleType, input.tex.zw); //(Detail) Sample the rock color from the texture using the sampler at this texture coordinate location.
			textureColor = textureColor * 0.75f;
			if (mappingColor.b < 0.65f)
				textureColor = (alphaValue * textureColor) + ((1.0 - alphaValue) * grassColor);	// Combine the two textures based on the alpha value.
		} else
		if ( mappingColor.r >= 0.1f) {	//RED for mud...
			textureColor = mudTexture.Sample(SampleType, input.tex.zw); //(Detail) Sample the rock color from the texture using the sampler at this texture coordinate location.
			textureColor = textureColor * 0.75f;
			textureColor = (alphaValue * textureColor) + ((1.0 - alphaValue) * grassColor) ;	// Combine the two textures based on the alpha value.
		}
		// TERRAIN:
		else
		{	
			// Sample the slope color from the texture using the sampler at this texture coordinate location.
			slopeColor = slopeTexture.Sample(SampleType, input.tex); // tex = tex.xy ?? think...yes

			// Sample the rock color from the texture using the sampler at this texture coordinate location.
			rockColor = rockTexture.Sample(SampleType, input.tex);

			//CH18 - Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
			//------------------------------------------------------------------------------------------------------

			// Calculate the slope of this point.
			slope = 1.0f - input.normal.y;

			//CH17 - Now determine the slope for this pixel, which is just one subtracted from the Y normal.
			//------------------------------------------------------------------------------------------------------

			//CH17
			// Since we have the slope we can now use it in some if statements and determine which texture to use based on the slope of the pixel. 
			// To make things look smooth we do a linear interpolation between the textures so the transition between each one 
			// isn't a sharp line in the terrain.

			// Determine which texture to use based on height.
			if(slope < 0.2f)
			{
				blendAmount = slope / 0.2f;
				textureColor = lerp(grassColor, slopeColor, blendAmount);
			}else
			if((slope < 0.7f) && (slope >= 0.2f))
			{
				blendAmount = (slope - 0.2f) * (1.0f / (0.7f - 0.2f));
				textureColor = lerp(slopeColor, rockColor, blendAmount);
			}else
			if(slope >= 0.7f) 
			{
				textureColor = rockColor;
			}
		}// TERRAIN...

		// 1- Set the color of the fog to grey.
		//------------------------------------------------------------------------------------------------------
		// CH40: 1- Set the color of the fog to grey.
		if (input.inputPosition.y <= 0.0f) {
			//textureColor =  textureColor * sandTexture.Sample(SampleType, input.tex.zw);             
							//R     G     B
			fogColor = float4(0.0f, 0.0f, 0.2f, 1.0f);
		}

		// CH21
		if ( mappingColor.b >= 0.1f ) {
			// Calculate the amount of light on this pixel based on the bump map normal value.
			//CH21 - Bump MAP
			//------------------------------------------------------------------------------------------------------
			lightIntensity = saturate(dot(normapMapFunc(input, depthValue), -lightDirection));
		} else {
			// CH06- Calculate the amount of light on this pixel.
			lightIntensity = saturate(dot(input.normal, -lightDirection));
		}

		//LIGHT: VSambientColor	+ (diffuseColor * lightIntensity)
		color += (diffuseColor * lightIntensity);	// Determine the final diffuse color based on the diffuse color and the amount of light intensity.

		textureColor.a=1.0f;						// Disable Alphablend for: Only Grass...
		color = color * textureColor;				// The texture pixel is combined with the light color to create the final color result.

		//CH23:
		if (input.inputPosition.y > 0.0f) 
		{
			textureLightColor = shaderTexture.Sample(SampleType, input.texMapping.xy);
			color = color * textureLightColor;
		}

		// 2- The fog color equation does a linear interpolation between the texture color and the fog color based on the fog factor input value.
		color = input.fogFactor * color + (1.0 - input.fogFactor) * fogColor;	// FOG: Calculate the final color using the fog effect equation.

		
		return color;
	} else {
		clip (-1.0); return (float4)0; // this fixel is too far away (on fog), SKIP it! CLIP IF: (x is less than zero)
	}
}
