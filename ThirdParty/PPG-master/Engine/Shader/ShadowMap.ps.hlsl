struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float4 pos : POSITION0;
    float4 wPosition : POSITION1;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
    float2 texCoord : TEXCOORD0;
};

float4 main(PixelShaderInput IN) : SV_TARGET
{
    float depthValue = IN.pos.z / IN.pos.w;
    // Get the depth value of the pixel by dividing the Z pixel depth by the homogeneous W coordinate.
    float4 color = float4(depthValue, depthValue, depthValue, 1.0f);
    return color;
}
