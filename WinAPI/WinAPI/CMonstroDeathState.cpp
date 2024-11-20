#include "pch.h"
#include "CMonstroDeathState.h"

#include "CLevelMgr.h"
#include "CObj.h"
#include "CFlipbookPlayer.h"
#include "CTimeMgr.h"

#include "CObj.h"
#include "CMonstro.h"

CMonstroDeathState::CMonstroDeathState()
	: m_curTime(0)
{
}

CMonstroDeathState::~CMonstroDeathState()
{
}

void CMonstroDeathState::Enter()
{
	m_curTime = 0.f;
}

void CMonstroDeathState::FinalTick()
{
	CMonstro* pMonstro = dynamic_cast<CMonstro*>(GetOwnerObj());


	if (m_curTime >= .5f)
	{
		DeleteObject(GetOwnerObj());
	}
	m_curTime += DT;
}

void CMonstroDeathState::Exit()
{
}
