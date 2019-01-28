#pragma once
#include <fstream>
#include <filesystem>
#include "CAnimation.h"
#include "InstanceRenderer.h"

// ~~ ios 옵션 ~~
// app : 기존 파일에 추가, ate : 시작 위치가 파일 끝, trunc : 파일 내용 지우고 염
// binary : 바이너리 모드, nocreate : 열지 않고 있는지만 판단
// cur, beg, end...

// ~~ ifstream ~~
// >> 연산, open(url), close(), read(Type, byte)
// seekg(이동값, 기준위치 ios::cur, beg, end...), tellog() : 현재 위치 반환
// getLine(ref, maxLength, findChar) -> string::getLine(ifstream, string)
// eof(), 

// ~~ ofstream ~~
// << 연산, 
// seekg()


struct SceneDataAse 
{
	wstring FileName;		// 파일 이름
	int FirstFrame	 = 0;	// 시작 프레임
	int LastFrame	 = 0;	// 끝 프레임
	int FrameSpeed	 = 0;	// 초당 프레임 개수
	int TickPerFrame = 0;	// 1프레임 틱 값
	//int MeshCount;		// 메쉬오브젝트 개수
	//int MaxWeight;		// 정점 당 가중치
	//int BindPose;			// 바인딩 포즈 에니메이션 여부
};

struct MaterialDataAse
{
	UINT	MaterialIndex;
	wstring MaterialName;
	wstring MaterialClassName;	// 클래스 타입(Standard, SubMaterial...)
	vector<MaterialDataAse> SubMaterials;
	Texture* pTexture = nullptr;
	//vector<Tex>;
	//UINT SubCount;
	//UINT TexMapCount;
	MaterialDataAse* pParent = nullptr;
};



class AseParser
{
protected:
	// 검색용 열거형 태그
	enum class EAseTag : char {
		Scene, MaterialList, GeomObject, HelperObject
	};
	enum class EScene : char {
		_MaterialList = 0,	
		FileName, FirstFrame, LastFrame, FrameSpeed, TickPerFrame,
	};
	enum class EMaterial : char {
		_Material = 0,	_GeomObject, _HelperObject,
		MatrialCount, MaterialName, MaterialClass, 
		NumSubMaterial, SubMaterial,
		MapDiffuse, BitMap, MapReflect
	};
	enum class EMaterial_Class : char {
		Standard, MultySubObject,
	};
	enum class EHelper : char {
		_GeomObject = 0, _HelperObject,
		NodeName, NodeParent, HelperClass, 
		NodeTM, BoundingMin, BoundingMax, Anim
	};
	enum class EGeom : char {
		_GeomObject = 0, _HelperObject,
		NodeName,	NodeParent,		MaterialRef,
		NodeTM,			Mesh,		Anim,	
	};
	enum class EGeom_Mesh : char {
		_MaterialRef = 0,	_NodeTM,			_Animation,		
		TimeValue,
		NumVertex,		VertexList,			Vertex,			// posision
		NumFaces,		FaceList,			Face,
		NumTexVertex,	TexVertexList,		TexVertex,		// texture
		NumTexFaces,	TexFaceList,		TexFace,
		NumColorVertex, ColorVertexList,	ColorVertex,	// color
		NumColorFace,	ColorFaceList,		ColorFace,
		Nomals,			FaceNormal,			VertexNormal,	// normal 
	};
	enum class EObject_NodeTM : char {
		_GeomObject = 0,_HelperObject,
		_MaterialRef,	_Mesh,		_Animation,
		NodeName, 
		TM_ROW0,		TM_ROW1,		TM_ROW2,		TM_ROW3,
		TM_POS,			TM_ROTAXIS, 
		TM_ROTANGLE,	TM_SCALE, 
		TM_SCALEAXIS,	TM_SCALEAXISANG,
	};
	enum class EObject_Animation {
		_GeomObject = 0, _HelperObject, _MaterialRef,
		NodeName,			
		PositionTrack,	 PositionSample,
		RotationTrack,	 RotationSample,
		ScaleTrack,		 ScaleSample,
	};
protected:
	filesystem::path m_filePath;					// 파일 경로
	std::streampos m_fileSize;						// 파일 바이트

	forward_list<wistringstream> m_LineBuffers;				// 전체 라인 배열
	forward_list<wistringstream>::iterator m_curLineIter;	// 현재 라인
	wstring m_subString;		// 임시 스트링
	SceneDataAse m_SceneData;		// 씬 데이터

	// 검색용 태그 맵
	static map<wstring, EAseTag>			m_AseTag;
	static map<wstring, EScene>				m_SceneTag;
	static map<wstring, EMaterial>			m_MaterialTag;
	static map<wstring, EMaterial_Class>	m_MaterialClass;
	static map<wstring, EHelper>			m_HelperTag;
	static map<wstring, EGeom>				m_GeomTag;
	static map<wstring, EGeom_Mesh>			m_GeomMesh;
	static map<wstring, EObject_NodeTM>		m_ObjectNodeTM;
	static map<wstring, EObject_Animation>	m_ObjectAnimation;

	vector<shared_ptr<MaterialDataAse> >	m_MaterialList;
	map<wstring, GameObject*>			m_ObjectList;
	map<wstring, InstanceRenderer*>		m_Instances;
private:
	// 버퍼 내용으로 객체 생성
	bool LoadFileData(GameObject** ppReturnObj) noexcept;
	// 각 데이터 채움
	bool LoadScene(SceneDataAse* pScene) noexcept;
	bool LoadMaterialList() noexcept;
	bool LoadHelperObject() noexcept;
	bool LoadGeomObject()	noexcept;
	bool LoadGeomMesh(vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList, const D3DXMATRIX& matWorld)	noexcept;
	bool LoadObjectNodeTM(GameObject* pObject, CAnimation* pAnimation) noexcept;
	bool LoadObjectAnimation(vector<Anim>* pAnimations) noexcept;
	void SetVertexIndex(vector<Vertex_PNCT>* pTempVertex, vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList) noexcept;
	void Clear() noexcept;				// 버퍼 초기화
public:
	// 파일 불러오고 객체 생성후 반환
	bool CreateFromFile(GameObject** ppReturnObj, const wstring_view& fileName, const wstring_view& directory = L"..\\..\\data") noexcept;
public:
	AseParser() noexcept;
	virtual ~AseParser() = default;
};