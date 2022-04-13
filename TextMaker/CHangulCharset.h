#pragma once

#define VOWEL 0x0001
#define CONSONANT 0x0002

enum E_CONSTRUCT_STATE
{
	BLINK,						// ����ִ� ����
	ONLY_CHOSEONG,				// �ʼ��� �ִ� ����
	ONLY_JUNGSEONG,				// �߼��� �ִ� ����
	NO_JONGSEONG_COMB_ABLE_H,		// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_ABLE_N,		// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_ABLE_M,		// ��ħ ���� ���հ��� ���� "��"
	NO_JONGSEONG_COMB_UNABLE,	// ��ħ ���� ���պҰ� ����
	ONE_JONGSEONG_COMB_ABLE_R,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_S,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_F,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_ABLE_Q,	// Ȭ��ħ ���հ��� ���� "��"
	ONE_JONGSEONG_COMB_UNABLE,	// Ȭ��ħ ���պҰ� ����
	DOUBLE_JONGSEONG			// �ֹ�ħ ����
};

struct CONSTRUCT
{
	int choseong;
	int jungseong;
	int jongseong;
};

class CHangulCharset : public ICharSet
{
private:
	// �ʼ� 19��
	const char* charset_cho[19] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" };
	// �߼� 21��
	const char* charset_jung[21] = { "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" };
	// ���� 28��
	const char* charset_jong[28] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" };
	// Shift ���� �ѱ۹��� 19��
	const char* charset_Useless[19] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" };
	// ���Ϲ��� 19 + 21 = 40��
	int charset_single[40] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163 };

	E_CONSTRUCT_STATE state = BLINK;
	CONSTRUCT stCurrentConstruct;
	
	int FindIndex(const char* chararray[], int SizeofArray, const char* c);
	std::string StrFromVirtualKey(int VirtualKey);
	int AssemUnicode(CONSTRUCT stCurrentConstruct);
	CONSTRUCT DisassemUnicode(std::string strUnderConstruct);
	int CheckChar(std::string c);

public:
	void Update(int nVirtualKey, ST_STRING_CONTEXT& context);
};

