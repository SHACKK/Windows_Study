#include "stdafx.h"
#include "CStringBuilder.h"
#include "KeyInput.h"

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