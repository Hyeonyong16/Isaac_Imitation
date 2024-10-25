#pragma once
#include "CLevel.h"

class CTexture;

class CLevel_Editor_Sprite :
    public CLevel
{
private:
    CTexture*   m_AtlasTexture;
    HMENU       m_hMenu;

    bool        m_bSpriteMenu;

    HWND        m_hDlgHandle;   // ��޸��� ���̾�α� �ڵ� �ޱ�


    // Sprite Set ���� ����
    Vec2        m_SpritePos;
    Vec2        m_SpriteScale;

    Vec2        m_dbgRectPos;   // ����׿� �簢�� ��ǥ ���

    bool        m_bDrawSprite;  // Picture Control �� Sprite �� �ٽ� �׷��� �ϴ°�
    bool        m_bMouseLBtnPressed;    // ���콺 Ŭ�� ���� �� drawRect ����


    // Flipbook ���� ����

    // collider ���� ����



public:
    void SetSpriteMenu(bool _bSpriteMenu) { m_bSpriteMenu = _bSpriteMenu; }
    void SetDlgHandle(HWND _hwnd) { m_hDlgHandle = _hwnd; }
    void SetSpritePos(Vec2 _pos) { m_SpritePos = _pos; }
    void SetSpriteScale(Vec2 _scale) { m_SpriteScale = _scale; }
    void SetDrawSprite(bool _drawSprite) { m_bDrawSprite = _drawSprite; }
   

    bool GetSpriteMenu() { return m_bSpriteMenu; }
    Vec2 GetSpritePos() { return m_SpritePos; }

public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:

    // ������ �� �Լ�
    void LoadAtlasTexture();    // atlas �ؽ��� ��������
    void SaveSprite();          // �Է��� ��ġ�� sprite ����
    void LoadSprite();          // ������ sprite ��ġ ��������
    void SaveFlipbook();        // ���� ������ sprite ������ flipbook ����
    void LoadFlipbook();        // flipbook ������ ������




public:
    CLevel_Editor_Sprite();
    ~CLevel_Editor_Sprite();
};

