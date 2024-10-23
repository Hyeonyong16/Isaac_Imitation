#include "pch.h"
#include "CUIMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CUI.h"

#include "CKeyMgr.h"

CUIMgr::CUIMgr()
	: m_FocusedUI(nullptr)
	, m_PriorityUI(nullptr)
{

}

CUIMgr::~CUIMgr()
{

}

void CUIMgr::Tick()
{
	// 현재 레벨안에 있는 UI 들을 가져온다.
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// 레벨에 추가된 최상위 부모 UI 들
	vector<CObj*>& vecUI = pCurLevel->m_vecObjects[(UINT)LAYER_TYPE::UI];

	// 마우스 왼쪽버튼이 눌린 상황 체크
	KEY_STATE LBtnState = CKeyMgr::GetInst()->GetKeyState(KEY::LBTN);

	// reverse itertor 를 이용해서 vector 를 역순으로 순회한다.
	vector<CObj*>::reverse_iterator riter = vecUI.rbegin();
	for (; riter != vecUI.rend(); ++riter)
	{
		// UI 레이어에 있는 오브젝트가 진짜 UI 객체인지 확인 및 다운캐스팅
		CUI* pParentUI = dynamic_cast<CUI*>((*riter));
		assert(pParentUI);

		// 최상위 부모 UI 포함, 본인의 자식들 중 가장 우선순위가 높은 UI 를 찾아서 반환한다.
		CUI* pPriorityUI = GetPriorityUI(pParentUI);
		if (nullptr == pPriorityUI)
		{
			ReleaseCheck(pParentUI);
			continue;
		}

		// UI 가 눌렸는지 확인
		if (LBtnState == KEY_STATE::TAP)
		{
			pPriorityUI->m_LBtnDown = true;
			pPriorityUI->MouseLBtnDown();

			// vector 에서 제거 후 가장 뒤로 보내준다.
			vecUI.erase((++riter).base());
			vecUI.push_back(pParentUI);
			break;
		}

		// UI 가 클릭됐는지 체크
		if (LBtnState == KEY_STATE::RELEASED)
		{
			if (pPriorityUI->m_LBtnDown)
				pPriorityUI->MouseLBtnClikced();
			else
				pPriorityUI->MouseReleased();

			// LBtn 키가 해제되었기 때문에, 눌린 상태를 해제한다.			
			ReleaseCheck(pParentUI);
		}
	}
}

CUI* CUIMgr::GetPriorityUI(CUI* _ParentUI)
{
	CUI* pPriorityUI = nullptr;

	// Queue 역할을 할 List
	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모 UI 입력
	queue.push_back(_ParentUI);

	// Queue 가 비어있지 않다면 반복문 실행
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pUI->m_vecChildUI.size(); ++i)
		{
			queue.push_back(pUI->m_vecChildUI[i]);
		}

		if (pUI->IsMouseHover())
			pPriorityUI = pUI;
	}

	return pPriorityUI;
}

void CUIMgr::ReleaseCheck(CUI* _UI)
{
	// Queue 역할을 할 List
	static list<CUI*> queue;
	queue.clear();

	// 최상위 부모 UI 입력
	queue.push_back(_UI);

	// Queue 가 비어있지 않다면 반복문 실행
	while (!queue.empty())
	{
		CUI* pUI = queue.front();
		queue.pop_front();

		for (size_t i = 0; i < pUI->m_vecChildUI.size(); ++i)
		{
			queue.push_back(pUI->m_vecChildUI[i]);
		}

		if (KEY_RELEASED(KEY::LBTN))
			pUI->m_LBtnDown = false;
	}
}