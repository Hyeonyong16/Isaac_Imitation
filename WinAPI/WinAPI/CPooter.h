#pragma once
#include "CMonster.h"

enum POOTER_ANIM_STATE
{
    POOTER_IDLE_LEFT,
    POOTER_IDLE_RIGHT,

    POOTER_ATTACK_LEFT,
    POOTER_ATTACK_RIGHT,
};

class CFlipbookPlayer;


class CPooter :
    public CMonster
{
private:
    char    m_LookDir;     // ���Ͱ� ����, ������ �ٶ� ����

    float   m_DetectRange;    // ���� ����
    float   m_AttRange;       // ���� ����

    float   m_AttSpeed;     // �ʴ� �߻� �ӵ�
    float   m_AccTime;      // �ð� ����

    Vec2    m_moveDir;     // ���� ����

    CFlipbookPlayer* m_monsterFlipbook;

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;

    virtual void BeginOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void Overlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;
    virtual void EndOverlap(CCollider* _Collider, CObj* _OtherObject, CCollider* _OtherCollider) override;

private:
    void CreatePooterFlipbook();

public:
    char GetLookDir() { return m_LookDir; }
    void SetLookDir(char _dir) { m_LookDir = _dir; }

    float GetDetectRange() { return m_DetectRange; }
    float GetAttRange() { return m_AttRange; }

    void SetMaxSpeed(float _speed);
    Vec2 GetMoveDir() { return m_moveDir; }
    void SetMoveDir(Vec2 _dir) { m_moveDir = _dir; }

    float GetAttSpeed() { return m_AttSpeed; }
    float GetAccTime() { return m_AccTime; }

    void SetAttSpeed(float _speed) { m_AttSpeed = _speed; }
    void SetAccTime(float _time) { m_AccTime = _time; }

public:
    CPooter();
    ~CPooter();

};

