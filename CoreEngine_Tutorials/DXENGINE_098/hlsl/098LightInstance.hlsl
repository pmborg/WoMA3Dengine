// --------------------------------------------------------------------------------------------
// Filename: 098LightInstance.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
*	-------------------------------------------------------------------------------------------
*	Code by  : Pedro Borges - pmborg@yahoo.com
*   Lights by: ChatGPT
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#define PS_USE_LIGHT		 //23
#define PS_USE_ALFA_TEXTURE	 //33
#define PS_USE_ALFACOLOR 	 //33
#define PS_USE_SPECULAR		 //34

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
    float3 position : POSITION; //21
    float2 texCoords : TEXCOORD; //22
    float3 normal : NORMAL; //23
    float3 instancePosition : INSTANCEPOS; //40: D3D11_INPUT_PER_INSTANCE_DATA
};

// PIXEL:
struct PSIn
{
    float4 position : SV_POSITION; // 21
    float2 texCoords : TEXCOORD0; // 22
    float3 normal : NORMAL; // 23 & 47: LIGHT+BUMP
    float4 cameraPosition : WS; // FOG & SPECULAR       input * WV
    float3 viewDirection : TEXCOORD1; // 44 Specular
    float3 worldPos : TEXCOORD2; // 98
};

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh

Texture2D shaderTexture; //: register(t0);	// 21:
Texture2D AlfaMapTexture; //: register(t1);	// 43: AlfaMap

SamplerState SampleType; //: register(s0);	// 3D (default) WRAP


////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"  // b0 and b1
#include "light98.hlsli"  // light functions for 98


////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
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

    //matrix<float, 4, 4> rotationAroundY =
    //{
    //    { 1, 0, 0, 0 }, //cos(VSrotY), 0.0f, -sin(VSrotY), 0.0f,
    //    { 0, 1, 0, 0 }, //0.0f, 1.0f, 0.0f, 0.0f,
    //    { 0, 0, 1, 0 }, //sin(VSrotY), 0.0f, cos(VSrotY), 0.0f,
    //    { 0, 0, 0, 1 } //0.0f, 0.0f, 0.0f, 1.0f
    //};
    //rotationAroundY[0].x = cos(VSrotY);
    //rotationAroundY[0].z = -sin(VSrotY);
    //rotationAroundY[2].x = sin(VSrotY);
    //rotationAroundY[2].z = cos(VSrotY);
    
    
    
    // 1) Rotate the vertex in object space
    float c = cos(VSrotY);
    float s = sin(VSrotY);
    float4x4 rotationAroundY =
    {
        c, 0, -s, 0,
        0, 1, 0, 0,
        s, 0, c, 0,
        0, 0, 0, 1
    };

    
    
    float4 position = float4(input.position, 1.0f);
    position = mul(position, rotationAroundY);

    float3 pos;
    pos.x = input.instancePosition.x * (1 / worldMatrix._m00);
    pos.y = input.instancePosition.y * (1 / worldMatrix._m11);
    pos.z = input.instancePosition.z * (1 / worldMatrix._m22);
    
	//[2nd] TRANSLATE the instance
	// Update the position of the vertices based on the data for this particular instance.
    position.x += pos.x;
    position.y += pos.y;
    position.z += pos.z;

	//[3th] PRESPECTIVE the instance
	// POSITION: Calculate the position of the vertex against the world, view, and projection matrices
    if (VS_USE_WVP)
    {
        position = mul(position, WVP); // Calculate the position of the vertex against the world, view, and projection matrices
    }
    else
    {
        position = mul(position, worldMatrix);
        position = mul(position, view); //viewMatrix
        position = mul(position, projection); //projectionMatrix
    }

    output.position = position;
    
	//22: TEXTURE: Store the texture coordinates for the pixel shader:
    output.texCoords = input.texCoords;

	

	//23: LIGHT: NORMAL (we need to recalculate normals due rotations)
    matrix world = worldMatrix * rotationAroundY;
    if (VShasLight || VShasSpecular)
    {
        world._m30 = position.x;
        world._m31 = position.y;
        world._m32 = position.z;
       
        output.normal = normalize(mul(input.normal, (float3x3) world));
    }
    
	//34: SPECULAR
#if defined PS_USE_SPECULAR
    cameraPosition = mul(float4(input.position, 1), WV); // FOG: Calculate the camera position.
    output.cameraPosition = cameraPosition;
    float4 worldPosition = position; //mul(float4(input.position, 1), world); // P
    //worldPosition.xyz += pos;
    output.worldPos = worldPosition;
    
    if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
    {
        output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz); // L = Lp - p (L = lightDirection)
    }
#endif

    return output;
}


////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(PSIn input) : SV_TARGET
{
    float4 textureColor = pixelColor; // SET PIXEL COLOR
    float lightIntensity = 0;
    
    //return float4(length(input.worldPos) / 300.0, 0, 0, 1);
    
	//-----------------------------------------------------------------------------------
	// 23: LIGHT
    if (hasLight)
    {
        if (lightType == 1)	
            lightIntensity = PSlightFunc1(input.normal);
        else
            lightIntensity = PSlightFunc2(input.normal);

        //float plIntensity = ComputePointLights(input.worldPos, normalize(input.normal));
        float plIntensity = ComputeSpotLights(input.cameraPosition, normalize(input.normal));
        
        //return float4(plIntensity, plIntensity, plIntensity, 1);
        float totalLight = saturate(emissiveColor + ambientColor + lightIntensity + plIntensity);
        
        if (hasTexture)
        {
            textureColor.rgb = shaderTexture.Sample(SampleType, input.texCoords) * totalLight;
        }
        else
        {
            textureColor.rgb = diffuseColor * totalLight;
        }
    }

#if defined PS_USE_ALFA_TEXTURE // 33: Alfa Map: (Optional AlfaMap for blending textures)
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
            float fPhoneValue = saturate(dot(Reflection, input.viewDirection)); // (R.V)
            float4 specular = pow(fPhoneValue, nShininess); // Ls = (R.V)^alfa (alfa Determine the amount of specular light based on the reflection vector, viewing direction, and specular power.)

            color = color * textureColor;
            textureColor = saturate(textureColor + specular); // specular = Ls (contribution of the light source) * Ks (specular component of the material)
        }
    }

#endif

    return textureColor;
}