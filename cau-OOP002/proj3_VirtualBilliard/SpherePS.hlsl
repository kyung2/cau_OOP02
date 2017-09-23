
sampler2D DiffuseSampler = sampler_state{
	Texture = (DiffuseMap);
};

struct PS_INPUT{
	float2 mTexCoord : TEXCOORD0;
};

float4 ps_main(PS_INPUT Input) : COLOR
{
	//float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	//return albedo.rgba;
	return(float4(0.0f, 1.0f, 0.0f, 1.0f));
}