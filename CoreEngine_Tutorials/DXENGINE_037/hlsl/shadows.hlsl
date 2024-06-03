////////////////////////////////////////////////////////////////////////////////
float4 HasShadows(float4 lightViewPosition)
////////////////////////////////////////////////////////////////////////////////
{
    float2  projectTexCoord;        //45
    float   bias = 5.0f / 256.0f;   //45: Set the bias value for fixing the floating point precision issues.

    // Calculate the projected texture coordinates
    projectTexCoord.x = lightViewPosition.x / lightViewPosition.w / 2.0f + 0.5f;
    projectTexCoord.y = -lightViewPosition.y / lightViewPosition.w / 2.0f + 0.5f;

    float depthValue = ShadowMapTextureTexture.Sample(SampleType, projectTexCoord).r;

    // is on a shadow?
    if (depthValue > 0)
    {
        // Calculate the depth of the pixel
        float lightDepthValue = lightViewPosition.z / lightViewPosition.w;

        // Compare the depth of the shadow map value and the depth of the light to determine whether to shadow or to light this pixel
        // If the light is in front of the object then light the pixel, if not then shadow this pixel since an object (occluder) is casting a shadow on it
        if (lightDepthValue > (depthValue + bias))
            return float4 (0.55f, 0.55f, 0.55f, 1); //On Shadow: return 55% of light
    }

    return 1; //Not on Shadow: return 100% of light
}