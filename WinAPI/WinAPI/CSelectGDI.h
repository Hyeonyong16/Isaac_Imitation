#pragma once

// ���� ���೻���� 
// https://youtu.be/fyNb6tK0vGU?si=t9_gBLLnsFnR1Hna		������Ʈ ��Ʈ
// https://youtu.be/As-EL_jee5Q?si=xA_VkRVAH3fsjfDn		GDI ������

class CSelectGDI
{
private:
	HDC m_DC;
	HGDIOBJ m_PrevObject;	// SelectObject �Լ��� ���� �� ���� ������Ʈ ���� ��ȯ��

public:
	CSelectGDI(HDC _dc, HGDIOBJ _InputObject);
	~CSelectGDI();
};

