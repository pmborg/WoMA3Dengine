//WomaIntegrityCheck = 1234525217;

struct VertexInputType
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 tex : TEXCOORD;
    //float4 tangent : TANGENT; //AQUIT
    float4 boneWeight : WEIGHTS;
    uint4 boneIds : BONEINDICES;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float4 teste : TEXCOORD1;
};

cbuffer ConstantBuffer// : register(b0) //cbPerObject
{
    float OPanimation;
    float pad1;
    float pad2;
    float pad3;
    
    matrix worldMatrix; //1
    matrix viewMatrix; //2
    matrix projectionMatrix; //3    
    
    //matrix gWorldInvTranspose;  //4
    //matrix gWorldViewProj;      //5
    //matrix gWorldViewProjTex;   //6
    
    //matrix gTexTransform;       //7
    //matrix gShadowTransform;    //8
    
    // Material:
    //float4 Ambient;
    //float4 Diffuse;
    //float4 Specular; // w = SpecPower
    //float4 Reflect;
    
    matrix BoneTransforms[128];
}

PixelInputType mainVS(VertexInputType input)
{
    PixelInputType output;
    
    //input.position.w = 1;
    
    //V1:
/*
	// Init array or else we get strange warnings about SV_POSITION.
    float weights[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
    weights[0] = input.boneWeight.x;
    weights[1] = input.boneWeight.y;
    weights[2] = input.boneWeight.z;
    weights[3] = input.boneWeight.w;
    //weights[3] = 1.0f - weights[0] - weights[1] - weights[2];
    
	// Apply bone transforms based on indices and weights
    float3 transformedPosition = float3(0.0, 0.0, 0.0);
    float3 transformedNormal = float3(0.0, 0.0, 0.0);

	// Skinning: Iterate over the maximum number of bones (4 for each vertex in this case)
    for (int i = 0; i < 4; i++)
    {
		// Get bone matrix based on bone index
        matrix boneMatrix = BoneTransforms[input.boneIds[i]];

		// Apply bone transformation with the associated weight
        transformedPosition += weights[i] * mul(input.position, boneMatrix);
        transformedNormal += weights[i] * mul(input.normal, (float3x3) boneMatrix);
    }
    float4 pos = float4(transformedPosition, 1.0f);
 */

    //V2:
    float4 pos;// = float4(0, 0, 0, 0);
    if (OPanimation > 0)
    {
        //matrix boneTransform = BoneTransforms[input.boneIds[0]] * input.boneWeight[0]
        //+ BoneTransforms[input.boneIds[1]] * input.boneWeight[1];
        //+ BoneTransforms[input.boneIds[2]] * input.boneWeight[2];
        //+ BoneTransforms[input.boneIds[3]] * input.boneWeight[3];
        matrix boneTransform = input.boneWeight[0] * BoneTransforms[input.boneIds[0]];
        for (int i = 1; i < 4; ++i)
        {
            boneTransform += input.boneWeight[i] * BoneTransforms[input.boneIds[i]];
        }
        
        //NEW???
        pos = mul(float4(input.position, 1), boneTransform);
        //pos = mul(boneTransform, float4(input.position, 1));
    }
    else
    {
        pos = float4(input.position, 1);
    }
    
    output.position = mul(mul(mul(pos, worldMatrix), viewMatrix), projectionMatrix);
    //NEW????
    //output.position = mul(mul(pos, viewMatrix), projectionMatrix);
    
    //output.normal = normalize(transformedNormal); // Store the texture coordinates for the pixel shader.
    output.tex = input.tex;
    
    return output;
}

/////////////
// GLOBALS //
/////////////
Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

float4 mainPS(PixelInputType input) : SV_TARGET
{
    float4 textureColor = shaderTexture.Sample(SampleType, input.tex);

    //return float4(1,1,1,1);
    return textureColor;
}