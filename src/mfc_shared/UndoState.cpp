// UndoState.cpp: implementation of the CUndoState class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "SceneEdit.h"
#include "UndoState.h"

extern "C" {
	#include "zlib.h"
}

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
CUndoState::CUndoState()
{
	m_Buffer = NULL;
	m_Size = m_CompSize = 0;
}


//////////////////////////////////////////////////////////////////////////
CUndoState::~CUndoState()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void CUndoState::Cleanup()
{
	if(m_Buffer) delete [] m_Buffer;
	m_Buffer = NULL;
	m_Size = m_CompSize = 0;
	m_Name = "";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUndoState::SetData(BYTE *Buffer, DWORD Size, CString Name)
{
	Cleanup();

	DWORD CompSize = Size*2;
	BYTE* comp_buffer = new BYTE[CompSize];
	
	if(compress(comp_buffer, &CompSize, Buffer, Size)!=Z_OK){
		delete [] comp_buffer;
		return E_FAIL;
	}

	m_Buffer = new BYTE[CompSize];
	m_Size = Size;
	m_CompSize = CompSize;
	memcpy(m_Buffer, comp_buffer, CompSize);
	delete [] comp_buffer;

	m_Name = Name;
	
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
BYTE* CUndoState::GetData(bool AddZero)
{
	DWORD Size = m_Size;
	if(AddZero) Size++;

	BYTE* ret = new BYTE[Size];
	memset(ret, 0, Size);

	if(uncompress(ret, &Size, m_Buffer, m_CompSize)!=Z_OK){
		delete [] ret;
		return NULL;
	}
	else return ret;
}
