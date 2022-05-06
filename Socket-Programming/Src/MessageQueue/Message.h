#pragma once
#include "../Socket_Server/Socket_Server.h"

class CMessage
{
private:
	std::vector<std::wstring> v_ChatData = {
	{L"임창현 : Chatting Test"},
	{L"이준성 : Hello Chat World"}
	};
public:
	void InsertMessage(std::wstring strUserId, std::wstring strRecvMsg);
	std::vector<std::wstring> GetMessgae();

	void MsgBroadcast(CSocketServer server);
};

