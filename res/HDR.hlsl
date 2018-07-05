
Texture2D<float4> sdrTex : register(t0);
RWTexture2D<float4> outHdrTex : register(u0);

float3 ApplyREC2084Curve(float3 L)
{
	float m1 = 2610.0 / 4096.0 / 4;
	float m2 = 2523.0 / 4096.0 * 128;
	float c1 = 3424.0 / 4096.0;
	float c2 = 2413.0 / 4096.0 * 32;
	float c3 = 2392.0 / 4096.0 * 32;
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
	float4 brightnessScale;
};

[numthreads(8, 8, 1)]
void CopyHDR( uint2 dtid : SV_DispatchThreadID )
{
	float4 col = sdrTex[dtid];
	// so apparently 1,1,1, is 80 nits paperwhite, meaning this is going to be sdr looking.
	col *= brightnessScale.x;
	//col.rgb = REC709toREC2020(col.rgb);

	col.rgb = ApplyREC2084Curve(col.rgb);

	outHdrTex[dtid] = col;


}