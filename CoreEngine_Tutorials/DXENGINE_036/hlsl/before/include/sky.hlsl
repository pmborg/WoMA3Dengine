#if defined PS_USE_SKY
//30: SKY
////////////////////////////////////////////////////////////////////////////////
float4 Sky(float cameraPosition, float originalPosition_y, float2 tex)
////////////////////////////////////////////////////////////////////////////////
{
	//static const float4 fogColor = float4(87.0f/256.0f, 87/256.0f, 87.0f/256.0f, 1.0f);			//31
	//static const float4 waterColor = float4(38.0f / 256.0f, 70 / 256.0f, 91.0f / 256.0f, 1.0f); //

	//if (originalPosition_y < 50) // On Lower half of sky sphere just render fog
	//	return waterColor * fade;

	//if (cameraPosition < 0)
	//	return 0; // waterColor * fade;
	//----------------------------------------------------------------------------------------------------------
	// On Upper half of sky sphere render day/night:
	float4 dayTextureColor = shaderTexture.Sample(SampleType, tex);		// GET NIGHT: shaderTexture = DAY

  #if defined PS_USE_ALFA_TEXTURE
	float4 nightTextureColor = AlfaMapTexture.Sample(SampleType, tex);  // GET DAY: AlfaMapTexture = Night
	return lerp(nightTextureColor, dayTextureColor, fade);				// Fade: 1 = Day / 0 = Night
  #else
	return dayTextureColor;
  #endif

}
#endif