#include "stdafx.h"
#include "CStringBuilder.h"
#include "KeyInput.h"

bool GetKeyInput(int &nVitrualKey)
{ 
	
};

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