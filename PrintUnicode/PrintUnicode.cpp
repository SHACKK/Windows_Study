#include <stdio.h>
#include <Windows.h>
#include <tchar.h>

int main()
{
    wchar_t szTest[11172 + 1];
    for (int i = 0; i < 11172; i++)
        szTest[i] = L'가' + i;

    HANDLE hFile = CreateFile(TEXT("C:\\Users\\LCH\\Desktop\\SHACKK\\Windows_Study\\Unicode.txt"), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);

    DWORD dwWritten = 0;
    char szBOM[2] = { 0xFF, 0xFE };
    WriteFile(hFile, szBOM, sizeof(szBOM), &dwWritten, NULL);
    WriteFile(hFile, szTest, sizeof(szTest), &dwWritten, NULL);
    CloseHandle(hFile);
    return 0;
}