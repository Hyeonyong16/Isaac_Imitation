#pragma once

struct Vec2
{
public:
	float x;
	float y;

public:
	float Length()
	{
		return sqrt(x * x + y * y);
	}

	void Normalize()
	{
		float Len = Length();
		x /= Len;
		y /= Len;
	}

	// 내적
	float Dot(Vec2 _Other)
	{
		return x * _Other.x + y * _Other.y;
	}

	Vec2 operator + (Vec2 _Other) { return Vec2(x + _Other.x, y + _Other.y); }
	Vec2 operator + (float _f) { return Vec2(x + _f, y + _f); }
	void operator += (Vec2 _Other) { x += _Other.x; y += _Other.y; }
	void operator += (float _f) { x += _f; y += _f; }

	Vec2 operator - (Vec2 _Other) { return Vec2(x - _Other.x, y - _Other.y); }
	Vec2 operator - (float _f) { return Vec2(x - _f, y - _f); }
	void operator -= (Vec2 _Other) { x -= _Other.x; y -= _Other.y; }
	void operator -= (float _f) { x -= _f; y -= _f; }

	Vec2 operator * (Vec2 _Other) { return Vec2(x * _Other.x, y * _Other.y); }
	Vec2 operator * (float _f) { return Vec2(x * _f, y * _f); }
	void operator *= (Vec2 _Other) { x *= _Other.x; y *= _Other.y; }
	void operator *= (float _f) { x *= _f; y *= _f; }

	Vec2 operator / (Vec2 _Other) { assert(_Other.x && _Other.y); return Vec2(x / _Other.x, y / _Other.y); }
	Vec2 operator / (float _f) { assert(_f); return Vec2(x / _f, y / _f); }
	void operator /= (Vec2 _Other) { assert(_Other.x && _Other.y); x /= _Other.x; y /= _Other.y; }
	void operator /= (float _f) { assert(_f); x /= _f; y /= _f; }

	bool operator != (Vec2 _Other)
	{
		if (x == _Other.x && y == _Other.y) return false;
		else return true;
	}

public:
	Vec2()
		: x(0.f)
		, y(0.f)
	{}

	Vec2(float _x, float _y)
		: x(_x)
		, y(_y)
	{}

	Vec2(POINT _pt)
		: x((float)_pt.x)
		, y((float)_pt.y)
	{}

	Vec2(int _x, int _y)
		: x((float)_x)
		, y((float)_y)
	{}

	~Vec2()
	{}
};

struct tTask
{
	TASK_TYPE Type;
	DWORD_PTR Param0;
	DWORD_PTR Param1;
};

struct tDbgRenderInfo
{
	DEBUG_SHAPE		Type;
	Vec2			Position0;
	Vec2			Position1;
	Vec2			Scale;
	PEN_TYPE		Color;
	float			Duration;	// 표시할 시간
	float			Time;		// 표시 후 경과된 시간
};

struct tTile
{
	int ImgIdx;
};

struct tRoomInfo
{
	Vec2			Pos;		// 방의 위치
	ROOM_INFO		RoomInfo;	// 방의 종류
};

struct tObjInfo
{
	wstring			ObjTypeKey;	// sprite 를 받아서 키값 > 키는 몬스터 종류
	Vec2			Pos;		// 위치 ( 12 * 7 )	
};

struct tMonInfo
{
	float MaxHP;
	float CurHP;

	float MaxMP;
	float CurMP;

	float Att;
	float Def;
	float Dex;

	float AttRange;
	float DetectRange;

	float Speed;
};