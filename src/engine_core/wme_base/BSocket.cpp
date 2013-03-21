// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\BSocket.h"


//////////////////////////////////////////////////////////////////////////
CBSocket::CBSocket(CBGame* inGame, SOCKET Socket) : CBScriptable(inGame)
{
	m_Socket = Socket;

	m_HostAddress = NULL;
	m_HostPort = 0;
	m_ServerPort = 0;
	
	m_ReadDataBuf = new CBDynBuffer(Game, 1024, 512);

	m_MaxConnections = 10;
	m_Connected = false;
	m_DataPending = 0;
}

//////////////////////////////////////////////////////////////////////////
CBSocket::~CBSocket(void)
{
	CSysClassRegistry::GetInstance()->EnumInstances(CBGame::InvalidateValues, "CScValue", (void*)this);

	Close();
	SAFE_DELETE(m_ReadDataBuf);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::Close()
{
	if(m_Socket!=INVALID_SOCKET)
	{
		shutdown(m_Socket, SD_BOTH);
		closesocket(m_Socket);
	}
	m_Socket = INVALID_SOCKET;

	SAFE_DELETE_ARRAY(m_HostAddress);
	m_HostPort = 0;
	m_Connected = 0;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::GoAsync()
{
	if(m_Socket==INVALID_SOCKET) return E_FAIL;

	int Ret = WSAAsyncSelect(m_Socket, Game->m_Renderer->m_Window, WM_WINSOCK, (FD_CLOSE | FD_CONNECT | FD_READ | FD_ACCEPT));
	if(!Ret) return S_OK;
	else
	{
		Game->LOG(0, "WSAAsyncSelect failed: %ld", WSAGetLastError());
		return E_FAIL;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::Connect(char* HostAddress, int HostPort)
{
	Close();

	// create socket
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_Socket == INVALID_SOCKET)
	{
		Game->LOG(0, "Error creating socket: %ld", WSAGetLastError());
		return E_FAIL;
	}

	// resolve host address
	u_long addr = inet_addr(HostAddress);
	if (addr == INADDR_NONE)
	{
		hostent* HE = gethostbyname(HostAddress);
		if (HE == 0)
		{
			Game->LOG(0, "Error resolving host address '%s': %ld", HostAddress, WSAGetLastError());
			Close();
			return E_FAIL;
		}
		addr = *((u_long*)HE->h_addr_list[0]);
	}

	HRESULT Ret = GoAsync();
	if(FAILED(Ret))
	{
		Close();
		return Ret;
	}

	// connect to host
	sockaddr_in ClientService;
	ClientService.sin_family = AF_INET;
	ClientService.sin_addr.s_addr = addr;
	ClientService.sin_port = htons(HostPort);

	if(connect(m_Socket, (SOCKADDR*)&ClientService, sizeof(ClientService) ) == SOCKET_ERROR)
	{
		if(WSAGetLastError() != WSAEWOULDBLOCK)
		{
			Game->LOG(0, "Error connecting to '%s' on port %d: %ld", HostAddress, HostPort, WSAGetLastError());
			Close();
			return E_FAIL;
		}
	}

	CBUtils::SetString(&m_HostAddress, HostAddress);
	m_HostPort = HostPort;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::Send(BYTE* Buffer, int Size)
{
	if(m_Socket == INVALID_SOCKET) return E_FAIL;

	if(send(m_Socket, (const char*)Buffer, Size, 0)==SOCKET_ERROR)
	{
		Game->LOG(0, "Error sending data: %ld", WSAGetLastError());
		return E_FAIL;
	}
	else return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::OnReadEvent()
{
	m_DataPending++;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::ProcessPendingData()
{
#define BUFFER_SIZE 100
	// cache loaded data and raise DataArrival event

	if(!m_DataPending) return S_OK;

	if(m_Socket == INVALID_SOCKET) return E_FAIL;
	BYTE Buf[BUFFER_SIZE];
	
	int BytesReceived = recv(m_Socket, (char*)Buf, BUFFER_SIZE, 0);
	if(BytesReceived==0)
	{
		// closed
		Game->LOG(0, "Socket closed");
		m_Connected = false;
		m_DataPending = 0;
		return S_OK;
	}
	else if(BytesReceived==SOCKET_ERROR)
	{
		Game->LOG(0, "Error reading data: %ld", WSAGetLastError());
		m_DataPending = 0;
		return E_FAIL;
	}
	else
	{
		Game->LOG(0, "Received %d bytes", BytesReceived);
		m_ReadDataBuf->PutBytes(Buf, BytesReceived);
		m_DataPending--;
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::OnAcceptEvent()
{
	SOCKET AcceptSocket = accept(m_Socket, NULL, NULL);

	if(AcceptSocket == INVALID_SOCKET || AcceptSocket == SOCKET_ERROR)
	{
		Game->LOG(0, "Error Accepting connection: %ld", WSAGetLastError());
		return E_FAIL;
	}
	else
	{
		Game->LOG(0, "Accepted: %d", AcceptSocket);
		CBSocket* Connection = new CBSocket(Game, AcceptSocket);
		Connection->m_Connected = true;
		Game->m_NetworkMgr->AddSocket(Connection);

		return Connection->GoAsync();
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::OnConnectEvent()
{
	m_Connected = true;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::Bind(int ServerPort)
{
	Close();

	// create socket
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(m_Socket == INVALID_SOCKET)
	{
		Game->LOG(0, "Error creating socket: %ld", WSAGetLastError());
		return E_FAIL;
	}

	sockaddr_in ServerService;
	ServerService.sin_family = AF_INET;
	ServerService.sin_addr.s_addr = INADDR_ANY;
	ServerService.sin_port = htons(ServerPort);

	if(bind(m_Socket, (SOCKADDR*)&ServerService, sizeof(ServerService) ) == SOCKET_ERROR)
	{
		Game->LOG(0, "Error binding to port %d: %ld", ServerPort, WSAGetLastError());
		Close();
		return E_FAIL;
	}
	else
	{
		HRESULT Ret = GoAsync();
		if(FAILED(Ret)) Close();
		else
		{
			m_ServerPort = ServerPort;
		}
		return Ret;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSocket::Listen()
{
	if (listen(m_Socket, m_MaxConnections) == SOCKET_ERROR)
	{
		Game->LOG(0, "Error listening on socket: %ld", WSAGetLastError());
		return E_FAIL;
	}
	else return S_OK;
}