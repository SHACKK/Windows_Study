#pragma once
#include "ICharSet.h"

#define VOWEL 0x0001		//모음
#define CONSONANT 0x0002	//자음

#define UNICODE_BASE 44032		// 0xAC00
#define NUM_OF_CHOSEONG 19		// 초성 수
#define NUM_OF_JUNGSEONG 21		// 중성 수
#define NUM_OF_JONGSEONG 28		// 종성 수

#define CONSTRUCT_DEFAULT 99	// 구조체 default 값

enum E_CONSTRUCT_STATE
{
	BLINK,						// 비어있는 상태
	ONLY_CHOSEONG,				// 초성만 있는 음절
	ONLY_JUNGSEONG,				// 중성만 있는 음절
	NO_JONGSEONG_COMB_ABLE_H,	// 받침 없는 조합가능 음절 "ㅗ"
	NO_JONGSEONG_COMB_ABLE_N,	// 받침 없는 조합가능 음절 "ㅜ"
	NO_JONGSEONG_COMB_ABLE_M,	// 받침 없는 조합가능 음절 "ㅡ"
	NO_JONGSEONG_COMB_UNABLE,	// 받침 없는 조합불가 음절
	ONE_JONGSEONG_COMB_ABLE_R,	// 홑받침 조합가능 음절 "ㄱ"
	ONE_JONGSEONG_COMB_ABLE_S,	// 홑받침 조합가능 음절 "ㄴ"
	ONE_JONGSEONG_COMB_ABLE_F,	// 홑받침 조합가능 음절 "ㄹ"
	ONE_JONGSEONG_COMB_ABLE_Q,	// 홑받침 조합가능 음절 "ㅂ"
	ONE_JONGSEONG_COMB_UNABLE,	// 홑밭침 조합불가 음절
	DOUBLE_JONGSEONG			// 쌍받침 음절
};

enum E_CONSONANT_TYPE
{
	R,
	S,
	F,
	Q,
	NORMAL
};

enum E_VOWEL_TYPE
{
	H,
	N,
	M,
	NORMAL
};

class CHangulCharset : public ICharSet
{
private:
	const char* charset_cho[19] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
	const char* charset_jung[21] = { "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
	const char* charset_jong[28] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };

	const char* charset_Useless[19] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" };

	int charset_single[40] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163 };

	E_CONSTRUCT_STATE state = BLINK;
	CONSTRUCT stCurrentConstruct = { CONSTRUCT_DEFAULT, CONSTRUCT_DEFAULT, 0 };
	
	std::string StrFromVirtualKey(int VirtualKey);
	int GetIndexNum(const char* chararray[], int SizeofArray, const char* c);
	int CheckChar(std::string c);
	int AssemUnicode(CONSTRUCT stCurrentConstruct);
	CONSTRUCT DisassemUnicode(std::string strUnderConstruct);
	E_CONSONANT_TYPE CheckConsonantType(std::string consonant);
	E_VOWEL_TYPE CheckVowelType(std::string vowel);

public:
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};