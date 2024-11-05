#pragma once
#include "CObj.h"

class CTexture;
class CCollider;
class CFSM;
class CRigidBody;

class CMonster :
    public CObj
{
private:
    tMonInfo    m_Info;

    int         m_Dir;
    float       m_Speed;
    Vec2        m_InitPos;
    float       m_Dist;

    //CTexture*   m_Tex;

protected:
    CCollider*  m_Collider;
    CFSM*       m_FSM;
    CRigidBody* m_RigidBody;

public:
    void SetSpeed(float _Speed) { m_Speed = _Speed; }
    void SetDistance(float _Dist) { m_Dist = _Dist; }

    const tMonInfo& GetMonInfo() { return m_Info; }
    void SetMonInfo(tMonInfo& _info) { m_Info = _info; }

    void DecreaseHP(float _dmg) { m_Info.SetCurHP(m_Info.CurHP - _dmg); }

    void SetInitPos(Vec2 _pos) { m_InitPos = _pos; }

    CFSM* GetFSM() { return m_FSM; }
    void SetFSM(CFSM* _fsm) { m_FSM = _fsm; }

    CRigidBody* GetRigidBody() { return m_RigidBody; }

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

public:
    CMonster();
    ~CMonster();
};

