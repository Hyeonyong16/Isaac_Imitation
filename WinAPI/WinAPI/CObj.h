#pragma once
#include "CBase.h"

class CComponent;
class CCollider;

class CObj : 
	public CBase
{
private:
	Vec2				m_Pos;		// 오브젝트의 위치 Position
	Vec2                m_PrevPos;
	Vec2				m_Scale;	// 오브젝트의 크기 Scale
	vector<CComponent*>	m_Component;
	LAYER_TYPE			m_LayerType;

	bool				m_Dead;		// 해당 오브젝트가 삭제 예정 오브젝트인지

public:
	virtual void Begin();			// 레벨 시작 시
	virtual void Tick() = 0;		// 오브젝트가 할 일
	virtual void FinalTick() final;	// 오브젝트가 소유한 component가 할 일
	virtual void Render();			// 오브젝트를 그리기

	virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) {}
	virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) {}
	virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) {}

public:
	void SetPos(Vec2 _Pos) { m_Pos = _Pos; }
	void SetPos(float _x, float _y) { m_Pos = Vec2(_x, _y); }
	void SetScale(Vec2 _Scale) { m_Scale = _Scale; }
	void SetScale(float _x, float _y) { m_Scale = Vec2(_x, _y); }

	Vec2 GetPos() { return m_Pos; }
	Vec2 GetRenderPos();
	Vec2 GetScale() { return m_Scale; }
	LAYER_TYPE GetLayerType() { return m_LayerType; }
	
	bool IsDead() { return m_Dead; }

	CComponent* AddComponent(CComponent* _Component);
	CComponent* GetComponent(const wstring& _Name);
	CComponent* GetComponent(COMPONENT_TYPE _Type);

	template<typename T>
	T* GetComponent();

public:
	CObj();
	~CObj();

	friend class CLevel;
	friend class CTaskMgr;

};

template<typename T>
T* CObj::GetComponent()
{
	for (size_t i = 0; i < m_Component.size(); ++i)
	{
		if (dynamic_cast<T*>(m_Component[i]))
			return (T*)m_Component[i];
	}

	return nullptr;
}
