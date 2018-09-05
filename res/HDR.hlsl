
Texture2D<float4> linearTex : register(t0);
Texture2D<float4> originalTex : register(t1);

RWTexture2D<float4> outHdrTex : register(u0);

float3 ApplyREC2084Curve(float3 L, float maxLuminance)
{
	float m1 = 2610.0 / 4096.0 / 4;
	float m2 = 2523.0 / 4096.0 * 128;
	float c1 = 3424.0 / 4096.0;
	float c2 = 2413.0 / 4096.0 * 32;
	float c3 = 2392.0 / 4096.0 * 32;

	// L = FD / 10000, so if FD == 10000, then L = 1.
	// so to scale max luminance, we want to multiply by maxLuminance / 10000
	float maxLightScale = maxLuminance / 10000.0f;
	L *= maxLightScale;

	float3 Lp = pow(L, m1);
	return pow((c1 + c2 * Lp) / (1 + c3 * Lp), m2);
}

float3 REC709toREC2020(float3 RGB709)
{
	static const float3x3 ConvMat =
	{
		0.627402, 0.329292, 0.043306,
		0.069095, 0.919544, 0.011360,
		0.016394, 0.088028, 0.895578
	};
	return mul(ConvMat, RGB709);
}

cbuffer cb : register(b0)
{
	float brightnessScale;
	float gamma;
	bool enabled;
	float wcgScale;
};

float3 ApplyREC709Curve(float3 x)
{
	return x < 0.0181 ? 4.5 * x : 1.0993 * pow(x, 0.45) - 0.0993;
}

float3 ApplySRGBCurve(float3 x)
{
	// Approximately pow(x, 1.0 / 2.2)
	return x < 0.0031308 ? 12.92 * x : 1.055 * pow(x, 1.0 / 2.4) - 0.055;
}

float ApplySRGBCurve(float x)
{
	// Approximately pow(x, 1.0 / 2.2)
	return x < 0.0031308 ? 12.92 * x : 1.055 * pow(x, 1.0 / 2.4) - 0.055;
}

float3 RemoveSRGBCurve(float3 x)
{
	// Approximately pow(x, 2.2)
	return x < 0.04045 ? x / 12.92 : pow((x + 0.055) / 1.055, 2.4);
}

// it appears that 256 is the brightest color on the screen, so we should map that to 10,000 nits.
// though after tonemapping, it just goes to 1,1,1

static const float tenThousand = 10000.0f;


float Luminance(float3 linearColor)
{
	return 0.2126 * linearColor.r + 0.7152 * linearColor.g + 0.0722 * linearColor.b;
}

float A = 0.15;
float B = 0.50;
float C = 0.10;
float D = 0.20;
float E = 0.02;
float F = 0.30;
float W = 11.2;

float3 Uncharted2Tonemap(float3 x)
{
	return ((x*(A*x + C * B) + D * E) / (x*(A*x + B) + D * F)) - E / F;
}


float Uncharted2Tonemap(float x)
{
	return ((x*(A*x + C * B) + D * E) / (x*(A*x + B) + D * F)) - E / F;
}

[numthreads(8, 8, 1)]
void CopyHDR(uint2 dtid : SV_DispatchThreadID)
{
	float4 linearColor = linearTex[dtid];
	float4 originalColor = originalTex[dtid];

	float4 col;

	if (!enabled)
	{
		col = originalColor;
		col.rgb = RemoveSRGBCurve(col.rgb);
		col.rgb = REC709toREC2020(col.rgb);
		col.rgb = ApplyREC2084Curve(col.rgb, 180);
	}
	else
	{
//		float3 scale = 10000.0f / 256.0f;
		//float3 nits = REC709toREC2020(linearColor.rgb) * scale;
		//col.rgb = nits;
		//col.a = 1.0f;

		originalColor.rgb = RemoveSRGBCurve(originalColor.rgb);

		float pbrLuminance = Luminance(linearColor);
		float tonemappedBrightness = Luminance(originalColor);

		float3 wcgColor = originalColor.rgb;
		originalColor.rgb = REC709toREC2020(originalColor.rgb);
		
		linearColor.rgb = REC709toREC2020(linearColor.rgb);

		float toneMapRatio = pbrLuminance / tonemappedBrightness;

		toneMapRatio = lerp(1, toneMapRatio, brightnessScale);

		toneMapRatio = pow(toneMapRatio, 1.0f / gamma);
		originalColor.rgb = lerp(originalColor.rgb, wcgColor, wcgScale);

		col.rgb = ApplyREC2084Curve(originalColor.rgb * toneMapRatio, 200);

	}



	outHdrTex[dtid] = col;


}