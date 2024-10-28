#pragma once
#include "CLevel.h"

class CSprite;
//class CTexture;

class CLevel_MainMenu :
    public CLevel
{
private:
    //CSprite* m_MainMenuSprite;
    //CTexture* m_MainMenuTex;


public:
    virtual void Begin() override;
    virtual void Tick() override;
    virtual void Render() override;
    virtual void End() override;

public:
    void ChangeStage1Level();
    void ChangeEditorLevel();

public:
    CLevel_MainMenu();
    ~CLevel_MainMenu();
};

