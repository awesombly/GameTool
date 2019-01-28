#pragma once
#include "Transform.h"
//#include "Component.h"
//#include "Physics.h"
#include "Collider.h"



enum class EObjType : char {
	Dummy,
	Map,
	Object,
	Character,
	Enemy,
	Image,
	Camera,
	Light,
	Effect,
	UI,
};


// 오브젝트로 기능 가능한 기본 클래스
class GameObject : public Transform, public IDxBasis, public Physics
{
protected:
	GameObject*						 m_pParent = nullptr;	 // 부모 포인터
	forward_list<GameObject*>		 m_childList;			 // 자식 리스트
	map<EComponent, forward_list<Component*> > m_components; // 컴포넌트 리스트

	D3DXMATRIX m_matLocal;			// Local 행렬(사실 월드ㅎ)
	D3DXMATRIX m_matScale	 = Matrix::Identity; // Scale 행렬
	D3DXMATRIX m_matRotation = Matrix::Identity; // Rotation 행렬
	
	bool  m_isEnable	= true;		// 활성화 여부
	bool  m_isGlobal	= false;	// ObjectManager 릴리즈시 유지 여부
	bool  m_isStatic	= false;	// SRT 갱신 여부
	bool  m_isBillBoard = false;	// 빌보드 행렬 적용 여부

	float m__;	// 안씀
public:
	UINT	 m_keyValue;							// 유일한 키값
	wstring	 m_myName;								// 객체 이름
	EObjType m_objType = EObjType::Dummy;			// 객체 타입
public:
	virtual void UpdateMatrix()												noexcept;
	// 컴포넌트 추가, 삭제, 검색
	void AddComponent(Component* pComponent)								noexcept;
	void AddComponent(const initializer_list<Component*>& components)		noexcept;
	void RemoveComponent(Component* pComponent)								noexcept;
	forward_list<Component*>* GetComponentList(const EComponent& eCompType) noexcept;
	map<EComponent, forward_list<Component*> >& GetComponentList()			noexcept;
	Component* GetComponent(const EComponent& eCompType)					noexcept;
	Collider* GetCollider()													noexcept;
	// 부모 설정
	virtual void SetParent(GameObject* pParent)						  	    noexcept;
	GameObject*  GetParent()										  const noexcept;
	GameObject*  GetRoot()											  	    noexcept;
	void		 CutParent(const bool& pushObject = true) noexcept;
	void		 CutParent(const bool& pushObject, const bool& isPostEvent) noexcept;
	forward_list<GameObject*>* GetChildList()						  	    noexcept;
	// 부모월드 * 자식로컬
	void SetWorldPosition(const D3DXVECTOR3& position)						noexcept;
	void SetWorldRotation(const D3DXQUATERNION& rotation)					noexcept;
	void SetWorldScale(const D3DXVECTOR3& scale)							noexcept;
	D3DXVECTOR3	   GetWorldPosition()								  const noexcept;
	D3DXQUATERNION GetWorldRotation()								  const noexcept;
	D3DXVECTOR3	   GetWorldScale()									  const noexcept;
	// 행렬 설정, 반환
	void  SetWorldMatrix(const D3DXMATRIX& matWorld)						noexcept;
	const D3DXMATRIX& GetWorldMatrix()								  const noexcept;
	const D3DXMATRIX& GetScaleMatrix()								  const noexcept;
	const D3DXMATRIX& GetRotationMatrix()							  const noexcept;

	void SetKeyValue(const UINT& keyValue)									noexcept;

	bool isEnable()															noexcept;
	void isEnable(const bool& isEnable, const bool& putDisablePool = false) noexcept;
	bool isGlobal()															noexcept;
	void isGlobal(const bool& isGlobal)										noexcept;
	bool isStatic()															noexcept;
	void isStatic(const bool& isStatic)										noexcept;
	bool isBillBoard()														noexcept;
	void isBillBoard(const bool& isBillBoard)								noexcept;
	//bool isMoved()									noexcept;
	//void isMoved(const bool& m_isMoved)				noexcept;

	virtual bool Init()														noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)				noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)						noexcept override;
	virtual bool Release()													noexcept override;
	virtual GameObject* clone()												noexcept;		// 자기 객체 복사 반환
protected:
	GameObject* cloneChild(GameObject* pObject)								noexcept;		// 자식 복제용
protected:
	friend class AseParser;
	friend class MaxImporter;
	friend class Dialog_Hierarchy;
	friend class CAnimation;
public:
	GameObject() = default;
	GameObject(const wstring_view& myName, const EObjType& eType = EObjType::Dummy);
	GameObject(const wstring_view& myName, const std::initializer_list<Component*>& components, const EObjType& eType = EObjType::Dummy);
	GameObject(const wstring_view& myName, Component* pComponent, const EObjType& eType = EObjType::Dummy);
	virtual ~GameObject() noexcept = default;
};