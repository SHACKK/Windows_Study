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

struct CONSTRUCT
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
