#include "Basis.hlsl"
Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txDepthMap : register(t1);
//TextureCube g_txCubeMap   : register(t2);

SamplerState samLinear : register (s0);
SamplerState samShadowMap : register(s1);
SamplerComparisonState samComShadowMap : register (s2);

static const int SMapSize = 1024;
static const float EPSILON = 0.005f;


struct VS_OUTPUT_Shadow
{
	float4 pos		 : SV_POSITION;
	float4 col		 : COLOR0;
	float2 tex		 : TEXCOORD0;
	float4 TexShadow : TEXCOORD1;
};

// ±×¸²ÀÚ ±íÀÌ¸Ê ¸¸µé¶§
struct VS_OUTPUT_DepthMap
{
	float4 pos		: SV_POSITION;
	float2 dep		: TEXCOORD0;
};



VS_OUTPUT_Shadow VS_Shadow(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_Shadow output = (VS_OUTPUT_Shadow)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.col = input.col;
	output.tex = input.tex;
	// ÅØ½ºÃ³ ÁÂÇ¥
	output.TexShadow = mul(float4(input.pos, 1.0f), g_matShadow);
	return output;
}
 
float4 PS_Shadow(VS_OUTPUT_Shadow input) : SV_Target
{
	const float	g_iNumKernel = 3;
	float4 vDiffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	float fLightAmount = 0.0f;
	float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;

	const float fdelta = 1.0f / SMapSize;
	int iHalf = (g_iNumKernel - 1) / 2;
	for (int v = -iHalf; v <= iHalf; v++)
	{
		for (int u = -iHalf; u <= iHalf; u++)
		{
			float2 vOffset = float2(u * fdelta, v * fdelta);
			fLightAmount += g_txDepthMap.SampleCmpLevelZero(samComShadowMap,
															ShadowTexColor.xy + vOffset, ShadowTexColor.z);
		}
	}
	fLightAmount /= g_iNumKernel * g_iNumKernel;
	float fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
	float4 vFinalColor = vDiffuseColor * max(0.5f, fLightAmount);
	vFinalColor.a = 1.0f;
	return  vFinalColor;
}

float4 PS_NO_CMP(VS_OUTPUT_Shadow input) : SV_Target
{
	//const float	g_iNumKernel = 3;
	//float4 shadow  = g_txDepthMap.Sample( samShadowMap, input.TexShadow.xy / input.TexShadow.w ); 
	float4 FinalColor = g_txDiffuse.Sample(samLinear, input.tex);

	float LightAmount = 1.0f;
	float4 vPosLight = input.TexShadow;

	for (int iLight = 0; iLight < 1; iLight++)
	{
		//transform from RT space to texture space.
		float2 ShadowTexC = vPosLight.xy / vPosLight.w;
		// ShadowTexC.y = 1.0f - ShadowTexC.y;
		// transform to texel space
		float2 texelpos = SMapSize * ShadowTexC;
		// Determine the lerp amounts           
		float2 lerps = frac(texelpos);
		//read in bilerp stamp, doing the shadow checks
		float sourcevals[4];
		sourcevals[0] = (g_txDepthMap.Sample(samShadowMap, ShadowTexC) + EPSILON < vPosLight.z / vPosLight.w)
			? 0.0f : 1.0f;
		sourcevals[1] = (g_txDepthMap.Sample(samShadowMap, ShadowTexC
											 + float2(1.0f / SMapSize, 0)) + EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
		sourcevals[2] = (g_txDepthMap.Sample(samShadowMap, ShadowTexC
											 + float2(0.0f, 1.0f / SMapSize)) + EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
		sourcevals[3] = (g_txDepthMap.Sample(samShadowMap, ShadowTexC
											 + float2(1.0f / SMapSize, 1.0 / SMapSize)) + EPSILON < vPosLight.z / vPosLight.w) ? 0.0f : 1.0f;
		// lerp between the shadow values to calculate our light amount
		LightAmount = lerp(lerp(sourcevals[0], sourcevals[1], lerps.x),
						   lerp(sourcevals[2], sourcevals[3], lerps.x),
						   lerps.y);
	}
	return  FinalColor * max(0.5f, LightAmount);
}




//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT_DepthMap VS_DepthMap(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_DepthMap output = (VS_OUTPUT_DepthMap)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.dep = output.pos.zw;
	//output.dep.y = (output.pos.w - NEAR) / (FAR - NEAR);
	return output;
}
//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS_DepthMap(VS_OUTPUT_DepthMap input) : SV_Target
{
	float shadow = input.dep.x / input.dep.y;
	return float4(shadow, shadow, shadow, 1.0f);
}

float4 PS_PlaneShadow(VS_INPUT_PNCTT input) : SV_Target
{
	return float4(0.0f, 0.0f, 0.0f, 0.55f);
}