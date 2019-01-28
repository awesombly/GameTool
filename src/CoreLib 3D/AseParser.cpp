#include "AseParser.h"
#include "ObjectManager.h"
//#include "AseObject.h"
//#include "InstanceRenderer.h"


map<wstring, AseParser::EAseTag>			AseParser::m_AseTag;
map<wstring, AseParser::EScene>				AseParser::m_SceneTag;
map<wstring, AseParser::EMaterial>			AseParser::m_MaterialTag;
map<wstring, AseParser::EMaterial_Class>	AseParser::m_MaterialClass;
map<wstring, AseParser::EHelper>			AseParser::m_HelperTag;
map<wstring, AseParser::EGeom>				AseParser::m_GeomTag;
map<wstring, AseParser::EObject_NodeTM>		AseParser::m_ObjectNodeTM;
map<wstring, AseParser::EGeom_Mesh>			AseParser::m_GeomMesh;
map<wstring, AseParser::EObject_Animation>	AseParser::m_ObjectAnimation;

AseParser::AseParser() noexcept
{
	// 한번만 실행
	static std::once_flag flag;
	std::call_once(flag, [&]() {
		// ~~~~~~~~~~~~~~~~~~~~통괄 항목~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		m_AseTag[L"SCENE"]					= EAseTag::Scene;
		m_AseTag[L"MATERIAL_LIST"]			= EAseTag::MaterialList;
		m_AseTag[L"GEOMOBJECT"]				= EAseTag::GeomObject;
		m_AseTag[L"HELPEROBJECT"]			= EAseTag::HelperObject;

		// ~~~~~~~~~~~~~~~~~~~~Scene 항목~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		m_SceneTag[L"MATERIAL_LIST"]		 = EScene::_MaterialList;
		m_SceneTag[L"SCENE_FILENAME"]		 = EScene::FileName;
		m_SceneTag[L"SCENE_FIRSTFRAME"]		 = EScene::FirstFrame;
		m_SceneTag[L"SCENE_LASTFRAME"]		 = EScene::LastFrame;
		m_SceneTag[L"SCENE_FRAMESPEED"]		 = EScene::FrameSpeed;
		m_SceneTag[L"SCENE_TICKSPERFRAME"]	 = EScene::TickPerFrame;

		// ~~~~~~~~~~~~~~~~~~Material 항목~~~~~~~~~~~~~~~~~~~~~~~~~~~
		m_MaterialTag[L"MATERIAL"]			 = EMaterial::_Material;
		m_MaterialTag[L"GEOMOBJECT"]		 = EMaterial::_GeomObject;
		m_MaterialTag[L"HELPEROBJECT"]		 = EMaterial::_HelperObject;
		m_MaterialTag[L"MATERIAL_COUNT"]	 = EMaterial::MatrialCount;
		m_MaterialTag[L"MATERIAL_NAME"]		 = EMaterial::MaterialName;
		m_MaterialTag[L"MATERIAL_CLASS"]	 = EMaterial::MaterialClass;
		m_MaterialTag[L"NUMSUBMTLS"]		 = EMaterial::NumSubMaterial;
		m_MaterialTag[L"SUBMATERIAL"]		 = EMaterial::SubMaterial;
		m_MaterialTag[L"BITMAP"]			 = EMaterial::BitMap;
		m_MaterialTag[L"MAP_DIFFUSE"]		 = EMaterial::MapDiffuse;
		m_MaterialTag[L"MAP_REFLECT"]		 = EMaterial::MapReflect;

		m_MaterialClass[L"Standard"]		 = EMaterial_Class::Standard;
		m_MaterialClass[L"Multi/Sub-Object"] = EMaterial_Class::MultySubObject;

		// ~~~~~~~~~~~~~~~~~~~~~HelperObject 항목~~~~~~~~~~~~~~~~~~~~~~~
		m_HelperTag[L"GEOMOBJECT"]			= EHelper::_GeomObject;
		m_HelperTag[L"HELPEROBJECT"]		= EHelper::_HelperObject;
		m_HelperTag[L"NODE_NAME"]			= EHelper::NodeName;
		m_HelperTag[L"NODE_PARENT"]			= EHelper::NodeParent;
		m_HelperTag[L"HELPER_CLASS"]		= EHelper::HelperClass;
		m_HelperTag[L"NODE_TM"]				= EHelper::NodeTM;
		m_HelperTag[L"BOUNDINGBOX_MIN"]		= EHelper::BoundingMin;
		m_HelperTag[L"BOUNDINGBOX_MAX"]		= EHelper::BoundingMax;
		m_HelperTag[L"TM_ANIMATION"]		= EHelper::Anim;

		// ~~~~~~~~~~~~~~~~~~~~~GeomObject 항목~~~~~~~~~~~~~~~~~~~~~~~
		m_GeomTag[L"GEOMOBJECT"]			= EGeom::_GeomObject;
		m_GeomTag[L"HELPEROBJECT"]			= EGeom::_HelperObject;
		m_GeomTag[L"MATERIAL_REF"]			= EGeom::MaterialRef;
		m_GeomTag[L"NODE_NAME"]				= EGeom::NodeName;
		m_GeomTag[L"NODE_PARENT"]			= EGeom::NodeParent;
		m_GeomTag[L"NODE_TM"]				= EGeom::NodeTM;
		m_GeomTag[L"MESH"]					= EGeom::Mesh;
		m_GeomTag[L"TM_ANIMATION"]			= EGeom::Anim;
		
		// Mesh 파트
		m_GeomMesh[L"MATERIAL_REF"]		 = EGeom_Mesh::_MaterialRef;
		m_GeomMesh[L"NODE_TM"]			 = EGeom_Mesh::_NodeTM;
		m_GeomMesh[L"TM_ANIMATION"]		 = EGeom_Mesh::_Animation;
		m_GeomMesh[L"TIMEVALUE"]		 = EGeom_Mesh::TimeValue;
		// position						 
		m_GeomMesh[L"MESH_NUMVERTEX"]	 = EGeom_Mesh::NumVertex;
		m_GeomMesh[L"MESH_VERTEX_LIST"]  = EGeom_Mesh::VertexList;
		m_GeomMesh[L"MESH_VERTEX"]		 = EGeom_Mesh::Vertex;
		m_GeomMesh[L"MESH_NUMFACES"]	 = EGeom_Mesh::NumFaces;
		m_GeomMesh[L"MESH_FACE_LIST"]	 = EGeom_Mesh::FaceList;
		m_GeomMesh[L"MESH_FACE"]		 = EGeom_Mesh::Face;
		// texture						 
		m_GeomMesh[L"MESH_NUMTVERTEX"]	 = EGeom_Mesh::NumTexVertex;
		m_GeomMesh[L"MESH_TVERTLIST"]	 = EGeom_Mesh::TexVertexList;
		m_GeomMesh[L"MESH_TVERT"]		 = EGeom_Mesh::TexVertex;
		m_GeomMesh[L"MESH_NUMTVFACES"]	 = EGeom_Mesh::NumTexFaces;
		m_GeomMesh[L"MESH_TFACELIST"]	 = EGeom_Mesh::TexFaceList;
		m_GeomMesh[L"MESH_TFACE"]		 = EGeom_Mesh::TexFace;
		// color						 
		m_GeomMesh[L"MESH_NUMCVERTEX"]	 = EGeom_Mesh::NumColorVertex;
		m_GeomMesh[L"MESH_CVERTLIST"]	 = EGeom_Mesh::ColorVertexList;
		m_GeomMesh[L"MESH_VERTCOL"]		 = EGeom_Mesh::ColorVertex;
		m_GeomMesh[L"MESH_NUMCVFACES"]	 = EGeom_Mesh::NumColorFace;
		m_GeomMesh[L"MESH_CFACELIST"]	 = EGeom_Mesh::ColorFaceList;
		m_GeomMesh[L"MESH_CFACE"]		 = EGeom_Mesh::ColorFace;
		// normal						 
		m_GeomMesh[L"MESH_NORMALS"]		 = EGeom_Mesh::Nomals;
		m_GeomMesh[L"MESH_FACENORMAL"]	 = EGeom_Mesh::FaceNormal;
		m_GeomMesh[L"MESH_VERTEXNORMAL"] = EGeom_Mesh::VertexNormal;

		// TM 파트
		m_ObjectNodeTM[L"GEOMOBJECT"]		= EObject_NodeTM::_GeomObject;
		m_ObjectNodeTM[L"HELPEROBJECT"]		= EObject_NodeTM::_HelperObject;
		m_ObjectNodeTM[L"MESH"]				= EObject_NodeTM::_Mesh;
		m_ObjectNodeTM[L"TM_ANIMATION"]		= EObject_NodeTM::_Animation;
		m_ObjectNodeTM[L"MATERIAL_REF"]		= EObject_NodeTM::_MaterialRef;
		m_ObjectNodeTM[L"NODE_NAME"]		= EObject_NodeTM::NodeName;
		m_ObjectNodeTM[L"TM_ROW0"]			= EObject_NodeTM::TM_ROW0;
		m_ObjectNodeTM[L"TM_ROW1"]			= EObject_NodeTM::TM_ROW1;
		m_ObjectNodeTM[L"TM_ROW2"]			= EObject_NodeTM::TM_ROW2;
		m_ObjectNodeTM[L"TM_ROW3"]			= EObject_NodeTM::TM_ROW3;
		m_ObjectNodeTM[L"TM_POS"]			= EObject_NodeTM::TM_POS;
		m_ObjectNodeTM[L"TM_ROTAXIS"]		= EObject_NodeTM::TM_ROTAXIS;
		m_ObjectNodeTM[L"TM_ROTANGLE"]		= EObject_NodeTM::TM_ROTANGLE;
		m_ObjectNodeTM[L"TM_SCALE"]			= EObject_NodeTM::TM_SCALE;
		m_ObjectNodeTM[L"TM_SCALEAXIS"]		= EObject_NodeTM::TM_SCALEAXIS;
		m_ObjectNodeTM[L"TM_SCALEAXISANG"]  = EObject_NodeTM::TM_SCALEAXISANG;

		// Anim 파트
		m_ObjectAnimation[L"GEOMOBJECT"]			= EObject_Animation::_GeomObject;
		m_ObjectAnimation[L"HELPEROBJECT"]			= EObject_Animation::_HelperObject;
		m_ObjectAnimation[L"MATERIAL_REF"]			= EObject_Animation::_MaterialRef;
		m_ObjectAnimation[L"NODE_NAME"]				= EObject_Animation::NodeName;
		m_ObjectAnimation[L"CONTROL_POS_TRACK"]		= EObject_Animation::PositionTrack;
		m_ObjectAnimation[L"CONTROL_POS_SAMPLE"]	= EObject_Animation::PositionSample;
		m_ObjectAnimation[L"CONTROL_ROT_TRACK"]		= EObject_Animation::RotationTrack;
		m_ObjectAnimation[L"CONTROL_ROT_SAMPLE"]	= EObject_Animation::RotationSample;
		m_ObjectAnimation[L"CONTROL_SCALE_TRACK"]	= EObject_Animation::ScaleTrack;
		m_ObjectAnimation[L"CONTROL_SCALE_SAMPLE"]	= EObject_Animation::ScaleSample;
	});
}

void AseParser::Clear() noexcept
{
	m_filePath.clear();
	m_fileSize = 0;

	m_LineBuffers.clear();
	m_subString.clear();

	m_MaterialList.clear();
	m_ObjectList.clear();
	
	m_SceneData.FileName.clear();
	m_SceneData.FirstFrame = 0;
	m_SceneData.LastFrame = 0;
	m_SceneData.FrameSpeed = 0;
	m_SceneData.TickPerFrame = 0;
}

bool AseParser::CreateFromFile(GameObject** ppReturnObj, const wstring_view& fileName, const wstring_view& directory) noexcept
{
	Clear();
	m_filePath = m_filePath / directory.data() / fileName.data();

	// 연결
	wifstream readStream(m_filePath);
	if (!readStream.is_open())
		return false;
	// 파일 크기 얻기
	readStream.seekg(0, ios::end);
	m_fileSize = readStream.tellg();
	readStream.seekg(1, ios::beg);

	m_LineBuffers.clear();
	// *로 줄 구분
	m_LineBuffers.emplace_front(L"");
	m_curLineIter = m_LineBuffers.begin();
	while (std::getline(readStream, m_subString, L'*'))
	{
		m_LineBuffers.emplace_after(m_curLineIter, m_subString);
		++m_curLineIter;
	}

	LoadFileData(ppReturnObj);
	return true;
}

bool AseParser::LoadFileData(GameObject** ppReturnObj) noexcept
{
	// 태그값 검사
	m_curLineIter = m_LineBuffers.begin();
	++m_curLineIter;
	while (m_curLineIter != m_LineBuffers.end())
	{
		*m_curLineIter >> m_subString;
		auto findIter = m_AseTag.find(m_subString);
		if (findIter == m_AseTag.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			++m_curLineIter;
			continue;
		}
		
		// 태그 따라 분기
		switch (findIter->second)
		{
		case EAseTag::Scene:
		{
			LoadScene(&m_SceneData); 
			continue;
		}	break;
		case EAseTag::MaterialList:
		{
			LoadMaterialList();
			continue;
		}	break;
		case EAseTag::GeomObject:
		{
			LoadGeomObject();
			continue;
		}	break;
		case EAseTag::HelperObject:
		{
			LoadHelperObject();
			continue;
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}

		++m_curLineIter;
	}
	
	// 반환될 객체
	if(*ppReturnObj == nullptr)
		*ppReturnObj = new GameObject();
	(*ppReturnObj)->m_myName = m_filePath.filename().c_str();
	(*ppReturnObj)->m_objType = EObjType::Object;
	// 부모 설정
	for (auto& [name, pObject] : m_ObjectList)
	{
		for (auto&[insName, pInstance] : m_Instances)
		{
			if (name == insName)
			{
				pObject->AddComponent(new Instancer(pInstance));
				break;
			}
		}
		
		while (pObject->m_pParent != nullptr)
		{
			pObject = pObject->m_pParent;
		}
		pObject->SetParent(*ppReturnObj);
	}

	for (auto&[name, pInstance] : m_Instances)
	{
		ObjectManager::GetInstance().PushInstance(pInstance);
	}


	return true;
}

bool AseParser::LoadScene(SceneDataAse* pScene) noexcept
{
	// 씬 찾고, 씬 태그 검사
	while (true)
	{
		*(++m_curLineIter) >> m_subString;

		// 태그 찾고 Material이면 Scene 종료
		auto findIter = m_SceneTag.find(m_subString);
		if (findIter == m_SceneTag.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			continue;
		}

		// 태그 따라 Scene 구조체 채움
		switch (findIter->second)
		{
		case EScene::_MaterialList:
		{
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EScene::FileName:
		{
			// 변수 빼기
			*m_curLineIter >> pScene->FileName;
		}	break;
		case EScene::FirstFrame:
		{
			*m_curLineIter >> pScene->FirstFrame;
		}	break;
		case EScene::LastFrame:
		{
			*m_curLineIter >> pScene->LastFrame;
		}	break;
		case EScene::FrameSpeed:
		{
			*m_curLineIter >> pScene->FrameSpeed;
		}	break;
		case EScene::TickPerFrame:
		{
			*m_curLineIter >> pScene->TickPerFrame;
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}
	}

	return true;
}

bool AseParser::LoadMaterialList() noexcept
{
	MaterialDataAse* pMaterial = nullptr;
	UINT tempInt = 0;
	wstring textureName;

	++m_curLineIter;
	// 메테리얼 태그 검사
	while (m_curLineIter != m_LineBuffers.end())
	{
		*m_curLineIter >> m_subString;
		auto findIter = m_MaterialTag.find(m_subString);
		if (findIter == m_MaterialTag.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			++m_curLineIter;
			continue;
		}

		// 태그 따라 Material 구조체 채움
		switch (findIter->second)
		{
		case EMaterial::_GeomObject:
		case EMaterial::_HelperObject:
		{
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EMaterial::MatrialCount:
		{
			// 
		}	break;
		case EMaterial::_Material: 
		{
			pMaterial = new MaterialDataAse();
			m_MaterialList.emplace_back(pMaterial);
		}	break;
		case EMaterial::MaterialName:
		{
			*m_curLineIter >> pMaterial->MaterialName;
			pMaterial->MaterialName.pop_back();
			pMaterial->MaterialName.erase(pMaterial->MaterialName.begin());
		}	break;
		case EMaterial::MaterialClass:
		{
			*m_curLineIter >> pMaterial->MaterialClassName;
			pMaterial->MaterialClassName.pop_back();
			pMaterial->MaterialClassName.erase(pMaterial->MaterialClassName.begin());
		}	break;
		case EMaterial::NumSubMaterial:
		{
			*m_curLineIter >> tempInt;
			pMaterial->SubMaterials.resize(tempInt);
			for (auto& iter : pMaterial->SubMaterials)
			{
				iter.pParent = pMaterial;
			}
		}	break;
		case EMaterial::SubMaterial:
		{
			*m_curLineIter >> tempInt;
			
			auto iter = m_MaterialClass.find(pMaterial->MaterialClassName);
			switch (iter->second)
			{
			case EMaterial_Class::Standard:
			{
				//if(pMaterial->pParent != nullptr)
				pMaterial = pMaterial->pParent;
				pMaterial = &pMaterial->SubMaterials[tempInt];
				pMaterial->MaterialIndex = tempInt;
			}	break;
			case EMaterial_Class::MultySubObject:
			{
				pMaterial = &pMaterial->SubMaterials[tempInt];
				pMaterial->MaterialIndex = tempInt;
			}	break;
			}
		}	break;
		case EMaterial::MapDiffuse:
		{
			//
		}	break; 
		case EMaterial::BitMap:
		{
			*m_curLineIter >> textureName;
			textureName.erase(0, 1);
			textureName.pop_back();
			filesystem::path texPath(textureName);
			pMaterial->pTexture = DxManager::GetInstance().GetTexture(texPath.filename().c_str(), true);
			if(pMaterial->pTexture == nullptr)
				pMaterial->pTexture = DxManager::GetInstance().GetTexture(L"None.png");
		}	break;
		case EMaterial::MapReflect:
		{
			//
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif
		}	break;
		} 

		++m_curLineIter;
	}
	
	return true;
}

bool AseParser::LoadHelperObject() noexcept
{
	GameObject* pObject = nullptr;
	CAnimation* pAnimation = nullptr;

	++m_curLineIter;
	// 태그 찾기
	while (m_curLineIter != m_LineBuffers.end())
	{
		*m_curLineIter >> m_subString;
		auto findIter = m_HelperTag.find(m_subString);
		if (findIter == m_HelperTag.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			++m_curLineIter;
			continue;
		}

		// 태그 따라 분기
		switch (findIter->second)
		{
		case EHelper::_GeomObject:
		case EHelper::_HelperObject:
		{
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EHelper::NodeName:
		{
			// 이름 얻고 객체 생성
			*m_curLineIter >> m_subString;
			m_subString.pop_back();
			m_subString.erase(m_subString.begin());

			pObject = new GameObject(m_subString, EObjType::Object);
			m_ObjectList[m_subString] = pObject;

			pAnimation = new CAnimation();
			// 애니메이션 프레임 지정
			if (m_SceneData.FrameSpeed != 0)
				pAnimation->m_animFrame = 1.0f / m_SceneData.FrameSpeed;
			pObject->AddComponent(pAnimation);
		}	break;
		case EHelper::NodeParent:
		{
			// 부모-자식 설정
			*m_curLineIter >> m_subString;
			m_subString.pop_back();
			m_subString.erase(m_subString.begin());

			auto iter = m_ObjectList.find(m_subString);
			if (iter == m_ObjectList.end())
			{
				ErrorMessage(""s + __FUNCTION__ + " -> : Parent Not Found!");
			}

			pObject->SetParent(iter->second);
		}	break;
		case EHelper::HelperClass:
		{
			//
		}	break;
		case EHelper::NodeTM:
		{
			LoadObjectNodeTM(pObject, pAnimation);
			continue;
		}	break;
		case EHelper::BoundingMin:
		{
			// +바운딩 박스 추가
		}	break;
		case EHelper::BoundingMax:
		{
			//
		}	break;
		case EHelper::Anim:
		{
			LoadObjectAnimation(&pAnimation->m_Animations);
			continue;
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}

		++m_curLineIter;
	}

	return true;
}

bool AseParser::LoadGeomObject() noexcept
{
	InstanceRenderer*	pInstance = nullptr;
	GameObject*			pObject = nullptr;
	CAnimation*			pAnimation = nullptr;
	int tempInt = 0;
	++m_curLineIter;
	// 태그 찾기
	while (m_curLineIter != m_LineBuffers.end())
	{
		*m_curLineIter >> m_subString;
		auto findIter = m_GeomTag.find(m_subString);
		if (findIter == m_GeomTag.end())
		{
#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
#endif
			++m_curLineIter;
			continue;
		}

		// 태그 따라 분기
		switch (findIter->second)
		{
		case EGeom::MaterialRef:
		{
			*m_curLineIter >> tempInt;
			MaterialDataAse* pMaterial = &(*m_MaterialList[tempInt]);
			{
				if (pMaterial->pTexture != nullptr)
				{
					pInstance->m_pSpriteList->emplace_back(pMaterial->pTexture, 0.0f);
				}

				for (auto& iter : pMaterial->SubMaterials)
				{
					if (iter.pTexture != nullptr)
					{
						pInstance->m_pSpriteList->emplace_back(iter.pTexture, 0.0f);
					}
				}
			}
		}
		case EGeom::_GeomObject:
		case EGeom::_HelperObject:
		{
			//  다 채우고 객체 요소 구성
			pInstance->Create();
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EGeom::NodeName:
		{
			// 이름 얻고 객체 생성
			*m_curLineIter >> m_subString;
			m_subString.pop_back();
			m_subString.erase(m_subString.begin());
			
			pInstance = new InstanceRenderer(m_subString);
			pInstance->m_myName = m_subString;
			pInstance->m_pSpriteList = new vector<Sprite>();
			pInstance->isMultiTexture(true);
			m_Instances[m_subString] = pInstance;

			pObject = new GameObject(m_subString, EObjType::Object);
			m_ObjectList[m_subString] = pObject;

			pAnimation = new CAnimation();
			// 애니메이션 프레임 지정
			if (m_SceneData.FrameSpeed != 0)
				pAnimation->m_animFrame = 1.0f / m_SceneData.FrameSpeed;
			pObject->AddComponent(pAnimation);
		}	break;
		case EGeom::NodeParent:
		{
			// 부모-자식 설정
			*m_curLineIter >> m_subString;
			m_subString.pop_back();
			m_subString.erase(m_subString.begin());
			
			auto iter = m_ObjectList.find(m_subString);
			if (iter == m_ObjectList.end())
			{
				ErrorMessage(""s + __FUNCTION__ + " -> : Parent Not Found!");
			}
			pObject->SetParent(iter->second);
		}	break;
		case EGeom::NodeTM:
		{
			LoadObjectNodeTM(pObject, pAnimation);
			continue;
		}	break;
		case EGeom::Mesh:
		{
			LoadGeomMesh(&pInstance->m_vertexList, &pInstance->m_indexList, pObject->m_matLocal);
			continue;
		}	break;
		case EGeom::Anim:
		{
			LoadObjectAnimation(&pAnimation->m_Animations);
			continue;
		}	break;
		default:
		{
#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
#endif	
		}	break;
		}

		++m_curLineIter;
	}
	return true;
}



bool AseParser::LoadGeomMesh(vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList, const D3DXMATRIX& matWorld) noexcept
{
	int	  tempInt = 0;
	vector<Vertex_PNCT> tempVertex;		// 임시 정점 배열
	vector<D3DXVECTOR3> posVertex;
	vector<D3DXVECTOR2> texVertex;
	vector<D3DXVECTOR4> colVertex;
	vector<D3DXVECTOR3> norVertex;
	
	D3DXMATRIX matInWorld;
	D3DXMatrixInverse(&matInWorld, nullptr, &matWorld);

	// Mesh 데이터 검사
	while (++m_curLineIter != m_LineBuffers.end())
	{
		// 태그 찾기
		*m_curLineIter >> m_subString;
		auto findIter = m_GeomMesh.find(m_subString);
		if (findIter == m_GeomMesh.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			continue;
		}

		// 태그 따라 Mesh(정점) 채움
		switch (findIter->second)
		{
		case EGeom_Mesh::_NodeTM:
		case EGeom_Mesh::_Animation:
		case EGeom_Mesh::_MaterialRef:
		{
			// 정점 최적화
			SetVertexIndex(&tempVertex, pVertexList, pIndexList);
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EGeom_Mesh::TimeValue:
		{
			// 
		}	break;
		// =========================== position =========================
		case EGeom_Mesh::NumVertex:
		{
			*m_curLineIter >> tempInt;
			posVertex.resize(tempInt);
		}	break;
		case EGeom_Mesh::VertexList:
		{
			// 정점들 로컬로 변환
			for (auto& iter : posVertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt >> iter.x >> iter.z >> iter.y;
				D3DXVec3TransformCoord(&iter, &iter, &matInWorld);
			}
		}	break;
		case EGeom_Mesh::Vertex:
		{
			// 
		}	break;
		// =========================== position face =========================
		case EGeom_Mesh::NumFaces:
		{
			*m_curLineIter >> tempInt;
			tempVertex.resize(tempInt * 3);
			for (auto& iter : tempVertex)
			{
				iter.col = Color::White;
			}
		}	break;
		case EGeom_Mesh::FaceList:
		{
			// 해당 정점 사용 메테리얼
			UINT matIndex = 0;
			auto matIter = m_curLineIter;
			// position 및 사용 메테리얼 번호 입력 
			for (auto vertex = tempVertex.begin(); vertex != tempVertex.end(); ++vertex)
			{
				*(++++++matIter)   >> m_subString >> matIndex;
				*(++m_curLineIter) >> m_subString >> m_subString >> m_subString >> tempInt;
				vertex->pos = posVertex[tempInt];
				vertex->col.w = (float)matIndex + 0.1f;

				*m_curLineIter >> m_subString >> tempInt;
				(++vertex)->pos = posVertex[tempInt];
				vertex->col.w = (float)matIndex + 0.1f;
				
				*m_curLineIter >> m_subString >> tempInt;
				(++vertex)->pos = posVertex[tempInt];
				vertex->col.w = (float)matIndex + 0.1f;

				// 남은 2줄 넘김
				++++m_curLineIter;
			}
		}	break;
		case EGeom_Mesh::Face:
		{
			// 
		}	break;
		// =========================== texture =========================
		case EGeom_Mesh::NumTexVertex:
		{
			*m_curLineIter >> tempInt;
			texVertex.resize(tempInt);
		}	break;
		case EGeom_Mesh::TexVertexList:
		{
			for (auto& iter : texVertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt >> iter.x >> iter.y;
				iter.y = 1.0f - iter.y;
			}
		}	break;
		case EGeom_Mesh::TexVertex:
		{
			//
		}	break;
		// =========================== texture face =========================
		case EGeom_Mesh::NumTexFaces:
		{
			// 
		}	break;
		case EGeom_Mesh::TexFaceList:
		{
			for (auto vertex = tempVertex.begin(); vertex != tempVertex.end(); ++vertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt;

				*m_curLineIter >> tempInt;
				vertex->tex		= texVertex[tempInt];

				*m_curLineIter >> tempInt;
				(++vertex)->tex = texVertex[tempInt];

				*m_curLineIter >> tempInt;
				(++vertex)->tex = texVertex[tempInt];
			}
		}	break;
		case EGeom_Mesh::TexFace:
		{
			// 
		}	break;
		// =========================== color =========================
		case EGeom_Mesh::NumColorVertex:
		{
			*m_curLineIter >> tempInt;
			colVertex.resize(tempInt);
		}	break;
		case EGeom_Mesh::ColorVertexList:
		{
			// 정점 컬러 구축
			for (auto& iter : colVertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt >> iter.x >> iter.y >> iter.z;
			}
		}	break;
		case EGeom_Mesh::ColorVertex:
		{
			// 
		}	break;
		// =========================== color face =========================
		case EGeom_Mesh::NumColorFace:
		{
			// 
		}	break;
		case EGeom_Mesh::ColorFaceList:
		{
			// 컬러값 대입
			for (auto vertex = tempVertex.begin(); vertex != tempVertex.end(); ++vertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt;

				*m_curLineIter >> tempInt;
				vertex->col.x = colVertex[tempInt].x;
				vertex->col.y = colVertex[tempInt].y;
				vertex->col.z = colVertex[tempInt].z;

				*m_curLineIter >> tempInt;
				(++vertex)->col.x = colVertex[tempInt].x;
				vertex->col.y = colVertex[tempInt].y;
				vertex->col.z = colVertex[tempInt].z;

				*m_curLineIter >> tempInt;
				(++vertex)->col.x = colVertex[tempInt].x;
				vertex->col.y = colVertex[tempInt].y;
				vertex->col.z = colVertex[tempInt].z;
			}
		}	break;
		case EGeom_Mesh::ColorFace:
		{
			// 
		}	break;
		// =========================== normal =========================
		case EGeom_Mesh::Nomals:
		{
			// 
		}	break;
		case EGeom_Mesh::FaceNormal:
		{
			// 노말값 변환
			++m_curLineIter;
			for (auto vertex = tempVertex.begin(); vertex != tempVertex.end(); ++vertex)
			{
				*(++m_curLineIter) >> m_subString >> tempInt >> vertex->nor.x >> vertex->nor.z >> vertex->nor.y;
				// ++ 조명 연산시 노말값 로컬로 바꿔줘야댈듯
				//D3DXVec3TransformCoord(&vertex->nor, &vertex->nor, &matInWorld);
				++vertex;

				*(++m_curLineIter) >> m_subString >> tempInt >> vertex->nor.x >> vertex->nor.z >> vertex->nor.y;
				D3DXVec3TransformCoord(&vertex->nor, &vertex->nor, &matInWorld);
				++vertex;

				*(++m_curLineIter) >> m_subString >> tempInt >> vertex->nor.x >> vertex->nor.z >> vertex->nor.y;
				D3DXVec3TransformCoord(&vertex->nor, &vertex->nor, &matInWorld);
				++m_curLineIter;
			}
		}	break;
		case EGeom_Mesh::VertexNormal:
		{
			//
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}
	}
	return true;
}

bool AseParser::LoadObjectNodeTM(GameObject* pObject, CAnimation* pAnimation) noexcept
{
	D3DXMatrixIdentity(&pObject->m_matLocal);
	D3DXMatrixIdentity(&pAnimation->m_matAnim);

	while (++m_curLineIter != m_LineBuffers.end())
	{
		// 태그 찾기
		*m_curLineIter >> m_subString;
		auto findIter = m_ObjectNodeTM.find(m_subString);
		if (findIter == m_ObjectNodeTM.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			continue;
		}

		// Matrix 채움
		switch (findIter->second)
		{
		case EObject_NodeTM::_GeomObject:
		case EObject_NodeTM::_HelperObject:
		case EObject_NodeTM::_Mesh:
		case EObject_NodeTM::_Animation:
		case EObject_NodeTM::_MaterialRef:
		{
			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EObject_NodeTM::NodeName:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_ROW0:
		{
			*m_curLineIter >> pObject->m_matLocal._11
				>> pObject->m_matLocal._13 >> pObject->m_matLocal._12;
		}	break;
		case EObject_NodeTM::TM_ROW1:
		{
			*m_curLineIter >> pObject->m_matLocal._31
				>> pObject->m_matLocal._33 >> pObject->m_matLocal._32;
		}	break;
		case EObject_NodeTM::TM_ROW2:
		{
			*m_curLineIter >> pObject->m_matLocal._21
				>> pObject->m_matLocal._23 >> pObject->m_matLocal._22;
		}	break;
		case EObject_NodeTM::TM_ROW3:
		{
			*m_curLineIter >> pObject->m_matLocal._41
				>> pObject->m_matLocal._43 >> pObject->m_matLocal._42;
			
			Anim anim;
			// 부모 있으면 역행렬 곱함
			if (pObject->m_pParent != nullptr)
			{
				D3DXMATRIX matInverse;
 				D3DXMatrixInverse(&matInverse, nullptr, &pObject->m_pParent->m_matLocal);
				matInverse = pObject->m_matLocal * matInverse;

				D3DXMatrixDecompose(&anim.m_scale, &anim.m_rotation, &anim.m_position, &matInverse);
			}
			else
			{
				D3DXMatrixDecompose(&anim.m_scale, &anim.m_rotation, &anim.m_position, &pObject->m_matLocal);
			}
			D3DXMatrixIdentity(&anim.matScaleAxis);
			pAnimation->m_Animations.push_back(anim);
		}	break;
		case EObject_NodeTM::TM_POS:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_ROTAXIS:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_ROTANGLE:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_SCALE:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_SCALEAXIS:
		{
			// 
		}	break;
		case EObject_NodeTM::TM_SCALEAXISANG:
		{
			// 
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}
	}

	return true;
}

bool AseParser::LoadObjectAnimation(vector<Anim>* pAnimations) noexcept
{
	int	  curIndex = 0;
	
	int	  prePos = 0;
	int	  preRot = 0;
	int	  preSca = 0;

	D3DXVECTOR3		Axis	= Vector3::Zero;
	D3DXQUATERNION	preQuat	= Quaternion::Base;
	float Angle = 0.0f;

	// 애니메이션 프레임 할당
	pAnimations->resize(m_SceneData.LastFrame - m_SceneData.FirstFrame + 1);
	for (auto& iter : *pAnimations)
	{
		D3DXMatrixIdentity(&iter.matScaleAxis);
	}
	
	// Anim  
	while (++m_curLineIter != m_LineBuffers.end())
	{
		// 태그 찾기
		*m_curLineIter >> m_subString;
		auto findIter = m_ObjectAnimation.find(m_subString);
		if (findIter == m_ObjectAnimation.end())
		{
		#ifdef _DEBUG
			//ErrorMessage(L""s + __FUNCTIONW__ + L" -> 미등록된 태그! : " + m_subString);
		#endif
			continue;
		}

		// Anim 채움
		switch (findIter->second)
		{
		case EObject_Animation::_GeomObject:
		case EObject_Animation::_HelperObject:
		case EObject_Animation::_MaterialRef:
		{
			// Position 프레임이 덜 나왔다면 보간?
			curIndex = (int)pAnimations->size();
			if (prePos + 1 < curIndex)
			{
				//int frameCnt = curIndex - prePos;

				for (int i = prePos + 1; i < curIndex; ++i)
				{
					//(*pAnimations)[i].m_position = Lerp((*pAnimations)[prePos].m_position, (*pAnimations)[0].m_position, (float)i / frameCnt);
					(*pAnimations)[i].m_position = (*pAnimations)[prePos].m_position;
				}
			}

			// Rotation 프레임이 덜 나왔다면 보간?
			curIndex = (int)pAnimations->size();
			if (preRot + 1 < curIndex)
			{
				//int frameCnt = curIndex - preRot;
				
				for (int i = preRot + 1; i < curIndex; ++i)
				{
					//D3DXQuaternionSlerp(&(*pAnimations)[i].m_rotation, &(*pAnimations)[preRot].m_rotation, &(*pAnimations)[0].m_rotation, (float)i / frameCnt);
					(*pAnimations)[i].m_rotation = (*pAnimations)[preRot].m_rotation;
				}
			}

			// Scale 프레임이 덜 나왔다면 보간?
			curIndex = (int)pAnimations->size();
			if (preSca + 1 < curIndex)
			{
				//int frameCnt = curIndex - preSca;

				for (int i = preSca + 1; i < curIndex; ++i)
				{
					//(*pAnimations)[i].m_scale = Lerp((*pAnimations)[preSca].m_scale, (*pAnimations)[0].m_scale, (float)i / frameCnt);
					(*pAnimations)[i].m_scale = (*pAnimations)[preSca].m_scale;
					(*pAnimations)[i].matScaleAxis = (*pAnimations)[preSca].matScaleAxis;
				}
			}

			m_curLineIter->seekg(0, ios::beg);
			return true;
		}	break;
		case EObject_Animation::NodeName:
		{
			// 
		}	break;
		case EObject_Animation::PositionTrack:
		{
			prePos = 0;
		}	break;
		case EObject_Animation::PositionSample:
		{
			*m_curLineIter >> curIndex;
			curIndex /= m_SceneData.TickPerFrame;
			*m_curLineIter  >> (*pAnimations)[curIndex].m_position.x
							>> (*pAnimations)[curIndex].m_position.z
							>> (*pAnimations)[curIndex].m_position.y;
			// 중간 프레임 보간
			if (prePos + 1 < curIndex)
			{
				int frameCnt = curIndex - prePos;
				for (int i = prePos + 1; i < curIndex; ++i)
				{
					(*pAnimations)[i].m_position = Lerp((*pAnimations)[prePos].m_position, (*pAnimations)[curIndex].m_position, (float)i / frameCnt);
				}
			}
			prePos = curIndex;
		}	break;
		case EObject_Animation::RotationTrack:
		{
			preRot = 0;
		}	break;
		case EObject_Animation::RotationSample:
		{
			*m_curLineIter >> curIndex;
			curIndex /= m_SceneData.TickPerFrame;
			*m_curLineIter  >> Axis.x
							>> Axis.z
							>> Axis.y
							>> Angle;
			// 로테이션 누적
			D3DXQuaternionRotationAxis(&(*pAnimations)[curIndex].m_rotation, &Axis, Angle);
			(*pAnimations)[curIndex].m_rotation = (*pAnimations)[curIndex].m_rotation * preQuat;

			// 중간 프레임 보간
			if (preRot + 1 < curIndex)
			{
				int frameCnt = curIndex - preRot;

				for (int i = preRot + 1; i < curIndex; ++i)
				{
					(*pAnimations)[i].m_rotation = Lerp((*pAnimations)[preRot].m_rotation, (*pAnimations)[curIndex].m_rotation, (float)i / frameCnt);
				}
			}
			preQuat = (*pAnimations)[curIndex].m_rotation;
			preRot = curIndex;
		}	break;
		case EObject_Animation::ScaleTrack:
		{
			preSca = 0;
		}	break;
		case EObject_Animation::ScaleSample:
		{
			*m_curLineIter >> curIndex;
			curIndex /= m_SceneData.TickPerFrame;
			*m_curLineIter  >> (*pAnimations)[curIndex].m_scale.x
							>> (*pAnimations)[curIndex].m_scale.z
							>> (*pAnimations)[curIndex].m_scale.y
							>> Axis.x
							>> Axis.z
							>> Axis.y
							>> Angle;
			D3DXMatrixRotationAxis(&(*pAnimations)[curIndex].matScaleAxis, &Axis, Angle);
			// 중간 프레임 보간
			if (preSca + 1 < curIndex)
			{
				int frameCnt = curIndex - preSca;

				for (int i = preSca + 1; i < curIndex; ++i)
				{
					(*pAnimations)[i].m_scale = Lerp((*pAnimations)[preSca].m_scale, (*pAnimations)[curIndex].m_scale, (float)i / frameCnt);
					(*pAnimations)[i].matScaleAxis = (*pAnimations)[curIndex].matScaleAxis;
				}
			}
			preSca = curIndex;
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(L""s + __FUNCTIONW__ + L" -> 처리되지 않은 태그! : " + m_subString + L"(" + to_wstring((int)findIter->second) + L")");
		#endif	
		}	break;
		}
	}

	return true;
}

void AseParser::SetVertexIndex(vector<Vertex_PNCT>* pTempVertex, vector<Vertex_PNCT>* pVertexList, vector<WORD>* pIndexList) noexcept
{
	size_t vertex = 0;
	size_t vertexSize = 0;
	size_t tempSize = pTempVertex->size();
	bool notFound = false;
	for (size_t temp = 0; temp < tempSize; ++temp)
	{
		notFound = true;
		vertexSize = pVertexList->size();
		for (vertex = 0; vertex < vertexSize; ++vertex)
		{
			if ((*pTempVertex)[temp] == (*pVertexList)[vertex])
			{
				pIndexList->push_back((WORD)vertex);
				notFound = false;
				break;
			}
		}
		if (notFound)
		{
			pVertexList->emplace_back(std::move((*pTempVertex)[temp]));
			pIndexList->push_back((WORD)pVertexList->size() - 1);
		}
	}
}