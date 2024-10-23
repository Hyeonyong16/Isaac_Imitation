#include "pch.h"
#include "CTaskMgr.h"

#include "CLevelMgr.h"
#include "CLevel.h"
#include "CObj.h"

CTaskMgr::CTaskMgr()
{

}

CTaskMgr::~CTaskMgr()
{

}

void CTaskMgr::Tick()
{
	for (size_t i = 0; i < m_Garbage.size(); ++i)
	{
		delete m_Garbage[i];
	}
	m_Garbage.clear();


	for (size_t i = 0; i < m_Task.size(); ++i)
	{
		switch (m_Task[i].Type)
		{
		case TASK_TYPE::CREATE_OBJECT:
		{
			CObj* pObject = (CObj*)m_Task[i].Param0;
			LAYER_TYPE Type = (LAYER_TYPE)m_Task[i].Param1;
			CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(pObject, Type);
			pObject->Begin();
		}
			break;

		case TASK_TYPE::DELETE_OBJECT:
		{
			CObj* pObject = (CObj*)m_Task[i].Param0;

			// 만일 동일 프레임에 삭제 요청이 여러번 들어온 경우를 대비
			if (!pObject->IsDead())
			{
				pObject->m_Dead = true;
				m_Garbage.push_back(pObject);
			}
		}
			break;

		case TASK_TYPE::CHANGE_LEVEL:
		{
			LEVEL_TYPE eNextLevel = (LEVEL_TYPE)m_Task[i].Param0;
			CLevelMgr::GetInst()->ChangeLevel(eNextLevel);
		}
			break;
		}
	}

	m_Task.clear();
}
