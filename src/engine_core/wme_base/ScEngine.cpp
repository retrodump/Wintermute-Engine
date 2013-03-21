// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "ScEngine.h"
#include "BStringUtil.h"
#include <algorithm>



IMPLEMENT_PERSISTENT(CScEngine, true);

#define COMPILER_DLL "dcscomp.dll"
//////////////////////////////////////////////////////////////////////////
CScEngine::CScEngine(CBGame* inGame):CBBase(inGame)
{
	Game->LOG(0, "Initializing scripting engine...");

	char CompilerPath[MAX_PATH];
	strcpy(CompilerPath, COMPILER_DLL);

	m_CompilerDLL = LoadLibrary(CompilerPath);
	if(m_CompilerDLL==NULL)
	{
		char ModuleName[MAX_PATH];
		CBPlatform::GetModuleFileName(NULL, ModuleName, MAX_PATH);

		// switch to exe's dir
		char* ExeDir = CBUtils::GetPath(ModuleName);
		sprintf(CompilerPath, "%s%s", ExeDir, COMPILER_DLL);
		m_CompilerDLL = LoadLibrary(CompilerPath);

		delete [] ExeDir;
	}
	if(m_CompilerDLL!=NULL)
	{
		// bind compiler's functionality
		ExtCompileBuffer  = (DLL_COMPILE_BUFFER) GetProcAddress(m_CompilerDLL, "CompileBuffer"); 
		ExtCompileFile    = (DLL_COMPILE_FILE)   GetProcAddress(m_CompilerDLL, "CompileFile");   
		ExtReleaseBuffer  = (DLL_RELEASE_BUFFER) GetProcAddress(m_CompilerDLL, "ReleaseBuffer"); 
		ExtSetCallbacks   = (DLL_SET_CALLBACKS)  GetProcAddress(m_CompilerDLL, "SetCallbacks");  
		ExtDefineFunction = (DLL_DEFINE_FUNCTION)GetProcAddress(m_CompilerDLL, "DefineFunction");
		ExtDefineVariable = (DLL_DEFINE_VARIABLE)GetProcAddress(m_CompilerDLL, "DefineVariable");

		if(!ExtCompileBuffer||!ExtCompileFile||!ExtReleaseBuffer||!ExtSetCallbacks||!ExtDefineFunction||!ExtDefineVariable)
		{
			m_CompilerAvailable = false;
			FreeLibrary(m_CompilerDLL);
			m_CompilerDLL = NULL;
		}
		else
		{
			/*
			// publish external methods to the compiler
			CALLBACKS c;
			c.Dll_AddError = AddError;
			c.Dll_CloseFile = CloseFile;
			c.Dll_LoadFile = LoadFile;
			ExtSetCallbacks(&c, Game);
			*/

			m_CompilerAvailable = true;
		}
	}
	else m_CompilerAvailable = false;


	if(m_CompilerAvailable) Game->LOG(0, "  Script compiler bound successfuly");
	else Game->LOG(0, "  Script compiler is NOT available");

	m_Globals = new CScValue(Game);

	
	// register 'Game' as global variable
	if(!m_Globals->PropExists("Game"))
	{
		CScValue val(Game);
		val.SetNative(Game, true);
		m_Globals->SetProp("Game", &val);
	}

	// register 'Math' as global variable
	if(!m_Globals->PropExists("Math"))
	{
		CScValue val(Game);
		val.SetNative(Game->m_MathClass, true);
		m_Globals->SetProp("Math", &val);
	}

	// register 'Directory' as global variable
	if(!m_Globals->PropExists("Directory"))
	{
		CScValue val(Game);
		val.SetNative(Game->m_DirectoryClass, true);
		m_Globals->SetProp("Directory", &val);
	}

	// prepare script cache
	for(int i=0; i<MAX_CACHED_SCRIPTS; i++) m_CachedScripts[i] = NULL;	

	m_CurrentScript = NULL;

	m_FileToCompile = NULL;

	m_CompileErrorCallback = NULL;
	m_CompileErrorCallbackData = NULL;

	m_ParseElementCallback = NULL;
	m_ParseElementCallbackData = NULL;

	m_IsProfiling = false;
	m_ProfilingStartTime = 0;

	//EnableProfiling();
}


//////////////////////////////////////////////////////////////////////////
CScEngine::~CScEngine()
{
	Game->LOG(0, "Shutting down scripting engine");
	SaveBreakpoints();

	DisableProfiling();

	if(m_CompilerAvailable && m_CompilerDLL) FreeLibrary(m_CompilerDLL);
	Cleanup();

	for(int i=0; i<m_Breakpoints.GetSize(); i++)
	{
		SAFE_DELETE(m_Breakpoints[i]);
	}
	m_Breakpoints.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::Cleanup()
{
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(!m_Scripts[i]->m_Thread && m_Scripts[i]->m_Owner) m_Scripts[i]->m_Owner->RemoveScript(m_Scripts[i]);
		delete m_Scripts[i];
		m_Scripts.RemoveAt(i);
		i--;
	}

	m_Scripts.RemoveAll();


	SAFE_DELETE(m_Globals);

	EmptyScriptCache();

	m_CurrentScript = NULL; // ref only

	SAFE_DELETE_ARRAY(m_FileToCompile);

	m_CompileErrorCallback = NULL;
	m_CompileErrorCallbackData = NULL;

	m_ParseElementCallback = NULL;
	m_ParseElementCallbackData = NULL;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
BYTE* WINAPI CScEngine::LoadFile (void* Data, char* Filename, DWORD* Size)
{
	CBGame* Game = (CBGame*)Data;
	return Game->m_FileManager->ReadWholeFile(Filename, Size);
}


//////////////////////////////////////////////////////////////////////////
void WINAPI CScEngine::CloseFile (void* Data, BYTE* Buffer)
{
	delete [] Buffer;
}

//////////////////////////////////////////////////////////////////////////
void WINAPI CScEngine::AddError (void* Data, int Line, char* Text)
{
	CBGame* Game = (CBGame*)Data;

	if(Game)
	{
		if(Game->m_ScEngine && Game->m_ScEngine->m_FileToCompile)
			Game->LOG(0, "Compiling script '%s'...", Game->m_ScEngine->m_FileToCompile);
		Game->LOG(0, "  Error@line %d: %s", Line, Text);


		// redirect to an engine's own callback
		if(Game->m_ScEngine && Game->m_ScEngine->m_CompileErrorCallback)
		{
			Game->m_ScEngine->m_CompileErrorCallback(Line, Text, Game->m_ScEngine->m_CompileErrorCallbackData);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void WINAPI CScEngine::ParseElement (void* Data, int Line, int Type, void* ElementData)
{
	CBGame* Game = (CBGame*)Data;

	if(Game)
	{
		// redirect to an engine's own callback
		if(Game->m_ScEngine && Game->m_ScEngine->m_ParseElementCallback)
		{
			Game->m_ScEngine->m_ParseElementCallback(Line, Type, ElementData, Game->m_ScEngine->m_CompileErrorCallbackData);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
CScScript* CScEngine::RunScript(char *Filename, CBScriptHolder* Owner)
{
	BYTE* CompBuffer;
	DWORD CompSize;

	// get script from cache
	CompBuffer = GetCompiledScript(Filename, &CompSize);
	if(!CompBuffer) return NULL;

	// add new script
	CScScript* script = new CScScript(Game, this);
	HRESULT ret = script->Create(Filename, CompBuffer, CompSize, Owner);
	if(FAILED(ret))
	{
		Game->LOG(ret, "Error running script '%s'...", Filename);
		delete script;
		return NULL;
	}
	else
	{
		// publish the "self" pseudo-variable
		CScValue val(Game);
		if(Owner)val.SetNative(Owner, true);
		else val.SetNULL();

		script->m_Globals->SetProp("self", &val);
		script->m_Globals->SetProp("this", &val);
		
		m_Scripts.Add(script);
		Game->GetDebugMgr()->OnScriptInit(script);
		
		return script;
	}
}


//////////////////////////////////////////////////////////////////////////
BYTE* CScEngine::GetCompiledScript(char *Filename, DWORD *OutSize, bool IgnoreCache)
{
	int i;

	// is script in cache?
	if(!IgnoreCache)
	{
		for(i=0; i<MAX_CACHED_SCRIPTS; i++)
		{
			if(m_CachedScripts[i] && stricmp(m_CachedScripts[i]->m_Filename, Filename)==0)
			{
				m_CachedScripts[i]->m_Timestamp = timeGetTime();
				*OutSize = m_CachedScripts[i]->m_Size;
				return m_CachedScripts[i]->m_Buffer;
			}
		}
	}

	// nope, load it
	BYTE* CompBuffer;
	DWORD CompSize;
	bool CompiledNow = false;

	DWORD Size;

	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename, &Size);
	if(!Buffer)
	{
		Game->LOG(0, "CScEngine::GetCompiledScript - error opening script '%s'", Filename);
		return NULL;
	}

	// needs to be compiled?
	if(*(DWORD*)Buffer == SCRIPT_MAGIC)
	{
		CompBuffer = Buffer;
		CompSize = Size;
	}
	else {
		if(!m_CompilerAvailable)
		{
			Game->LOG(0, "CScEngine::GetCompiledScript - script '%s' needs to be compiled but compiler is not available", Filename);
			delete [] Buffer;
			return NULL;
		}

		CompiledNow = true;

		// publish external methods to the compiler
		CALLBACKS c;
		c.Dll_AddError = AddError;
		c.Dll_CloseFile = CloseFile;
		c.Dll_LoadFile = LoadFile;
		c.Dll_ParseElement = ParseElement;
		ExtSetCallbacks(&c, Game);

		// publish native interfaces
		Game->PublishNatives();

		SetFileToCompile(Filename);
		CompBuffer = ExtCompileFile(Filename, &CompSize);
		if(!CompBuffer)
		{
			Game->QuickMessage("Script compiler error. View log for details.");
			delete [] Buffer;
			return NULL;
		}
	}

	BYTE* ret = NULL;

	// add script to cache
	CScCachedScript* CachedScript = new CScCachedScript(Filename, CompBuffer, CompSize);
	if(CachedScript)
	{
		int index;
		DWORD MinTime = timeGetTime();
		for(i=0; i<MAX_CACHED_SCRIPTS; i++)
		{
			if(m_CachedScripts[i]==NULL)
			{
				index = i;
				break;
			}
			else if(m_CachedScripts[i]->m_Timestamp <= MinTime)
			{
				MinTime = m_CachedScripts[i]->m_Timestamp;
				index = i;
			}
		}

		if(m_CachedScripts[index]!=NULL) delete m_CachedScripts[index];
		m_CachedScripts[index] = CachedScript;

		ret = CachedScript->m_Buffer;
		*OutSize = CachedScript->m_Size;
	}


	// cleanup
	delete [] Buffer;
	if(CompiledNow) ExtReleaseBuffer(CompBuffer);

	return ret;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::Tick()
{
	int i;

	if(m_Scripts.GetSize()==0) return S_OK;


	// resolve waiting scripts
	for(i=0; i<m_Scripts.GetSize(); i++)
	{
			
		switch(m_Scripts[i]->m_State)
		{
			case SCRIPT_WAITING:
			{
				/*
				bool obj_found=false;
				for(int j=0; j<Game->m_RegObjects.GetSize(); j++)
				{
					if(Game->m_RegObjects[j] == m_Scripts[i]->m_WaitObject)
					{
						if(Game->m_RegObjects[j]->IsReady()) m_Scripts[i]->Run();
						obj_found = true;
						break;
					}
				}
				if(!obj_found) m_Scripts[i]->Finish(); // m_WaitObject no longer exists
				*/
				if(Game->ValidObject(m_Scripts[i]->m_WaitObject))
				{
					if(m_Scripts[i]->m_WaitObject->IsReady()) m_Scripts[i]->Run();
				}
				else m_Scripts[i]->Finish();
				break;
			}

			case SCRIPT_SLEEPING:
			{
				if(m_Scripts[i]->m_WaitFrozen)
				{
					if(m_Scripts[i]->m_WaitTime <= timeGetTime()) m_Scripts[i]->Run();
				}
				else
				{
					if(m_Scripts[i]->m_WaitTime <= Game->m_Timer) m_Scripts[i]->Run();
				}
				break;
			}

			case SCRIPT_WAITING_SCRIPT:
			{
				if(!IsValidScript(m_Scripts[i]->m_WaitScript) || m_Scripts[i]->m_WaitScript->m_State==SCRIPT_ERROR)
				{
					// fake return value				
					m_Scripts[i]->m_Stack->PushNULL();
					m_Scripts[i]->m_WaitScript = NULL;
					m_Scripts[i]->Run();
				}
				else
				{
					if(m_Scripts[i]->m_WaitScript->m_State==SCRIPT_THREAD_FINISHED)
					{
						// copy return value
						m_Scripts[i]->m_Stack->Push(m_Scripts[i]->m_WaitScript->m_Stack->Pop());
						m_Scripts[i]->Run();
						m_Scripts[i]->m_WaitScript->Finish();
						m_Scripts[i]->m_WaitScript = NULL;
					}
				}
				break;
			}
		} // switch
	} // for each script


	// execute scripts
	for(i=0; i<m_Scripts.GetSize(); i++)
	{

		// skip paused scripts
		if(m_Scripts[i]->m_State==SCRIPT_PAUSED) continue;

		// time sliced script
		if(m_Scripts[i]->m_TimeSlice > 0)
		{
			DWORD StartTime = timeGetTime();
			while(m_Scripts[i]->m_State==SCRIPT_RUNNING && timeGetTime()-StartTime < m_Scripts[i]->m_TimeSlice)
			{
				m_CurrentScript = m_Scripts[i];
				m_Scripts[i]->ExecuteInstruction();
			}
			if (m_IsProfiling && m_Scripts[i]->m_Filename) AddScriptTime(m_Scripts[i]->m_Filename, timeGetTime() - StartTime);
		}
		
		// normal script
		else
		{
			DWORD StartTime;
			bool isProfiling = m_IsProfiling;
			if (isProfiling) StartTime = timeGetTime();

			while(m_Scripts[i]->m_State==SCRIPT_RUNNING)
			{
				m_CurrentScript = m_Scripts[i];
				m_Scripts[i]->ExecuteInstruction();
			}
			if (isProfiling && m_Scripts[i]->m_Filename) AddScriptTime(m_Scripts[i]->m_Filename, timeGetTime() - StartTime);
		}
		m_CurrentScript = NULL;
	}

	RemoveFinishedScripts();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::TickUnbreakable()
{
	// execute unbreakable scripts
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(!m_Scripts[i]->m_Unbreakable) continue;

		while(m_Scripts[i]->m_State==SCRIPT_RUNNING)
		{
			m_CurrentScript = m_Scripts[i];
			m_Scripts[i]->ExecuteInstruction();
		}
		m_Scripts[i]->Finish();
		m_CurrentScript = NULL;
	}
	RemoveFinishedScripts();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::RemoveFinishedScripts()
{
	// remove finished scripts
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_State==SCRIPT_FINISHED || m_Scripts[i]->m_State==SCRIPT_ERROR)
		{
			if(!m_Scripts[i]->m_Thread && m_Scripts[i]->m_Owner) m_Scripts[i]->m_Owner->RemoveScript(m_Scripts[i]);
			Game->GetDebugMgr()->OnScriptShutdown(m_Scripts[i]);
			delete m_Scripts[i];
			m_Scripts.RemoveAt(i);
			i--;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CScEngine::GetNumScripts(int* Running, int* Waiting, int* Persistent)
{
	int running=0, waiting=0, persistent=0, total=0;

	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_State==SCRIPT_FINISHED) continue;
		switch(m_Scripts[i]->m_State)
		{
			case SCRIPT_RUNNING:
			case SCRIPT_SLEEPING:
			case SCRIPT_PAUSED:
				running++;
			break;
			case SCRIPT_WAITING: waiting++; break;
			case SCRIPT_PERSISTENT: persistent++; break;
		}
		total++;
	}
	if(Running) *Running = running;
	if(Waiting) *Waiting = waiting;
	if(Persistent)	*Persistent = persistent;

	return total;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::EmptyScriptCache()
{
	for(int i=0; i<MAX_CACHED_SCRIPTS; i++)
	{
		if(m_CachedScripts[i])
		{
			SAFE_DELETE(m_CachedScripts[i]);
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::ResetObject(CBObject* Object)
{
	// terminate all scripts waiting for this object
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_State==SCRIPT_WAITING && m_Scripts[i]->m_WaitObject==Object)
		{
			if(!Game->m_CompatKillMethodThreads) ResetScript(m_Scripts[i]);

			bool IsThread = m_Scripts[i]->m_MethodThread || m_Scripts[i]->m_Thread;
			m_Scripts[i]->Finish(!IsThread); // 1.9b1 - top-level script kills its threads as well
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::ResetScript(CScScript* Script)
{
	// terminate all scripts waiting for this script
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_State==SCRIPT_WAITING_SCRIPT && m_Scripts[i]->m_WaitScript==Script)
		{
			m_Scripts[i]->Finish();
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::Persist(CBPersistMgr* PersistMgr)
{
	if(!PersistMgr->m_Saving) Cleanup();

	PersistMgr->Transfer(TMEMBER(Game));
	PersistMgr->Transfer(TMEMBER(m_CurrentScript));
	PersistMgr->Transfer(TMEMBER(m_FileToCompile));
	PersistMgr->Transfer(TMEMBER(m_Globals));
	m_Scripts.Persist(PersistMgr);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::EditorCleanup()
{
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_Owner==NULL && (m_Scripts[i]->m_State==SCRIPT_FINISHED || m_Scripts[i]->m_State==SCRIPT_ERROR))
		{
			delete m_Scripts[i];
			m_Scripts.RemoveAt(i);
			i--;
		}
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::PauseAll()
{
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]!=m_CurrentScript) m_Scripts[i]->Pause();
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::ResumeAll()
{
	for(int i=0; i<m_Scripts.GetSize(); i++)
		m_Scripts[i]->Resume();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::SetFileToCompile(char *Filename)
{
	SAFE_DELETE_ARRAY(m_FileToCompile);
	m_FileToCompile = new char[strlen(Filename)+1];
	if(m_FileToCompile)
	{
		strcpy(m_FileToCompile, Filename);
		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::SetCompileErrorCallback(COMPILE_ERROR_CALLBACK Callback, void *Data)
{
	m_CompileErrorCallback = Callback;
	m_CompileErrorCallbackData = Data;
}

//////////////////////////////////////////////////////////////////////////
void CScEngine::SetParseElementCallback(PARSE_ELEMENT_CALLBACK Callback, void *Data)
{
	m_ParseElementCallback = Callback;
	m_ParseElementCallbackData = Data;
}


//////////////////////////////////////////////////////////////////////////
bool CScEngine::IsValidScript(CScScript *Script)
{
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]==Script) return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::ClearGlobals(bool IncludingNatives)
{
	m_Globals->CleanProps(IncludingNatives);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::DbgSendScripts(IWmeDebugClient* Client)
{
	// send global variables
	m_Globals->DbgSendVariables(Client, WME_DBGVAR_GLOBAL, NULL, 0);

	// process normal scripts first
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_Thread || m_Scripts[i]->m_MethodThread) continue;
		m_Scripts[i]->DbgSendScript(Client);
	}

	// and threads later
	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		if(m_Scripts[i]->m_Thread || m_Scripts[i]->m_MethodThread)
			m_Scripts[i]->DbgSendScript(Client);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::AddBreakpoint(char* ScriptFilename, int Line)
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;

	CScBreakpoint* Bp = NULL;
	for(int i=0; i<m_Breakpoints.GetSize(); i++)
	{
		if(stricmp(m_Breakpoints[i]->m_Filename, ScriptFilename)==0)
		{
			Bp = m_Breakpoints[i];
			break;
		}
	}
	if(Bp==NULL)
	{
		Bp = new CScBreakpoint(ScriptFilename);
		m_Breakpoints.Add(Bp);
	}
	bool Found = false;
	for(int i=0; i<Bp->m_Lines.GetSize(); i++)
	{
		if(Bp->m_Lines[i]==Line) return S_OK;
	}
	Bp->m_Lines.Add(Line);

	// refresh changes
	RefreshScriptBreakpoints();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::RemoveBreakpoint(char* ScriptFilename, int Line)
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;

	for(int i=0; i<m_Breakpoints.GetSize(); i++)
	{
		if(stricmp(m_Breakpoints[i]->m_Filename, ScriptFilename)==0)
		{
			for(int j=0; j<m_Breakpoints[i]->m_Lines.GetSize(); j++)
			{
				if(m_Breakpoints[i]->m_Lines[j]==Line)
				{
					m_Breakpoints[i]->m_Lines.RemoveAt(j);
					if(m_Breakpoints[i]->m_Lines.GetSize()==0)
					{
						SAFE_DELETE(m_Breakpoints[i]);
						m_Breakpoints.RemoveAt(i);
					}
					// refresh changes
					RefreshScriptBreakpoints();

					return S_OK;
				}
			}
			break;
		}
	}
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::RefreshScriptBreakpoints()
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;

	for(int i=0; i<m_Scripts.GetSize(); i++)
	{
		RefreshScriptBreakpoints(m_Scripts[i]);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::RefreshScriptBreakpoints(CScScript* Script)
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;

	if(!Script || !Script->m_Filename) return E_FAIL;

	for(int i=0; i<m_Breakpoints.GetSize(); i++)
	{
		if(stricmp(m_Breakpoints[i]->m_Filename, Script->m_Filename)==0)
		{
			Script->m_Breakpoints.Copy(m_Breakpoints[i]->m_Lines);
			return S_OK;
		}
	}
	if(Script->m_Breakpoints.GetSize() > 0) Script->m_Breakpoints.RemoveAll();

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::SaveBreakpoints()
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;


	char Text[512];
	char Key[100];

	int Count = 0;
	for(int i=0; i<m_Breakpoints.GetSize(); i++)
	{
		for(int j=0; j<m_Breakpoints[i]->m_Lines.GetSize(); j++)
		{
			Count++;
			sprintf(Key, "Breakpoint%d", Count);
			sprintf(Text, "%s:%d", m_Breakpoints[i]->m_Filename, m_Breakpoints[i]->m_Lines[j]);

			Game->m_Registry->WriteString("Debug", Key, Text);
		}
	}
	Game->m_Registry->WriteInt("Debug", "NumBreakpoints", Count);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CScEngine::LoadBreakpoints()
{
	if(!Game->GetDebugMgr()->m_Enabled) return S_OK;


	char Text[512];
	char Key[100];

	int Count = Game->m_Registry->ReadInt("Debug", "NumBreakpoints", 0);
	for(int i=1; i<=Count; i++)
	{
		DWORD BufSize = 512;
		sprintf(Key, "Breakpoint%d", i);
		Game->m_Registry->ReadString("Debug", Key, Text, &BufSize, "");
		
		char* Path = CBUtils::StrEntry(0, Text, ':');
		char* Line = CBUtils::StrEntry(1, Text, ':');

		if(Path!=NULL && Line!=NULL) AddBreakpoint(Path, atoi(Line));
		SAFE_DELETE_ARRAY(Path);
		SAFE_DELETE_ARRAY(Line);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::AddScriptTime(const char* Filename, DWORD Time)
{
	if (!m_IsProfiling) return;

	std::string fileName = Filename;
	CBStringUtil::ToLowerCase(fileName);
	m_ScriptTimes[fileName] += Time;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::EnableProfiling()
{
	if (m_IsProfiling) return;

	// destroy old data, if any
	m_ScriptTimes.clear();

	m_ProfilingStartTime = timeGetTime();
	m_IsProfiling = true;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::DisableProfiling()
{
	if (!m_IsProfiling) return;

	DumpStats();
	m_IsProfiling = false;
}


//////////////////////////////////////////////////////////////////////////
void CScEngine::DumpStats()
{
	DWORD totalTime = timeGetTime() - m_ProfilingStartTime;


	typedef vector <pair<DWORD, std::string> > TimeVector;
	TimeVector times;

	ScriptTimes::iterator it;
	for (it = m_ScriptTimes.begin(); it != m_ScriptTimes.end(); it++)
	{
		times.push_back(pair<DWORD, std::string> (it->second, it->first));
	}
	std::sort(times.begin(), times.end());


	TimeVector::reverse_iterator tit;

	Game->LOG(0, "***** Script profiling information: *****");
	Game->LOG(0, "  %-40s %fs", "Total execution time", (float)totalTime / 1000);

	for (tit = times.rbegin(); tit != times.rend(); tit++)
	{
		Game->LOG(0, "  %-40s %fs (%f%%)", tit->second.c_str(), (float)tit->first / 1000, (float)tit->first / (float)totalTime * 100);
	}
}
