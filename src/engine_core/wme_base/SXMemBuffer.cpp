// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SXMemBuffer.h"


IMPLEMENT_PERSISTENT(CSXMemBuffer, false);

//////////////////////////////////////////////////////////////////////////
CSXMemBuffer::CSXMemBuffer(CBGame* inGame, CScStack* Stack):CBScriptable(inGame)
{
	Stack->CorrectParams(1);
	m_Buffer = NULL;
	m_Size = 0;

	int NewSize = Stack->Pop()->GetInt();
	Resize(max(0, NewSize));
}

//////////////////////////////////////////////////////////////////////////
CSXMemBuffer::CSXMemBuffer(CBGame* inGame, void* Buffer):CBScriptable(inGame)
{
	m_Size = NULL;
	m_Buffer = Buffer;
}


//////////////////////////////////////////////////////////////////////////
CSXMemBuffer::~CSXMemBuffer()
{
	Cleanup();
}

//////////////////////////////////////////////////////////////////////////
void* CSXMemBuffer::ScToMemBuffer()
{
	return m_Buffer;
}

//////////////////////////////////////////////////////////////////////////
void CSXMemBuffer::Cleanup()
{
	if(m_Size) free(m_Buffer);
	m_Buffer = NULL;
	m_Size = 0;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CSXMemBuffer::Resize(int NewSize)
{
	int OldSize = m_Size;

	if(m_Size==0)
	{
		m_Buffer = malloc(NewSize);
		if(m_Buffer) m_Size = NewSize;
	}
	else
	{
		void* NewBuf = realloc(m_Buffer, NewSize);
		if(!NewBuf)
		{
			if(NewSize==0)
			{
				m_Buffer = NewBuf;
				m_Size = NewSize;
			}
			else return E_FAIL;
		}
		else
		{
			m_Buffer = NewBuf;
			m_Size = NewSize;
		}
	}

	if(m_Buffer && m_Size > OldSize)
	{
		memset((BYTE*)m_Buffer + OldSize, 0, m_Size - OldSize);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CSXMemBuffer::CheckBounds(CScScript* Script, int Start, int Length)
{
	if(m_Buffer==NULL)
	{
		Script->RuntimeError("Cannot use Set/Get methods on an uninitialized memory buffer");
		return false;
	}
	if(m_Size==0) return true;

	if(Start<0 || Length==0 || Start + Length > m_Size)
	{
		Script->RuntimeError("Set/Get method call is out of bounds");
		return false;
	}
	else return true;
}

//////////////////////////////////////////////////////////////////////////
char* CSXMemBuffer::ScToString()
{
	return "[membuffer object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXMemBuffer::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// SetSize
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SetSize")==0){
		Stack->CorrectParams(1);
		int NewSize = Stack->Pop()->GetInt();
		NewSize = max(0, NewSize);
		if(SUCCEEDED(Resize(NewSize))) Stack->PushBool(true);
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetBool
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetBool")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(bool))) Stack->PushNULL();
		else Stack->PushBool(*(bool*)((BYTE*)m_Buffer+Start));		

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetByte
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetByte")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(BYTE))) Stack->PushNULL();
		else Stack->PushInt(*(BYTE*)((BYTE*)m_Buffer+Start));

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetShort
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetShort")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(short))) Stack->PushNULL();
		else Stack->PushInt(65536 + *(short*)((BYTE*)m_Buffer+Start));		

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetInt / GetLong
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetInt")==0 || strcmp(Name, "GetLong")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(int))) Stack->PushNULL();
		else Stack->PushInt(*(int*)((BYTE*)m_Buffer+Start));		

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetFloat
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetFloat")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(float))) Stack->PushNULL();
		else Stack->PushFloat(*(float*)((BYTE*)m_Buffer+Start));		

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetDouble
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetDouble")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(double))) Stack->PushNULL();
		else Stack->PushFloat(*(double*)((BYTE*)m_Buffer+Start));

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetString")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		int Length = Stack->Pop()->GetInt();

		// find end of string
		if(Length==0 && Start >= 0 && Start < m_Size)
		{
			for(int i=Start; i<m_Size; i++)
			{
				if(((char*)m_Buffer)[i]=='\0')
				{
					Length = i - Start;
					break;
				}
			}
		}

		if(!CheckBounds(Script, Start, Length)) Stack->PushNULL();
		else
		{
			char* Str = new char[Length+1];
			strncpy(Str, (const char*)m_Buffer + Start, Length);
			Str[Length] = '\0';
			Stack->PushString(Str);
			delete [] Str;
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetPointer
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetPointer")==0){
		Stack->CorrectParams(1);
		int Start = Stack->Pop()->GetInt();
		if(!CheckBounds(Script, Start, sizeof(void*))) Stack->PushNULL();
		else
		{
			void* Pointer = *(void**)((BYTE*)m_Buffer+Start);
			CSXMemBuffer* Buf = new CSXMemBuffer(Game, Pointer);
			Stack->PushNative(Buf, false);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetBool
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetBool")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		bool Val = Stack->Pop()->GetBool();

		if(!CheckBounds(Script, Start, sizeof(bool))) Stack->PushBool(false);
		else
		{
			*(bool*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetByte
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetByte")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		BYTE Val = (BYTE)Stack->Pop()->GetInt();

		if(!CheckBounds(Script, Start, sizeof(BYTE))) Stack->PushBool(false);
		else
		{
			*(BYTE*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetShort
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetShort")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		short Val = (short)Stack->Pop()->GetInt();

		if(!CheckBounds(Script, Start, sizeof(short))) Stack->PushBool(false);
		else
		{
			*(short*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetInt / SetLong
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetInt")==0 || strcmp(Name, "SetLong")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		int Val = Stack->Pop()->GetInt();

		if(!CheckBounds(Script, Start, sizeof(int))) Stack->PushBool(false);
		else
		{
			*(int*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetFloat
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetFloat")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		float Val = (float)Stack->Pop()->GetFloat();

		if(!CheckBounds(Script, Start, sizeof(float))) Stack->PushBool(false);
		else
		{
			*(float*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetDouble
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetDouble")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		double Val = Stack->Pop()->GetFloat();

		if(!CheckBounds(Script, Start, sizeof(double))) Stack->PushBool(false);
		else
		{
			*(double*)((BYTE*)m_Buffer+Start) = Val;
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetString
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetString")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		char* Val = Stack->Pop()->GetString();

		if(!CheckBounds(Script, Start, strlen(Val)+1)) Stack->PushBool(false);
		else
		{
			memcpy((BYTE*)m_Buffer+Start, Val, strlen(Val)+1);
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetPointer
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetPointer")==0){
		Stack->CorrectParams(2);
		int Start = Stack->Pop()->GetInt();
		CScValue* Val = Stack->Pop();

		if(!CheckBounds(Script, Start, sizeof(void*))) Stack->PushBool(false);
		else
		{
			int Pointer = (int)Val->GetMemBuffer();
			memcpy((BYTE*)m_Buffer+Start, &Pointer, sizeof(void*));
			Stack->PushBool(true);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DEBUG_Dump
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DEBUG_Dump")==0){
		Stack->CorrectParams(0);
		if(m_Buffer && m_Size)
		{
			FILE* f = fopen("c:\\!!buffer.bin", "wb");
			fwrite(m_Buffer, m_Size, 1, f);
			fclose(f);
		}
		Stack->PushNULL();
		return S_OK;
	}

	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXMemBuffer::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type (RO)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("membuffer");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Size (RO)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Size")==0){
		m_ScValue->SetInt(m_Size);
		return m_ScValue;
	}

	else return CBScriptable::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXMemBuffer::ScSetProperty(char *Name, CScValue *Value)
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
	else*/ return CBScriptable::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXMemBuffer::Persist(CBPersistMgr* PersistMgr){

	CBScriptable::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Size));

	if(PersistMgr->m_Saving){
		if(m_Size>0) PersistMgr->PutBytes((BYTE*)m_Buffer, m_Size);
	}
	else{
		if(m_Size>0){
			m_Buffer = malloc(m_Size);
			PersistMgr->GetBytes((BYTE*)m_Buffer, m_Size);
		}
		else m_Buffer = NULL;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CSXMemBuffer::ScCompare(CBScriptable *Val)
{
	if(m_Buffer==Val->ScToMemBuffer()) return 0;
	else return 1;
}
