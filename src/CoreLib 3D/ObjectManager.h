#pragma once
#include "DxManager.h"
//#include "Sprite.h"
#include "InstanceRenderer.h"
#include "Light.h"

enum class ECamera : char{
	//Current,		// 적용중인 카메라
	Screen,			// 랜더 타겟용
	Main,			// 메인 카메라
	UI,				// 화면 직교 카메라
	MiniMap,
	Camera1,
	Camera2,
	Camera3,
};

class GameObject;
class Collider;

// 등록된 오브젝트를 관리하고 순회-동작 시키는 매니저
class ObjectManager : public IDxBasis, public ISingleton<ObjectManager>
{
private:
	// 오브젝트 리스트
	map<EObjType, forward_list<GameObject*> >	 m_ObjectList;		// 전체 순회용 리스트<타입>
	map<wstring, stack<GameObject*> >			 m_DisabledPull;	// 대기 상태 풀<이름>
	map<wstring, GameObject*>					 m_ProtoPull;		// 복사용 오브젝트 풀<이름>

	map<wstring, stack<Component*> >			 m_DisabledPullComp;// 대기 상태 컴포넌트 풀
	map<wstring, Component*>					 m_ComponentPull;	// 복사용 컴포넌트

	forward_list<Collider*>						 m_ColliderList;	// 충돌체 리스트
	forward_list<InstanceRenderer*>				 m_InstanceList;	// 인스턴싱용 리스트

	map<wstring, vector<Sprite> > m_SpriteList;						// 스프라이트 리스트
public:
	static map<UINT, GameObject*> KeyObjects;						// 서버 처리용 오브젝트들
	static UINT					  KeyCount;							// 키값 설정용
	static map<ECamera, Camera*>  Cameras;							// 카메라들
	static Camera*				  CurCamera;							// 적용중인 카메라
	static list<Light*>			  Lights;							// 조명들
	// 후처리 이벤트 등록용(함수, 인자, 인자)
	static queue<tuple<void(*)(void*, void*), void*, void*> > PostFrameEvent;
public:
	void ProcessPostEvent()	noexcept;
	// txt 파일 읽기
	bool ReadSpriteScript()	noexcept;
	// 카메라 설정
	void SetCurCamera(const ECamera& eCamera)	noexcept;
	// 리스트 반환
	vector<Sprite>* GetSpriteList(const wstring_view& spriteName)	  noexcept;
	map<wstring, vector<Sprite> > & GetSpriteList()					  noexcept;
	forward_list<GameObject*>* GetObjectList(const EObjType& objType) noexcept;
	// 오브젝트 가져오기 (대기 풀에 남아있거나, 복사 풀에 등록 필요)
	GameObject* TakeObject(const wstring_view& objName, const bool& pushObject = true) noexcept;
	bool SetProtoObject(GameObject* pObject)					noexcept;	// 복사 풀에 추가
	void PushObject(GameObject* pObject)						noexcept;	// 오브젝트 리스트에 추가
	void PopObject(GameObject* pObject)							noexcept;	// 오브젝트 리스트에서 꺼내기
	void DisableObject(GameObject* pObject)						noexcept;	// 오브젝트 비활성화(->대기 풀)
	bool RemoveObject(GameObject* pObject)						noexcept;	// 오브젝트 제거(삭제 이벤트 등록)
	// 컴포넌트 가져오기, 등록, 삭제
	Component* TakeComponent(const wstring_view& compName)		noexcept;
	bool SetProtoComponent(Component* pComponent)				noexcept;
	void DisableComponent(Component* pComponent)				noexcept;	// 컴포넌트 비활성화(->대기 풀)
	bool RemoveComponent(Component* pComponent)					noexcept;	// 컴포넌트 제거("")
	// 충돌체 리스트 추가, 제거
	forward_list<Collider*>& GetColliderList()					noexcept;
	void PushCollider(Collider* pCollider, const bool& isPostEvent = true) noexcept;
	void PopCollider(Collider* pCollider, const bool& isPostEvent = true)  noexcept;

	// 인스턴스 리스트 넣기, 빼기
	InstanceRenderer* PushInstance(InstanceRenderer* pInstance) noexcept;
	void PopInstance(InstanceRenderer* pInstance)				noexcept;				

	virtual bool Init()											noexcept override;
	virtual bool Frame(const float& spf, const float& accTime)	noexcept override;
	virtual bool Render(ID3D11DeviceContext* pDContext)			noexcept override;
	virtual bool Release()										noexcept override;
private:
	friend class ISingleton<ObjectManager>;
	friend class Dialog_Preset;
	ObjectManager() = default;
public:
	virtual ~ObjectManager() = default;
};