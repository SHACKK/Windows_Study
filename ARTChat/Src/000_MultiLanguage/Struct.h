#pragma once
#include <string>

struct ST_STRING_CONTEXT
{
	// 현재 커서 위치
	int nCursorPos; 

	// 현재까지 완료된 문자열
	std::wstring strContext;

	// 현재 조립중인 음절
	std::wstring strUnderConstruct;
};

struct ST_CONSTRUCT
{
public:
	// 초성 인덱스 번호
	int choseong;

	// 중성 인덱스 번호
	int jungseong;

	// 종성 인덱스 번호
	int jongseong;
	void clear();
};

struct ST_KEYSTATE
{
	int nVirtKey;
	short nID;
	bool bPressed;
	bool bReserved;     // 7바이트를 8바이트로 유격을 채워주는 역할
};

enum E_GAME_OBJECT_DIR
{
	GAME_OBJECT_DIR_LEFT = 1,
	GAME_OBJECT_DIR_RIGHT,
	GAME_OBJECT_DIR_UP,
	GAME_OBJECT_DIR_DOWN,
};
