#pragma once

struct ST_STRING_CONTEXT
{
	// 현재 커서 위치
	int nCursorPos; 

	// 현재까지 완료된 문자열
	std::string strContext;

	// 현재 조립중인 음절
	std::string strUnderConstruct;
};

struct CONSTRUCT
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
