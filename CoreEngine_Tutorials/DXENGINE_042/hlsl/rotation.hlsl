float3 RotateAroundYInDegrees(float3 vertex, float degrees)
{
	float alpha = degrees * 3.14159265358979323846f / 180.0;
	float sina, cosa;
	sincos(alpha, sina, cosa);
	float2x2 m = float2x2(cosa, -sina, sina, cosa);
	return float3(mul(m, vertex.xz), vertex.y).xzy;
}


float3 RotateAroundZInDegrees(float3 vertex, float degrees)
{
	float alpha = degrees * 3.14159265358979323846f / 180.0;
	float sina, cosa;
	sincos(alpha, sina, cosa);
	float2x2 m = float2x2(cosa, -sina, sina, cosa);
	return float3(mul(m, vertex.xy), vertex.z).zxy;
}