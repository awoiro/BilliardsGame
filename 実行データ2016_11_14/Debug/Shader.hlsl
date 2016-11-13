Texture2D g_texDecal;
SamplerState g_samLinear;

cbuffer global_0
{
	float4 g_lightDir;
	float4 g_eyePos;
};

cbuffer global_1
{
	matrix g_world;
	matrix g_WVP;
	//float4 g_ambient = float4(0, 0, 0, 0);
	//float4 g_diffuse = float4(1, 0, 0, 0);
	float4 g_specular = float4(1, 1, 1, 1);
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	//float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};

// 頂点シェーダー
VS_OUTPUT VS(float4 Pos : POSITION, float4 Norm : NORMAL, float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos = mul(Pos, g_WVP);
	output.Light = g_lightDir;
	output.Normal = mul(Norm, (float3x3) g_world);
	float3 PosWorld = mul(Pos, g_world);
	output.EyeVector = g_eyePos - PosWorld;
	output.Tex = Tex;

	//float3 Normal = normalize(output.Normal);
	//float3 LightDir = normalize(output.Light);
	//float3 ViewDir = normalize(output.EyeVector);
	//float4 NL = saturate(dot(Normal, LightDir));

	//float3 Reflect = normalize(2 * NL * Normal - LightDir);
	//float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4);

	//output.Color = g_diffuse * NL + specular*g_specular;

	return output;
}

// ピクセルシェーダー
float4 PS(VS_OUTPUT input) : SV_Target
{
	float3 color = g_texDecal.Sample(g_samLinear, input.Tex);

	float3 Normal = normalize(input.Normal);
	float3 LightDir = normalize(input.Light);
	float3 ViewDir = normalize(input.EyeVector);
	float4 NL = saturate(dot(Normal, LightDir));

	float harfNL = NL * 0.5 + 0.5;
	float3 hReflect = float3(color * harfNL * harfNL);

	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 6);

//	color += input.Color;
//	return color;

	//return float4(color * NL + specular, 1);
	return float4(hReflect, 0) + specular;
}