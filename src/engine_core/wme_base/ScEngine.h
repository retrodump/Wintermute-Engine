// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_SCENGINE_H__333E7BF9_8A01_4FBB_82FF_EAAFB50F27C1__INCLUDED_)
#define AFX_SCENGINE_H__333E7BF9_8A01_4FBB_82FF_EAAFB50F27C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"
#include "wme_debugger.h"

typedef BYTE* (*DLL_COMPILE_BUFFER)(BYTE* Buffer, char* Source, DWORD BufferSize, DWORD* CompiledSize);
typedef BYTE* (*DLL_COMPILE_FILE)(char* Filename, DWORD* CompiledSize);
typedef void  (*DLL_RELEASE_BUFFER)(unsigned char* Buffer);
typedef void  (*DLL_SET_CALLBACKS)(CALLBACKS* callbacks, void* Data);
typedef int   (*DLL_DEFINE_FUNCTION)(char* Name);
typedef int   (*DLL_DEFINE_VARIABLE)(char* Name);

typedef void (__stdcall *COMPILE_ERROR_CALLBACK)(int Line, char* Text , void* Data);
typedef void (__stdcall *PARSE_ELEMENT_CALLBACK)(int Line, int Type, void* ElementData, void* Data);

#define MAX_CACHED_SCRIPTS 20

class CScEngine : public CBBase  
{
public:
	class CScCachedScript
	{
	public:
		CScCachedScript(char* Filename, BYTE* Buffer, DWORD Size){
			m_Timestamp = timeGetTime();
			m_Buffer = new BYTE[Size];
			if(m_Buffer) memcpy(m_Buffer, Buffer, Size);
			m_Size = Size;
			m_Filename = new char[strlen(Filename)+1];
			if(m_Filename) strcpy(m_Filename, Filename);
		};

		~CScCachedScript(){
			if(m_Buffer) delete [] m_Buffer;
			if(m_Filename) delete [] m_Filename;
		};

		DWORD m_Timestamp;
		BYTE* m_Buffer;
		DWORD m_Size;
		char* m_Filename;
	};

	class CScBreakpoint
	{
	public:
		CScBreakpoint(char* Filename)
		{
			m_Filename = NULL;
			CBUtils::SetString(&m_Filename, Filename);
		}

		~CScBreakpoint()
		{
			if(m_Filename) delete [] m_Filename;
			m_Lines.RemoveAll();
		}

		char* m_Filename;
		CBArray<int, int> m_Lines;
	};




public:
	HRESULT DbgSendScripts(IWmeDebugClient* Client);

	CBArray<CScBreakpoint*, CScBreakpoint*> m_Breakpoints;
	HRESULT AddBreakpoint(char* ScriptFilename, int Line);
	HRESULT RemoveBreakpoint(char* ScriptFilename, int Line);
	HRESULT RefreshScriptBreakpoints();
	HRESULT RefreshScriptBreakpoints(CScScript* Script);
	HRESULT SaveBreakpoints();
	HRESULT LoadBreakpoints();

	HRESULT ClearGlobals(bool IncludingNatives=false);
	HRESULT TickUnbreakable();
	HRESULT RemoveFinishedScripts();
	bool IsValidScript(CScScript* Script);
	void SetCompileErrorCallback(COMPILE_ERROR_CALLBACK Callback, void* Data);
	void SetParseElementCallback(PARSE_ELEMENT_CALLBACK Callback, void* Data);

	COMPILE_ERROR_CALLBACK m_CompileErrorCallback;
	void* m_CompileErrorCallbackData;

	PARSE_ELEMENT_CALLBACK m_ParseElementCallback;
	void* m_ParseElementCallbackData;

	HRESULT SetFileToCompile(char* Filename);
	char* m_FileToCompile;
	CScScript* m_CurrentScript;
	HRESULT ResumeAll();
	HRESULT PauseAll();
	void EditorCleanup();
	HRESULT ResetObject(CBObject* Object);
	HRESULT ResetScript(CScScript* Script);
	HRESULT EmptyScriptCache();
	BYTE* GetCompiledScript(char* Filename, DWORD* OutSize, bool IgnoreCache = false);
	DECLARE_PERSISTENT(CScEngine, CBBase);
	HRESULT Cleanup();
	int GetNumScripts(int* Running=NULL, int* Waiting=NULL, int* Persistent=NULL);
	HRESULT Tick();
	CScValue* m_Globals;
	CScScript* RunScript(char* Filename, CBScriptHolder* Owner=NULL);
	bool m_CompilerAvailable;
	HINSTANCE m_CompilerDLL;
	CScEngine(CBGame* inGame);
	virtual ~CScEngine();
	static void WINAPI AddError (void* Data, int Line, char* Text);
	static BYTE* WINAPI LoadFile (void* Data, char* Filename, DWORD* Size);
	static void  WINAPI CloseFile (void* Data, BYTE* Buffer);
	static void WINAPI ParseElement (void* Data, int Line, int Type, void* ElementData);	
	DLL_COMPILE_BUFFER  ExtCompileBuffer;
	DLL_COMPILE_FILE    ExtCompileFile;
	DLL_RELEASE_BUFFER  ExtReleaseBuffer;
	DLL_SET_CALLBACKS   ExtSetCallbacks;
	DLL_DEFINE_FUNCTION ExtDefineFunction;
	DLL_DEFINE_VARIABLE ExtDefineVariable;

	CBArray<CScScript*, CScScript*> m_Scripts;

	void EnableProfiling();
	void DisableProfiling();
	bool IsProfiling() { return m_IsProfiling; }

	void AddScriptTime(const char* Filename, DWORD Time);
	void DumpStats();

private:
	
	CScCachedScript* m_CachedScripts[MAX_CACHED_SCRIPTS];
	bool m_IsProfiling;
	DWORD m_ProfilingStartTime;

	typedef std::map<std::string, DWORD> ScriptTimes;
	ScriptTimes m_ScriptTimes;

};

#endif // !defined(AFX_SCENGINE_H__333E7BF9_8A01_4FBB_82FF_EAAFB50F27C1__INCLUDED_)
