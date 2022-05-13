#include "pch.h"
#include "ConnectionSuper.h"
#include "Server.h"

void CConnectionSuper::ConnectionThread()
{
}

int CConnectionSuper::Establish(SOCKET acceptedSocket, CServer* pServer)
{
	return 0;
}

int CConnectionSuper::Send(const LPBYTE pBuffer, size_t BufferSize)
{
	return 0;
}

int CConnectionSuper::Recv(LPBYTE pBuffer, size_t BufferSize)
{
	return 0;
}
