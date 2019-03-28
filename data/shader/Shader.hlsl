#include "Basis.hlsl"

Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txCubeMap   : register(t2);
//Texture2D g_txDiffuse1 : register (t1);

SamplerState samLinear : register (s0);

// 선 입력
struct VS_INPUT_PC
{
	float3 pos : POSITION;
	float4 col : COLOR0;
};
// 선 출력
struct VS_OUTPUT_PC
{
	float4 pos : SV_POSITION;
	float4 col : COLOR0;
};


// 기본 정점
VS_OUTPUT_PNCT VS_Basic(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_PNCT output = (VS_OUTPUT_PNCT)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	
	input.nor = normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(input.nor, (output.pos.w - fNEAR) / (fFAR - fNEAR));

	output.col = input.col;
	output.tex = input.tex;
	return output;

}
VS_OUTPUT_PNCT VS_Light(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_PNCT output = (VS_OUTPUT_PNCT)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	input.nor = normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(input.nor, (output.pos.w - fNEAR) / (fFAR - fNEAR));

#ifdef DirectLight
	float3 vLightDir = -cb_LightVector.xyz;
#else
	float3 vLightDir = normalize(cb_LightVector.xyz - output.pos.xyz);
#endif
	float fDot = lerp(dot(vLightDir, input.nor), 1.0f, 0.2f) + 0.2f;
	output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;

	//output.col = input.col;
	output.tex = input.tex;
	return output;
}
// 선 정점 쉐이더
VS_OUTPUT_PC VS_PC(VS_INPUT_PC input)
{
	VS_OUTPUT_PC output = (VS_OUTPUT_PC)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.col = input.col;
	return output;
}
// 기본 픽셀
PBUFFER_OUTPUT PS_Basic(VS_OUTPUT_PNCT input)
{
	PBUFFER_OUTPUT output;
	output.color0 = g_txDiffuse.Sample(samLinear, input.tex) * input.col;
	if (output.color0.w < 0.1f)
		discard;
	//output.color1 = float4(input.nor.xyz * 0.5f + 0.5f, input.nor.w);
	//input.nor.w = 1.0f;
	output.color1 = input.nor;
	return output;
}


// 선 픽셀 쉐이더
float4 PS_PC(VS_OUTPUT_PC input) : SV_TARGET
{
	return input.col;
}

// 프러스텀 픽셀 쉐이더
float4 PS_Frustum(VS_OUTPUT_PNCT input) : SV_TARGET
{
	return float4(0.8f, 1.0f, 0.8f, 0.6f);
}



//[maxvertexcount(20)]		// 최대 프리미티브 정점
////			   PrimitiveType(point, line, triangle, lineadj, triangleadj)
//void GS_Sphere(triangle VS_OUTPUT_PNCT input[3], inout TriangleStream<VS_OUTPUT_PNCT> TriStream)
//{
//	VS_OUTPUT_PNCT output;
//	// 삼각형 중점 계산
//	float3 centerPos = (input[0].pos.xyz + input[1].pos.xyz + input[2].pos.xyz) / 3.0f;
//
//	// 정점당 계산된 중심점을 사용해 페이스 추가
//	for (int i = 0; i < 3; ++i)
//	{
//		// 하나의 삼각형을 세개의 삼격형으로 분할
//		output.pos = input[i].pos;
//		TriStream.Append(output);
//
//		int iNext = (i + 1) % 3;
//		output.pos = input[iNext].pos;
//		TriStream.Append(output);
//		output.pos = float4(centerPos, 1.0f);
//		TriStream.Append(output);
//		// primitive Strip을 종료후 재시작, primitive 단위로 실행?
//		TriStream.RestartStrip();
//	}
//	TriStream.RestartStrip();
//}