cbuffer ConstantBuffer:register(b0) {
	matrix g_wvp;
};

float4 main(float4 Pos : POSITION) : SV_POSITION
{
	float4 pos = mul(Pos,g_wvp);
	return pos;
}