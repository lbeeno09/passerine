cbuffer MeshConstantBuffer : register(b0)
{
	matrix world;
	matrix view;
	matrix projection;
};

struct VS_INPUT
{
	float3 pos : POSITION;
	float4 color : COLOR;
};

struct PS_INPUT
{
	float4 pos : SV_POSITION;
	float4 color : COLOR;
};

PS_INPUT VSMain(VS_INPUT input)
{
	PS_INPUT output;
	float4 pos = float4(input.pos, 1.0f);
	pos = mul(pos, world);
	pos = mul(pos, view);
	pos = mul(pos, projection);
	output.pos = pos;
	output.color = input.color;
	return output;
}

float4 PSMain(PS_INPUT input) : SV_TARGET
{
	return input.color;
}