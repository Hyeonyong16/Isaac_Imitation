#include "pch.h"
#include "CFSM.h"

#include "CState.h"

CFSM::CFSM()
	: CComponent(COMPONENT_TYPE::FSM)
	, m_CurState(nullptr)
{
}

CFSM::~CFSM()
{
	Delete_Map(m_mapState);
}

void CFSM::FinalTick()
{
	if (nullptr == m_CurState)
		return;

	m_CurState->FinalTick();
}

CState* CFSM::FindState(const wstring& _Key)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_Key);

	if (iter == m_mapState.end())
		return nullptr;

	return iter->second;
}

void CFSM::AddState(const wstring& _Key, CState* _State)
{
	assert(!FindState(_Key));


	_State->m_Owner = this;
	m_mapState.insert(make_pair(_Key, _State));
}


void CFSM::ChangeState(const wstring& _NextState)
{
	CState* pNextState = FindState(_NextState);
	assert(pNextState);

	if (nullptr != m_CurState)
		m_CurState->Exit();

	m_CurState = pNextState;

	m_CurState->Enter();
}

wstring CFSM::GetCurState()
{
	map<wstring, CState*>::iterator iter = m_mapState.begin();
	for (; iter != m_mapState.end(); ++iter)
	{
		if (iter->second == m_CurState)
			return iter->first;
	}
	return L"error";
}
