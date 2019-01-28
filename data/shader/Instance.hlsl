#include "Basis.hlsl"

Texture2D g_txDiffuse  : register (t0);
Texture2D g_txDiffuse1 : register (t1);
Texture2D g_txDiffuse2 : register (t2);
Texture2D g_txDiffuse3 : register (t3);
Texture2D g_txDiffuse4 : register (t4);
Texture2D g_txDiffuse5 : register (t5);
Texture2D g_txDiffuse6 : register (t6);
Texture2D g_txDiffuse7 : register (t7);
Texture2D g_txDiffuse8 : register (t8);
Texture2D g_txDiffuse9 : register (t9);
Texture2D g_txDiffuse10 : register (t10);
Texture2D g_txDiffuse11 : register (t11);
Texture2D g_txDiffuse12 : register (t12);
Texture2D g_txDiffuse13 : register (t13);
Texture2D g_txDiffuse14 : register (t14);
Texture2D g_txDiffuse15 : register (t15);
SamplerState samLinear : register (s0);


// 인스턴스 입력
struct VS_INPUT_Instance
{
	float3 pos : POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;

	matrix mTransform : mTransform;
	float1 src : TEXTURE;
	float4 ic : InsCOLOR;
	uint   InstanceId : SV_InstanceID;
};

// 인스턴스 출력
struct VS_OUTPUT_Instance
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float1 src : TEXTURE;
	uint   i : INDEX;
};



// 텍스쳐 선택
float4 GetTextureColor(uint index, float2 texUV)
{
	switch (index)
	{
	case 0:
		return g_txDiffuse.Sample(samLinear,  texUV);	break;
	case 1:
		return g_txDiffuse1.Sample(samLinear, texUV);	break;
	case 2:
		return g_txDiffuse2.Sample(samLinear, texUV);	break;
	case 3:
		return g_txDiffuse3.Sample(samLinear, texUV);	break;
	case 4:
		return g_txDiffuse4.Sample(samLinear, texUV);	break;
	case 5:
		return g_txDiffuse5.Sample(samLinear, texUV);	break;
	case 6:
		return g_txDiffuse6.Sample(samLinear, texUV);	break;
	case 7:
		return g_txDiffuse7.Sample(samLinear, texUV);	break;
	case 8:
		return g_txDiffuse8.Sample(samLinear, texUV);	break;
	case 9:
		return g_txDiffuse9.Sample(samLinear, texUV);	break;
	case 10:
		return g_txDiffuse10.Sample(samLinear, texUV);	break;
	case 11:
		return g_txDiffuse11.Sample(samLinear, texUV);	break;
	case 12:
		return g_txDiffuse12.Sample(samLinear, texUV);	break;
	case 13:
		return g_txDiffuse13.Sample(samLinear, texUV);	break;
	case 14:
		return g_txDiffuse14.Sample(samLinear, texUV);	break;
	case 15:
		return g_txDiffuse15.Sample(samLinear, texUV);	break;
	}
	return float4(1, 1, 1, 1);
	//return g_txDiffuse.Sample(samLinear, input.tex);
}

// 파티클 정점 쉐이더
VS_OUTPUT_Instance VS_Particle(VS_INPUT_Instance input)
{
	VS_OUTPUT_Instance output = (VS_OUTPUT_Instance)0;
	output.pos = mul(float4(input.pos, 1.0f), input.mTransform);
	output.pos = mul(output.pos, g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.col = input.ic;
	output.tex = input.tex;
	output.src = input.src;
	output.i = input.InstanceId;
	return output;
}

// 인스턴스 정점 쉐이더
VS_OUTPUT_Instance VS_Instance(VS_INPUT_Instance input)
{
	VS_OUTPUT_Instance output = (VS_OUTPUT_Instance)0;
	output.pos = mul(float4(input.pos, 1.0f), input.mTransform);
	output.pos = mul(output.pos, g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.src = input.col.a;
	input.col.a = 1.0f;

	//float3 light = -cb_LightVector.xyz;// -output.pos.xyz;
	//float fDot = lerp(dot(normalize(light), input.nor), 1.0f, 0.3f) + 0.2f;
	//output.col = input.col * float4(fDot, fDot, fDot, 1.0f);

	//float3 vNormal = normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(input.nor.xyz, (output.pos.w - NEAR) / (FAR - NEAR));

	output.col = input.col;
	output.tex = input.tex;
	output.i = input.InstanceId;
	return output;
}

// 인스턴스 픽셀 쉐이더
PBUFFER_OUTPUT PS_Instance(VS_OUTPUT_Instance input) : SV_TARGET
{
	if (input.src < 0)
		discard;
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color1 = input.nor;
	uint index = (uint)input.src;
	output.color0 = GetTextureColor(index, input.tex) * input.col;
	return output;
}

// 싱글인스턴스 픽셀 쉐이더(파티클)
PBUFFER_OUTPUT PS_SingleInstance(VS_OUTPUT_Instance input) : SV_TARGET
{
	if (input.src < 0)
		discard;
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color1 = input.nor;
	//uint index = (uint)input.src;
	
	output.color0 = g_txDiffuse.Sample(samLinear, input.tex) * input.col;
	return output;
}