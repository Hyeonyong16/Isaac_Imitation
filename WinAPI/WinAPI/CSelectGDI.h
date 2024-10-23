#pragma once

// 당일 진행내용은 
// https://youtu.be/fyNb6tK0vGU?si=t9_gBLLnsFnR1Hna		오브젝트 파트
// https://youtu.be/As-EL_jee5Q?si=xA_VkRVAH3fsjfDn		GDI 렌더링

class CSelectGDI
{
private:
	HDC m_DC;
	HGDIOBJ m_PrevObject;	// SelectObject 함수도 선택 시 이전 오브젝트 값을 반환함

public:
	CSelectGDI(HDC _dc, HGDIOBJ _InputObject);
	~CSelectGDI();
};

