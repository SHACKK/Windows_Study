#pragma once
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
};

