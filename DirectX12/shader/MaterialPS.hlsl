cbuffer ConstantBuffer:register(b0) {
	matrix g_w;
	matrix g_wvp;
	float4 g_lightDir;
	float4 g_diffuse;
};


struct PS_IN {
	float4 pos      : SV_POSITION;
	float3 normal   : NORMAL;
};

float4 main(PS_IN psin) : SV_Target
{

	float3 normal = normalize(psin.normal);
	float3 light = normalize(g_lightDir);
	//“àÏ‚ğ‹‚ß‚Ä‚¢‚é
	float t = dot(psin.normal,light);

	if (t < 0.0f) {
		t = 0.0f;
	}


	float4 final = g_diffuse;
	final *= t;
	//float4 final = g_diffuse;
	return final;
}
