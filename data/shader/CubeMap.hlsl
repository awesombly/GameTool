#include "Basis.hlsl"

//Texture2D   g_txDiffuse   : register(t0);
//Texture2D	g_txNormalMap : register(t1);
TextureCube g_txCubeMap   : register(t0);
SamplerState samLinear : register (s0);

// 큐브맵 출력
struct VS_OUTPUT_Cube
{
	float4 pos : SV_POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR0;
	float3 tex : TEXCOORD0;
};
// SO 구생성 구조
struct SO_VS_INPUT
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};

struct SO_VS_OUTPUT
{
	float3 pos : POSITION;
	float3 nor : NORMAL;
	float4 col : COLOR;
	float2 tex : TEXCOORD;
};


// 스카이 정점
VS_OUTPUT_Cube VS_Sky(VS_INPUT_PNCTT input)
{
	VS_OUTPUT_Cube output = (VS_OUTPUT_Cube)0;
	output.pos = mul(float4(input.pos, 1.0f), g_matWorld);
	output.pos = mul(output.pos, g_matView);
	output.pos = mul(output.pos, g_matProj);
	output.pos = output.pos.xyww;
	
	//output.nor = input.nor;
	output.col = input.col;
	output.tex = normalize(input.pos.xyz);
	output.nor = normalize(mul(input.nor, (float3x3)g_matWorld));
	return output;
}
// 스카이 픽셀
float4 PS_Sky(VS_OUTPUT_Cube input) : SV_TARGET
{
	return g_txCubeMap.Sample(samLinear, input.tex);// *input.col;
}













//===========================================================================
// SO
SO_VS_OUTPUT VS_SO(SO_VS_INPUT input)
{
	SO_VS_OUTPUT output = (SO_VS_OUTPUT)0;
	output.pos = input.pos;
	output.nor = input.nor;
	output.col = input.col;
	output.tex = input.tex;
	return output;
}

// 로컬 정점의 크기를 정규화
SO_VS_OUTPUT NormalizeVertex(SO_VS_OUTPUT vertex)
{
	SO_VS_OUTPUT newVertex;
	newVertex.pos = normalize(vertex.pos.xyz);
	// 정점이 원점을 중심으로 하는 노말 벡터가 됨
	newVertex.nor = newVertex.pos;
	newVertex.col = vertex.col;
	newVertex.tex = vertex.tex;
	return newVertex;
}

void TriAppend(SO_VS_OUTPUT v0, SO_VS_OUTPUT v1, SO_VS_OUTPUT v2, inout TriangleStream<SO_VS_OUTPUT> TriStream)
{
	TriStream.Append(NormalizeVertex(v0));
	TriStream.Append(NormalizeVertex(v1));
	TriStream.Append(NormalizeVertex(v2));
	// 삼각형 단위로 스트립 구성
	TriStream.RestartStrip();
}

[maxvertexcount(12)]
void GS_SO(triangle SO_VS_OUTPUT input[3], inout TriangleStream<SO_VS_OUTPUT> TriStream)
{
	SO_VS_OUTPUT center0, center1, center2;
	center0.pos = (input[0].pos.xyz + input[1].pos.xyz) / 2.0f;
	center0.nor = (input[0].nor.xyz + input[1].nor.xyz) / 2.0f;
	center0.col = (input[0].col		+ input[1].col)	  / 2.0f;
	center0.tex = (input[0].tex.xy  + input[1].tex.xy) / 2.0f;

	center1.pos = (input[1].pos.xyz + input[2].pos.xyz) / 2.0f;
	center1.nor = (input[1].nor.xyz + input[2].nor.xyz) / 2.0f;
	center1.col = (input[1].col		+ input[2].col) / 2.0f;
	center1.tex = (input[1].tex.xy  + input[2].tex.xy) / 2.0f;

	center2.pos = (input[0].pos.xyz + input[2].pos.xyz) / 2.0f;
	center2.nor = (input[0].nor.xyz + input[2].nor.xyz) / 2.0f;
	center2.col = (input[0].col		+ input[2].col)	  / 2.0f;
	center2.tex = (input[0].tex.xy  + input[2].tex.xy)  / 2.0f;

	TriAppend(input[0], center0, center2, TriStream);
	TriAppend(center0, input[1], center1, TriStream);
	TriAppend(input[2], center2, center1, TriStream);
	TriAppend(center0, center1, center2, TriStream);
}