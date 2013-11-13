// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"


class CBNetworkMgr : public CBBase
{
public:
	CBNetworkMgr(CBGame* inGame);
	virtual ~CBNetworkMgr(void);
	HRESULT OnWinsockEvent(WPARAM wparam, LPARAM lparam);
	HRESULT InitLoop();
	HRESULT AddSocket(CBSocket* Socket);

	CBSocket* Connect(char* HostAddress, int HostPort);	
	CBSocket* CreateServer(int ServerPort);

private:
	CBSocket* GetSocket(SOCKET s);
	CBArray<CBSocket*, CBSocket*> m_Sockets;
	bool m_Initialized;
	HRESULT InitWinSock();
	HRESULT ShutdownWinSock();
};
