#pragma once
#ifndef __CENGINE_H__
#define __CENGINE_H__

#include "CTexture.h"

// ���α׷��� �ֻ��� ������
class CEngine
{
	SINGLE(CEngine);	// �̱��� ���� - define.h �� ����
private:
	HINSTANCE	m_hInst;		// ���μ��� �ּ�
	HWND		m_hWnd;			// ���� ������ �ڵ�
	Vec2		m_Resolution;	// ���� ������ �ػ�
	HDC			m_hDC;			// Main DC(Device Context) �׸��� ���� ������, like �׸��� ���� ����ü

	CTexture*	m_BackBuffer;	// ����� �뵵 �ؽ���

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

// �̱���
// - ��ü ���� ������ 1���� ����
// ���� ������ �����ϴ� ���

// ����: �ϳ��� ��ü�� ����, ������ ���� ��ü�� ��ȯ
// ��� 1. ��ü�� Data ������ ���� (��������, �������� ��)
//			���� - ������ �����ϴ�.
//			     - ��ü�� �Ҹ��� �Ű澲�� �ʾƵ� �ȴ�.
//			���� - ���α׷� ���� ���� ������ �Ұ���
/*
class CEngine
{
public:
	// CEngine ��ü�� ���� ��ȯ�ϴ� �Լ��� ����
	// ����Լ� �ȿ����� private �� ������ ����
	// ����Լ��� ȣ���Ϸ��� ��ü�� �ʿ� > ���� ��� �Լ��� ��ü�� ��� ȣ�� ����
	// �Լ� �տ� static Ű����

	// ���� ����Լ�
	// - ��ü ���� ȣ�� ������ ����Լ� ( this �����Ͱ� �������� �ʴ´�. )
	// - private �� ������ ����

	static CEngine* GetInst()
	{
		// ���� ������ Data ������ ���� -> ���� �ش� �Լ� ���ٽÿ��� �̹� �����Ǿ� �־ �� ���� X
		static CEngine engine;
		return &engine;
	}

private:
	CEngine();
	CEngine(const CEngine& _Origin) = delete;			// ���� ������ ���ϰ� ����.
	void operator =(const CEngine& _Origin) = delete;	// ���� ������ ���ϰ� ����.
	~CEngine();

};
*/

// ��� 2. ��ü�� �����Ҵ� (Heap ����)
//			���� - ��Ÿ�� ���߿� ����, ������ �����ϴ�.
//			���� - ������ �����ϴ�
//			     - ��ü�� �Ҹ��� �����ؾ� �Ѵ�.
/*
class CEngine
{
private:
	// ���� ��� ����
	// - ��ü �ȿ� ������� �ʴ�.
	// - ������ ��������
	// - ����� Ŭ���� ���� ��������
	static CEngine*		m_Inst;	

	// ����: �ϳ��� ��ü�� ����, ������ ���� ��ü�� ��ȯ
	// ��� 2. ��ü�� �����Ҵ� (Heap ����)
	//			���� - ��Ÿ�� ���߿� ����, ������ �����ϴ�.
	//			���� - ������ �����ϴ�
	//			     - ��ü�� �Ҹ��� �����ؾ� �Ѵ�.
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
