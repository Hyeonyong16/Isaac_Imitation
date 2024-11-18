#include "pch.h"
#include "CPooterDeathState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CTimeMgr.h"

#include "CObj.h"
#include "CPooter.h"

CPooterDeathState::CPooterDeathState()
	: m_curTime(0)
{
}

CPooterDeathState::~CPooterDeathState()
{
}

void CPooterDeathState::Enter()
{
	m_curTime = 0.f;
}

void CPooterDeathState::FinalTick()
{
	CPooter* pPooter = dynamic_cast<CPooter*>(GetOwnerObj());

	if (m_curTime >= .5f)
	{
		DeleteObject(GetOwnerObj());
	}
	m_curTime += DT;
}

void CPooterDeathState::Exit()
{
}
