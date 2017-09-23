matrix gLocalMatrix : Local;
matrix gWorldMatrix : World;
matrix gViewMatrix : View;
matrix gProjectionMatrix : Projection;

float4 gWorldCameraPosition;
float4 gWorldLightPosition;
float gSpeedVectorU;
float gSpeedVectorV;
struct VS_INPUT
{
	float4 mPosition : POSITION;
	float3 Normal : NORMAL0;
	float2 mTexCoord : TEXCOORD0;

};

struct VS_OUTPUT
{
	float4 mPosition : POSITION;
	float2 mTexCoord : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;

};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.mPosition = mul(Input.mPosition, gLocalMatrix);
	Output.mPosition = mul(Output.mPosition, gWorldMatrix);

	float3 lightDir = Output.mPosition.xyz - gWorldLightPosition.xyz;
	lightDir = normalize(lightDir);

	float3 viewDir = normalize(Output.mPosition.xyz - gWorldCameraPosition.xyz);

	Output.mViewDir = viewDir;


	Output.mPosition = mul(Output.mPosition, gViewMatrix);
	Output.mPosition = mul(Output.mPosition, gProjectionMatrix);

	float3 worldNormal = mul(Input.Normal, (float3x3)gWorldMatrix);
	worldNormal = normalize(worldNormal);
	Output.mDiffuse = dot(-lightDir, worldNormal);
	Output.mReflection = reflect(lightDir, worldNormal);
	//Output.mTexCoord = Input.mTexCoord + float2(gSpeedVectorU, gSpeedVectorV);

	//Input.mPosition.w = 1.0f;
	//Output.mDiffuse = Input.Normal * dot(-lightDir, worldNormal);
	Output.mTexCoord = Input.mTexCoord;
	return Output;
}

texture DiffuseMap : TEX2D;
sampler DiffuseSampler = sampler_state
{
	Texture = (DiffuseMap);
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = MIRROR;
	AddressV = MIRROR;
	
};

struct PS_INPUT{
	float2 mTexCoord : TEXCOORD0;
	float3 mDiffuse : TEXCOORD1;
	float3 mViewDir : TEXCOORD2;
	float3 mReflection : TEXCOORD3;
};

float3 gLightColor;

float4 ps_main(PS_INPUT Input) : COLOR
{
	//float3 diffuse = saturate(Input.mDiffuse);

	//float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	//return albedo * float4(diffuse, 1);
	//return float4(0.0f, 0.0f, 0.0f, 1.0f);
	float4 albedo = tex2D(DiffuseSampler, Input.mTexCoord);
	float3 diffuse = albedo.rgb * saturate(Input.mDiffuse);

	float3 reflection = normalize(Input.mReflection);
	float3 viewDir = normalize(Input.mViewDir);

	float3 specular = 0;

	if (diffuse.x > 0){
		specular = saturate(dot(reflection, -viewDir));
		specular = pow(specular, 20.0f);
	}

	float3 ambient = float3(0.5f, 0.5f, 0.5f) * albedo;

	return float4(ambient + diffuse + specular, 1);



}


//--------------------------------------------------------------//
// Technique Section for ColorShader
//--------------------------------------------------------------//
technique ColorShader
{
	pass Pass_1
	{
		VertexShader = compile vs_3_0 vs_main();
		PixelShader = compile ps_3_0 ps_main();
	}

}
