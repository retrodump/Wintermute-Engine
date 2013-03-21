// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "BScriptable.h"

class CSXFile : public CBScriptable  
{
public:
	DECLARE_PERSISTENT(CSXFile, CBScriptable);
	CScValue* ScGetProperty(char* Name);
	HRESULT ScSetProperty(char *Name, CScValue *Value);
	HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	char* ScToString();
	CSXFile(CBGame* inGame, CScStack* Stack);
	virtual ~CSXFile();
private:
	CBFile* m_ReadFile;
	FILE* m_WriteFile;
	int m_Mode; // 0..none, 1..read, 2..write, 3..append
	bool m_TextMode;
	void Close();
	void Cleanup();
	DWORD GetPos();
	DWORD GetLength();
	bool SetPos(DWORD Pos, TSeek Origin=SEEK_TO_BEGIN);
	char* m_Filename;
};
