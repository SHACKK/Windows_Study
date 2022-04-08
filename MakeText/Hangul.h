#pragma once
#include "stdafx.h"
#include "Hangul.h"

struct CompletedEumjeol;
int FindUnicode(CompletedEumjeol stsyllabel);
int FindIndex(const char* chararray[], const char* c, int SizeofArray);
int SortChar(const char* c);
int SortEumjeol(CompletedEumjeol stsyllabel);
void ResetEumjeol(CompletedEumjeol& stsyllabel);
int InsertChar(CompletedEumjeol& stsyllabel, char c);
void DeleteEumjeol(CompletedEumjeol& stsyllabel, int status);
int IsKeyboardChar(char c);