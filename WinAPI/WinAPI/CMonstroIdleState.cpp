#include "pch.h"
#include "CMonstroIdleState.h"

#include "CEngine.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CCamera.h"

#include "CObj.h"
#include "CMonstro.h"

#include "CTimeMgr.h"
#include "ctime"

CMonstroIdleState::CMonstroIdleState()
	: m_TargetObject(nullptr)
	, m_curTime(0)
{
}

CMonstroIdleState::~CMonstroIdleState()
{
}

void CMonstroIdleState::Enter()
{
	m_curTime = 0;

	// Level 안에 있는 플레이어를 찾는다.
	if (nullptr == m_TargetObject)
	{
		m_TargetObject = CLevelMgr::GetInst()->FindObjectByName(LAYER_TYPE::PLAYER, L"Player");
	}

	CMonstro* pMonstro = dynamic_cast<CMonstro*>(GetOwnerObj());
	assert(pMonstro);
	pMonstro->SetCanHit(true);
}

void CMonstroIdleState::FinalTick()
{
	CMonstro* pMonstro = dynamic_cast<CMonstro*>(GetOwnerObj());
	assert(pMonstro);
	
	if(pMonstro->GetIsActive())
	{
		CFlipbookPlayer* pMonstroFlipbook = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(COMPONENT_TYPE::FLIPBOOKPLAYER));
		assert(pMonstroFlipbook);

		pMonstroFlipbook->Play(MONSTRO_IDLE, 1.f, false, false);

		if (pMonstro->GetMonInfo().CurHP <= 0)
		{
			GetFSM()->ChangeState(L"Death");
		}

		else
		{
			if (m_curTime >= 1.f)
			{
				srand(time(NULL));
				int pattern = rand() % 3;

				wchar_t str[255];
				swprintf_s(str, 255, L"pattern : %d", pattern);
				TextOut(CEngine::GetInst()->GetSecondDC(), 10, 200, str, wcslen(str));

				if (pattern == 0)
				{
					GetFSM()->ChangeState(L"Jump");
				}

				else if (pattern == 1)
				{
					GetFSM()->ChangeState(L"Chase");
				}

				else
				{
					GetFSM()->ChangeState(L"Attack");
				}
			}
		}

		m_curTime += DT;
	}

	
}

void CMonstroIdleState::Exit()
{
}
