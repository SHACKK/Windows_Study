#include "pch.h"
#include "Message.h"

void CMessage::InsertMessage(std::wstring strUserId, std::wstring strRecvMsg)
{
	std::wstring strChatData = strUserId + L" : " + strRecvMsg;
	v_ChatData.push_back(strChatData);
}

std::vector<std::wstring> CMessage::GetMessage()
{
	return v_ChatData;
}

void CMessage::MsgBroadcast(CSocketServer server)
{
	
}
