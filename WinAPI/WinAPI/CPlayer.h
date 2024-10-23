#pragma once
#include "CObj.h"

class CCollider;
class CTexture;
class CFlipbookPlayer;
class CRigidBody;

class CPlayer :
    public CObj
{
private:
    float       m_Speed;        // �÷��̾� �ӵ�
    float       m_AttSpeed;     // �ʴ� �߻� �ӵ�
    float       m_AccTime;      // �ð� ����
    CCollider*  m_HitBox;

    CTexture*   m_Texture;

    CFlipbookPlayer* m_FlipbookPlayer;

    CRigidBody* m_RigidBody;

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

public:
    CPlayer();
    ~CPlayer();
};
