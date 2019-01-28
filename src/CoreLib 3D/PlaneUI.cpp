#include "PlaneUI.h"
#include "ObjectMAnager.h"


PlaneUI::PlaneUI(const wstring_view& myName, const wstring_view& srcName,
				 const string_view& vertexShaderName, const string_view& pixelShaderName) noexcept
{
	SetInfo(myName, EComponent::UI, srcName, vertexShaderName, pixelShaderName);
	Create();
}

bool PlaneUI::Init() noexcept
{
	//RPlane::Init();
	m_ppCamera = &ObjectManager::Cameras[ECamera::UI];
	return true;
}

bool PlaneUI::Frame(const float& spf, const float& accTime)	noexcept
{
	if (!m_isEnable) return false;
	RPlane::Frame(spf, accTime);
	return true;
}

bool PlaneUI::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	DxManager::GetInstance().SetDepthStencilState(EDepthS::D_Always_S_Off);
	RPlane::Render(pDContext);
	DxManager::GetInstance().SetDepthStencilState(EDepthS::Current);
	return true;
}

bool PlaneUI::Release()	noexcept
{
	RPlane::Release();
	return true;
}

Component* PlaneUI::clone() noexcept
{
	//return (Component*)(new PlaneUI(*this));
	return (new PlaneUI(*this))->cloneAddition();
}