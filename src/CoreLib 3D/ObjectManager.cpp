#include "ObjectManager.h"
#include "ColliderAABB.h"
#include "ColliderOBB.h"
#include <fstream>
//#include "../../include/model/AHeroObj.h"
#include "LoadingParameter.h"

map<UINT, GameObject*> ObjectManager::KeyObjects;
UINT				   ObjectManager::KeyCount = 0;
map<ECamera, Camera*>  ObjectManager::Cameras;
Camera*				   ObjectManager::CurCamera;
list<Light*>		   ObjectManager::Lights;
queue<tuple<void(*)(void*, void*), void*, void*> >	 ObjectManager::PostFrameEvent;

//static const float g_fMaxSize = 1024.0f;

bool ObjectManager::Init() noexcept
{
	// 조명 생성
	Lights.emplace_back(new Light(L"Light", ELight::Point));
	//PushObject(Lights.back());

	// 카메라 생성
	Cameras[ECamera::Main] = new Camera(L"MainCamera");
	Cameras[ECamera::Main]->SetMainCamera();
	Cameras[ECamera::Main]->isGlobal(true);
	Cameras[ECamera::UI] = new Camera(L"UICamera", false);
	Cameras[ECamera::UI]->isGlobal(true);
	Cameras[ECamera::Screen] = new Camera(L"Screen", false);
	Cameras[ECamera::Screen]->SetOrthoWH(1.0f, 1.0f);
	Cameras[ECamera::Screen]->isGlobal(true);
	D3DXMatrixIdentity(&Cameras[ECamera::Screen]->m_matWorld);
	Cameras[ECamera::MiniMap] = new Camera(L"MiniMap", true);
	//Cameras[ECamera::MiniMap]->SetOrthoWH(700.0f, 700.0f);
	Cameras[ECamera::MiniMap]->SetPosition(Vector3::Up * 300);
	Cameras[ECamera::MiniMap]->SetRotation(Quaternion::Up * PI * 0.5f);
	Cameras[ECamera::MiniMap]->isGlobal(true);
	Cameras[ECamera::MiniMap]->Frame(0.0f, 0.0f);

	Camera::SelectCamera = Cameras[ECamera::Main];
	CurCamera = Cameras[ECamera::Main];

	// RenderTarget 초기화
	DxManager::GetInstance().InitRTView();

	//m_RT.m_DSFormat = DXGI_FORMAT_R32_TYPELESS;
	//m_RT.Create(DxManager::GetDevice(), g_fMaxSize, g_fMaxSize);
	return true;
}


bool ObjectManager::Frame(const float& spf, const float& accTime) noexcept
{
	Camera::SelectCamera->Frame(spf, accTime);

	for (auto& iter : Lights)
	{
		iter->Frame(spf, accTime);
		//iter->SetPosition(CurCamera->GetWorldPosition() + Vector3::Up * 250.0f + Vector3::Backward * 500.0f);
		//iter->SetRotation(CurCamera->GetWorldRotation() + Quaternion::Up * PI * 0.25f);
	}
	for (auto& outiter : m_ObjectList)
	{
		for (auto& initer : outiter.second)
		{
			initer->Frame(spf, accTime);
		}
	}
	for (auto& iter : m_InstanceList)
	{
		iter->Frame(spf, accTime);
	}
	// 충돌 리스트 초기화
	for (auto& iter : m_ColliderList)
	{
		iter->ClearCollisionList();
	}

	// 후처리 이벤트
	while (!PostFrameEvent.empty())
	{
		auto& [postEvent, param1, param2] = PostFrameEvent.front();
		postEvent(param1, param2);
		PostFrameEvent.pop();
	}
	return true;
}

bool ObjectManager::Render(ID3D11DeviceContext* pDContext) noexcept
{
	Camera::SelectCamera->Render(pDContext);

	//Lights.front()->SetFocus(Vector3::Zero);
	for (auto& iter : Lights)
	{
		iter->Render(pDContext);
	}
	CurCamera = Lights.front();
	// ================================== 깊이맵 생성 ======================================
	DxManager::Get().m_RTDSViewShadow.ClearView(pDContext);
	DxManager::Get().SetViewPort(EViewPort::Main);
	DxManager::Get().SetDepthStencilState(EDepthS::Basic);
	DxManager::Get().SetRasterizerState(ERasterS::DepthBias);

#pragma region DepthMap Render
	/*auto pMainCamera = Cameras[ECamera::Main];
	for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::AObject))
	{
		((AHeroObj*)iter)->SetMatrix(nullptr, &CurCamera->m_matView, &CurCamera->m_matProj);
		((AHeroObj*)iter)->SetVSShader(DxManager::Get().m_VShaderList["VS_DepthMapPNCT"]);
		((AHeroObj*)iter)->SetPSShader(nullptr);
		iter->Render(DxManager::GetDContext());
		((AHeroObj*)iter)->ReturnVSShader();
		((AHeroObj*)iter)->ReturnPSShader();
		((AHeroObj*)iter)->SetMatrix(nullptr, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	}
	for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Character))
	{
		((AHeroObj*)iter)->SetMatrix(nullptr, &CurCamera->m_matView, &CurCamera->m_matProj);
		((AHeroObj*)iter)->SetVSShader(DxManager::Get().m_VShaderList["VS_DepthMapPNCT"]);
		((AHeroObj*)iter)->SetPSShader(nullptr);
		iter->Render(DxManager::GetDContext());
		((AHeroObj*)iter)->ReturnVSShader();
		((AHeroObj*)iter)->ReturnPSShader();
		((AHeroObj*)iter)->SetMatrix(nullptr, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	}
	for (auto& iter : *ObjectManager::Get().GetObjectList(EObjType::Enemy))
	{
		((AHeroObj*)iter)->SetMatrix(nullptr, &CurCamera->m_matView, &CurCamera->m_matProj);
		((AHeroObj*)iter)->SetVSShader(DxManager::Get().m_VShaderList["VS_DepthMapPNCT"]);
		((AHeroObj*)iter)->SetPSShader(nullptr);
		iter->Render(DxManager::GetDContext());
		((AHeroObj*)iter)->ReturnVSShader();
		((AHeroObj*)iter)->ReturnPSShader();
		((AHeroObj*)iter)->SetMatrix(nullptr, &pMainCamera->m_matView, &pMainCamera->m_matProj);
	}*/
	// 깊이맵 랜더
	for (auto& iter : m_ObjectList[EObjType::Object])
	{
		auto RendererLIst = (forward_list<Renderer*>*)iter->GetComponentList(EComponent::Renderer);
		if (RendererLIst == nullptr)
			continue;
		for (auto& pRenderer : *RendererLIst)
		{
			if (pRenderer != nullptr)
			{
				pRenderer->PrevRender(pDContext);
				pDContext->VSSetShader(DxManager::Get().m_VShaderList["VS_DepthMap"], nullptr, 0);
				pDContext->PSSetShader(nullptr, nullptr, 0);
				pRenderer->PostRender(pDContext);
			}
		}
	}
	for (auto& iter : m_ObjectList[EObjType::Map])
	{
		auto RendererLIst = (forward_list<Renderer*>*)iter->GetComponentList(EComponent::Renderer);
		if (RendererLIst == nullptr)
			continue;
		for (auto& pRenderer : *RendererLIst)
		{
			if (pRenderer != nullptr)
			{
				pRenderer->PrevRender(pDContext);
				pDContext->VSSetShader(DxManager::Get().m_VShaderList["VS_DepthMap"], nullptr, 0);
				pDContext->PSSetShader(nullptr, nullptr, 0);
				//pDContext->PSSetShader(DxManager::GetInstance().m_PShaderList["PS_DepthMap"], nullptr, 0);
				pRenderer->PostRender(pDContext);
			}
		}
	}
	DxManager::Get().SetDepthStencilState(EDepthS::Current);
	DxManager::Get().SetRasterizerState(ERasterS::Current);
	DxManager::Get().SetViewPort(EViewPort::Main);
	DxManager::Get().m_RTDSView.Setting(pDContext);
	CurCamera = Cameras[ECamera::Main];
#pragma endregion

	// 오브젝트 랜더
	for (auto&[type, outiter] : m_ObjectList)
	{
		switch (type)
		{
		 case EObjType::Character:
		 case EObjType::Enemy:
		 case EObjType::AObject:
		 case EObjType::Object:
		 case EObjType::Map:
		 {
			 // ============================================= 쉐도우(+) 랜더 ============================================================
			 DxManager::Get().SetSamplerState(0, ESamTextureS::Wrap, ESamFilterS::Linear);
			 DxManager::Get().SetSamplerState(1, ESamTextureS::Clamp, ESamFilterS::Linear);
			 DxManager::Get().SetSamplerState(2, ESamTextureS::Border, ESamFilterS::CompLinearPoint, 0, D3D11_COMPARISON_LESS);
			 //DxManager::GetInstance().SetRasterizerState(ERasterS::CullBack);
			 // 깊이맵 쉐도우
			 for (auto& initer : outiter)
			 {
				pDContext->PSSetShaderResources(4, 1, &DxManager::Get().m_RTDSViewShadow.m_pTexSRViews[0]);
				initer->Render(pDContext);
			 }
			 //DxManager::GetInstance().SetDepthStencilState(EDepthS::Current);
			 //DxManager::GetInstance().SetRasterizerState(ERasterS::Current);
			 DxManager::Get().SetSamplerState(0, ESamTextureS::Current, ESamFilterS::Current);
			 DxManager::Get().SetSamplerState(1, ESamTextureS::Current, ESamFilterS::Current);
			 DxManager::Get().SetSamplerState(2, ESamTextureS::Current, ESamFilterS::Current);
		 }	break;
		 case EObjType::Effect:
		 case EObjType::UI:
		 {
			 continue;
		 }	break;
		 default:
		 {
		  for (auto& initer : outiter)
		  {
		  	 initer->Render(pDContext);
		  }
		 }	break;
		}
	}
	// 이펙 랜더
	for (auto& iter : m_ObjectList[EObjType::Effect])
	{
		iter->Render(pDContext);
	}
	for (auto& iter : m_ObjectList[EObjType::UI])
	{
		iter->Render(pDContext);
	}

	// 인스턴스 랜더
	for (auto& iter : m_InstanceList)
	{
		iter->Render(pDContext);
	}

	// ========================== 미니맵 =============================
	if (Input::isDebug)
	{
		DxManager::Get().ClearDepthStencilView();
		DxManager::Get().SetViewPort(EViewPort::MiniMap);
		CurCamera = Cameras[ECamera::MiniMap];
		for (auto& iter : Lights)
		{
			iter->Render(pDContext);
		}
		for (auto&[type, objects] : m_ObjectList)
		{
			if (type == EObjType::UI)
				continue;
			for (auto& iter : objects)
			{
				iter->Render(pDContext);
			}
		}
		for (auto& iter : m_InstanceList)
		{
			iter->Render(pDContext);
		}
		CurCamera = Cameras[ECamera::Main];
		DxManager::Get().SetViewPort(EViewPort::Main);
	}
	return true;
}

bool ObjectManager::Release() noexcept
{
	for (auto& outIter : m_ObjectList)
	{
		for (auto& inIter : outIter.second)
		{
			// Global이 아닌 오브젝트 제거
			if (inIter->isGlobal())
				continue;
			inIter->isEnable(false);
			RemoveObject(inIter);
		}
	}
	return true;
}

void ObjectManager::ProcessPostEvent()	noexcept
{
	// 후처리 이벤트
	while (!PostFrameEvent.empty())
	{
		auto&[postEvent, param1, param2] = PostFrameEvent.front();
		postEvent(param1, param2);
		PostFrameEvent.pop();
	}
}

// 스트라이트 리스트
bool ObjectManager::ReadSpriteScript() noexcept
{
	std::wifstream readStream(L"../../data/script/sprite.txt");
	if (!readStream.is_open())
	{
		ErrorMessage(__FUNCTION__ + " -> Read File Error!"s);
		return false;
	}

	Texture*	_pTexture = nullptr;
	float		_frame	  = 0.0f;
	D3DXVECTOR4 _vector4;
	wstring		_objName, _srcName;
	wstring		subString;

	while (std::getline(readStream, subString))
	{
		if (subString.empty()) 
			break;
		wistringstream lineStream(subString);
		lineStream >> _objName >> _srcName;
		///
		++LoadClass::LoadingCount;
		LoadClass::LoadingString = subString.c_str();
		this_thread::sleep_for(chrono::milliseconds(20));

		_pTexture = DxManager::Get().GetTexture(_srcName);
		while (std::getline(readStream, subString))
		{
			if (subString.empty())
				break;

			wistringstream lineStream2(subString);
			//
			_frame	 = 0.0f;
			_vector4 = Vector4::Zero;
			lineStream2 >> _frame >> _vector4.x >> _vector4.y >> _vector4.z >> _vector4.w;

			_vector4.x /= _pTexture->GetTexWidth();
			_vector4.y /= _pTexture->GetTexHeight();
			_vector4.z /= _pTexture->GetTexWidth();
			_vector4.w /= _pTexture->GetTexHeight();

			m_SpriteList[_objName].emplace_back(_pTexture, _frame, _vector4);
		}
	}
	return true;
}

void ObjectManager::SetCurCamera(const ECamera& eCamera) noexcept
{
	CurCamera = Cameras[eCamera];
}

forward_list<GameObject*>* ObjectManager::GetObjectList(const EObjType& objType) noexcept
{
	return &m_ObjectList[objType];
}

map<EObjType, forward_list<GameObject*> >& ObjectManager::GetObjectList() noexcept
{
	return m_ObjectList;
}

vector<Sprite>* ObjectManager::GetSpriteList(const wstring_view& spriteName) noexcept
{
	auto iter = m_SpriteList.find(spriteName.data());
	if (iter == m_SpriteList.end())
	{
		auto pTexture = DxManager::GetInstance().GetTexture(spriteName.data());
		// 스프라이트 생성하고 스프라이트 리스트로 추가
		m_SpriteList[spriteName.data()].emplace_back(pTexture, 0.0f);
		return &m_SpriteList[spriteName.data()];
	}
	return &iter->second;
}

map<wstring, vector<Sprite> >& ObjectManager::GetSpriteList() noexcept
{
	return m_SpriteList;
}

GameObject* ObjectManager::TakeObject(const wstring_view& objName, const bool& pushObject) noexcept
{
	GameObject* pObject = nullptr;
	auto&& finder = m_DisabledPull.find(objName.data());
	if (finder == m_DisabledPull.end() ||
		finder->second.empty())
	{
		//대기 풀이 비었다면 복사 생성
		auto&& iter = m_ProtoPull.find(objName.data());
		if (iter == m_ProtoPull.end())
		{
			ErrorMessage(__FUNCTIONW__ + L" -> "s + objName.data() + L" : 미등록된 오브젝트!");
			return nullptr;
		}
		pObject = iter->second->clone();			// 복사
	}
	else
	{
		// 대기 풀이 있다면 꺼내옴
		pObject = finder->second.top();
		finder->second.pop();
		//auto pComp = pObject->GetComponentList(EComponent::Collider);
		//if (pComp != nullptr)
		//{
		//	for (auto& iter : *pComp)
		//	{
		//		PushCollider((Collider*)iter);
		//	}
		//}
		//pComp = pObject->GetComponentList(EComponent::Renderer);
		//if (pComp != nullptr)
		//{
		//	for (auto& iter : *pComp)
		//	{
		//		iter->Update();
		//	}
		//}

		auto pCompList = pObject->GetComponentList();
		for (auto& [eType, pList] : pCompList)
		{
			for (auto& iter : pList)
			{
				if(eType == EComponent::Collider)
					PushCollider((Collider*)iter);
				iter->Update();
			}
		}
	}
	//myObj->Init();
	pObject->isEnable(true);
	
	if (pushObject)
	{
		PushObject(pObject);
	}
	return pObject;
}

bool ObjectManager::SetProtoObject(GameObject* pObject) noexcept
{
	++LoadClass::LoadingCount;
	LoadClass::LoadingString = L"Setting " + pObject->m_myName;
	this_thread::sleep_for(chrono::milliseconds(50));

	if (m_ProtoPull.find(pObject->m_myName) != m_ProtoPull.end())
	{
		ErrorMessage(__FUNCTION__ + " -> 중복된 이름!"s);
		return false;
	}
	m_ProtoPull[pObject->m_myName] = pObject;

	// KeyObject 제거
	KeyObjects.erase(pObject->m_keyValue);
	// 충돌체 있을시 리스트 제거
	auto pColliders = pObject->GetComponentList(EComponent::Collider);
	if (pColliders != nullptr)
	{
		for(auto& iter : *pColliders)
		{
			PopCollider((Collider*)iter);
		}
	}
	return true;
}

void ObjectManager::PushObject(GameObject* pObject, const bool& isPostEvent) noexcept
{
	if (isPostEvent)
	{
		static auto pPushEvent = [](void* pObj, void*) {
			ObjectManager::Get().PushObject((GameObject*)pObj, false);
		};
		PostFrameEvent.emplace(pPushEvent, pObject, nullptr);
	}
	else
	{
		auto& objList = m_ObjectList[pObject->m_objType];
		if (find(objList.begin(), objList.end(), pObject) == objList.end())
		{
			objList.push_front(pObject);
			pObject->isEnable(true);
		}
		else
		{
			ErrorMessage(__FUNCTIONW__ + L" -> 중복 객체 등록, "s + pObject->m_myName + L" : "  + to_wstring(pObject->m_keyValue));
		}
	}
}

void ObjectManager::PopObject(GameObject* pObject) noexcept
{
	auto& findList = m_ObjectList[pObject->m_objType];
	findList.remove(pObject);
	//auto&& iter = find(findList.begin(), findList.end(), pObject);
	//if (iter == findList.end())
	//{
	//	//ErrorMessage(__FUNCTIONW__ + L" -> "s + pObject->m_myName + L", Not Found!" );
	//	return;
	//}
	// 충돌체 제거
	auto pColliders = pObject->GetComponentList(EComponent::Collider);
	if (pColliders != nullptr)
	{
		for (auto& pCol : *pColliders)
		{
			PopCollider((Collider*)pCol, false);
		}
	}
	//if (pObject->GetParent() != nullptr)
	//{
	//	pObject->CutParent(false);
	//}
	//findList.remove(*iter);
}

void ObjectManager::DisableObject(GameObject* pObject) noexcept
{
	static auto disableEvent = [](void* pVoid, void*) {
		GameObject* pObj = (GameObject*)pVoid;
		pObj->isEnable(false);
		pObj->CutParent(false);
		pObj->SetPosition(Vector3::Up * 100000.0f);
		ObjectManager::Get().PopObject(pObj);
		ObjectManager::Get().m_DisabledPull[pObj->m_myName].push(pObj);
		auto pColliders = pObj->GetComponentList(EComponent::Collider);
		if (pColliders != nullptr)
		{
			for (auto& iter : *pColliders)
			{
				((Collider*)iter)->ClearIgnoreList(false);
				ObjectManager::Get().PopCollider((Collider*)iter, false);
			}
		}
	};

	if (pObject != nullptr && pObject->isEnable())
	{
		pObject->isEnable(false);
		PostFrameEvent.emplace(disableEvent, pObject, nullptr);
	}
	//else
	//	ErrorMessage(__FUNCTIONW__ + L" -> 비활성 오브젝트 : "s + pObject->m_myName);
}

bool ObjectManager::RemoveObject(GameObject* pObject) noexcept
{
	if (pObject->isGlobal())
	{
		ErrorMessage(__FUNCTIONW__ + L" -> "s + pObject->m_myName + L" : 삭제할 수 없는 오브젝트!");
		return false;
	}
	pObject->isEnable(false);

	static auto deleteEvent = [](void* pVoid, void*) {
		GameObject* pObj = (GameObject*)pVoid;
		pObj->CutParent(false);
		ObjectManager::GetInstance().PopObject(pObj);
		pObj->Release();
		delete pObj;
	};

	PostFrameEvent.emplace(deleteEvent, pObject, nullptr);
	return true;
}

bool ObjectManager::RemoveComponent(Component* pComponent) noexcept
{
	if (pComponent == nullptr)
		return false;
	pComponent->isEnable(false);

	static auto deleteEvent = [](void* pVoid, void* pObj) {
		Component* pComp   = (Component*)pVoid;
		GameObject* pObject = (GameObject*)pObj;

		pComp->m_pParent->GetComponentList()[pComp->m_comptType].remove(pComp);
		pComp->Release();
		delete pComp;
		if (pObject->GetComponentList(EComponent::Collider) == nullptr)
		{
			delete pObject->m_pPhysics;
			pObject->m_pPhysics = nullptr;
		}
	};

	PostFrameEvent.emplace(deleteEvent, pComponent, pComponent->m_pParent);
	return true;
}


Component* ObjectManager::TakeComponent(const wstring_view& compName) noexcept
{
	Component* pComponent = nullptr;

	if (m_DisabledPullComp.find(compName.data()) == m_DisabledPullComp.end() ||
		m_DisabledPullComp[compName.data()].empty())
	{
		auto&& iter = m_ComponentPull.find(compName.data());
		if (iter == m_ComponentPull.end())
		{
			ErrorMessage(__FUNCTIONW__ + L" -> "s + compName.data() + L" : 미등록된 컴포넌트!");
			return nullptr;
		}
		pComponent = iter->second->clone();			// 복사
	}
	else
	{
		// 대기 풀이 있다면 꺼내옴
		pComponent = m_DisabledPullComp[compName.data()].top();
		m_DisabledPullComp[compName.data()].pop();
	}
	//pComponent->Init();
	pComponent->isEnable(true);
	return pComponent;
}

bool ObjectManager::SetProtoComponent(Component* pComponent) noexcept
{
	if (m_ComponentPull.find(pComponent->m_myName) != m_ComponentPull.end())
	{
		ErrorMessage(__FUNCTION__ + " -> 중복된 이름!"s);
		return false;
	}
	m_ComponentPull[pComponent->m_myName] = pComponent;
	//pComponent->isEnable(false);

	if (pComponent->m_comptType == EComponent::Collider)
	{
		PopCollider((Collider*)pComponent);
	}
	return true;
}

void ObjectManager::DisableComponent(Component* pComponent) noexcept
{
	if (pComponent == nullptr)
	{
		ErrorMessage(__FUNCTION__ + " -> Component is Null"s);
		return;
	}

	static auto disableEvent = [](void* pVoid, void*) {
		Component* pComp = (Component*)pVoid;
		pComp->isEnable(false);
		//ObjectManager::Get().PopObject(pComp);
		pComp->m_pParent->GetComponentList()[pComp->m_comptType].remove(pComp);
		ObjectManager::Get().m_DisabledPullComp[pComp->m_myName].push(pComp);

		if (pComp->m_comptType == EComponent::Collider)
		{
			ObjectManager::Get().PopCollider((Collider*)pComp, false);
		}
	};

	PostFrameEvent.emplace(disableEvent, pComponent, nullptr);
}

const forward_list<Collider*>& ObjectManager::GetColliderList() noexcept
{
	return m_ColliderList;
}

void ObjectManager::PushCollider(Collider* pCollider, const bool& isPostEvent) noexcept
{
	if (isPostEvent)
	{
		static auto pPushEvent = [](void* pCollider, void*) {
			ObjectManager::Get().PushCollider((Collider*)pCollider, false);
		};
		PostFrameEvent.emplace(pPushEvent, pCollider, nullptr);
	}
	else
	{
		m_ColliderList.push_front(pCollider);
	}
}

void ObjectManager::PopCollider(Collider* pCollider, const bool& isPostEvent) noexcept
{
	if (isPostEvent)
	{
		static auto pPopEvent = [](void* pCollider, void*) {
			ObjectManager::Get().PopCollider((Collider*)pCollider, false);
		};
		PostFrameEvent.emplace(pPopEvent, pCollider, nullptr);
	}
	else
	{
		m_ColliderList.remove(pCollider);
	}
}



InstanceRenderer* ObjectManager::PushInstance(InstanceRenderer* pInstance) noexcept
{
	for (auto&& iter = m_InstanceList.begin(); iter != m_InstanceList.end(); ++iter)
	{
		if (*iter == pInstance)
		{
			ErrorMessage(__FUNCTION__ + " -> 이미 등록된 인스턴스!"s);
			return nullptr;
		}
	}

	m_InstanceList.push_front(pInstance);
	//pInstance->isEnable(true);
	return pInstance;
}

void ObjectManager::PopInstance(InstanceRenderer* pInstance) noexcept
{
	for (auto&& iter = m_InstanceList.begin(); iter != m_InstanceList.end(); ++iter)
	{
		if (*iter == pInstance)
		{
			m_InstanceList.remove(*iter);
			return;
		}
	}
	//ErrorMessage(__FUNCTIONW__ + L" -> "s + pObject->m_myName + L", Not Found!" );
}