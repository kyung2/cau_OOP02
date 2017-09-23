float4x4 gWorld;
float4x4 gView;
float4x4 gProj;

struct VS_INPUT{
	float4 mPosition : POSITION;
	//float4 mTexCoord : TEXCOORD0;
};

struct VS_OUTPUT{
	float4 mPosition : POSITION;
	//float4 mTexCoord : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.mPosition = mul(Input.mPosition, gWorld);
	Output.mPosition = mul(Input.mPosition, gView);
	Output.mPosition = mul(Input.mPosition, gProj);

	//Output.mTexCoord = Input.mTexCoord;

	return Output;
}