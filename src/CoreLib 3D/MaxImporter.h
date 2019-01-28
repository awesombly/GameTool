#pragma once
#include <filesystem>
#include <fstream>
#include "GameObject.h"
#include "CAnimation.h"
#include "InstanceRenderer.h"


enum class EClassType : char {
	Geom = 0, Biped,
	Bone, Dummy,
};

struct SceneData
{
	int   TotalFrame = 0;	 // 총 프레임 수
	float SPF		 = 0.0f; // 프레임 간격
	int   MeshCount;		 // 메쉬오브젝트 개수
	int   MaxWeight;		 // 정점 당 가중치
	bool  BindPose;			 // 바인딩 포즈 에니메이션 여부
}; 

struct MaterialData
{
	UINT	MaterialIndex;
	//wstring MaterialName;
	//wstring MaterialClassName;	// 클래스 타입(Standard, SubMaterial...)
	vector<MaterialData> SubMaterials;
	Texture* pTexture = nullptr;
	//vector<Tex>;
	//UINT SubCount;
	//UINT TexMapCount;
	MaterialData* pParent = nullptr;
};

class ParticleSystem;

class MaxImporter
{
protected:
	// 검색용 열거형 태그
	enum class EAseTag : char {
		Scene, MaterialList, Mesh
	};
protected:
	filesystem::path m_filePath;							// 파일 경로
	std::streampos	 m_fileSize;							// 파일 바이트

	forward_list<istringstream> m_LineBuffers;				// 전체 라인 배열
	forward_list<istringstream>::iterator m_curLineIter;	// 현재 라인
	string m_subString;		// 임시 스트링
	SceneData m_SceneData;		// 씬 데이터

	// 검색용 태그 맵
	static map<string, EAseTag>			m_AseTag;

	vector<MaterialData>				m_MaterialList;
	map<string, GameObject*>			m_Objects;
	map<wstring, InstanceRenderer*>		m_Instances;
private:
	// 버퍼 내용으로 객체 생성
	bool LoadFileData(GameObject** ppReturnObj)			noexcept;
	// 각 데이터 채움
	bool LoadScene(SceneData* pScene)					noexcept;
	bool LoadMaterialList(const int& parentCnt)			noexcept;
	bool LoadObject(const int& objCnt)					noexcept;
	// 버퍼 초기화
	void Clear()										noexcept;
	// Particle
	//void LoadParticleSystem(GameObject** ppReturnObj)	noexcept;
public:
	// 파일 불러오고 객체 생성후 반환
	bool CreateFromFile(GameObject** ppReturnObj, const wstring_view& fileName, const wstring_view& directory = L"..\\..\\data") noexcept;
	ParticleSystem* CreateFromParticle(const wstring_view& fileName, const wstring_view& directory = L"..\\..\\data")	noexcept;
public:
	MaxImporter() noexcept;
	virtual ~MaxImporter() = default;
};