#pragma once

enum KEY_STATE
{
	NONE,		// �� ���� ����
	TAP,		// ���� �� ( �̹� �����ӿ� ���� ������ �� )
	PRESSED,	// ������ ������ ����
	RELEASED,	// �� ��
};

enum KEY	// ���� �� Ű�� enum���� ����
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

	KEY_END,	// KEY enum ���� ��ü ������ �ǹ�
};

struct KeyInfo
{
	KEY_STATE	State;			// ���� ����
	bool		bPrevPressed;	// ���� �����ӿ� ���� ����
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

