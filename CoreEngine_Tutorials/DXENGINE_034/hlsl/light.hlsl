////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc1(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, -lightDirection));							// Calculate the amount of light on this pixel
}
////////////////////////////////////////////////////////////////////////////////
float4 PSlightFunc2(float3 Normal)
////////////////////////////////////////////////////////////////////////////////
{
	return saturate(dot(Normal, lightDirection));							// Calculate the amount of light on this pixel
}