#include "ShaderData.hlsl"

// 상수 버퍼 : 버퍼 0번
cbuffer cbMatrix: register(b0)
{
	matrix	g_matWorld		: packoffset(c0);
	matrix	g_matView		: packoffset(c4);
	matrix	g_matProj		: packoffset(c8);
	matrix	g_matNormal		: packoffset(c12);
	float4  g_MeshColor     : packoffset(c16);
};
// 환경 데이터
cbuffer cbObjectData : register(b5)
{
	float4 cb_LightVector;
	float4 cb_EyePos;
	float4 cb_EyeDir;
}
// 조명? 데이터
cbuffer cbLightMaterial : register(b2)
{
	float4 cb_AmbientLightColor : packoffset(c0);
	float4 cb_DiffuseLightColor : packoffset(c1);
	float3 cb_SpecularLightColor: packoffset(c2);
	float  cb_SpecularPower		: packoffset(c2.w);
};
// 메테리얼, 쉐도우 데이터
cbuffer cbMaterial : register (b3)
{
	float			cb_useLight     : packoffset(c0.x);
	float			cb_useNormalMap : packoffset(c0.y);
	float			cb_useEnviMap   : packoffset(c0.z);
	float			cb_useShadow    : packoffset(c0.w);
	float4x4		g_matShadow  : packoffset(c1);
	float			g_iObjectID  : packoffset(c5.x);
	float			g_iNumKernel : packoffset(c5.y);
	//float			g_iDummy	 : packoffset(c4.zw);
}
// 큐브맵 행렬
cbuffer cbCubeViewMatrix : register(b4)
{
	matrix g_matCubeView[6] : packoffset(c0);
}


// MRT용
struct PBUFFER_OUTPUT
{
	float4 color0 : SV_TARGET0;
	float4 color1 : SV_TARGET1;
};

// PNCTT 정점 입력
struct VS_INPUT_PNCTT
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR0;			// 0번 레지스터로 출력
	float2 tex : TEXCOORD0;
	float3 tan : TANGENT;
};
struct VS_INPUT_PNCT
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};
// 정점 출력(노말+깊이)
struct VS_OUTPUT_PNCT
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float4 col : COLOR0;
	float2 tex : TEXCOORD0;
};