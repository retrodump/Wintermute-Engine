// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BScriptable.h"
#include <winsock2.h>

class CSXTcpClient : public CBScriptable  
{
public:
	bool m_Initialized;
	HRESULT InitWinSock();
	HRESULT ShutdownWinSock();
	SOCKET m_Socket;
	SOCKET InitSocket();
	HRESULT ConnectToSocket(char* Address, int Port);

	virtual int ScCompare(CBScriptable* Val);
	DECLARE_PERSISTENT(CSXTcpClient, CBScriptable);
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	char* ScToString();
	CSXTcpClient(CBGame* inGame, CScStack* Stack);
	virtual ~CSXTcpClient();
};
