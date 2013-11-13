// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"

class CBSocket : public CBScriptable
{
public:
	CBSocket(CBGame* inGame, SOCKET Socket=INVALID_SOCKET);
	virtual ~CBSocket(void);

	HRESULT Connect(char* HostAddress, int HostPort);
	HRESULT Close();
	SOCKET GetSocket(){ return m_Socket; };
	HRESULT Send(BYTE* Buffer, int Size);
	HRESULT OnReadEvent();
	HRESULT OnAcceptEvent();
	HRESULT OnConnectEvent();
	HRESULT ProcessPendingData();

	HRESULT Bind(int ServerPort);
	HRESULT Listen();

private:
	CBDynBuffer* m_ReadDataBuf;
	HRESULT GoAsync();
	SOCKET m_Socket;
	char* m_HostAddress;
	int m_HostPort;
	int m_ServerPort;
	int m_MaxConnections;
	bool m_Connected;
	int m_DataPending;
};
