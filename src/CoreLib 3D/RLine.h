#pragma once
#include "Renderer.h"


// 하나의 선을 나타내는 객체
class RLine : public Renderer
{
private:
	vector<Vertex_PC> m_vertexList;				// 정점 리스트 덮어씀
	bool m_isDebugLine;
private:
	HRESULT Create()	noexcept override;
	HRESULT CreateVertexBuffer() override;
public:
	void SetLineInfo(ID3D11DeviceContext* pDContext, const D3DXVECTOR3& start, const D3DXVECTOR3& end, const D3DXVECTOR4& color, const bool& isDebugLine = false) noexcept;

	bool isDebugLine();
	void isDebugLine(const bool& isDebugLine);

	bool PrevRender(ID3D11DeviceContext* pDContext)		noexcept override;
	bool PostRender(ID3D11DeviceContext* pDContext)		noexcept override;
	bool Init()											noexcept override;
	bool Frame(const float& spf, const float& accTime)	noexcept override;
	bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	bool Release()										noexcept override;
	Component* clone() noexcept override;
protected:
	RLine() = default;
public:
	RLine(const wstring_view& myName, const string_view& vertexShaderName = "VS_PC", const string_view& pixelShaderName = "PS_PC") noexcept;
	virtual ~RLine() = default;
};