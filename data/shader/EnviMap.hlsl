#include "Basis.hlsl"

Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
SamplerState samLinear : register (s0);


struct VS_OUTPUT_Envi
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
	float3 ref : TEXCOORD1;
	float3 fre : TEXCOORD2;
	float3 eye : TEXCOORD3;
	//float3 v : TEXCOORD4;
};

struct GS_INPUT_Envi
{
	float4 pos : SV_POSITION;
	//float4 col : COLOR0;
	//float2 tex : TEXCOORD0;
	//float3 nor : TEXCOORD1;
	//float3 ref : TEXCOORD2;
	//float3 fre : TEXCOORD3;
	//float3 eye : TEXCOORD4;
	float3 v : TEXCOORD0;
};
struct GS_OUTPUT_Envi
{
	float4 pos :SV_POSITION;
	float3 v : TEXCOORD0;
	uint index : SV_RenderTargetArrayIndex;
};


//// ¹Ý»ç º¤ÅÍ °è»ê
//float3 Reflect(float3 incident, float3 normal)
//{
//	float cosI = dot(normal, incident);
//	return incident - 2.0f * cosI * normal;
//}
//// ±¼Àý º¤ÅÍ °è»ê
//float3 Refract(float3 incident, float3 normal, float etaRatio)
//{
//	float cosI = dot(incident, normal);
//	float cosT2 = 1.0f - etaRatio * etaRatio * (1.0f - cosI * cosI);
//	float3 T = etaRatio * incident + ((etaRatio * cosI - sqrt(abs(cosT2))) * normal);
//	return T;
//}
// ÇÁ·¹³Ú °è»ê
float ComputeFresnel(float3 reflect, float3 normal, float f0)
{
	float cosAngle = 1.0f - saturate(dot(reflect, normal));
	float result = pow(cosAngle, 5.0f);
	result = saturate(mad(result, 1.0f - saturate(f0), f0));
	return result;
}


// È¯°æ Á¤Á¡
VS_OUTPUT_Envi VS_Envi(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_Envi output = (VS_OUTPUT_Envi)0;

	float4 WorldPos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(WorldPos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	
	float3 vNormal = normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(vNormal, (output.pos.w - fNEAR) / (fFAR - fNEAR));

#ifdef DirectLight
	float3 vLightDir = -cb_LightVector.xyz;
#else
	float3 vLightDir = normalize(cb_LightVector.xyz - WorldPos.xyz);
#endif
	float fDot = lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.15f) + 0.2f;
	output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;
	output.tex = input.tex;

	// camera/eye -> V?
	float3 incident = output.eye = normalize(WorldPos.xyz - cb_EyePos.xyz);
	// R = I - 2 * N * (I.N)	?
	//output.ref = normalize(incident - 2.0f * output.nor * dot(incident, output.nor));
	output.ref = normalize(reflect(incident, output.nor.xyz));
	output.fre = normalize(refract(incident, output.nor.xyz, 1.0f / refAtNormal_Incidence));
	
	return output;
}


/////////////// ÇÈ¼¿ ½¦ÀÌ´õ
PBUFFER_OUTPUT PS_Envi(VS_OUTPUT_Envi input) : SV_TARGET
{
	float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);

	PBUFFER_OUTPUT output;
	output.color0 = lerp(diffuseColor, reflectColor, 0.5f) * input.col;
	output.color1 = input.nor;
	return output;
}
// ÇÁ·¹³Ú ÀÌ¿ë?
PBUFFER_OUTPUT PS_Fresnel(VS_OUTPUT_Envi input) : SV_TARGET
{
	float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// ¹Ý»ç ÄÃ·¯

	float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
	float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);

	// µðÇ»Áî¸Ê°ú ¹Ý»ç¸Ê º¸°£
	float4 color = lerp(diffuseColor, reflectColor, fresnel + 0.3f) * input.col;
	color.a = 1.0f;

	PBUFFER_OUTPUT output;
	output.color0 = color;
	output.color1 = input.nor;
	return output;
}
// ¹Ý»ç + ±¼Àý
PBUFFER_OUTPUT PS_Refraction(VS_OUTPUT_Envi input) : SV_Target
{
	//float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
	float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// ¹Ý»ç ÄÃ·¯
	float4 refractColor = g_txEnviMap.Sample(samLinear, input.fre);		// ±¼Àý ÄÃ·¯

	float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
	float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);

	// µðÇ»Áî, ±¼Àý, ¹Ý»ç º¸°£
	float4 color = lerp(refractColor, reflectColor, fresnel * 7.0f) * input.col * 1.15f;
	//color = lerp(refractColor, color, fresnel * 0.1f);
	color.a = 1.0f;

	PBUFFER_OUTPUT output;
	output.color0 = color;
	output.color1 = input.nor;
	return output;
}




///////////////////////////////////////////////////////////////////////////
GS_INPUT_Envi VS_EnviDy(VS_INPUT_PNCTT input)
{
	GS_INPUT_Envi output = (GS_INPUT_Envi)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.v = normalize(input.pos);
	return output;
}

// Å¥ºê¸Ê ·£´õ¸µ
[maxvertexcount(24)]
void GS_EnviDy(triangle GS_INPUT_Envi input[3], inout TriangleStream<GS_OUTPUT_Envi> cubeMapStream)
{
	for (int iPlane = 0; iPlane < 6; ++iPlane)
	{
		GS_OUTPUT_Envi output;
		output.index = iPlane;
		for (int v = 0; v < 3; ++v)
		{
			output.pos = mul(mul(input[v].pos, g_matCubeView[iPlane]), g_matProj);
			output.pos = output.pos.xyww;
			output.v = input[v].v;
			//output.col = input[v].col;
			//output.tex = input[v].tex;
			//output.nor = input[v].nor;
			//output.ref = input[v].r;
			//output.fre = input[v].f;
			//output.eye = input[v].e;
			cubeMapStream.Append(output);
		}
		cubeMapStream.RestartStrip();
	}
}

float4 PS_EnviDy(GS_OUTPUT_Envi input) : SV_TARGET
{
	return g_txEnviMap.Sample(samLinear, input.v);
}