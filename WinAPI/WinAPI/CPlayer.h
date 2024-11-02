#pragma once
#include "CObj.h"

class CCollider;
class CTexture;
class CFlipbookPlayer;
class CRigidBody;
class CFSM;

enum PLAYER_HEAD_ANIM_STATE
{
    ISAAC_HEAD_UP,
    ISAAC_HEAD_DOWN,
    ISAAC_HEAD_LEFT,
    ISAAC_HEAD_RIGHT,

    ISAAC_HEAD_IDLE_UP,
    ISAAC_HEAD_IDLE_DOWN,
    ISAAC_HEAD_IDLE_LEFT,
    ISAAC_HEAD_IDLE_RIGHT,
};

class CPlayer :
    public CObj
{
private:
    float       m_Speed;        // 플레이어 속도
    float       m_AttSpeed;     // 초당 발사 속도
    float       m_AccTime;      // 시간 누적
    CCollider*  m_HitBox;

    //CTexture*   m_Texture;

    CFSM* m_FSM;

    CFlipbookPlayer* m_FlipbookHead;
    //CFlipbookPlayer* m_FlipbookBody;

    CRigidBody* m_RigidBody;

    bool isAttack[4];   // 왼 오 위 아래 순서
    list<char> AttackQueue; // 각 방향 키 들어온 순서

    char m_headDir;     // 머리가 바라볼 방향
    char m_bodyDir;     // 몸이 바라볼 방향

    bool m_isDamaged;     // 데미지를 받았는지 여부
    bool m_isAttacking;      // 공격중인가

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider);

private:
    void CreatePlayerFlipbook();
    void CreateFlipbook(const wstring& _FlipbookName, CTexture* _Atlas, Vec2 _LeftTop, Vec2 _Slice, int MaxFrame);
    
    void SetHeadBodyDir(char _head, char _body) { m_headDir = _head; m_bodyDir = _body; }

public:
    char GetHeadDir() { return m_headDir; }
    char GetBodyDir() { return m_bodyDir; }
    Vec2 GetVelocity();

    bool GetIsDamaged() { return m_isDamaged; }
    void SetIsDamaged(bool _isDamaged) { m_isDamaged = _isDamaged; }

    bool GetIsAttacking() { return m_isAttacking; }

    int GetAttackQueueSize() { return AttackQueue.size(); }

    float GetAccTime() { return m_AccTime; }

public:
    CPlayer();
    ~CPlayer();
};

