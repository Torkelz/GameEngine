#pragma pack_matrix(row_major)

cbuffer b0 : register(c0)
{
	float4x4 projection;
};
cbuffer b1 : register(c1)
{
	float4x4 view;
};

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VSOutput
{
	float4 position : SV_Position;
};

VSOutput VS(VSInput input)
{
	VSOutput output;
	output.position = mul(projection, mul(view, float4(input.position, 1)));
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	return float4(1, 0, 0, 1);
}