#pragma once

enum KEY_STATE
{
	NONE,		// 안 눌린 상태
	TAP,		// 누를 때 ( 이번 프레임에 최초 눌렸을 때 )
	PRESSED,	// 누른걸 유지한 상태
	RELEASED,	// 땔 때
};

enum KEY	// 자주 쓸 키를 enum으로 생성
{
	Q, W, E, R, T, Y,
	A, S, D, F, G, H,
	Z, X, C, V, B, N, M,

	ENTER,
	ESC,
	LSHIFT,
	ALT,
	CTRL,
	SPACE,

	UP, DOWN, LEFT, RIGHT,

	NUM0,
	NUM1,
	NUM2,
	NUM3,
	NUM4,
	NUM5,
	NUM6,
	NUM7,
	NUM8,
	NUM9,

	LBTN,
	RBTN,

	KEY_END,	// KEY enum 값의 전체 개수를 의미
};

struct KeyInfo
{
	KEY_STATE	State;			// 현재 상태
	bool		bPrevPressed;	// 이전 프레임에 눌린 정보
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<KeyInfo>			m_vecKeyInfo;
	Vec2					m_MousePos;

public:
	KEY_STATE GetKeyState(KEY _key)
	{
		return m_vecKeyInfo[_key].State;
	}
	Vec2 GetMousePos() { return m_MousePos; }

	bool IsMouseOffScreen();

public:
	void Init();
	void Tick();
	
};

