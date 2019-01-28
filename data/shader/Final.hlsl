#include "Basis.hlsl"
Texture2D   g_txDiffuse   : register(t0);
Texture2D	g_txNormalMap : register(t1);
TextureCube g_txEnviMap   : register(t2);
Texture2D	g_txDepthMap : register(t4);

SamplerState samLinear : register (s0);
SamplerState samShadowMap : register(s1);
SamplerComparisonState samComShadowMap : register (s2);

static const int SMapSize = 1024;
static const float EPSILON = 0.005f;
// 노말값에 대한 반사율?
static const float refAtNormal_Incidence = 1.33f;


struct VS_OUTPUT_Mix
{
	float4 pos		 : SV_POSITION;
	float4 nor		 : NORMAL;
	float4 col		 : COLOR0;
	float2 tex		 : TEXCOORD0;
	// 노말
	float3 vEye		 : TEXCOORD1;
	float3 vHalf	 : TEXCOORD2;
	float3 vLightDir : TEXCOORD3;
	// 환경
	float3 ref		 : TEXCOORD4;
	float3 fre		 : TEXCOORD5;
	float3 eye		 : TEXCOORD6;
	// 쉐도우
	float4 TexShadow : TEXCOORD7;
};


// 프레넬 계산
float ComputeFresnel(float3 reflect, float3 normal, float f0)
{
	float cosAngle = 1.0f - saturate(dot(reflect, normal));
	float result = pow(cosAngle, 5.0f);
	result = saturate(mad(result, 1.0f - saturate(f0), f0));
	return result;
}




VS_OUTPUT_Mix VS_Final(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_Mix output = (VS_OUTPUT_Mix)0;
	float4 WorldPos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(WorldPos, g_matView);
	output.pos = mul(output.pos, g_matProj);

	float3 vNormal = normalize(mul(input.nor, (float3x3)g_matNormal));
	output.nor = float4(vNormal, (output.pos.w - NEAR) / (FAR - NEAR));
	output.tex = input.tex;

	//float3 vLightDir = -cb_LightVector;// normalize(cb_LightVector.xyz - WorldPos.xyz);
	float3 vLightDir = normalize(cb_LightVector.xyz - WorldPos.xyz);

	// 노말
	if(cb_useNormalMap)
	{
		//float3 vNormal = input.nor;
		output.vEye = normalize(cb_EyePos.xyz - WorldPos.xyz);

		float3 tangent = normalize(mul(input.tan, (float3x3)g_matNormal));
		float3 biNormal = normalize(cross(vNormal, tangent));

		float3x3 matTangent = { tangent.x, biNormal.x, vNormal.x,
			tangent.y, biNormal.y, vNormal.y,
			tangent.z, biNormal.z, vNormal.z };

		output.vHalf = normalize(mul(normalize(vLightDir + output.vEye), matTangent));
		output.vLightDir = normalize(mul(vLightDir, matTangent));
		output.vEye = normalize(mul(output.vEye, matTangent));
		output.col = input.col;
	}
	else
	{
		float fDot = lerp(dot(vLightDir, output.nor.xyz), 1.0f, 0.15f) + 0.2f;
		output.col = float4(fDot, fDot, fDot, 1.0f) * input.col;
	}

	// 환경
	if(cb_useEnviMap)
	{
		// camera/eye -> V?
		float3 incident = output.eye = normalize(WorldPos.xyz - cb_EyePos.xyz);
		// R = I - 2 * N * (I.N)	?
		//output.ref = normalize(incident - 2.0f * output.nor * dot(incident, output.nor));
		output.ref = normalize(reflect(incident, output.nor.xyz));
		output.fre = normalize(refract(incident, output.nor.xyz, 1.0f / refAtNormal_Incidence));
	}

	// 쉐도우 텍스처 좌표
	if(cb_useShadow)
	{
		output.TexShadow = mul(float4(input.pos, 1.0f), g_matShadow);
	}
	return output;
}
 
PBUFFER_OUTPUT PS_Final(VS_OUTPUT_Mix input) : SV_Target
{
	PBUFFER_OUTPUT output = (PBUFFER_OUTPUT)0;
	output.color1 = input.nor;
	output.color0 = g_txDiffuse.Sample(samLinear, input.tex);

	// 환경
	if(cb_useEnviMap)
	{
		uint type = cb_useEnviMap;
		switch (type)
		{
			case 1:
			{
				float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
				float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);
			
				output.color0 = lerp(output.color0, reflectColor, 0.5f);
			} break;
			case 2:
			{
				float4 diffuseColor = g_txDiffuse.Sample(samLinear, input.tex);
				float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// 반사 컬러
			
				float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
				float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);
			
				// 디퓨즈맵과 반사맵 보간
				output.color0 = lerp(output.color0, reflectColor, fresnel + 0.3f);
			} break;
			case 3:
			{
				float4 reflectColor = g_txEnviMap.Sample(samLinear, input.ref);		// 반사 컬러
				float4 refractColor = g_txEnviMap.Sample(samLinear, input.fre);		// 굴절 컬러
			
				float r0 = pow(1.0f - refAtNormal_Incidence, 2.0f) / pow(1.0f + refAtNormal_Incidence, 2.0f);
				float fresnel = ComputeFresnel(input.ref, input.nor.xyz, r0);
			
				// 굴절, 반사 보간
				output.color0 *= lerp(refractColor, reflectColor, fresnel * 7.0f)/* * input.col*/ * 1.15f;
				//color = lerp(refractColor, color, fresnel * 0.1f);
				//color.a = 1.0f;
			} break;
		}
	}
	// 노말
	if(cb_useNormalMap)
	{
		// 디퓨즈 조명(노말맵)
		float4 normal = g_txNormalMap.Sample(samLinear, input.tex);
		normal = normalize((normal - 0.5f) * 2.0f);

		// saturate == clamp
		float fDot = saturate(dot(normal.xyz, input.vLightDir));// +0.15f;
		float3 LightColor = cb_DiffuseLightColor.rgb * fDot;

		// 스펙큘러 조명
		float3 R = reflect(-input.vLightDir, normal.xyz);
		float3 SpecularColor = cb_SpecularLightColor.rgb * pow(saturate(dot(R, input.vEye)), cb_SpecularPower);
		//float3 SpecularColor	= cb_SpecularLightColor.rgb * pow(saturate(dot(input.vHalf, normal.xyz )), cb_SpecularPower);

		// 최종 컬러 조합
		//return vDiffuseColor * float4(LightColor + SpecularColor, 1.0f) * input.col;
		output.color0 *= float4(LightColor + SpecularColor + 0.2f, 1.0f)/* * input.col*/;
	}
	// 음영 /// 음영 끄면 color도 미적용됨..
	if (cb_useLight)
	{
		output.color0 *= input.col * cb_useLight;
	}
	// 쉐도우
	if(cb_useShadow)
	{
		static const float	iNumKernel = 3;
		float fLightAmount = 0.0f;
		float3 ShadowTexColor = input.TexShadow.xyz / input.TexShadow.w;

		const float fdelta = 1.0f / SMapSize;
		int iHalf = (iNumKernel - 1) / 2;
		for (int v = -iHalf; v <= iHalf; v++)
		{
			for (int u = -iHalf; u <= iHalf; u++)
			{
				float2 vOffset = float2(u * fdelta, v * fdelta);
				fLightAmount += g_txDepthMap.SampleCmpLevelZero(samComShadowMap,
					ShadowTexColor.xy + vOffset, ShadowTexColor.z);
			}
		}
		fLightAmount /= iNumKernel * iNumKernel;
		///float fColor = float4(fLightAmount, fLightAmount, fLightAmount, 1.0f);
		//output.color0 *= max(0.5f, fLightAmount);

		output.color0 *= max(cb_useShadow, fLightAmount);
		
		//output.color0 = cb_useShadow > fLightAmount ? output.color0 * cb_useShadow : output.color0;
	}
	
	output.color0.w = 1.0f;
	return  output;
}