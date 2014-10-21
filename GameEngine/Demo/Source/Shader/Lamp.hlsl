#pragma pack_matrix(row_major)

SamplerState textureSampler : register(s0);
Texture2D diffuseTex : register(t0);

cbuffer b0 : register(c0)
{
	float4x4 projection;
};
cbuffer b1 : register(c1)
{
	float4x4 view;
};
cbuffer b2 : register(c2)
{
	float4x4 world;
};

struct VSInput
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

struct VSOutput
{
	float4 position : SV_Position;
	float3 wpos : POSITION;
	float3 normal : NORMAL;
	float2 texCoord : TEXCOORD;
};

VSOutput VS(VSInput input)
{
	VSOutput output;
	output.wpos = input.position;
	output.position = mul(projection, mul(view, mul(world, float4(input.position, 1))));
	output.normal = normalize(mul(world, float4(input.normal, 0.f)).xyz);
	output.texCoord = input.texCoord;
	return output;
}

float4 PS(VSOutput input) : SV_Target
{
	float4 texColor = diffuseTex.Sample(textureSampler, input.texCoord);

	return texColor;
	float3 light = float3(-10, 0, 0);
	float3 litColor = 0;
	//The vector from surface to the light
	float3 lightVec = input.wpos - light;
	float lightintensity;
	float3 lightDir;
	float3 reflection;
	float3 specular = 1;
	float3 ambient = float3(0.5, 0.5, 0.5);
	float3 diffuse = float3(0.4, 0.4, 0.4);
	float shininess = 32;
	//the distance deom surface to light
	float d = length(lightVec);
	float fade;
	if (d > 500)
		return float4(float3(0.5f, 0.5f, 0.5f) * texColor.xyz, 1);
	fade = 1 - (d / 500);
	//Normalize light vector
	lightVec /= d;
	litColor = ambient.xyz;
	//Add ambient light term
	lightintensity = saturate(dot(input.normal, lightVec));
	litColor += diffuse.xyz * lightintensity;
	lightDir = -lightVec;
	if (lightintensity > 0.0f)
	{
		float3 viewDir = normalize(light - float3(0, 0, 50));
			float3 ref = reflect(-lightDir, normalize(input.normal));
			float scalar = max(dot(ref, viewDir), 0.0f);
		float specFac = 1.0f;
		for (int i = 0; i < shininess; i++)
			specFac *= scalar;
		litColor += specular.xyz * specFac;
	}
	litColor = litColor * texColor.xyz;
	return float4(litColor*fade, 1);
}