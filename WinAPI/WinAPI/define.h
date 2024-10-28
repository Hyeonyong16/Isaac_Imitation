// 자주 사용하게될 구문들을 매크로로 만들어 놓을 헤더파일

#pragma once
#ifndef __DEFINE_H__
#define __DEFINE_H__

#define SINGLE(Type) public:\
						static Type* GetInst()\
						{\
							static Type engine;\
							return &engine;\
						}\
						private:\
							Type();\
							Type(const Type& _Origin) = delete;\
							void operator =(const Type& _Origin) = delete;\
							~Type();

#define SELECT_PEN(Pen) CSelectGDI SelectPen(CEngine::GetInst()->GetSecondDC(), CEngine::GetInst()->GetPen(Pen));
#define SELECT_BRUSH(Brush) CSelectGDI SelectBrush(CEngine::GetInst()->GetSecondDC(), CEngine::GetInst()->GetBrush(Brush));

#define DT CTimeMgr::GetInst()->GetDeltaTime()

#define KEY_CHECK(key, state) (CKeyMgr::GetInst()->GetKeyState(key) == state)

#define KEY_NONE(key) KEY_CHECK(key, KEY_STATE::NONE)
#define KEY_TAP(key) KEY_CHECK(key, KEY_STATE::TAP)
#define KEY_PRESSED(key) KEY_CHECK(key, KEY_STATE::PRESSED)
#define KEY_RELEASED(key) KEY_CHECK(key, KEY_STATE::RELEASED)

#define DELETE(p) if(p != nullptr) {delete p; p = nullptr;}

#define PI 3.1415926535f

#define TILE_SIZE_X 125
#define TILE_SIZE_Y 103
#define TILE_SIZE 64

#endif //__DEFINE_H__