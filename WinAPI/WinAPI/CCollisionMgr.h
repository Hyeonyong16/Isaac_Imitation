#pragma once

union COLLISION_ID
{
	struct
	{
		UINT Left;
		UINT Right;
	};

	ULONGLONG ID;
};

class CCollider;

class CCollisionMgr
{
	SINGLE(CCollisionMgr);
private:
	UINT					m_Matrix[(UINT)LAYER_TYPE::END];
	map<ULONGLONG, bool>	m_ColInfo;

public:
	void CollisionCheck(LAYER_TYPE _Left, LAYER_TYPE _Right);
	void CollisionCheckClear()
	{ 
		memset(m_Matrix, 0, sizeof(UINT) * (UINT)LAYER_TYPE::END); 
		m_ColInfo.clear();
	}

public:
	void Tick();

private:
	void CollisionBtwLayer(LAYER_TYPE _Left, LAYER_TYPE _Right);
	void CollisionBtwCollider(CCollider* _LeftCol, CCollider* _RightCol);
	bool IsCollision(CCollider* _LeftCol, CCollider* _RightCol);

};

