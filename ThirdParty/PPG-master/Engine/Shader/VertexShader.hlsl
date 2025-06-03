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

struct AppData
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 texCoord : TEXCOORD;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float4 boneIds : BLENDINDICES; //AQUIFLOAT
    //uint4 boneIds : BLENDINDICES0; //AQUIFLOAT
    float4 boneWeight : BLENDWEIGHT;
};

struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float4 pos : POSITION0;
    float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

VertexShaderOutput main(AppData input)
{
    //PB
    //bool skinningEnabled = (BoneTransforms[127]._11 > 0);
    //if (skinningEnabled)
    {
        matrix boneTransform = input.boneWeight[0] * BoneTransforms[(uint) input.boneIds[0]];
        for (int i = 1; i < 4; ++i)
        {
            boneTransform += input.boneWeight[i] * BoneTransforms[(uint) input.boneIds[i]];
        }
        
        input.position = mul(boneTransform, float4(input.position, 1.0f)).xyz;
        input.normal = normalize(mul(boneTransform, float4(input.normal, 0.0f)).xyz);
        input.tangent = normalize(mul(boneTransform, float4(input.tangent, 0.0f)).xyz);
        input.binormal = normalize(mul(boneTransform, float4(input.binormal, 0.0f)).xyz);
    }

    VertexShaderOutput OUT;
    OUT.wPosition = mul(model, float4(input.position, 1.0f));
    matrix vp = mul(projection, view);
    OUT.position = mul(vp, OUT.wPosition);
    OUT.pos = OUT.position;
    OUT.texCoord = input.texCoord;

    // assume a uniform scaling is observed
    // otherwise have have to multiply by transpose(inverse(model))
    // inverse should be calculated in the application (CPU)
    OUT.normal = normalize(input.normal);
    OUT.tangent = normalize(input.tangent);
    OUT.binormal = normalize(input.binormal);

    return OUT;
}
