#pragma once

struct ST_STRING_CONTEXT
{
	// ���� Ŀ�� ��ġ
	int nCursorPos; 

	// ������� �Ϸ�� ���ڿ�
	std::string strContext;

	// ���� �������� ����
	std::string strUnderConstruct;
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