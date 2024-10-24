#pragma once

enum class PEN_TYPE
{
	RED,
	GREEN,
	BLUE,
	END,
};

enum class BRUSH_TYPE
{
	RED,
	GREEN,
	BLUE,
	GRAY,
	HOLLOW,
	END,
};

enum class LEVEL_TYPE
{
	START,
	STAGE_0,
	STAGE_1,
	STAGE_2,
	EDITOR_TILE,
	EDITOR_SPRITE,

	END,
};

enum class TASK_TYPE
{
	// Param0 : 오브젝트의 주소, Param1 : 레이어 종류
	CREATE_OBJECT,

	// Param0 : 오브젝트의 주소
	DELETE_OBJECT,

	// Param0 : 바꿀 레벨
	CHANGE_LEVEL,
};

enum class LAYER_TYPE
{
	BACKGROUND,
	TILE,
	DEFAULT,
	PLAYER,
	PLAYER_OBJECT,
	MONSTER,
	MONSTER_OBJECT,

	// 7 ~ 30

	UI = 31,
	END = 32
};

enum class COMPONENT_TYPE
{
	COLLIDER,
	RIGIDBODY,
	FLIPBOOKPLAYER,
	FSM,
	TILEMAP,
};

enum class ASSET_TYPE
{
	TEXTURE,	// 이미지
	SOUND,		// 소리
	SPRITE,		// 특정 텍스쳐의 특정 부위
	FLIPBOOK,	// Sprite 들의 집합체
};

enum class DEBUG_SHAPE
{
	RECT,
	CIRCLE,
	LINE,
};