cbuffer ConstantBuffer:register(b0) {
	matrix g_wvp;
};


float4 main(float4 Pos : SV_POSITION) : SV_Target
{
	//float4 pos = mul(Pos,1);
	//return pos;

	return float4(1.0f,0.0f,1.0f,1.0f);
}
