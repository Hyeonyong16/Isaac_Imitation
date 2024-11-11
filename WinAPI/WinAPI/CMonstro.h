#pragma once
#include "CMonster.h"

class CFlipbookPlayer;

enum MONSTRO_ANIM_STATE
{
    MONSTRO_IDLE,
    MONSTRO_ATTACK,
    MONSTRO_CHASE,
    MONSTRO_JUMP,
    MONSTRO_DOWN,
};

class CMonstro :
    public CMonster
{
private:
    bool m_isJumping;   // ���� �������ΰ�
    bool m_isGround;

    bool m_isChaseToJump;   // ���� ������ �Ѿƿ��� �����ΰ�
    int m_JumpCount;      // ���� ���� Ƚ��

    int m_curPattern;       // ���� ����
                            // 0: ���(����)    1: ����    2: ����   3: �Ѿ� ����

    CFlipbookPlayer* m_monsterFlipbook;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

public:
    bool GetIsJumping() { return m_isJumping; }
    void SetIsJumping(bool _isJump) { m_isJumping = _isJump; }

    bool GetIsGround() { return m_isGround; }
    void SetIsGround(bool _isGround) { m_isGround = _isGround; }

    bool GetIsChaseToJump() { return m_isChaseToJump; }
    void SetIsChaseToJump(bool _isChase) { m_isChaseToJump = _isChase; }

    int GetJumpCount() { return m_JumpCount; }
    void SetJumpCount(int _Count) { m_JumpCount = _Count; }
    void AddJumpCount() { ++m_JumpCount; }

    int GetCurPattern() { return m_curPattern; }
    void SetCurPattern(int _pattern) { m_curPattern = _pattern; }

    CFlipbookPlayer* GetFlipbook() { return m_monsterFlipbook; }

    void CreateMonstroFlipbook();

public:
    CMonstro();
    ~CMonstro();

};

