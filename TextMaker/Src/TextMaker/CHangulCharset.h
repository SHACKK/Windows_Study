#pragma once
#include "ICharSet.h"

#define VOWEL 0x0001		//����
#define CONSONANT 0x0002	//����

#define UNICODE_BASE 44032		// 0xAC00, Unicode �⺻ ��
#define NUM_OF_CHOSEONG 19		// �ʼ� ��
#define NUM_OF_JUNGSEONG 21		// �߼� ��
#define NUM_OF_JONGSEONG 28		// ���� ��

#define CONSTRUCT_DEFAULT 99	// ����ü default ��

enum E_CONSTRUCT_STATE
{
	BLINK,						// ����ִ� ����
	ONLY_CHOSEONG,				// �ʼ��� �ִ� ����
	ONLY_JUNGSEONG,				// �߼��� �ִ� ����
	NO_JONGSEONG_COMB_ABLE_H,	// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_ABLE_N,	// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_ABLE_M,	// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_UNABLE,	// ��ħ ���� ���պҰ� ����
	ONE_JONGSEONG_COMB_ABLE_R,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_S,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_F,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_Q,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_UNABLE,	// Ȭ��ħ ���պҰ� ����
	DOUBLE_JONGSEONG			// �ֹ�ħ ����
};

std::map<int, std::string> mStringFromVitrualKey
{
	{0x41, "a"},
	{0x42, "b"},
	{0x43, "c"},
	{0x44, "d"},
	{0x45, "e"},
	{0x46, "f"},
	{0x47, "g"},
	{0x48, "h"},
	{0x49, "i"},
	{0x4A, "j"},
	{0x4B, "k"},
	{0x4C, "l"},
	{0x4D, "m"},
	{0x4E, "n"},
	{0x4F, "o"},
	{0x50, "p"},
	{0x51, "q"},
	{0x52, "r"},
	{0x53, "s"},
	{0x54, "t"},
	{0x55, "u"},
	{0x56, "v"},
	{0x57, "w"},
	{0x58, "x"},
	{0x59, "y"},
	{0x5A, "z"}
};

class CHangulCharset : public ICharSet
{
private:
	const char* charset_cho[19] = 
	{ "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
	const char* charset_jung[21] = 
	{ "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
	const char* charset_jong[28] = 
	{ "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };

	const char* charset_Useless[19] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" };

	int charset_single[40] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163 };

	E_CONSTRUCT_STATE state = BLINK;
	ST_CONSTRUCT stCurrentConstruct = { CONSTRUCT_DEFAULT, CONSTRUCT_DEFAULT, 0 };
	
	std::string StrFromVirtualKey(int VirtualKey);
	int GetIndexNum(const char* chararray[], int SizeofArray, const char* c);
	int CheckStr(std::string c);
	int AssemUnicode(ST_CONSTRUCT stCurrentConstruct);
	ST_CONSTRUCT DisassemUnicode(std::string strUnderConstruct);
	//E_CONSONANT_TYPE CheckConsonantType(const char* consonant);
	//E_VOWEL_TYPE CheckVowelType(const char* vowel);
	void DeleteChar(int nVirtualKey, ST_STRING_CONTEXT& context);

public:
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};