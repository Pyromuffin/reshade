
Texture2D<float4> sdrTex : register(t0);
RWTexture2D<float4> outHdrTex : register(u0);

[numthreads(8, 8, 1)]
void CopyHDR( uint2 dtid : SV_DispatchThreadID )
{
	float4 col = sdrTex[dtid];
	// so apparently 1,1,1, is 80 nits paperwhite, meaning this is going to be sdr looking.

	outHdrTex[dtid] = col;


}