#define PS_USE_SPECULAR

cbuffer PerObject : register(b0)
{
    matrix model;
}

cbuffer PerFrame : register(b1)
{
    matrix view;
}

cbuffer PerApplication : register(b2)
{
    matrix projection;
}

cbuffer BoneTransformation : register(b3)
{
    matrix BoneTransforms[128];
}

struct VertexShaderInput
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    //float4 boneIds : BLENDINDICES; //AQUIFLOAT
    uint4 boneIds : BLENDINDICES0; //AQUIFLOAT
    float4 boneWeight : BLENDWEIGHT;
}; 

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    //float4 pos : POSITION0;
    //float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
#if defined PS_USE_SPECULAR
	float3 viewDirection		: TEXCOORD1;			// 34 Specular
	float4 cameraPosition		: WS;					// 34 Specular
#endif
};


VertexShaderOutput main(VertexShaderInput input)
{
    matrix vp;
    VertexShaderOutput OUT;
    
    bool skinningEnabled = (BoneTransforms[127]._11 == 127); //AQUI-ANIM
    if (skinningEnabled)                                      //AQUI-ANIM
    {
        matrix boneTransform = input.boneWeight[0] * BoneTransforms[input.boneIds[0]];
        for (int i = 1; i < 4; ++i)
        {
            boneTransform += input.boneWeight[i] * BoneTransforms[input.boneIds[i]];
        }
        input.position = mul(boneTransform, float4(input.position, 1.0f)).xyz;
        input.normal = normalize(mul(boneTransform, float4(input.normal, 0.0f)).xyz);
        input.tangent = normalize(mul(boneTransform, float4(input.tangent, 0.0f)).xyz);
        input.binormal = normalize(mul(boneTransform, float4(input.binormal, 0.0f)).xyz);
    }

    vp = mul(projection, view);
    //OUT.wPosition = mul(model, float4(input.position, 1.0f));
    OUT.position = mul(vp, mul(model, float4(input.position, 1.0f)));
    //OUT.pos = OUT.position;
    OUT.texCoord = input.texCoord;

    // assume a uniform scaling is observed
    // otherwise have have to multiply by transpose(inverse(model))
    // inverse should be calculated in the application (CPU)
    OUT.normal = normalize(input.normal);
    OUT.tangent = normalize(input.tangent);
    OUT.binormal = normalize(input.binormal);

    return OUT;
}


