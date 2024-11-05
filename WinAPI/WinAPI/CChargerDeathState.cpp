#include "pch.h"
#include "CChargerDeathState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CTimeMgr.h"

#include "CObj.h"
#include "CCharger.h"

CChargerDeathState::CChargerDeathState()
	: m_curTime(0)
{
}

CChargerDeathState::~CChargerDeathState()
{
}

void CChargerDeathState::Enter()
{
	m_curTime = 0.f;
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	//CFlipbookPlayer* pChargerFlipbook = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Charger_Flipbook"));
	//assert(pChargerFlipbook);

	//pChargerFlipbook->Play(CHARGER_DEATH, 5.f, false);
}

void CChargerDeathState::FinalTick()
{
	CCharger* pCharger = dynamic_cast<CCharger*>(GetOwnerObj());
	//CFlipbookPlayer* pChargerFlipbook = dynamic_cast<CFlipbookPlayer*>(GetOwnerObj()->GetComponent(L"Charger_Flipbook"));
	//assert(pChargerFlipbook);

	//if (pChargerFlipbook->isFinish())
	//{
	//	DeleteObject(pCharger);
	//}
	if (m_curTime >= .5f)
	{
		DeleteObject(GetOwnerObj());
	}
	m_curTime += DT;
}

void CChargerDeathState::Exit()
{
}
