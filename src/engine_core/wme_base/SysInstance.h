// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#ifndef __WmeSysInstance_H__
#define __WmeSysInstance_H__


class CSysClass;

class CSysInstance  
{
public:
	CSysInstance(void* Instance, int ID, CSysClass* sysClass);
	virtual ~CSysInstance();

	int GetID() const { return m_ID; }
	int GetSavedID() const { return m_SavedID; }
	void* GetInstance() const { return m_Instance; }
	CSysClass* GetClass() const { return m_Class; }

	void SetSavedID(int id) { m_SavedID = id; }

private:
	bool m_Used;
	int m_ID;
	int m_SavedID;
	void* m_Instance;
	CSysClass* m_Class;
};

#endif
