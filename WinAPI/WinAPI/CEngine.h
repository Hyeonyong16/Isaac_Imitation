#pragma once
#ifndef __CENGINE_H__
#define __CENGINE_H__

#include "CTexture.h"

// 프로그램의 최상위 관리자
class CEngine
{
	SINGLE(CEngine);	// 싱글턴 구조 - define.h 에 정의
private:
	HINSTANCE	m_hInst;		// 프로세스 주소
	HWND		m_hWnd;			// 메인 윈도우 핸들
	Vec2		m_Resolution;	// 메인 윈도우 해상도
	HDC			m_hDC;			// Main DC(Device Context) 그리기 관련 관리자, like 그리기 도구 집합체

	CTexture*	m_BackBuffer;	// 백버퍼 용도 텍스쳐

	HPEN		m_Pen[(UINT)PEN_TYPE::END];
	HBRUSH		m_Brush[(UINT)BRUSH_TYPE::END];

public:
	HWND GetMainWndHwnd() { return m_hWnd; }
	Vec2 GetResolution() { return m_Resolution; }
	HPEN GetPen(PEN_TYPE _Type) { return m_Pen[(UINT)_Type]; }
	HBRUSH GetBrush(BRUSH_TYPE _Type) { return m_Brush[(UINT)_Type]; }
	HDC GetMainDC() { return m_hDC; }
	HDC GetSecondDC() { return m_BackBuffer->GetDC(); }

	void ChangeWindowSize(Vec2 _vResolution);

public:
	int Init(HINSTANCE _hInst, POINT _Resolution);
	void Progress();

private:
	void CreateGDIObject();
	void CreateSecondBuffer();
	
};

// 싱글턴
// - 객체 생성 개수를 1개로 제한
// 정적 변수로 생성하는 방법

// 목적: 하나의 객체만 생성, 생성된 유일 객체를 반환
// 방법 1. 객체를 Data 영역에 생성 (전역변수, 정적변수 등)
//			장점 - 구현이 간단하다.
//			     - 객체의 소멸을 신경쓰지 않아도 된다.
//			단점 - 프로그램 실행 도중 해제가 불가능
/*
class CEngine
{
public:
	// CEngine 객체를 만들어서 반환하는 함수를 제공
	// 멤버함수 안에서는 private 에 접근이 가능
	// 멤버함수를 호출하려면 객체가 필요 > 정적 멤버 함수는 객체가 없어도 호출 가능
	// 함수 앞에 static 키워드

	// 정적 멤버함수
	// - 객체 없이 호출 가능한 멤버함수 ( this 포인터가 존재하지 않는다. )
	// - private 에 접근이 가능

	static CEngine* GetInst()
	{
		// 정적 변수는 Data 영역에 저장 -> 추후 해당 함수 접근시에도 이미 생성되어 있어서 또 생성 X
		static CEngine engine;
		return &engine;
	}

private:
	CEngine();
	CEngine(const CEngine& _Origin) = delete;			// 복사 생성을 못하게 만듬.
	void operator =(const CEngine& _Origin) = delete;	// 대입 연산을 못하게 만듬.
	~CEngine();

};
*/

// 방법 2. 객체를 동적할당 (Heap 영역)
//			장점 - 런타임 도중에 생성, 삭제가 유연하다.
//			단점 - 구현이 복잡하다
//			     - 객체의 소멸을 관리해야 한다.
/*
class CEngine
{
private:
	// 정적 멤버 변수
	// - 객체 안에 들어있지 않다.
	// - 유일한 전역변수
	// - 선언된 클래스 전용 전역변수
	static CEngine*		m_Inst;	

	// 목적: 하나의 객체만 생성, 생성된 유일 객체를 반환
	// 방법 2. 객체를 동적할당 (Heap 영역)
	//			장점 - 런타임 도중에 생성, 삭제가 유연하다.
	//			단점 - 구현이 복잡하다
	//			     - 객체의 소멸을 관리해야 한다.
public:
	static CEngine* GetInst()
	{
		if (nullptr == m_Inst)
		{
			m_Inst = new CEngine;
		}

		return m_Inst;
	}

	static void Destroy()
	{
		if (nullptr != m_Inst)
		{
			delete m_Inst;
			m_Inst = nullptr;
		}
	}

private:
	CEngine();

public:
	~CEngine();
};
*/

#endif	// __CENGINE_H__
