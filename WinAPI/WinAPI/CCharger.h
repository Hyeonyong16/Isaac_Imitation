#pragma once
#include "CMonster.h"

enum CHARGER_ANIM_STATE
{
    CHARGER_IDLE_UP,
    CHARGER_IDLE_DOWN,
    CHARGER_IDLE_LEFT,
    CHARGER_IDLE_RIGHT,

    CHARGER_ATTACK_UP,
    CHARGER_ATTACK_DOWN,
    CHARGER_ATTACK_LEFT,
    CHARGER_ATTACK_RIGHT,
};

class CFlipbookPlayer;

class CCharger :
    public CMonster
{
private:
    char                m_moveDir;          // 이동 방향을 나타낼 Dir
    char                m_prevDir;

    char                m_TurnDir;          // 왼쪽, 오른쪽 전환 방향
    bool                m_isAttacking;      // 현재 공격중인가(돌진중)
    bool                m_isTurn;           // 다른 콜라이더에 부딪쳐서 방향 전환여부


    CFlipbookPlayer*    m_monsterFlipbook;



public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

private:
    void CreateChargerFlipbook();


public:
    bool GetIsAttacking() { return m_isAttacking; }
    bool GetIsTurn() { return m_isTurn; }

    void SetIsAttacking(bool _isAttacking) { m_isAttacking = _isAttacking; }
    void SetIsTurn(bool _isTurn) { m_isTurn = _isTurn; }
    void SetMoveDir(char _moveDir) { m_moveDir = _moveDir; }


public:
    CCharger();
    ~CCharger();
};

