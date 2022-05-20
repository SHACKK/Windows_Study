#pragma once
class CPacketHandlerSuper
{
protected:
	std::vector<CPacketHandlerSuper*> m_vecHandler;
public:
	virtual DWORD GetType() = 0;
	virtual void OnRecv(LPVOID pData, size_t tDataSize) = 0;
};