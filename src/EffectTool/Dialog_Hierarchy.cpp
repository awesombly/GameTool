#include "stdafx.h"
#include "EffectTool.h"
#include "Dialog_Hierarchy.h"
#include "afxdialogex.h"
#include "Dialog_Inspector.h"
#include "Dialog_Preset.h"

Dialog_Hierarchy* Dialog_Hierarchy::Instance = nullptr;

// Dialog_Preset 대화 상자
IMPLEMENT_DYNAMIC(Dialog_Hierarchy, CFormView)

Dialog_Hierarchy::Dialog_Hierarchy()
	: CFormView(IDD_DIALOG_Hierarchy)
{
	Instance = this;
}


Dialog_Hierarchy::~Dialog_Hierarchy()
{}


void Dialog_Hierarchy::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TreeList, m_TreeList);
	DDX_Control(pDX, ID_Duplicate, m_DuplicateButton);
	DDX_Control(pDX, ID_Delete, m_RemoveButton);
}

Dialog_Hierarchy* Dialog_Hierarchy::CreateOne(CWnd* pParent)
{
	Dialog_Hierarchy* pForm = new Dialog_Hierarchy();
	pForm->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, CRect(0, 0, 500, 500), pParent, 0, NULL);
	return pForm;
}

BEGIN_MESSAGE_MAP(Dialog_Hierarchy, CFormView)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TreeList, &Dialog_Hierarchy::OnTvnSelchangedTreetlist)
	ON_BN_CLICKED(ID_Duplicate, &Dialog_Hierarchy::OnBnClickedDuplicate)
	ON_BN_CLICKED(ID_Delete, &Dialog_Hierarchy::OnBnClickedDelete)
	ON_BN_CLICKED(IDC_Focus, &Dialog_Hierarchy::OnBnClickedButton1)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TreeList, &Dialog_Hierarchy::OnTvnBegindragTreelist)
	ON_BN_CLICKED(IDC_Composite, &Dialog_Hierarchy::OnBnClickedComposite)
	ON_BN_CLICKED(ID_Export, &Dialog_Hierarchy::OnBnClickedExport)
	ON_BN_CLICKED(ID_SetProto, &Dialog_Hierarchy::OnBnClickedSetproto)
END_MESSAGE_MAP()


// 선택 오브젝트 반환
GameObject* Dialog_Hierarchy::GetSelectObject()
{
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return nullptr;
	}
	return (GameObject*)m_TreeList.GetItemData(item);
}

// 오브젝트->노드 항목 추가
void Dialog_Hierarchy::AddTreeItem(GameObject* pObject)
{
	auto parent = m_TreeList.InsertItem(pObject->m_myName.c_str(), 0, 0, TVI_ROOT, TVI_LAST);
	m_TreeList.SetItemData(parent, (DWORD_PTR)pObject);
	// 자식 순회
	AddTreeNode(pObject, parent, 1);
}
// 자식 재귀 추가
void Dialog_Hierarchy::AddTreeNode(GameObject* pObject, const HTREEITEM& parent, const int& level)
{
	for (auto& iter : *pObject->GetChildList())
	{
		auto ppparent = m_TreeList.InsertItem(iter->m_myName.c_str(), 0, level, parent, TVI_SORT);
		m_TreeList.SetItemData(ppparent, (DWORD_PTR)iter);

		if (!iter->GetChildList()->empty())
			AddTreeNode(iter, ppparent, level + 1);
	}
}

// 오브젝트 데이터 출력
void Dialog_Hierarchy::ExportObject(const filesystem::path fpath, GameObject* pObject)
{
	ofstream outStream(fpath);
	if (!outStream.is_open())
	{
		MessageBox(L" : Export Error!", fpath.c_str());
		return;
	}
	//outStream << "데이터 확인용 간이 출력 입니다." << endl;
	ExportChild(outStream, pObject);

	MessageBox(L"출력이 완료되었습니다.", fpath.c_str());
}
// 자식 재귀 출력
void Dialog_Hierarchy::ExportChild(ofstream& outStream, GameObject* pObject)
{
	//outStream << "Name " << WCharToChar(pObject->m_myName.c_str()) << " " << endl;
	//outStream.write(reinterpret_cast<const char*>((Transform*)pObject), sizeof(Transform)) << endl;
	
	for (auto&[type, components] : pObject->GetComponentList())
	{
		switch (type)
		{
		 case EComponent::Renderer:
		 {
		 	for (auto& outIter : components)
		 	{
		 		switch (((Renderer*)outIter)->m_eRenderType)
		 		{
		 		 case ERenderType::Basic:
		 		 {
		 		 	//for (auto& iter : ((Renderer*)outIter)->m_vertexList)
		 		 	//{
		 		    //	 outStream << "Position : "
		 		    //		 << iter.pos.x << " "
		 		    //		 << iter.pos.y << " "
		 		    //		 << iter.pos.z << ", "
		 		    //		 << "Normal : "
		 		    //		 << iter.nor.x << " "
		 		    //		 << iter.nor.y << " "
		 		    //		 << iter.nor.z << ", "
		 		    //		 << "Texture : "
		 		    //		 << iter.tex.x << " "
		 		    //		 << iter.tex.y << endl;
		 		 	//}
		 		 }	break;
		 		 case ERenderType::Particle:
		 		 {
		 		 	auto pParticle = (ParticleSystem*)outIter;
		 		 	outStream << "Particle " << WCharToChar(pObject->m_myName.c_str()) << " " << WCharToChar(pParticle->m_srcName.c_str()) << endl;
					outStream.write(reinterpret_cast<const char*>((ParticleInfo*)pParticle), sizeof(ParticleInfo));
		 		 }	break;
		 		}
		 	}
		 }
		}
	}

	//if (auto pRenderer = pObject->GetComponentList(EComponent::Animation);
	//	pRenderer != nullptr)
	//{
	//	for (auto& outIter : *pRenderer)
	//	{
	//		for (auto& anim : ((CAnimation*)outIter)->m_Animations)
	//		{
	//			outStream << "Anim Pos : "
	//				<< anim.m_position.x << " "
	//				<< anim.m_position.y << " "
	//				<< anim.m_position.z << ", "
	//				<< endl
	//				<< "Anim Rot : "
	//				<< anim.m_rotation.x << " "
	//				<< anim.m_rotation.y << " "
	//				<< anim.m_rotation.z << " "
	//				<< anim.m_rotation.w << ", "
	//				<< endl
	//				<< "Anim Sca : "
	//				<< anim.m_scale.x << " "
	//				<< anim.m_scale.y << " "
	//				<< anim.m_scale.z << " "
	//				<< endl << endl;
	//			//<< "Anim Axi : " 
	//			//<< anim.matScaleAxis << endl << endl;
	//		}
	//	}
	//}

	for (auto pChild : pObject->m_childList)
	{
		ExportChild(outStream, pChild);
	}
}


// ============================ Dialog_Preset 메시지 처리기 ===========================


// 리스트 클릭
void Dialog_Hierarchy::OnTvnSelchangedTreetlist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	auto curItem = pNMTreeView->itemNew.hItem;
	auto pCurObject = (GameObject*)m_TreeList.GetItemData(curItem);

	// 객체 선택 갱신
	if (curItem == NULL)
	{
		MessageBox(L"잘못된 접근", L"삐빅-");
		return;
	}
	Dialog_Inspector::Instance->UpdateObjectData(pCurObject);
	// 삭제 버튼 활성화 처리
	if (pCurObject->isGlobal())
		m_RemoveButton.EnableWindow(0);
	else
		m_RemoveButton.EnableWindow(1);

	// 노드 이동:삽입
	if (m_ClickItem != NULL)
	{
		if (m_ClickItem == NULL)
		{
			MessageBox(L"잘못된 접근.", L"삐빅-");
			return;
		}
		if (auto parent = m_TreeList.GetNextItem(m_ClickItem, TVGN_PARENT);
			parent == curItem || m_ClickItem == curItem)
		{
			MessageBox(L"의미 없는 항목 선택.", L"삐빅-");
			return;
		}

		// 자식될 객체
		auto pObject = (GameObject*)m_TreeList.GetItemData(m_ClickItem);

		// 자식인지 검사
		auto pParent = pCurObject;
		while (pParent->m_pParent != nullptr)
		{
			pParent = pParent->m_pParent;
			if (pParent == pObject)
			{
				MessageBox(L"자식 객체로 이동할 수 없습니다.", L"삐빅-");
				return;
			}
		}

		auto item = m_TreeList.InsertItem(pObject->m_myName.c_str(), 0, 1, curItem, TVI_LAST);
		m_TreeList.SetItemData(item, (DWORD_PTR)pObject);

		pObject->SetParent(pCurObject);

		// 아이템 추가
		AddTreeNode(pObject, item, 2);
		// 기존 항목 삭제
		m_TreeList.DeleteItem(m_ClickItem);

		m_ClickItem = NULL;
	}


	*pResult = 0;
	pNMTreeView;
}

// 리스트 드래그
void Dialog_Hierarchy::OnTvnBegindragTreelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// 리스트 드래그 시작
	m_ClickItem = NULL;
	*pResult = 0;	pNMHDR;
}


// 복사 버튼
void Dialog_Hierarchy::OnBnClickedDuplicate()
{
	// 복제
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pClone = ((GameObject*)m_TreeList.GetItemData(item))->clone();
	pClone->m_myName = pClone->m_myName + L"(copy)";
	pClone->CutParent();
	ObjectManager::GetInstance().PushObject(pClone);
	// 트리 항목 추가
	AddTreeItem(pClone);
}

// 삭제 버튼
void Dialog_Hierarchy::OnBnClickedDelete()
{
	// 삭제
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	Dialog_Inspector::Instance->UpdateObjectData(nullptr);
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	ObjectManager::GetInstance().RemoveObject(pObject);
	m_TreeList.DeleteItem(item);
}

// 포커스 버튼
void Dialog_Hierarchy::OnBnClickedButton1()
{
	// 객체로 포커스 맞춤
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	ObjectManager::Cameras[ECamera::Main]->SetPosition(pObject->GetWorldPosition());
	ObjectManager::Cameras[ECamera::Main]->Translate(ObjectManager::Cameras[ECamera::Main]->GetForward() * -(13.0f + D3DXVec3Length(&pObject->GetScale()) / 3));
}

// 병합 버튼
void Dialog_Hierarchy::OnBnClickedComposite()
{
	// 합체
	if (m_ClickItem != NULL)
	{
		m_ClickItem = NULL;
		return;
	}
	m_ClickItem = m_TreeList.GetSelectedItem();
	if (m_ClickItem == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
}

// 내보내기 버튼
void Dialog_Hierarchy::OnBnClickedExport()
{
	// 객체 데이터 저장
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pObject = (GameObject*)m_TreeList.GetItemData(item);

	CFileDialog fileDia(false, L"", (pObject->m_myName + L".eff").c_str());				// true = 불러오기, false = 저장인듯
	if (fileDia.DoModal() != IDOK)
		return;

	std::filesystem::path fpath;
	fpath = fileDia.GetPathName().GetBuffer();

	std::thread exporter(&Dialog_Hierarchy::ExportObject, this, fpath, pObject);
	exporter.detach();
}


// 등록 버튼
void Dialog_Hierarchy::OnBnClickedSetproto()
{
	// 객체 데이터 저장
	auto item = m_TreeList.GetSelectedItem();
	if (item == NULL)
	{
		MessageBox(L"객체를 선택해 주세요.", L"삐빅-");
		return;
	}
	auto pObject = ((GameObject*)m_TreeList.GetItemData(item))->clone();
	
	if (! ObjectManager::GetInstance().SetProtoObject(pObject))
	{
		MessageBox(L"이미 등록된 이름 입니다.", L"삐빅-");
		return;
	}
	Dialog_Preset::Instance->m_ListBox.AddString(pObject->m_myName.data());
}