#pragma once

class CTexture;

enum POST_PROCESS
{
	FADE_IN,
	FADE_OUT,
	HEART,
};

struct tCamEffect
{
	POST_PROCESS	Effect;		// 진행중인 효과 종류	
	float			Time;		// 효과 진행시간 계산
	float			Duration;	// 효과 최대 진행 시간
};

class CCamera
{
	SINGLE(CCamera);
private:
	Vec2		m_LookAt;	// 카메라가 바라보고 있는 곳
	Vec2		m_Diff;		// 카메라가 보는 지점과 해상도 중앙의 위치 차이값
	Vec2		m_Offset;	// 카메라가 바라보는 위치에서 추가적인 Offset

	CObj*	m_Target;	// 카메라 타겟 오브젝트

	float		m_Duration;    // 진동 유지시간
	float		m_Amplitude;   // 진촉
	float		m_Frequency;   // 초당 진동횟수
	float		m_Time;		   // 누적시간
	float		m_Dir;		   // 진동 방향
	bool		m_bOscillation;// 진동 On / Off

	CTexture* m_CamTex;
	CTexture* m_CamTex1;

	list<tCamEffect>	m_CamEffectList;

public:
	void Init();
	void Tick();
	void Render();

private:
	void Oscillation();

public:
	Vec2 GetRenderPos(Vec2 _RealPos) { return _RealPos - m_Diff; }
	Vec2 GetRealPos(Vec2 _RenderPos) { return _RenderPos + m_Diff; }

	Vec2 GetLookAt() { return m_LookAt + m_Offset; }
	void SetLookAt(Vec2 _pos) { m_LookAt = _pos; }

	void SetTarget(CObj* _Target) { m_Target = _Target; }
	void Oscillation(float _Duration, float _Amplitude, float _Frequency)
	{
		m_Duration = _Duration;
		m_Amplitude = _Amplitude;
		m_Frequency = _Frequency;
		m_Time = 0.f;
		m_bOscillation = true;
		m_Dir = 1.f;
	}

	void PostProcessEffect(POST_PROCESS _Type, float _Duration)
	{
		m_CamEffectList.push_back(tCamEffect{ _Type , 0.f, _Duration });
	}
};

