#pragma once
#include <string>

struct ST_STRING_CONTEXT
{
	// ���� Ŀ�� ��ġ
	int nCursorPos; 

	// ������� �Ϸ�� ���ڿ�
	std::wstring strContext;

	// ���� �������� ����
	std::wstring strUnderConstruct;
};

struct ST_CONSTRUCT
{
public:
	// �ʼ� �ε��� ��ȣ
	int choseong;

	// �߼� �ε��� ��ȣ
	int jungseong;

	// ���� �ε��� ��ȣ
	int jongseong;
	void clear();
};

struct ST_KEYSTATE
{
	int nVirtKey;
	short nID;
	bool bPressed;
	bool bReserved;     // 7����Ʈ�� 8����Ʈ�� ������ ä���ִ� ����
};

enum E_GAME_OBJECT_DIR
{
	GAME_OBJECT_DIR_LEFT = 1,
	GAME_OBJECT_DIR_RIGHT,
	GAME_OBJECT_DIR_UP,
	GAME_OBJECT_DIR_DOWN,
};
