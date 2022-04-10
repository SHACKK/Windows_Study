#pragma once

class Hangul
{
private:
	const char* keyboard_char[26] = { "q","w","e","r","t","y","u","i","o","p","a","s","d","f","g","h","j","k","l","z","x","c","v","b","n","m" };
	const char* charset_cho[19] = { "r","R","s","e","E","f","a","q","Q","t","T","d","w","W","c","z","x","v","g" }; // 19개
	const char* charset_jung[21] = { "k","o","i","O","j","p","u","P","h","hk","ho","hl","y","n","nj","np","nl","b","m","ml","l" }; // 21개
	const char* charset_jong[28] = { "", "r","R","rt","s","sw","sg","e","f","fr","fa","fq","ft","fx","fv","fg","a","q","qt","t","T","d","w","c","z","x","v","g" }; // 28개
	const char* charset_Useless[19] = { "F","A","D","C","Z","X","V","G","K","I","J","U","N","B","H","Y","M","L","S" }; // 19개
	int charset_single[40] = { 0x3131, 0x3132, 0x3134, 0x3137, 0x3138, 0x3139, 0x3141, 0x3142, 0x3143, 0x3145, 0x3146, 0x3147, 0x3148, 0x3149, 0x314A, 0x314B, 0x314C, 0x314D, 0x314E, 0x314F, 0x3150, 0x3151, 0x3152, 0x3153, 0x3154, 0x3155, 0x3156, 0x3157, 0x3158, 0x3159, 0x315A, 0x315B, 0x315C, 0x315D, 0x315E, 0x315F, 0x3160, 0x3161, 0x3162, 0x3163 };

	struct CompletedEumjeol
	{
		int flag = 0;
		int choseong = 99;
		int jungseong = 99;
		int jongseong = 0;
	}stsyllabel;
	int FindUnicode(CompletedEumjeol stsyllabel); //CompletedEumjeol 구조체를 통해 유니코드값을 계산 후 리턴
	int FindIndex(const char* chararray[], const char*c, int SizeofArray); // 배열의 몇번째 값인지 검색, 없으면 -1 반환
	int SortChar(const char* c); // 자음인지 모음인지 판단
	int SortEumjeol(CompletedEumjeol stsyllabel); // 현재 CompletedEumjeol 구조체의 상태를 리턴 (000, 010, 100, 110, 111)
	void ResetEumjeol(CompletedEumjeol& stsyllabel); // CompletedEumjeol 구조체 초기화
	void DeleteEumjeol(CompletedEumjeol& stsyllabel, int status); // CompletedEumjeol의 값을 차례대로 지움
	int IsKeyboardChar(char c); // 키보드 입력값인지 판단
	int InsertChar(CompletedEumjeol& stsyllabel, char c); // CompletedEumjeol 구조체에 c를 채워넣는 함수
	//CompletedEumjeol stsyllabel;
public:

	std::wstring AssembleHangul(std::wstring strCurrentContext, char c);
};
