#include "MaxImporter.h"
#include "ObjectManager.h"
#include "ParticleSystem.h"

map<string, MaxImporter::EAseTag> MaxImporter::m_AseTag;

MaxImporter::MaxImporter() noexcept
{
	// 한번만 실행
	static std::once_flag flag;
	std::call_once(flag, [&]() {
		// ~~~~~~~~~~~~~~~~~~~~통괄 항목~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
		m_AseTag["Scene"]		= EAseTag::Scene;
		m_AseTag["Material"]	= EAseTag::MaterialList;
		m_AseTag["Mesh"]		= EAseTag::Mesh;
	});
}

void MaxImporter::Clear() noexcept
{
	m_filePath.clear();
	m_fileSize = 0;

	m_LineBuffers.clear();
	m_subString.clear();

	m_MaterialList.clear();
	m_Objects.clear();
	m_Instances.clear();

	m_SceneData = { 0, };
}


ParticleSystem* MaxImporter::CreateFromParticle(const wstring_view& fileName, const wstring_view& directory) noexcept
{
	Clear();
	m_filePath = m_filePath / directory.data() / fileName.data();
	// 연결
	ifstream readStream(m_filePath);
	if (!readStream.is_open())
		return nullptr;
	//// 파일 크기 얻기
	//readStream.seekg(0, ios::end);
	//m_fileSize = readStream.tellg();
	//readStream.seekg(0, ios::beg);

	m_LineBuffers.emplace_front("");
	m_curLineIter = m_LineBuffers.begin();
	std::getline(readStream, m_subString);
	m_LineBuffers.emplace_after(m_curLineIter, m_subString);
	++m_curLineIter;
	///
	string objName, srcName;
	*m_curLineIter >> m_subString >> objName >> srcName;

	auto pParticle = new ParticleSystem(CharToWChar(objName.c_str()), new Particle(), CharToWChar(srcName.c_str()));
	readStream.read((char*)(ParticleInfo*)(pParticle), sizeof(ParticleInfo));

	return pParticle;
}

bool MaxImporter::CreateFromFile(GameObject** ppReturnObj, const wstring_view& fileName, const wstring_view& directory) noexcept
{
	Clear();
	m_filePath = m_filePath / directory.data() / fileName.data();
	/// 이펙트
	if (m_filePath.extension() == L".eff")
	{
		auto pParticle = CreateFromParticle(fileName, directory);

		// 반환될 객체
		if (*ppReturnObj == nullptr)
		{
			*ppReturnObj = new GameObject(m_filePath.filename().c_str(), pParticle, EObjType::Effect);
		}
		else
		{
			(*ppReturnObj)->m_myName = m_filePath.filename().c_str();
			(*ppReturnObj)->m_objType = EObjType::Effect;
			(*ppReturnObj)->AddComponent(pParticle);
		}
	}
	else
	{
		// 연결
		ifstream readStream(m_filePath);
		if (!readStream.is_open())
			return false;
		// 파일 크기 얻기
		readStream.seekg(0, ios::end);
		m_fileSize = readStream.tellg();
		readStream.seekg(0, ios::beg);

		m_LineBuffers.emplace_front("");
		m_curLineIter = m_LineBuffers.begin();
		while (std::getline(readStream, m_subString))
		{
			m_LineBuffers.emplace_after(m_curLineIter, m_subString);
			++m_curLineIter;
		}

		LoadFileData(ppReturnObj);
	}
	return true;
}

bool MaxImporter::LoadFileData(GameObject** ppReturnObj) noexcept
{
	int tempInt = 0;
	// 태그값 검사
	m_curLineIter = m_LineBuffers.begin();
	while (m_curLineIter != m_LineBuffers.end())
	{
		*m_curLineIter >> m_subString;
		auto findIter = m_AseTag.find(m_subString);
		if (findIter == m_AseTag.end())
		{
#ifdef _DEBUG
			//ErrorMessage(__FUNCTIONW__ + L" -> 미등록된 태그! : "s + m_subString);
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
		}	break;
		case EAseTag::MaterialList:
		{
			*m_curLineIter >> tempInt;
			LoadMaterialList(tempInt);
		}	break;
		case EAseTag::Mesh:
		{
			*m_curLineIter >> tempInt;
			LoadObject(tempInt);
			continue;
		}	break;
		default:
		{
		#ifdef _DEBUG
			ErrorMessage(__FUNCTION__ + " -> 처리되지 않은 태그! : "s + m_subString + "(" + to_string((int)findIter->second) + ")");
		#endif	
		}	break;
		}
		++m_curLineIter;
	}

	// 반환될 객체
	if (*ppReturnObj == nullptr)
	{
		*ppReturnObj = new GameObject(m_filePath.filename().c_str(), EObjType::Object);
	}
	else
	{
		(*ppReturnObj)->m_myName = m_filePath.filename().c_str();
		(*ppReturnObj)->m_objType = EObjType::Object;
	}
	// 부모 없는 애들 설정
	GameObject* pObject = nullptr;
	for (auto& [name, pObj] : m_Objects)
	{
		pObject = pObj;
		while (pObject->m_pParent != nullptr)
		{
			pObject = pObject->m_pParent;
		}
		pObject->SetParent(*ppReturnObj);
	}
	// Frame이 중간에 돌아서 마지막에 넣음
	for (auto&[name, pInstance] : m_Instances)
	{
		ObjectManager::GetInstance().PushInstance(pInstance);
	}

	return true;
}

bool MaxImporter::LoadScene(SceneData* pScene) noexcept
{
	*m_curLineIter >> pScene->SPF >> pScene->TotalFrame >> pScene->MeshCount >> pScene->MaxWeight >> pScene->BindPose;
	return true;
}

bool MaxImporter::LoadMaterialList(const int& parentCnt) noexcept
{
	if (parentCnt <= 0)
		return true;
	m_MaterialList.resize(parentCnt);
	MaterialData* pMaterial = nullptr;
	MaterialData* pChild = nullptr;
	int tempInt = 0;
	int childCnt = 0;
	int textureCnt = 0;

	for(int iParent = 0; iParent < parentCnt; ++iParent)
	{
		pMaterial = &m_MaterialList[iParent];
		*(++m_curLineIter) >> pMaterial->MaterialIndex >> childCnt >> textureCnt;
		for (int iTex = 0; iTex < textureCnt; ++iTex)
		{
			*m_curLineIter >> m_subString;
			pMaterial->pTexture = DxManager::GetInstance().GetTexture(CharToWChar(m_subString.c_str()), true);
		}
		if (childCnt <= 0)
			continue;

		pMaterial->SubMaterials.resize(childCnt);
		for (int iChild = 0; iChild < childCnt; ++iChild)
		{
			pChild = &pMaterial->SubMaterials[iChild];
			pChild->pParent = pMaterial;
			// + 재귀?, Sub의 Sub는 미지원중
			*(++m_curLineIter) >> pChild->MaterialIndex >> tempInt >> textureCnt;
			for (int iTex = 0; iTex < textureCnt; ++iTex)
			{
				*m_curLineIter >> m_subString;
				pChild->pTexture = DxManager::GetInstance().GetTexture(CharToWChar(m_subString.c_str()), true);
				if (pChild->pTexture == nullptr)
				{
					pChild->pTexture = DxManager::GetInstance().GetTexture(L"Model/"s + CharToWChar(m_subString.c_str()));
					if (pChild->pTexture == nullptr)
					{
						pChild->pTexture = DxManager::GetInstance().GetTexture(L"Effect3d/"s + CharToWChar(m_subString.c_str()));
						if (pChild->pTexture == nullptr)
						{
							ErrorMessage(__FUNCTION__ + " -> Not Find Texture"s);
						}
					}
				}
			}
		}
	}

	return true;
}

bool MaxImporter::LoadObject(const int& objCnt) noexcept
{
	int eType, matRef;
	//wstring parentName;
	GameObject* pObject = nullptr;
	CAnimation* pAnimation = nullptr;
	InstanceRenderer* pInstance = nullptr;
	map<string, string> parentList;

	for(int i = 0; i < objCnt; ++i)
	{
		*(++m_curLineIter) >> eType >> matRef >> m_subString >> parentList[m_subString];
		//parentList[m_subString] = parentName;
		// 객체 생성
		pObject = new GameObject(CharToWChar(m_subString.c_str()), EObjType::Object);
		m_Objects[m_subString] = pObject;
		// NodeTM
		*(++m_curLineIter) >> pObject->m_matLocal._11;// >> pObject->m_matLocal._12 >> pObject->m_matLocal._13 >> pObject->m_matLocal._14
							//>> pObject->m_matLocal._21 >> pObject->m_matLocal._22 >> pObject->m_matLocal._23 >> pObject->m_matLocal._24
							//>> pObject->m_matLocal._31 >> pObject->m_matLocal._32 >> pObject->m_matLocal._33 >> pObject->m_matLocal._34
							//>> pObject->m_matLocal._41 >> pObject->m_matLocal._42 >> pObject->m_matLocal._43 >> pObject->m_matLocal._44;
		/// ++ 바운딩 박스
		++m_curLineIter;

		switch ((EClassType)eType)
		{
		case EClassType::Geom:
		{
			// 메쉬 생성
			pInstance = new InstanceRenderer(CharToWChar(m_subString.c_str()));
			pInstance->m_pSpriteList = new vector<Sprite>();
			pInstance->isMultiTexture(true);
			// 랜더러 메테리얼 설정
			MaterialData* pMaterial = &m_MaterialList[matRef];
			{
				if (pMaterial->pTexture != nullptr)
				{
					pInstance->m_pSpriteList->emplace_back(pMaterial->pTexture, 0.0f);
					pInstance->isMultiTexture(false);
				}
				for (auto& iter : pMaterial->SubMaterials)
				{
					if (iter.pTexture != nullptr)
					{
						pInstance->m_pSpriteList->emplace_back(iter.pTexture, 0.0f);
					}
					else
					{
						ErrorMessage(__FUNCTION__ + " -> "s + m_subString.data() + ", " + to_string(iter.MaterialIndex) + " : Material nullptr!");
						pInstance->m_pSpriteList->emplace_back(DxManager::GetInstance().GetTexture(L"None.png"), 0.0f);
					}
				}
			}

			// 버텍스 등록
			*(++m_curLineIter) >> m_subString >> matRef;
			pInstance->m_vertexList.resize(matRef);
			for (int index = 0; index < matRef; ++index)
			{
				*(++m_curLineIter) >> pInstance->m_vertexList[index].col.w
					>> pInstance->m_vertexList[index].pos.x >> pInstance->m_vertexList[index].pos.y >> pInstance->m_vertexList[index].pos.z
					>> pInstance->m_vertexList[index].nor.x >> pInstance->m_vertexList[index].nor.y >> pInstance->m_vertexList[index].nor.z
					>> pInstance->m_vertexList[index].col.x >> pInstance->m_vertexList[index].col.y >> pInstance->m_vertexList[index].col.z
					>> pInstance->m_vertexList[index].tex.x >> pInstance->m_vertexList[index].tex.y;
				pInstance->m_vertexList[index].col.w += 0.1f;
			}
			// 인덱스 등록
			*(++m_curLineIter) >> m_subString >> matRef;
			pInstance->m_indexCount = matRef;
			pInstance->m_indexList.resize(matRef);
			for (int index = 0; index < matRef; ++index)
			{
				*m_curLineIter >> pInstance->m_indexList[index];
			}
			// 정점 없으면 삭제
			if (pInstance->m_vertexList.size() == 0)
			{
				delete pInstance->m_pSpriteList;
				delete pInstance;
			}
			else
			{
				pInstance->Create();
				//pInstance->AddInstance(pObject);
				pObject->AddComponent(new Instancer(pInstance));
				m_Instances[pObject->m_myName] = pInstance;
			}
		}	break;
		case EClassType::Bone:
		case EClassType::Biped:
		case EClassType::Dummy:
		{
			++++m_curLineIter;
		}	break;
		}

		*(++m_curLineIter) >> m_subString >> matRef;
		// 애니메이션 등록
		if (matRef >= 1)
		{
			pAnimation = new CAnimation();
			pAnimation->m_animFrame = m_SceneData.SPF;

			pAnimation->m_Animations.resize(matRef);
			for (int in = 0; in < matRef; ++in)
			{
				pAnimation->m_Animations[in].matScaleAxis = Matrix::Identity;
				*(++m_curLineIter)
					>> pAnimation->m_Animations[in].m_position.x >> pAnimation->m_Animations[in].m_position.y >> pAnimation->m_Animations[in].m_position.z
					>> pAnimation->m_Animations[in].m_rotation.x >> pAnimation->m_Animations[in].m_rotation.y >> pAnimation->m_Animations[in].m_rotation.z >> pAnimation->m_Animations[in].m_rotation.w
					>> pAnimation->m_Animations[in].m_scale.x	 >> pAnimation->m_Animations[in].m_scale.y	  >> pAnimation->m_Animations[in].m_scale.z
					>> pAnimation->m_Animations[in].matScaleAxis._11 >> pAnimation->m_Animations[in].matScaleAxis._12 >> pAnimation->m_Animations[in].matScaleAxis._13
					>> pAnimation->m_Animations[in].matScaleAxis._21 >> pAnimation->m_Animations[in].matScaleAxis._22 >> pAnimation->m_Animations[in].matScaleAxis._23
					>> pAnimation->m_Animations[in].matScaleAxis._31 >> pAnimation->m_Animations[in].matScaleAxis._32 >> pAnimation->m_Animations[in].matScaleAxis._33;
			}
			//D3DXMatrixDecompose(&pAnimation->m_Animations[0].m_scale, &pAnimation->m_Animations[0].m_rotation, &pAnimation->m_Animations[0].m_position, &pObject->m_matLocal);
			pObject->AddComponent(pAnimation);
		}
	}

	for(auto& iter : m_Objects)
	{
		if (parentList[iter.first].empty())
			continue;
		// 부모 설정
		auto finder = m_Objects.find(parentList[iter.first]);
		if (finder == m_Objects.end())
		{
			ErrorMessage(__FUNCTION__ + " -> : Parent Not Found!"s, true);
		}
		iter.second->SetParent(finder->second);
	}
	return true;
}