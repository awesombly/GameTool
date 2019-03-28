#include "Basis.hlsl"

Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
SamplerState samLinear	  : register(s0);


struct VS_OUTPUT_PNCTV3
{
	float4 pos			: SV_POSITION;
	float4 nor			: NORMAL;
	float4 col			: COLOR0;
	float2 tex			: TEXCOORD0;
	float3 vEye			: TEXCOORD1;
	float3 vHalf		: TEXCOORD2;
	float3 vLightDir	: TEXCOORD3;
};

// 노말맵 정점
VS_OUTPUT_PNCTV3 VS_Normal(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_PNCTV3 output = (VS_OUTPUT_PNCTV3)0;
	float4 WorldPos = mul(float4(input.pos, 1.0f),  g_matWorld);
	output.pos		= mul(WorldPos,	  g_matView);
	output.pos		= mul(output.pos, g_matProj);
	
	output.tex = input.tex;
	output.col = input.col;

	float3 vNormal = normalize(mul(input.nor, (float3x3)g_matNormal));
	//float3 vNormal = input.nor;
	output.nor = float4(vNormal, (output.pos.w - fNEAR) / (fFAR - fNEAR));
	output.vEye = normalize(cb_EyePos.xyz - WorldPos.xyz);

	float3 tangent = normalize(mul(input.tan, (float3x3)g_matNormal));
	float3 biNormal = normalize(cross(vNormal, tangent));

	float3x3 matTangent = { tangent.x, biNormal.x, vNormal.x,
							tangent.y, biNormal.y, vNormal.y,
							tangent.z, biNormal.z, vNormal.z };
#ifdef DirectLight
	float3 vLightDir = -cb_LightVector.xyz;
#else
	float3 vLightDir = normalize(cb_LightVector.xyz - WorldPos.xyz);
#endif
	//float fDot = lerp(dot(vLightDir, output.nor), 1.0f, 0.3f) + 0.3f;

	output.vHalf	 = normalize(mul(normalize(vLightDir + output.vEye), matTangent));
	output.vLightDir = normalize(mul(vLightDir, matTangent));
	output.vEye		 = normalize(mul(output.vEye, matTangent));

	return output;
}


PBUFFER_OUTPUT PS_Normal(VS_OUTPUT_PNCTV3 input) : SV_TARGET
{
	// 기본 텍스쳐 컬러 
	/*	float4 normal		=	g_txNormalMap.Sample( g_samLinear, input.t );
	float1 x			=	input.t.x+normal.x*0.1f*cb_vLightVector.z;
	float1 y			=	input.t.y+normal.y*0.1f*cb_vLightVector.x;
	float2 uv			=	float2(input.t.y, y);
	*/

	// 디퓨즈 컬러
	float4 DiffuseColor = g_txDiffuse.Sample(samLinear, input.tex);

	// 디퓨즈 조명(노말맵)
	float4 normal = g_txNormalMap.Sample(samLinear, input.tex);
	normal = normalize((normal - 0.5f) * 2.0f);

	// saturate == clamp
	float fDot = saturate(dot(normal.xyz, input.vLightDir)) + 0.15f;
	float3 LightColor = cb_DiffuseLightColor.rgb * fDot;

	// 스펙큘러 조명
	float3 R = reflect(-input.vLightDir, normal.xyz);
	float3 SpecularColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R,            input.vEye)), cb_SpecularPower);
	//float3 SpecularColor	= cb_SpecularLightColor.rgb * pow(saturate(dot(input.vHalf, normal.xyz )), cb_SpecularPower);

	// 최종 컬러 조합
	//return DiffuseColor * float4(LightColor + SpecularColor, 1.0f) * input.col;
	PBUFFER_OUTPUT output;
	output.color0 = DiffuseColor * float4(LightColor + SpecularColor, 1.0f) * input.col;
	output.color1 = float4(input.nor);
	return output;
}