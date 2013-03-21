// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "SXString.h"
#include "SXArray.h"



IMPLEMENT_PERSISTENT(CSXString, false);

//////////////////////////////////////////////////////////////////////////
CSXString::CSXString(CBGame* inGame, CScStack* Stack):CBScriptable(inGame)
{
	m_String = NULL;
	m_Capacity = 0;
	
	Stack->CorrectParams(1);
	CScValue* Val = Stack->Pop();

	if(Val->IsInt()){
		m_Capacity = max(0, Val->GetInt());
		if(m_Capacity>0){
			m_String = new char[m_Capacity];
			memset(m_String, 0, m_Capacity);
		}
	}
	else{
		SetStringVal(Val->GetString());
	}

	if(m_Capacity==0) SetStringVal("");
}


//////////////////////////////////////////////////////////////////////////
CSXString::~CSXString()
{
	if(m_String) delete [] m_String;
}


//////////////////////////////////////////////////////////////////////////
void CSXString::SetStringVal(char *Val)
{
	int Len = strlen(Val);
	if(Len>=m_Capacity){
		m_Capacity = Len + 1;
		SAFE_DELETE_ARRAY(m_String);
		m_String = new char[m_Capacity];
		memset(m_String, 0, m_Capacity);
	}
	strcpy(m_String, Val);
}


//////////////////////////////////////////////////////////////////////////
char* CSXString::ScToString()
{
	if(m_String) return m_String;
	else return "[null string]";
}


//////////////////////////////////////////////////////////////////////////
void CSXString::ScSetString(char *Val)
{
	SetStringVal(Val);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXString::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// Substring
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Substring")==0){
		Stack->CorrectParams(2);
		int start = Stack->Pop()->GetInt();
		int end   = Stack->Pop()->GetInt();

		if(end < start) CBUtils::Swap(&start, &end);

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			start = max(0, min(start, strlen(m_String)));
			end   = max(0, min(end,   strlen(m_String)));
		
			char* dummy = new char[max(0, end-start+1)+1];
			dummy[max(0, end-start+1)] = '\0';
			strncpy(dummy, m_String+start, max(0, end-start+1));
			Stack->PushString(dummy);
			delete [] dummy;
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				start = max(0, min(start, wcslen(Temp)));
				end   = max(0, min(end,   wcslen(Temp)));

				wchar_t* dummy = new wchar_t[max(0, end-start+1)+1];
				dummy[max(0, end-start+1)] = '\0';
				wcsncpy(dummy, Temp+start, max(0, end-start+1));

				char* Temp2 = CBTextUtils::UnicodeToUtf8(dummy);
				if(Temp2)
				{
					Stack->PushString(Temp2);
					delete [] Temp2;
				}
				else Stack->PushNULL();

				delete [] dummy;
				delete [] Temp;
			}
			else Stack->PushNULL();			
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Substr
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Substr")==0){
		Stack->CorrectParams(2);
		int start = Stack->Pop()->GetInt();

		CScValue* val = Stack->Pop();
		int len = val->GetInt();

		if(!val->IsNULL() && len <= 0){
			Stack->PushString("");
			return S_OK;
		}

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			start = max(0, min(start, strlen(m_String)));
			int end;

			if(val->IsNULL()) end = strlen(m_String) /*- start*/;
			else {
				end = start + len - 1;
				end = max(0, min(end,   strlen(m_String)));
			}
		
			char* dummy = new char[max(0, end-start+1)+1];
			dummy[max(0, end-start+1)] = '\0';
			strncpy(dummy, m_String+start, max(0, end-start+1));
			Stack->PushString(dummy);
			delete [] dummy;
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				start = max(0, min(start, wcslen(Temp)));
				int end;

				if(val->IsNULL()) end = wcslen(Temp) /*- start*/;
				else {
					end = start + len - 1;
					end = max(0, min(end, wcslen(Temp)));
				}

				wchar_t* dummy = new wchar_t[max(0, end-start+1)+1];
				dummy[max(0, end-start+1)] = '\0';
				wcsncpy(dummy, Temp+start, max(0, end-start+1));

				char* Temp2 = CBTextUtils::UnicodeToUtf8(dummy);
				if(Temp2)
				{
					Stack->PushString(Temp2);
					delete [] Temp2;
				}
				else Stack->PushNULL();

				delete [] dummy;
				delete [] Temp;
			}
			else Stack->PushNULL();
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ToUpperCase
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToUpperCase")==0){
		Stack->CorrectParams(0);

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			char* dummy = new char[strlen(m_String)+1];
			strcpy(dummy, m_String);
			_strupr(dummy);
			Stack->PushString(dummy);
			delete [] dummy;
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				setlocale(LC_ALL, "");
				wchar_t* dummy = new wchar_t[wcslen(Temp)+1];
				wcscpy(dummy, Temp);
				_wcsupr(dummy);

				char* Temp2 = CBTextUtils::UnicodeToUtf8(dummy);
				if(Temp2)
				{
					Stack->PushString(Temp2);
					delete [] Temp2;
				}
				else Stack->PushNULL();

				delete [] dummy;
				delete [] Temp;
				setlocale(LC_ALL, "English");
			}
			else Stack->PushNULL();
		}

		return S_OK;
	}
	
	//////////////////////////////////////////////////////////////////////////
	// ToLowerCase
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ToLowerCase")==0){
		Stack->CorrectParams(0);

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			char* dummy = new char[strlen(m_String)+1];
			strcpy(dummy, m_String);
			_strlwr(dummy);
			Stack->PushString(dummy);
			delete [] dummy;
		}
		else
		{
			setlocale(LC_ALL, "");
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				wchar_t* dummy = new wchar_t[wcslen(Temp)+1];
				wcscpy(dummy, Temp);
				_wcslwr(dummy);

				char* Temp2 = CBTextUtils::UnicodeToUtf8(dummy);
				if(Temp2)
				{
					Stack->PushString(Temp2);
					delete [] Temp2;
				}
				else Stack->PushNULL();

				delete [] dummy;
				delete [] Temp;
			}
			else Stack->PushNULL();
			setlocale(LC_ALL, "English");
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IndexOf
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IndexOf")==0){
		Stack->CorrectParams(2);

		char* str = Stack->Pop()->GetString();
		int index = Stack->Pop()->GetInt();

		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			index = max(0, min(index,   strlen(m_String)));

			char* result = strstr(m_String+index, str);
			if(!result) Stack->PushInt(-1);
			else Stack->PushInt(result - m_String);
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				wchar_t* Temp2 = CBTextUtils::Utf8ToUnicode(str);
				if(Temp2)
				{
					index = max(0, min(index, wcslen(Temp)));

					wchar_t* result = wcsstr(Temp+index, Temp2);
					if(!result) Stack->PushInt(-1);
					else Stack->PushInt(result - Temp);
					
					delete [] Temp2;
				}
				else Stack->PushInt(-1);

				delete [] Temp;
			}
			else Stack->PushInt(-1);
		}

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Split
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Split")==0){
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();
		char Separators[MAX_PATH] = ",";
		if(!Val->IsNULL()) strcpy(Separators, Val->GetString());

		CSXArray* Array = new CSXArray(Game);
		if(!Array)
		{
			Stack->PushNULL();
			return S_OK;
		}
		
		if(Game->m_TextEncoding==TEXT_ANSI)
		{
			int Start = 0;
			for(int i=0; i<strlen(m_String)+1; i++)
			{
				char ch = m_String[i];
				if(ch=='/0' || strchr(Separators, ch))
				{
					char* Part = new char[i - Start + 1];
					if(i!=Start)
					{
						strncpy(Part, m_String + Start, i - Start);
						Part[i - Start] = '\0';
					}
					else Part[0] = '\0';

					Val = new CScValue(Game, Part);
					Array->Push(Val);
					delete [] Part;
					SAFE_DELETE(Val);
					Start = i + 1;
				}
			}
		}
		else
		{
			wchar_t* Temp = CBTextUtils::Utf8ToUnicode(m_String);
			if(Temp)
			{
				int Start = 0;
				for(int i=0; i<wcslen(Temp)+1; i++)
				{
					wchar_t ch = Temp[i];
					if(ch=='/0' || strchr(Separators, ch))
					{
						wchar_t* Part = new wchar_t[i - Start + 1];
						if(i!=Start)
						{
							wcsncpy(Part, Temp + Start, i - Start);
							Part[i - Start] = '\0';
						}
						else Part[0] = '\0';

						char* Temp2 = CBTextUtils::UnicodeToUtf8(Part);
						if(Temp2)
						{
							Val = new CScValue(Game, Temp2);
							Array->Push(Val);
							SAFE_DELETE(Val);
							delete [] Temp2;
						}

						delete [] Part;
						Start = i + 1;
					}
				}

				delete [] Temp;
			}
		}
		
		Stack->PushNative(Array, false);
		return S_OK;
	}

	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
CScValue* CSXString::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type (RO)
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("string");
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Length (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Length")==0){
		m_ScValue->SetInt(CBTextUtils::GetTextLength(Game->m_TextEncoding, (BYTE*)m_String));
		return m_ScValue;
	}
	//////////////////////////////////////////////////////////////////////////
	// Capacity
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Capacity")==0){
		m_ScValue->SetInt(m_Capacity);
		return m_ScValue;
	}

	else return m_ScValue;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXString::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// Capacity
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Capacity")==0){
		int NewCap = Value->GetInt();
		if(NewCap < strlen(m_String)+1) Game->LOG(0, "Warning: cannot lower string capacity");
		else if(NewCap != m_Capacity){
			char* NewStr = new char[NewCap];
			if(NewStr){
				memset(NewStr, 0, NewCap);
				strcpy(NewStr, m_String);
				SAFE_DELETE_ARRAY(m_String);
				m_String = NewStr;
				m_Capacity = NewCap;
			}
		}
		return S_OK;
	}

	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CSXString::Persist(CBPersistMgr* PersistMgr){

	CBScriptable::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Capacity));

	if(PersistMgr->m_Saving){
		if(m_Capacity>0) PersistMgr->PutBytes((BYTE*)m_String, m_Capacity);
	}
	else{
		if(m_Capacity>0){
			m_String = new char[m_Capacity];
			PersistMgr->GetBytes((BYTE*)m_String, m_Capacity);
		}
		else m_String = NULL;
	}
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CSXString::ScCompare(CBScriptable *Val)
{
	return strcmp(m_String, ((CSXString*)Val)->m_String);
}
