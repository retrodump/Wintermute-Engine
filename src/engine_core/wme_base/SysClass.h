// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#ifndef __WmeSysClass_H__
#define __WmeSysClass_H__


#include <set>
#include <map>
using namespace std;


class CSysClass  
{
public:
	CSysClass(const AnsiString& name, PERSISTBUILD build, PERSISTLOAD load, bool persistent_class);
	~CSysClass();
	
	int GetNumInstances();
	bool RemoveInstance(void* instance);
	CSysInstance* AddInstance(void* instance, int id, int savedId = -1);
	bool RemoveAllInstances();

	int GetInstanceID(void* pointer);
	void* IDToPointer(int savedID);

	void SetID(int id) { m_ID = id; }
	int GetID() const { return m_ID; }

	int GetSavedID() const { return m_SavedID; }

	bool IsPersistent() const { return m_Persistent; }

	AnsiString GetName() const { return m_Name; }

	void SaveTable(CBGame* Game, CBPersistMgr* PersistMgr);
	void LoadTable(CBGame* Game, CBPersistMgr* PersistMgr);

	void SaveInstances(CBGame* Game, CBPersistMgr* PersistMgr);
	void LoadInstance(void* instance, CBPersistMgr* PersistMgr);

	void InstanceCallback(SYS_INSTANCE_CALLBACK lpCallback, void* lpData);

	void ResetSavedIDs();

	void Dump(FILE* stream);
	
private:	
	int m_NumInst;
	bool m_Persistent;
	CSysClass* m_Next;
	int m_ID;
	int m_SavedID;
	AnsiString m_Name;
	PERSISTBUILD m_Build;
	PERSISTLOAD m_Load;		

	typedef std::set<CSysInstance*> Instances;
	Instances m_Instances;

	typedef std::map<void*, CSysInstance*> InstanceMap;
	InstanceMap m_InstanceMap;
};

#endif
