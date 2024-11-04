#pragma once

void CreateObject(class CObj* _NewObject, LAYER_TYPE _Layer);
void DeleteObject(CObj* _DeleteObj);
void ChangeLevel(LEVEL_TYPE _NextLevel);

bool IsValid(CObj*& _Object);

Vec2 Rotate(Vec2 _Dir, float _Angle);
bool GetClockWise(Vec2 _A, Vec2 _B);

class CAsset;
void SaveAssetRef(CAsset* _Asset, FILE* _File);
CAsset* LoadAssetRef(FILE* _File);

void DrawDebugRect(PEN_TYPE _Pen, Vec2 _Pos, Vec2 _Scale, float _Duration);
void DrawDebugCircle(PEN_TYPE _Pen, Vec2 _Pos, Vec2 _Scale, float _Duration);
void DrawDebugLine(PEN_TYPE _Pen, Vec2 _Start, Vec2 _End, float _Duration);

class CRigidBody;
void SetGroundInRigidBody(Vec2 _mainScale, Vec2 _objScale, Vec2 _mainPos, Vec2 _objPos, CRigidBody* _rigidBody);
void releaseGroundInRigidBody(Vec2 _mainScale, Vec2 _objScale, Vec2 _mainPos, Vec2 _objPos, CRigidBody* _rigidBody);

float Saturate(float _Ratio);