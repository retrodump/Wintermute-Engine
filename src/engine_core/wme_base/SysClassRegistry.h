// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#ifndef __WmeSysClassRegistry_H__
#define __WmeSysClassRegistry_H__


class CBGame;

class CSysClassRegistry  
{
public:
	static CSysClassRegistry* GetInstance();

	HRESULT EnumInstances(SYS_INSTANCE_CALLBACK lpCallback, const char* className, void* lpData);
	HRESULT LoadTable(CBGame* Game, CBPersistMgr* PersistMgr);
	HRESULT SaveTable(CBGame* Game, CBPersistMgr* PersistMgr, bool quickSave);
	HRESULT LoadInstances(CBGame* Game, CBPersistMgr* PersistMgr);
	HRESULT SaveInstances(CBGame* Game, CBPersistMgr* PersistMgr, bool quickSave);
	void* IDToPointer(int classID, int instanceID);
	bool GetPointerID(void* pointer, int* classID, int* instanceID);
	bool RegisterClass(CSysClass* classObj);
	bool UnregisterClass(CSysClass* classObj);
	bool RegisterInstance(const char* className, void* instance);
	bool UnregisterInstance(const char* className, void* instance);
	void DumpClasses(FILE* stream);
	int GetNextID();
	void AddInstanceToTable(CSysInstance* instance, void* pointer);

	CSysClassRegistry();
	virtual ~CSysClassRegistry();

	bool m_Disabled;
	int m_Count;
	
	typedef std::set<CSysClass*> Classes;
	Classes m_Classes;

	typedef std::map<AnsiString, CSysClass*> NameMap;
	NameMap m_NameMap;

	typedef std::map<int, CSysClass*> IdMap;
	IdMap m_IdMap;

	typedef std::map<void*, CSysInstance*> InstanceMap;
	InstanceMap m_InstanceMap;

	typedef std::map<int, CSysInstance*> SavedInstanceMap;
	SavedInstanceMap m_SavedInstanceMap;

};

#endif
