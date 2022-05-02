#pragma once
class CMessage
{
private:
	std::vector<std::wstring> v_ChatData;
public:
	void InsertMessage(std::wstring strUserId, std::wstring strRecvMsg);
	std::vector<std::wstring> GetMessgae();
};

