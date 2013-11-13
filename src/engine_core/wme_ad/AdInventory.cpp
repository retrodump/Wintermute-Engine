// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdInventory.h"


IMPLEMENT_PERSISTENT(CAdInventory, false);

//////////////////////////////////////////////////////////////////////////
CAdInventory::CAdInventory(CBGame* inGame):CBObject(inGame)
{
	m_ScrollOffset = 0;
}


//////////////////////////////////////////////////////////////////////////
CAdInventory::~CAdInventory()
{
	m_TakenItems.RemoveAll(); // ref only
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::InsertItem(char *Name, char *InsertAfter)
{
	if(Name==NULL) return E_FAIL;

	CAdItem* item = ((CAdGame*)Game)->GetItemByName(Name);	
	if(item==NULL) return E_FAIL;

	int InsertIndex=-1;
	for(int i=0; i<m_TakenItems.GetSize(); i++){
		if(_stricmp(m_TakenItems[i]->m_Name, Name)==0){
			m_TakenItems.RemoveAt(i);
			i--;
			continue;
		}
		if(InsertAfter && _stricmp(m_TakenItems[i]->m_Name, InsertAfter)==0) InsertIndex = i+1;
	}


	if(InsertIndex==-1) m_TakenItems.Add(item);
	else m_TakenItems.InsertAt(InsertIndex, item);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::RemoveItem(char *Name)
{
	if(Name==NULL) return E_FAIL;

	for(int i=0; i<m_TakenItems.GetSize(); i++){
		if(_stricmp(m_TakenItems[i]->m_Name, Name)==0){
			if(((CAdGame*)Game)->m_SelectedItem==m_TakenItems[i]) ((CAdGame*)Game)->m_SelectedItem = NULL;
			m_TakenItems.RemoveAt(i);
			return S_OK;
		}
	}

	return E_FAIL;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::RemoveItem(CAdItem* Item)
{
	if(Item==NULL) return E_FAIL;

	for(int i=0; i<m_TakenItems.GetSize(); i++){
		if(m_TakenItems[i]==Item){
			if(((CAdGame*)Game)->m_SelectedItem==m_TakenItems[i]) ((CAdGame*)Game)->m_SelectedItem = NULL;
			m_TakenItems.RemoveAt(i);
			return S_OK;
		}
	}

	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdInventory::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);
	
	m_TakenItems.Persist(PersistMgr);
	PersistMgr->Transfer(TMEMBER(m_ScrollOffset));

	return S_OK;
}