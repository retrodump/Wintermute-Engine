// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BNetworkMgr.h"


//////////////////////////////////////////////////////////////////////////
CBNetworkMgr::CBNetworkMgr(CBGame* inGame):CBBase(inGame)
{
	m_Initialized = false;
}


//////////////////////////////////////////////////////////////////////////
CBNetworkMgr::~CBNetworkMgr(void)
{
	for(int i=0; i<m_Sockets.GetSize(); i++)
	{
		delete m_Sockets[i];
	}
	m_Sockets.RemoveAll();


	ShutdownWinSock();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBNetworkMgr::InitLoop()
{
	for(int i=0; i<m_Sockets.GetSize(); i++)
	{
		m_Sockets[i]->ProcessPendingData();
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBNetworkMgr::InitWinSock()
{
	if(m_Initialized) return S_OK;

	WSADATA WsaData;
	int Res = WSAStartup(MAKEWORD(2,2), &WsaData);
	if(Res != NO_ERROR)
	{
		Game->LOG(0, "Error initializing WinSock2 library");
		m_Initialized = false;
		return E_FAIL;
	}
	else
	{
		m_Initialized = true;
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBNetworkMgr::ShutdownWinSock()
{
	if(m_Initialized)
	{
		WSACleanup();
		m_Initialized = false;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBNetworkMgr::AddSocket(CBSocket* Socket)
{
	m_Sockets.Add(Socket);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBNetworkMgr::OnWinsockEvent(WPARAM wparam, LPARAM lparam)
{
	int Event  = WSAGETSELECTEVENT(lparam);
	int ErrMsg = WSAGETSELECTERROR(lparam);
	SOCKET Socket = (SOCKET)wparam;

	switch(Event)
	{
		case FD_CONNECT:
			{
				Game->LOG(0, "FD_CONNECT %d", Socket);
				CBSocket* S = GetSocket(Socket);
				if(S) S->OnConnectEvent();
			}
			break;
		case FD_ACCEPT:  // New client connection
			{
				Game->LOG(0, "FD_ACCEPT %d", Socket);
				CBSocket* S = GetSocket(Socket);
				if(S) S->OnAcceptEvent();
			}
			break;
		case FD_OOB:  // Out of band data has arrived
			Game->LOG(0, "FD_OOB %d", Socket);
			break;
		case FD_READ:  // Incomming data to be read
			{
				Game->LOG(0, "FD_READ %d", Socket);
				CBSocket* S = GetSocket(Socket);
				if(S) S->OnReadEvent();
			}
			break;
		case FD_WRITE: // Permission granted to send data
			Game->LOG(0, "FD_WRITE %d", Socket);
			break;
		case FD_CLOSE:  // Disconnecting
			{
				Game->LOG(0, "FD_CLOSE %d", Socket);
				CBSocket* S = GetSocket(Socket);
				if(S) S->OnReadEvent();
			}
			break;
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
CBSocket* CBNetworkMgr::GetSocket(SOCKET s)
{
	for(int i=0; i<m_Sockets.GetSize(); i++)
	{
		if(m_Sockets[i]->GetSocket()==s) return m_Sockets[i];
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
CBSocket* CBNetworkMgr::Connect(char* HostAddress, int HostPort)
{
	if(FAILED(InitWinSock())) return NULL;

	CBSocket* Socket = new CBSocket(Game);
	if(!Socket) return NULL;

	HRESULT Ret = Socket->Connect(HostAddress, HostPort);
	if(FAILED(Ret))
	{
		SAFE_DELETE(Socket);
		return NULL;
	}
	m_Sockets.Add(Socket);

	return Socket;
}


//////////////////////////////////////////////////////////////////////////
CBSocket* CBNetworkMgr::CreateServer(int ServerPort)
{
	if(FAILED(InitWinSock())) return NULL;

	CBSocket* Socket = new CBSocket(Game);
	if(!Socket) return NULL;

	HRESULT Ret = Socket->Bind(ServerPort);
	if(FAILED(Ret))
	{
		SAFE_DELETE(Socket);
		return NULL;
	}
	m_Sockets.Add(Socket);

	return Socket;

}