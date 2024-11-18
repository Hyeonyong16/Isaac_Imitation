#pragma once
#include "CMonster.h"

enum CHARGER_ANIM_STATE
{
    CHARGER_MOVE_UP,
    CHARGER_MOVE_DOWN,
    CHARGER_MOVE_RIGHT,
    CHARGER_MOVE_LEFT,

    CHARGER_ATTACK_UP,
    CHARGER_ATTACK_DOWN,
    CHARGER_ATTACK_RIGHT,
    CHARGER_ATTACK_LEFT,
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
    bool                m_isTouchRock;

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
    void SetTurnDir(char _turnDir) { m_TurnDir = _turnDir; }

    char GetMoveDir() { return m_moveDir; }

    void SetMaxSpeed(float _speed);

    bool GetIsTouchRock() { return m_isTouchRock; }
    void SetIsTouchRock(bool _isTouch) { m_isTouchRock = _isTouch; }

public:
    CCharger();
    ~CCharger();
};

