// --------------------------------------------------------------------------------------------
// Filename: 077GSinstance.hlsl
// --------------------------------------------------------------------------------------------
/**********************************************************************************************
*	World of Middle Age (WoMA) - 3D Multi-Platform ENGINE 2025
*	-------------------------------------------------------------------------------------------
*	code by : Pedro Borges - pmborg@yahoo.com
*	Downloaded from : https://github.com/pmborg/WoMA3Dengine
*
**********************************************************************************************/
//WomaIntegrityCheck = 1234525217;

#define PS_USE_ALFACOLOR
#define PS_USE_ALFA_TEXTURE

//////////////
// TYPEDEFS //
//////////////

// VERTEX:
struct VSIn
{
	float3 position         : POSITION;				//21
	float2 texCoords        : TEXCOORD0; 			//22
	float3 normal	        : NORMAL;				//23
    float3 instancePosition : INSTANCEPOS;
    //float  rotY             : TEXCOORD1;
};

//GEOMETRY:
struct GSIn
{
    float4 position         : SV_POSITION;  // 21
	//float4 worldPos 		: POSITION;
    float2 texCoords        : TEXCOORD0;    // 22
    float3 normal           : NORMAL;       // 23 & 47: LIGHT+BUMP
	//float3 tangent 		: TANGENT;
#if defined PS_USE_SPECULAR
    float3 viewDirection    : TEXCOORD1;    // 44 Specular
#endif
    float4 cameraPosition   : WS; // FOG & SPECULAR
};

// PIXEL:
struct PSIn
{
    float4 position			: SV_POSITION;			// 21
	//float4 worldPos 		: POSITION;
    float2 texCoords		: TEXCOORD0;			// 22
	float3 normal			: NORMAL;				// 23 & 47: LIGHT+BUMP
	//float3 tangent 		: TANGENT;
#if defined PS_USE_SPECULAR
	float3 viewDirection	: TEXCOORD1;			// 44 Specular
#endif
	float4 cameraPosition	: WS;					// FOG & SPECULAR
};

/////////////
// GLOBALS //
/////////////
//Set on: DXmodelClass::RenderSubMesh

Texture2D shaderTexture; //: register(t0);	// 21:
Texture2D AlfaMapTexture; //: register(t1);	// 43: AlfaMap

SamplerState SampleType; //: register(s0);		// 3D (default) WRAP


////////////////
// CBUFFERS
////////////////
#include "cbuffer.hlsli"
#include "light.hlsli"

////////////////////////////////////////////////////////////////////////////////
// VERTEX SHADER
////////////////////////////////////////////////////////////////////////////////
GSIn VS_Main(VSIn input, uint instanceID : SV_InstanceID)
{
    GSIn output;
	float4 cameraPosition;

	// _m00, _m01,_m02, _m03
	// _m10, _m11,_m12, _m13
	// _m20, _m21,_m22, _m23
	// _m30, _m31,_m32, _m33

	matrix <float, 4, 4> rotationAroundY =
	{
		{ 1, 0, 0, 0 },		// cos(VSrotY),     0.0f,   -sin(VSrotY), 0.0f,
		{ 0, 1, 0, 0 },		// 0.0f,            1.0f,   0.0f,         0.0f,
		{ 0, 0, 1, 0 },		// sin(VSrotY),     0.0f,   cos(VSrotY),  0.0f,
		{ 0, 0, 0, 1 }		// 0.0f,            0.0f,   0.0f,         1.0f
	};
   
    float rY = VSrotY;
    //if (input.rotY != 0)
    //    rY = input.rotY;
    
    rotationAroundY[0].x =  cos(rY);
	rotationAroundY[0].z = -sin(rY);
	rotationAroundY[2].x =  sin(rY);
    rotationAroundY[2].z =  cos(rY);
    
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
	//if (VS_USE_WVP) {
	//	output.position = mul(float4(input.position, 1), WVP);	// Calculate the position of the vertex against the world, view, and projection matrices
	//}
	//else {
		position = mul(position, worldMatrix);
		position = mul(position, view);			//viewMatrix
		position = mul(position, projection);	//projectionMatrix
		output.position = position;
	//}

	//22: TEXTURE: Store the texture coordinates for the pixel shader:
	output.texCoords = input.texCoords;

	cameraPosition = mul(float4(input.position, 1), WV);								// FOG: Calculate the camera position.

	//23: LIGHT: NORMAL (we need to recalculate normals due rotations)
    if (VShasLight || VShasSpecular)
    {
        matrix world = worldMatrix * rotationAroundY;
        world._m30 = position.x;
        world._m31 = position.y;
        world._m32 = position.z;
       
        output.normal = normalize(mul(input.normal, (float3x3) world));
    }
    
	//34: SPECULAR
    output.cameraPosition = cameraPosition;
    
#if defined PS_USE_SPECULAR
	if (VShasSpecular)	// If enabled on material, calculate the Specular LIGHT
	{
		float4 worldPosition = mul(float4(input.position, 1), worldMatrix);			// P
		output.viewDirection = normalize(cameraPosition.xyz - worldPosition.xyz);	// L = Lp - p (L = lightDirection)
	}
#endif

	return output;
}

[maxvertexcount(3)]
void GS_Main(triangle GSIn input[3], inout TriangleStream<PSIn> triStream)
{
    PSIn output;
    float3 camPos = mul(input[0].position, WV).xyz;

    //1: Instance, too distante dont render:
    // Compute average distance of the triangle from the camera
    float3 triCenter = (input[0].position.xyz + input[1].position.xyz + input[2].position.xyz) / 3.0f;
    float dist = distance(camPos, triCenter);
    if (dist > 2000.0f)
        return; // Do not emit this triangle

    //2: Instance, Out of camera dont Render:
	//take the cross product of the input triangle edges in world space: 
    float3 wV0 = input[1].position.xyz - input[0].position.xyz;
    float3 wV1 = input[2].position.xyz - input[0].position.xyz;
    float3 wNormal = normalize(cross(wV0, wV1));
    if (dot(camPos, wNormal) <= 0.0f)
        return;
    
    // Emit triangle as usual
    [unroll]
    for (int i = 0; i < 3; ++i)
    {
        output.position = input[i].position;
        output.texCoords = input[i].texCoords;
        output.normal = input[i].normal;
#if defined PS_USE_SPECULAR
        output.viewDirection = input[i].viewDirection;
#endif
        output.cameraPosition = input[i].cameraPosition;
        triStream.Append(output);
    }

    triStream.RestartStrip();
}

////////////////////////////////////////////////////////////////////////////////
// PIXEL SHADER
////////////////////////////////////////////////////////////////////////////////
float4 PS_Main(GSIn input) : SV_TARGET
{
	float4	textureColor = pixelColor;    // SET PIXEL COLOR
	float	lightIntensity = 0;

	//-----------------------------------------------------------------------------------
	// 21 & 41: TEXTURE: Sample the pixel color from the texture using the sampler at this texture coordinate location
	if (hasTexture) 
		textureColor = shaderTexture.Sample(SampleType, input.texCoords);

    if (textureColor.r <= 0.1f && textureColor.g <= 0.1f && textureColor.b <= 0.1f)	// If the color is black on the texture then treat this pixel as transparent.
        discard;
    
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
    
    //return float4(1,1,1,1);
    
    if (fade < 1)
        textureColor.rgb *= fade;
    return textureColor;
}
    
    
    
