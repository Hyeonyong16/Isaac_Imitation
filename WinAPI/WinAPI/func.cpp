#include "pch.h"
#include "func.h"

#include "CTaskMgr.h"
#include "CDbgRender.h"

void CreateObject(CObj* _NewObject, LAYER_TYPE _Layer)
{
	tTask task = { TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)_NewObject, (DWORD_PTR)_Layer };
	CTaskMgr::GetInst()->AddTask(task);
}

void DeleteObject(CObj* _DeleteObj)
{
	tTask task = {};

	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param0 = (DWORD_PTR)_DeleteObj;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(LEVEL_TYPE _NextLevel)
{
	tTask task = {};

	task.Type = TASK_TYPE::CHANGE_LEVEL;
	task.Param0 = (DWORD_PTR)_NextLevel;

	CTaskMgr::GetInst()->AddTask(task);
}

#include "CObj.h"
bool IsValid(CObj*& _Object)
{
	if (nullptr == _Object)
		return false;

	else if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}

	return true;
}

Vec2 Rotate(Vec2 _Dir, float _Angle)
{
	_Dir.Normalize();

	Vec2 vRotate = Vec2(cosf(_Angle) * _Dir.x - sinf(_Angle) * _Dir.y
						, sinf(_Angle) * _Dir.x + cosf(_Angle) * _Dir.y);

	return vRotate;
}

bool GetClockWise(Vec2 _A, Vec2 _B)
{
	float F = _A.x * _B.y - _A.y * _B.x;

	if (0.f < F)
		return true;

	return false;
}

#include "CAssetMgr.h"
#include "CAsset.h"
void SaveAssetRef(CAsset* _Asset, FILE* _File)
{
	// 저장하려는 Asset 이 nullptr 이면 0 하나 저장
	int Asset = !!_Asset;
	fwrite(&Asset, sizeof(int), 1, _File);
	if (!Asset) return;

	// Asset 의 종류를 저장
	ASSET_TYPE type = _Asset->GetAssetType();
	fwrite(&type, sizeof(ASSET_TYPE), 1, _File);

	// 저장하려는 Asset 의 키값, 경로를 가져온다.
	const wstring& strKey = _Asset->GetKey();
	const wstring& strPath = _Asset->GetRelativePath();

	// Asset 의 Key, Path 문자열을 저장한다.
	size_t len = strKey.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(strKey.c_str(), sizeof(wchar_t), strKey.length(), _File);

	len = strPath.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(strPath.c_str(), sizeof(wchar_t), strPath.length(), _File);
}

CAsset* LoadAssetRef(FILE* _File)
{
	// 불러오려는 Asset 의 유무 확인
	int Asset = 0;
	fread(&Asset, sizeof(int), 1, _File);
	if (!Asset) return nullptr;

	// Asset 의 종류를 저장
	ASSET_TYPE type;
	fread(&type, sizeof(ASSET_TYPE), 1, _File);

	// 저장하려는 Asset 의 키값, 경로를 가져온다.
	wstring strKey, strPath;

	wchar_t buffer[255] = {};

	// Asset 의 Key, Path 문자열을 저장한다.
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);
	fread(buffer, sizeof(wchar_t), len, _File);
	strKey = buffer;

	wmemset(buffer, 0, 255);

	fread(&len, sizeof(size_t), 1, _File);
	fread(buffer, sizeof(wchar_t), len, _File);
	strPath = buffer;

	CAsset* pAsset = nullptr;
	switch (type)
	{
	case ASSET_TYPE::TEXTURE:
		pAsset = (CAsset*)CAssetMgr::GetInst()->LoadTexture(strKey, strPath);
		break;
	case ASSET_TYPE::SOUND:
		//pAsset = (CAsset*)CAssetMgr::GetInst()->LoadSound(strKey, strPath);
		break;
	case ASSET_TYPE::SPRITE:
		pAsset = (CAsset*)CAssetMgr::GetInst()->LoadSprite(strKey, strPath);
		break;
	case ASSET_TYPE::FLIPBOOK:
		pAsset = (CAsset*)CAssetMgr::GetInst()->LoadFlipbook(strKey, strPath);
		break;
	}

	return pAsset;
}

void DrawDebugRect(PEN_TYPE _Pen, Vec2 _Pos, Vec2 _Scale, float _Duration)
{
	tDbgRenderInfo info = {};

	info.Type = DEBUG_SHAPE::RECT;
	info.Color = _Pen;
	info.Position0 = _Pos;
	info.Scale = _Scale;
	info.Duration = _Duration;
	info.Time = 0.f;

	CDbgRender::GetInst()->AddDbgInfo(info);
}

void DrawDebugCircle(PEN_TYPE _Pen, Vec2 _Pos, Vec2 _Scale, float _Duration)
{
	tDbgRenderInfo info = {};

	info.Type = DEBUG_SHAPE::CIRCLE;
	info.Color = _Pen;
	info.Position0 = _Pos;
	info.Scale = _Scale;
	info.Duration = _Duration;
	info.Time = 0.f;

	CDbgRender::GetInst()->AddDbgInfo(info);
}

void DrawDebugLine(PEN_TYPE _Pen, Vec2 _Start, Vec2 _End, float _Duration)
{
	tDbgRenderInfo info = {};

	info.Type = DEBUG_SHAPE::LINE;
	info.Color = _Pen;
	info.Position0 = _Start;
	info.Position1 = _End;
	info.Duration = _Duration;
	info.Time = 0.f;

	CDbgRender::GetInst()->AddDbgInfo(info);
}

#include "CRigidBody.h"
void SetGroundInRigidBody(Vec2 _mainScale, Vec2 _objScale, Vec2 _mainPos, Vec2 _objPos, CRigidBody* _rigidBody)
{
	Vec2 mainScale = _mainScale;
	Vec2 objScale = _objScale;

	Vec2 scaleSum = (mainScale / 2) + (objScale / 2);
	Vec2 dist = _mainPos - _objPos;
	if (dist.x < 0) dist.x *= -1;
	if (dist.y < 0) dist.y *= -1;

	scaleSum -= dist;
	if (scaleSum.x > scaleSum.y)	// 겹친 부분의 x축 길이가 더 길다
	{
		if (_mainPos.y < _objPos.y)
			_rigidBody->SetGroundY(1);
		else
			_rigidBody->SetGroundY(-1);
	}

	else							// 겹친 부분의 y축 길이가 더 길다
	{
		if (_mainPos.x < _objPos.x)
			_rigidBody->SetGroundX(1);
		else
			_rigidBody->SetGroundX(-1);
	}
}

void releaseGroundInRigidBody(Vec2 _mainScale, Vec2 _objScale, Vec2 _mainPos, Vec2 _objPos, CRigidBody* _rigidBody)
{
	Vec2 mainScale = _mainScale;
	Vec2 objScale = _objScale;

	Vec2 scaleSum = (mainScale / 2) + (objScale / 2);
	Vec2 dist = _mainPos - _objPos;
	if (dist.x < 0) dist.x *= -1;
	if (dist.y < 0) dist.y *= -1;

	scaleSum -= dist;
	if (scaleSum.x > scaleSum.y)	// 겹친 부분의 x축 길이가 더 길다
	{
		_rigidBody->SetGroundY(0);
	}

	else							// 겹친 부분의 y축 길이가 더 길다
	{
		_rigidBody->SetGroundY(0);
	}
}

// 0.f ~ 1.f 범위 제한 함수
float Saturate(float _Ratio)
{
	if (_Ratio < 0.f)
		_Ratio = 0.f;
	else if (1.f < _Ratio)
		_Ratio = 1.f;

	return _Ratio;
}
