#include "Basis.hlsl"

Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txCubeMap   : register(t2);

SamplerState samLinear : register (s0);

// 기본 정점
VS_OUTPUT_PNCT VS_OutLine(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_PNCT output = (VS_OUTPUT_PNCT)0;
	
	matrix matWorld = g_matWorld;
	matWorld._11 *= 1.07f;
	matWorld._22 *= 1.07f;
	matWorld._33 *= 1.07f;
	output.pos = mul(float4(input.pos, 1.0f), matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	output.col = float4(1.0f, 0.0f, 0.0f, 1.0f);
	output.tex = input.tex;
	return output;
}
// 기본 픽셀
float4 PS_OutLine(VS_OUTPUT_PNCT input) : SV_TARGET
{
	return input.col;
}