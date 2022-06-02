#pragma once
#include "Connection.h"

bool Client_SendTest(ST_TEST_PARAM stTestParam);
bool Client_RecvTest(ST_TEST_PARAM stTestParam);
bool Connection_SendTest(ST_TEST_PARAM stTestParam);
bool Connection_RecvTest(ST_TEST_PARAM stTestParam);
bool Connection_SendChatDataTest(ST_TEST_PARAM stTestParam);
bool Connection_GetCurrentTimeStringTest(ST_TEST_PARAM stTestParam);
bool Client_RecvChatDataTest(ST_TEST_PARAM stTestParam);
bool Client_setUserNameTest(ST_TEST_PARAM stTestParam);
bool Server_BroadcastTest(ST_TEST_PARAM stTestParam);
bool Server_UpdateChatDataTest(ST_TEST_PARAM stTestParam);
