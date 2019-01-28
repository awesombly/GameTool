#include "GameObject.h"
#include "ObjectManager.h"
#include "Collider.h"

//GameObject::GameObject()
//{
//	
//}

//GameObject::~GameObject() noexcept
//{
//	ObjectManager::KeyObjects.erase(m_keyValue);
//}

GameObject::GameObject(const wstring_view& myName, const EObjType& eType)
{
	m_keyValue = ++ObjectManager::KeyCount;
	ObjectManager::KeyObjects[m_keyValue] = this;
	m_myName = myName;
	m_objType = eType;
	Init();
}

GameObject::GameObject(const wstring_view& myName, const std::initializer_list<Component*>& components, const EObjType& eType)
{
	m_keyValue = ++ObjectManager::KeyCount;
	ObjectManager::KeyObjects[m_keyValue] = this;
	m_myName = myName;
	m_objType = eType;
	AddComponent(components);
	Init();
}

GameObject::GameObject(const wstring_view& myName, Component* pComponent, const EObjType& eType)
{
	m_keyValue = ++ObjectManager::KeyCount;
	ObjectManager::KeyObjects[m_keyValue] = this;
	m_myName = myName;
	m_objType = eType;
	AddComponent(pComponent);
	Init();
}

bool GameObject::Init() noexcept
{
	//m_pParent = nullptr;
	m_Look = { 0.0f, 0.0f, 1.0f };
	m_Side = { 1.0f, 0.0f, 0.0f };
	m_Up = { 0.0f, 1.0f, 0.0f };

	m_position = { 0,0,0 };
	m_rotation = { 0,0,0,0 };
	m_scale = { 1,1,1 };
	return true;
}

bool GameObject::Frame(const float& spf, const float& accTime) noexcept
{
	if (!m_isEnable || m_isStatic) return false;
	UpdateMatrix();
	if (m_pPhysics != nullptr && usePhysics())
	{
		//m_pPhysics->m_prePosition = m_position;
		// 중력
		m_pPhysics->m_force += m_pPhysics->m_GravityScale * GravityPower * spf * Vector3::Down;
		// 항력
		m_pPhysics->m_force -= m_pPhysics->m_damping * spf * m_pPhysics->m_force;

		// 힘 적용
		if (GetVelocitySq() > 20.0f)
		{
			//m_pParent->isMoved(true);
			GetRoot()->Translate((GetTotalForce() + Vector3::Up) * spf * m_pPhysics->m_mass);
		}
	}
	for (auto& outIter : m_components)
	{
		for (auto& inIter : outIter.second)
		{
			inIter->Frame(spf, accTime);
		}
	}
	for (auto& iter : m_childList)
	{
		iter->Frame(spf, accTime);
	}
	return true;
}

bool GameObject::Render(ID3D11DeviceContext* pDContext) noexcept
{
	if (!m_isEnable) return false;
	
	for (auto& outIter : m_components)
	{
		for (auto& inIter : outIter.second)
		{
			inIter->Render(pDContext);
		}
	}
	for (auto& iter : m_childList)
	{
		iter->Render(pDContext);
	}
	return true;
}

bool GameObject::Release() noexcept
{
	ObjectManager::KeyObjects.erase(m_keyValue);
	for (auto& iter : m_childList)
	{
		iter->Release();
		delete iter;
	}
	for (auto& outIter : m_components)
	{
		for (auto& inIter : outIter.second)
		{
			if (outIter.first == EComponent::Collider)
			{
				((Collider*)inIter)->ClearIgnoreList(false);
				ObjectManager::Get().GetColliderList().remove((Collider*)inIter);
			}
			inIter->Release();
			delete inIter;
		}
	}
	m_childList.clear();
	m_components.clear();
	if (m_pPhysics != nullptr)
		delete m_pPhysics;
	//delete this;
	return true;
}


void GameObject::UpdateMatrix() noexcept
{
	D3DXMatrixScaling(&m_matScale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&m_matRotation, m_rotation.y, m_rotation.x, m_rotation.z);
	m_matLocal = m_matScale * m_matRotation;
	m_matLocal._41 = m_position.x;
	m_matLocal._42 = m_position.y;
	m_matLocal._43 = m_position.z;

	// 애니메이션 적용
	if (m_components.find(EComponent::Animation) != m_components.end())
	{
		for (auto& iter : m_components[EComponent::Animation])
			iter->Update();
	}

	D3DXMatrixTranspose(&m_matLocal, &m_matLocal);
	if (m_pParent != nullptr)
	{
		m_matLocal = m_pParent->m_matLocal * m_matLocal;
	}
	if (m_isBillBoard)
	{
		static D3DXMATRIX matBill;
		matBill = ObjectManager::CurCamera->m_matView;
		matBill._41 = 0.0f;
		matBill._42 = 0.0f;
		matBill._43 = 0.0f;
		//D3DXMatrixInverse(&matBill, nullptr, &matBill);
		m_matLocal *= matBill/* * m_matLocal*/;
	}

	//m_Side	= { m_matRotation._11, m_matRotation._12, m_matRotation._13 };
	//m_Up	= { m_matRotation._21, m_matRotation._22, m_matRotation._23 };
	//m_Look	= { m_matRotation._31, m_matRotation._32, m_matRotation._33 };

	m_Side	= { m_matLocal._11, m_matLocal._21, m_matLocal._31 };
	m_Up	= { m_matLocal._12, m_matLocal._22, m_matLocal._32 };
	m_Look	= { m_matLocal._13, m_matLocal._23, m_matLocal._33 };
}


void GameObject::SetParent(GameObject* pParent) noexcept
{
	static auto pEvent = [](void* pVoid, void* pVoid2) {
		auto pObject = (GameObject*)pVoid;
		auto pBase   = (GameObject*)pVoid2;

		auto&& iter = find(pBase->m_childList.begin(), pBase->m_childList.end(), pObject);
		if (pBase == pObject || pObject->m_pParent == pBase || iter != pBase->m_childList.end())
			return;
		if (pObject->m_pParent != nullptr)
			pObject->CutParent(false);
		pObject->m_pParent = pBase;
		pBase->m_childList.push_front(pObject);
		ObjectManager::GetInstance().PopObject(pObject);
	};

	ObjectManager::PostFrameEvent.emplace(pEvent, this, pParent);
}

GameObject* GameObject::GetParent() const noexcept
{
	return m_pParent;
}

GameObject* GameObject::GetRoot() noexcept
{
	GameObject* pRoot = this;
	while (pRoot->m_pParent != nullptr)
	{
		pRoot = pRoot->m_pParent;
	}
	return pRoot;
}


void GameObject::CutParent(const bool& pushObject) noexcept
{
	if (m_pParent == nullptr)
		return;
	auto iter = find(m_pParent->m_childList.begin(), m_pParent->m_childList.end(), this);
	if (iter != m_pParent->m_childList.end())
	{
		m_pParent->m_childList.remove(*iter);
	}
	m_pParent = nullptr;
	if (pushObject)
		ObjectManager::GetInstance().PushObject(this);
}

void GameObject::CutParent(const bool& pushObject, const bool& isPostEvent) noexcept
{
	if (isPostEvent)
	{
		static auto pEvent = [](void* pVoid, void*) {
			((GameObject*)pVoid)->CutParent(true, false);
		};

		ObjectManager::PostFrameEvent.emplace(pEvent, this, nullptr);
		return;
	}

	if (m_pParent == nullptr)
		return;
	auto iter = find(m_pParent->m_childList.begin(), m_pParent->m_childList.end(), this);
	if (iter != m_pParent->m_childList.end())
	{
		m_pParent->m_childList.remove(*iter);
	}
	m_pParent = nullptr;
	if (pushObject)
		ObjectManager::GetInstance().PushObject(this);
}


forward_list<GameObject*>* GameObject::GetChildList() noexcept
{
	return &m_childList;
}

void GameObject::AddComponent(Component* pComponent) noexcept
{
	pComponent->m_pParent = this;
	m_components[pComponent->m_comptType].push_front(pComponent);
	// Collider 일시
	if (pComponent->m_comptType == EComponent::Collider)
	{
		if(m_pPhysics == nullptr)
			m_pPhysics = new PhysicsInfo();
		((Collider*)pComponent)->m_pPhysics = m_pPhysics;
	}
	pComponent->Update();
}

void GameObject::AddComponent(const initializer_list<Component*>& components) noexcept
{
	for (auto& iter : components)
	{
		iter->m_pParent = this;
		m_components[iter->m_comptType].push_front(iter);
		// Collider 일시
		if (iter->m_comptType == EComponent::Collider)
		{
			if (m_pPhysics == nullptr)
				m_pPhysics = new PhysicsInfo();
			((Collider*)iter)->m_pPhysics = m_pPhysics;
		}
		iter->Update();
	}
}

void GameObject::RemoveComponent(Component* pComponent) noexcept 
{
	ObjectManager::GetInstance().RemoveComponent(pComponent);
}

forward_list<Component*>* GameObject::GetComponentList(const EComponent& eCompType) noexcept
{
	auto&& iter = m_components.find(eCompType);
	if (iter == m_components.end() || (*iter).second.empty())
		return nullptr;
	return &iter->second;
}

map<EComponent, forward_list<Component*> >& GameObject::GetComponentList() noexcept
{
	return m_components;
}

Component* GameObject::GetComponent(const EComponent& eCompType) noexcept
{
	auto&& iter = m_components.find(eCompType);
	if (iter == m_components.end() || (*iter).second.empty())
		return nullptr;
	return iter->second.front();
}

Collider* GameObject::GetCollider() noexcept
{
	auto&& iter = m_components.find(EComponent::Collider);
	if (iter == m_components.end() || (*iter).second.empty())
		return nullptr;
	return (Collider*)iter->second.front();
}

void GameObject::SetWorldPosition(const D3DXVECTOR3& position) noexcept 
{
	m_position = position - m_pParent->GetWorldPosition();
}

void GameObject::SetWorldRotation(const D3DXQUATERNION& rotation) noexcept
{
	m_rotation = rotation - m_pParent->GetWorldRotation();
}

void GameObject::SetWorldScale(const D3DXVECTOR3& scale) noexcept
{
	m_scale = Divide(scale, m_pParent->GetWorldScale());
}

D3DXVECTOR3	  GameObject::GetWorldPosition() const noexcept
{
	if (m_pParent == nullptr)
		return m_position;
	return m_position + m_pParent->GetWorldPosition();
}
D3DXQUATERNION GameObject::GetWorldRotation() const noexcept
{
	if (m_pParent == nullptr)
		return m_rotation;
	return m_rotation + m_pParent->GetWorldRotation();
	//return m_rotation * m_pParent->GetWorldRotation();
}
D3DXVECTOR3	  GameObject::GetWorldScale()	 const noexcept
{
	if (m_pParent == nullptr)
		return m_scale;
	return Product(m_scale, m_pParent->GetWorldScale());
}

void GameObject::SetWorldMatrix(const D3DXMATRIX& matWorld) noexcept
{
	m_matLocal = matWorld;
}

const D3DXMATRIX& GameObject::GetWorldMatrix()	  const noexcept
{
	return m_matLocal;
}

const D3DXMATRIX& GameObject::GetScaleMatrix() const noexcept
{
	return m_matScale;
}
const D3DXMATRIX& GameObject::GetRotationMatrix() const noexcept
{
	return m_matRotation;
}


void GameObject::isEnable(const bool& isEnable, const bool& putDisable) noexcept
{
	if (putDisable)
		ObjectManager::Get().DisableObject(this);
	else
		m_isEnable = isEnable;
}

bool GameObject::isEnable() noexcept
{
	return m_isEnable;
}

void GameObject::isGlobal(const bool& isGlobal) noexcept
{
	m_isGlobal = isGlobal;
}
bool GameObject::isGlobal() noexcept
{
	return m_isGlobal;
}

void GameObject::isStatic(const bool& isStatic) noexcept
{
	if(isStatic)
		UpdateMatrix();
	m_isStatic = isStatic;
}
bool GameObject::isStatic() noexcept
{
	return m_isStatic;
}

void GameObject::isBillBoard(const bool& isBillBoard) noexcept
{
	m_isBillBoard = isBillBoard;
}
bool GameObject::isBillBoard() noexcept
{
	return m_isBillBoard;
}


void GameObject::SetKeyValue(const UINT& keyValue) noexcept
{
	auto finder = ObjectManager::KeyObjects.find(m_keyValue);
	if (finder != ObjectManager::KeyObjects.end())
	{
		if (finder->second == this)
		{
			ObjectManager::KeyObjects.erase(m_keyValue);
		}
		//else
		//{
		//	ErrorMessage(__FUNCTION__ + " -> 중복된 키값!"s);
		//	return;
		//}
	}
	
	ObjectManager::KeyObjects[m_keyValue = keyValue] = this;
}

GameObject* GameObject::clone() noexcept
{
	return cloneChild(new GameObject(*this));
}

GameObject* GameObject::cloneChild(GameObject* pObject) noexcept
{
	ObjectManager::KeyObjects[pObject->m_keyValue = ++ObjectManager::KeyCount] = pObject;
	vector<GameObject*> addObjects;
	vector<Component*> addComponents;
	// Physics 재생성
	if (pObject->m_pPhysics != nullptr)
	{
		pObject->m_pPhysics = new PhysicsInfo(*pObject->m_pPhysics);
	}
	// 자식들 복사
	for (auto& iter : pObject->m_childList)
	{
		addObjects.emplace_back(iter->clone());
	}
	pObject->m_childList.clear();
	for (auto& iter : addObjects)
	{
		iter->m_pParent = nullptr;
		iter->SetParent(pObject);
		iter->isEnable(true);
	}
	// 컴포넌트 복사
	for (auto& outIter : pObject->m_components)
	{
		for (auto& inIter : outIter.second)
		{
			addComponents.emplace_back(inIter->clone());
		}
	}
	pObject->m_components.clear();
	for (auto& iter : addComponents)
	{
		pObject->AddComponent(iter);
	}

	return pObject;
}