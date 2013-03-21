// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SXTcpClient.h"



IMPLEMENT_PERSISTENT(CSXTcpClient, false);

//////////////////////////////////////////////////////////////////////////
CSXTcpClient::CSXTcpClient(CBGame* inGame, CScStack* Stack):CBScriptable(inGame)
{
	m_Initialized = false;
	m_Socket = NULL;

	InitWinSock();

	Stack->CorrectParams(1);
	CScValue* Val = Stack->Pop();
}


//////////////////////////////////////////////////////////////////////////
CSXTcpClient::~CSXTcpClient()
{
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::InitWinSock()
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
		m_Socket = InitSocket();
		
		if(m_Socket) return S_OK;
		else return E_FAIL;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::ShutdownWinSock()
{
	WSACleanup();
	m_Initialized = false;
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
SOCKET CSXTcpClient::InitSocket()
{
	SOCKET Ret = NULL;
	Ret = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(Ret == INVALID_SOCKET)
	{
		Game->LOG(0, "Error creating socket: %ld\n", WSAGetLastError());
		ShutdownWinSock();
		return NULL;
	}
	else return Ret;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::ConnectToSocket(char* Address, int Port)
{
	u_long addr = inet_addr(Address);
	if (addr == INADDR_NONE)
	{
		hostent* HE = gethostbyname(Address);
		if (HE == 0)
		{
			Game->LOG(0, "Error resolving host address '%s'", Address);
			ShutdownWinSock();
			return E_FAIL;
		}
		addr = *((u_long*)HE->h_addr_list[0]);
	}

	sockaddr_in ClientService;

	ClientService.sin_family = AF_INET;
	ClientService.sin_addr.s_addr = addr;
	ClientService.sin_port = htons(Port);

	if (connect(m_Socket, (SOCKADDR*)&ClientService, sizeof(ClientService) ) == SOCKET_ERROR)
	{
		Game->LOG(0, "Error connectiong to '%s' on port %d", Address, Port);
		ShutdownWinSock();
		return E_FAIL;
	}
	else return S_OK;

}

//////////////////////////////////////////////////////////////////////////
char* CSXTcpClient::ScToString()
{
	return "[tcp-client object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	/*
	//////////////////////////////////////////////////////////////////////////
	// Substring
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Substring")==0){
	Stack->CorrectParams(2);
	int start = Stack->Pop()->GetInt();
	int end   = Stack->Pop()->GetInt();

	if(end < start) Swap(&start, &end);

	start = max(0, min(start, strlen(m_String)));
	end   = max(0, min(end,   strlen(m_String)));

	char* dummy = new char[max(0, end-start+1)+1];
	dummy[max(0, end-start+1)] = '\0';
	strncpy(dummy, m_String+start, max(0, end-start+1));
	Stack->PushString(dummy);
	delete [] dummy;

	return S_OK;
	}

	else*/ return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXTcpClient::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type (RO)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("tcp-client");
		return m_ScValue;
	}

	else return CBScriptable::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::ScSetProperty(char *Name, CScValue *Value)
{
	/*
	//////////////////////////////////////////////////////////////////////////
	// Length
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Length")==0){
	int OrigLength = m_Length;
	m_Length = max(Value->GetInt(0), 0);

	char PropName[20];
	if(m_Length < OrigLength){
	for(int i=m_Length; i<OrigLength; i++){
	sprintf(PropName, "%d", i);
	m_Values->DeleteProp(PropName);
	}
	}
	return S_OK;
	}
	else*/ return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXTcpClient::Persist(CBPersistMgr* PersistMgr){

	CBScriptable::Persist(PersistMgr);

	//PersistMgr->Transfer(TMEMBER(m_Capacity));

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CSXTcpClient::ScCompare(CBScriptable *Val)
{
	//return strcmp(m_String, ((CSXTcpClient*)Val)->m_String);
	return 0;
}
