// TemplateMgr.cpp: implementation of the CTemplateMgr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TemplateMgr.h"
#include "utils_mfc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////
CTemplateMgr::CTemplateMgr()
{
	ResetParser();

	m_Description = "";
	m_NoDir = false;
}


//////////////////////////////////////////////////////////////////////
CTemplateMgr::~CTemplateMgr()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::ReadExtendedInfo(CStringArray& InArray)
{
	CString Params;
	
	m_Description = "";
	m_NoDir = false;
	Cleanup();

	
	m_CurrLine = 0;
	while(m_CurrLine<InArray.GetSize()){
		CString Line = InArray[m_CurrLine];

		CString Token = "";
		for(int i=0; i<Line.GetLength(); i++){
			if(Line[i]==' ' || Line[i]=='\t' || Line[i]=='(') break;
			else Token += Line[i];
		}

		// $$PROP
		if(Token=="$$PROP"){
			Params = Line.Right(Line.GetLength() - 6);
			Params.TrimLeft(" \t");

			CTemplateProp* prop = new CTemplateProp;
			prop->m_Name     = EntryEx(1, Params);
			prop->m_Caption  = EntryEx(2, Params);
			prop->m_Value    = EntryEx(3, Params);
			CString Choices  = EntryEx(4, Params);
			prop->m_Type     = EntryEx(5, Params);
			prop->m_MinValue = atoi(EntryEx(6, Params));
			prop->m_MaxValue = atoi(EntryEx(7, Params));
			if(Choices!=""){
				for(int i=1; i<=NumEntries(Choices, '|'); i++) prop->m_Choices.Add(Entry(i, Choices, '|'));
			}
			prop->m_Validate = (prop->m_MinValue!=0 || prop->m_MaxValue!=0);
			prop->m_Type.MakeLower();
			if(prop->m_Type!="string" && prop->m_Type!="unsigned" && prop->m_Type!="signed")
				prop->m_Type = "string";

			if(prop->m_Name==""){
				delete prop;
				continue;
			}

			if(prop->m_Caption=="") prop->m_Caption = prop->m_Name;

			for(int i=0; i<m_Props.GetSize(); i++){
				if(prop->m_Name == m_Props[i]->m_Name){
					delete m_Props[i];
					m_Props.RemoveAt(i);
				}
			}
			m_Props.Add(prop);
			m_Dict[prop->m_Name] = prop->m_Value;

			m_CurrLine++;
		}

		// $$DESC
		else if(Token=="$$DESC"){
			Params = Line.Right(Line.GetLength() - 6);
			Params.TrimLeft(" \t");

			if(m_Description!="") m_Description+="\n";
			m_Description += Params;
			m_CurrLine++;
		}

		// $$NODIR
		else if(Token=="$$NODIR"){
			m_NoDir = true;
			m_CurrLine++;
		}

		// normal line
		else{
			m_CurrLine++;
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::ParseTemplate(CStringArray& InArray, CStringArray& OutArray)
{	
	OutArray.RemoveAll();

	ResetParser();

	bool InLoop = false;
	CString macros="";

	m_CurrLine = 0;
	while(m_CurrLine<InArray.GetSize()){
		CString Line = InArray[m_CurrLine];

		CString Token = "";
		for(int i=0; i<Line.GetLength(); i++){
			if(Line[i]==' ' || Line[i]=='\t' || Line[i]=='(') break;
			else Token += Line[i];
		}
		

		// comment / definitions
		if(Line.Find("$$//")==0 || Token=="$$PROP" || Token=="$$DESC" || Token=="$$MERGE" || Token=="$$NODIR"){
			m_CurrLine++;
			continue;
		}
		// $$IF
		else if(Token=="$$IF"){
			m_IfNest++;

			if(m_IfNest>=MAX_IF_NEST){
				AddError("$$IF statements nested too deeply", m_CurrLine);
				return false;
			}

			// get macros			
			if(!GetMacroList(Line, macros, 4)){
				AddError("Syntax error in $$IF statement", m_CurrLine);
				return false;
			}

			// evaluate macros
			m_IfIsTrue[m_IfNest] = EvaluateMacroList(macros);
			if(m_IfIsTrue[m_IfNest]) m_IfProcessed[m_IfNest] = true;
			m_CurrLine++;
		}
		// $$ELIF
		else if(Token=="$$ELIF"){
			if(m_IfNest<0){
				AddError("$$ELIF statement without matching $$IF", m_CurrLine);
				return false;
			}

			if(m_IfHasElse[m_IfNest]){
				AddError("$$ELIF statement must precede any $$ELSE statements", m_CurrLine);
				return false;
			}

			// already processed by if
			if(m_IfProcessed[m_IfNest]==true){
				m_IfIsTrue[m_IfNest] = false;
			}
			else{
				// get macros			
				if(!GetMacroList(Line, macros, 6)){
					AddError("Syntax error in $$ELIF statement", m_CurrLine);
					return false;
				}
				m_IfIsTrue[m_IfNest] = EvaluateMacroList(macros);
				if(m_IfIsTrue[m_IfNest]) m_IfProcessed[m_IfNest] = true;
			}
			m_CurrLine++;
		}
		// $$ELSE
		else if(Token=="$$ELSE"){
			if(m_IfNest<0){
				AddError("$$ELSE statement without matching $$IF", m_CurrLine);
				return false;
			}

			if(m_IfHasElse[m_IfNest]){
				AddError("Multiple $$ELSE statements", m_CurrLine);
				return false;
			}

			m_IfIsTrue[m_IfNest] = !m_IfProcessed[m_IfNest];
			m_IfHasElse[m_IfNest] = true;
			m_CurrLine++;
		}
		// $$ENDIF
		else if(Token=="$$ENDIF"){
			if(m_IfNest<0){
				AddError("$$ENDIF statement without matching $$IF", m_CurrLine);
				return false;
			}
			m_IfNest--;
			m_CurrLine++;
		}
		// normal line
		else{
			bool IsTrue=false;
			if(m_IfNest<0) IsTrue = true;
			else{
				IsTrue = m_IfIsTrue[m_IfNest];
				for(int i=m_IfNest-1; i>=0; i--) IsTrue = (IsTrue && m_IfIsTrue[i]);
			}

			if(IsTrue){
				CString ProcessedLine;
				if(!ExpandMacros(Line, ProcessedLine)){
					AddError("Error expanding macros", m_CurrLine);
					return false;
				}
				OutArray.Add(ProcessedLine);
			}
			m_CurrLine++;
		}
	}
	
	if(m_IfNest>=0){
		AddError("Unterminated $$IF statement", m_CurrLine);
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::GetMacroList(CString Line, CString &Result, int Starting)
{
	Result = "";

	CString Token = "";

	bool InParenthesis = false;
	bool ListFound = false;
	for(int i=Starting; i<Line.GetLength(); i++){
		if(InParenthesis){
			if(Line[i]=='('){
				AddError("Multiple opening parenthesis", m_CurrLine);
				return false;
			}
			else if(Line[i]==')'){
				InParenthesis = false;
				break;
			}
			else{
				if(Line[i]==' ' || Line[i]=='\t' || Line[i]=='|'){
					// complete token
					if(Token!="" && Token!="!"){						
						if(Result!="" && Result[Result.GetLength()-1]!='|') return false;
						Result+=Token;
						Token = "";
					}

					if(Line[i]=='|'){
						if(i==Line.GetLength()-1 || Line[i+1]!='|') return false;
						else{
							Result+='|';
							i++;
						}
					}
				}
				else Token += Line[i];
			}
		}
		else{
			if(Line[i]=='('){
				InParenthesis = true;
				ListFound = true;
			}
			else if (Line[i]!=' ' && Line[i]!='\t'){
				CString Error;
				Error.Format("Unexpected character '%c' at column %d", Line[i], i+1);
				AddError(Error, m_CurrLine);
			}
		}
	}

	if(InParenthesis){
		AddError("Unclosed parenthesis", m_CurrLine);
		return false;
	}

	if(Token!=""){
		if(Result!="" && Result[Result.GetLength()-1]!='|') return false;
		Result+=Token;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::ExpandMacros(CString Line, CString &Result, bool Uppercase)
{
	Result = "";
	int LastPos = 0;

	int Pos = Line.Find("$$", LastPos);
	while(Pos>=0){
		int Pos2 = Line.Find("$$", Pos+2);
		if(Pos2<0){
			return false;
		}

		if(Pos>LastPos) Result += Line.Mid(LastPos, Pos - LastPos);

		CString Macro = Line.Mid(Pos+2, Pos2 - (Pos + 2));
		if(Macro=="") Result+="$$";
		else{
			if(Uppercase) Macro.MakeUpper();
			CString Value;
			if(!m_Dict.Lookup(Macro, Value)) Value = "";
			Result+=Value;
		}


		LastPos = Pos2 + 2;
		Pos = Line.Find("$$", LastPos);
	}

	if(LastPos<Line.GetLength()) Result += Line.Right(Line.GetLength() - LastPos);
	
	return true;
}


//////////////////////////////////////////////////////////////////////
void CTemplateMgr::AddError(CString Error, int Line)
{
	CString str;
	if(Line>=0)
		str.Format("Line %d: %s", Line+1, Error);
	else 
		str.Format("%s", Error);
	
	m_Errors.Add(str);
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::EvaluateMacroList(CString Macros)
{
	bool Result = false;
	for(int i=1; i<=NumEntries(Macros, '|'); i++){
		bool IsNeg = false;
		CString Macro = Entry(i, Macros, '|');
		if(Macro=="") continue;

		if(Macro[0]=='!'){
			IsNeg = true;
			Macro = Macro.Right(Macro.GetLength()-1);
		}
		CString Value;
		bool IsPresent = (m_Dict.Lookup(Macro, Value)!=FALSE);
		if(IsPresent){
			if(Value=="" || Value.CompareNoCase("0")==0 ||  Value.CompareNoCase("no")==0 || Value.CompareNoCase("false")==0)
				IsPresent = false;
		}

		if(IsNeg) IsPresent = !IsPresent;

		Result = Result || IsPresent;
	}
	return Result;
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::FileToFile(CString Source, CString Target)
{
	CStringArray SourceArray;
	CStringArray TargetArray;

	m_Errors.RemoveAll();

	CString Error;
	Error.Format("Parsing file '%s'...", Source);
	AddError(Error);

	CStdioFile f;
	CString line;
	if(f.Open(Source, CFile::modeRead | CFile::typeText)){
		while(f.ReadString(line)){
			SourceArray.Add(line);
		}
		f.Close();
	}
	else{		
		Error.Format("Error opening file '%s' for reading", Source);
		AddError(Error);
		return false;
	}

	if(!ReadExtendedInfo(SourceArray)) return false;
	if(!ParseTemplate(SourceArray, TargetArray)) return false;


	if(f.Open(Target, CFile::modeWrite | CFile::modeCreate | CFile::typeText)){
		for(int i=0; i<TargetArray.GetSize(); i++){
			f.WriteString(TargetArray[i] + "\n");
		}
		f.Close();
	}
	else{
		Error.Format("Error opening file '%s' for writing", Target);
		AddError(Error);
		return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////
void CTemplateMgr::ClearProps()
{
	for(int i=0; i<m_Props.GetSize(); i++) delete m_Props[i];
	m_Props.RemoveAll();
}


//////////////////////////////////////////////////////////////////////
bool CTemplateMgr::LoadDefinitions(CString Filename)
{
	CStringArray Lines;

	CStdioFile f;
	CString line;
	if(f.Open(Filename, CFile::modeRead | CFile::typeText)){
		while(f.ReadString(line)){
			Lines.Add(line);
		}
		f.Close();
	}
	else{
		CString Error;
		Error.Format("Error opening file '%s' for reading", Filename);
		AddError(Error);
		return false;
	}

	return ReadExtendedInfo(Lines);
}


//////////////////////////////////////////////////////////////////////
CString CTemplateMgr::GetErrorString()
{
	CString ret;
	for(int i=0; i<m_Errors.GetSize(); i++)
		ret += m_Errors[i] + "\n";
	return ret;
}


//////////////////////////////////////////////////////////////////////
void CTemplateMgr::Cleanup()
{
	m_Dict.RemoveAll();
	m_Errors.RemoveAll();
	ClearProps();
}


//////////////////////////////////////////////////////////////////////
void CTemplateMgr::ResetParser()
{
	for(int i=0; i<MAX_IF_NEST; i++){
		m_IfIsTrue[i] = false;
		m_IfProcessed[i] = false;
		m_IfHasElse[i] = false;
	}
	m_IfNest = -1;
	m_LoopStart = 0;

	m_CurrLine = 0;
}
