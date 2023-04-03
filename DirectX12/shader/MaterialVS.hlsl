cbuffer ConstantBuffer:register(b0) {
	matrix g_w;
	matrix g_wvp;
	float4 g_lightDir;
	float4 g_diffuse;
};

struct VS_IN {
	float4 pos      : POSITION;
	float3 normal   : NORMAL;
};

struct PS_IN {
	float4 pos      : SV_POSITION;
	float3 normal   : NORMAL;
};

PS_IN main(VS_IN vsin)
{
	PS_IN psin=(PS_IN)0;
	float4 pos = mul(vsin.pos,g_wvp);
	psin.pos=pos;
	psin.normal=mul(vsin.normal,(float3x3)g_w);
	return psin;
}