#include "stdafx.h"
#include "CStringBuilder.h"

bool GetKeyInput(int& nVirtualKey)
{
	int c = GetKeyState(nVirtualKey);
	return true;
}

int main()
{
	CStringBuilder builder;

	while (true)
	{
		int nVirtualKey = 0;
		bool bPressed = GetKeyInput(nVirtualKey);
		if (!bPressed)
			continue;
		builder.Input(nVirtualKey);
	}
	return 0;
}