#include "pch.h"
#include "CCollisionMgr.h"

#include "CLevel.h"
#include "CLevelMgr.h"
#include "CObj.h"
#include "CCollider.h"

CCollisionMgr::CCollisionMgr()
	: m_Matrix{}
{

}

CCollisionMgr::~CCollisionMgr()
{

}



void CCollisionMgr::CollisionCheck(LAYER_TYPE _Left, LAYER_TYPE _Right)
{
	// 입력으로 들어온 레이어 번호 중
	// 더 작은 값을 Matrix 의 행, 더 큰 값을 Matrix의 열로 사용
	UINT Row = (UINT)_Left;
	UINT Col = (UINT)_Right;

	if (Col < Row)
	{
		Row = (UINT)_Right;
		Col = (UINT)_Left;
	}
	
	// 이미 체크가 되어있으면
	if (m_Matrix[Row] & (1 << Col))
	{
		m_Matrix[Row] &= ~(1 << Col);
	}

	// 체크가 안되어있으면
	else
	{
		m_Matrix[Row] |= (1 << Col);
	}
}

void CCollisionMgr::Tick()
{
	for (UINT Row = 0; Row < (UINT)LAYER_TYPE::END; ++Row)
	{
		for (UINT Col = Row; Col < (UINT)LAYER_TYPE::END; ++Col)
		{
			if (!(m_Matrix[Row] & (1 << Col)))
				continue;

			CollisionBtwLayer((LAYER_TYPE)Row, (LAYER_TYPE)Col);
		}
	}
}

void CCollisionMgr::CollisionBtwLayer(LAYER_TYPE _Left, LAYER_TYPE _Right)
{
	const vector<CCollider*>& vecLeft = CLevelMgr::GetInst()->GetCurrentLevel()->GetColliders(_Left);
	const vector<CCollider*>& vecRight = CLevelMgr::GetInst()->GetCurrentLevel()->GetColliders(_Right);

	// 서로 다른 레이어간의 충돌 검사
	if(_Left != _Right)
	{
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = 0; j < vecRight.size(); ++j)
			{
				CollisionBtwCollider(vecLeft[i], vecRight[j]);
			}
		}
	}

	// 서로 같은 레이어간의 충돌 검사
	else
	{
		// 중복 검사 또는 자기 자신 스스로와 검사를 피하기 위해서 j 를 i + 1 부터 시작
		for (size_t i = 0; i < vecLeft.size(); ++i)
		{
			for (size_t j = i + 1; j < vecRight.size(); ++j)
			{
				CollisionBtwCollider(vecLeft[i], vecRight[j]);
			}
		}
	}
}

void CCollisionMgr::CollisionBtwCollider(CCollider* _LeftCol, CCollider* _RightCol)
{
	COLLISION_ID ColID = {};
	ColID.Left = _LeftCol->GetID();
	ColID.Right = _RightCol->GetID();

	map<ULONGLONG, bool>::iterator iter = m_ColInfo.find(ColID.ID);

	// 두 충돌체 조합이 등록된 적이 없다.
	if (iter == m_ColInfo.end())
	{
		// 등록을 시킨다.
		m_ColInfo.insert(make_pair(ColID.ID, false));
		iter = m_ColInfo.find(ColID.ID);
	}

	bool IsDead = _LeftCol->GetOwner()->IsDead() || _RightCol->GetOwner()->IsDead();

	// 현재 두 콜라이더가 겹쳐있을 때
	if (!IsDead && IsCollision(_LeftCol, _RightCol))
	{
		// 이전 프레임에 겹쳐있었다면
		if (iter->second)
		{
			_LeftCol->Overlap(_RightCol);
			_RightCol->Overlap(_LeftCol);
		}

		// 이전 프레임에 겹쳐있지 않았다면
		else
		{
			_LeftCol->BeginOverlap(_RightCol);
			_RightCol->BeginOverlap(_LeftCol);
			iter->second = true;	// 이전 프레임 충돌 정보를 갱신
		}
	}

	// 현재 두 콜라이더가 떨어져있을 때
	else
	{
		// 이전 프레임에 겹쳐있었다면
		if (iter->second)
		{
			_LeftCol->EndOverlap(_RightCol);
			_RightCol->EndOverlap(_LeftCol);
			iter->second = false;	// 이전 프레임 충돌 정보를 갱신
		}
	}
}

bool CCollisionMgr::IsCollision(CCollider* _LeftCol, CCollider* _RightCol)
{
	Vec2 LeftPos = _LeftCol->GetFinalPos();
	Vec2 RightPos = _RightCol->GetFinalPos();
	Vec2 vDiff = LeftPos - RightPos;

	Vec2 LeftScale = _LeftCol->GetScale();
	Vec2 RightScale = _RightCol->GetScale();

	// 겹쳐있다면 == 두 오브젝트 위치의 x, y 좌표 거리가 각 오브젝트 x, y 크기의 절반을 더한것보다 작으면
	if (fabs(vDiff.x) < (LeftScale.x + RightScale.x) / 2.f
		&& fabs(vDiff.y) < (LeftScale.y + RightScale.y) / 2.f)
	{
		return true;
	}

	return false;
}
