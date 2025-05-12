static const float2 arrBasePos[4] =
{
    float2(-1.0, 1.0),
    float2(1.0, 1.0),
    float2(-1.0, -1.0),
    float2(1.0, -1.0),
};
static const float2 arrUV[4] =
{
    float2(0.0, 0.0),
    float2(1.0, 0.0),
    float2(0.0, 1.0),
    float2(1.0, 1.0),
};

struct FullscreenVsOutput
{
    float4 position: SV_POSITION; // vertex position
    float2 texCoord: TEXCOORD0;
};

FullscreenVsOutput main(uint VertexID : SV_VertexID)
{
    FullscreenVsOutput output;
    output.position = float4(arrBasePos[VertexID].xy, 0.0, 1.0);
    output.texCoord = arrUV[VertexID].xy;
    return output;
}