// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#include "WinWindow.h"
#include "WinButton.h"
#include "WinStatic.h"
#include "WinCombo.h"
#include "WinEditor.h"
#include "WinTabCtrl.h"
#include "coll_templ.h"


typedef struct
{
	WORD dlgVer;
	WORD signature;
	DWORD helpID;
	DWORD exStyle;
	DWORD style;
	WORD cDlgItems;
	short x;
	short y;
	short cx;
	short cy;
} DLGTEMPLATEEX;


class CWinDialog : public CWinWindow
{
public:
	CWinDialog(void);
	virtual ~CWinDialog(void);

	HRESULT Create(int ResID, HWND Parent=NULL, HINSTANCE Instance=NULL);
	HRESULT CreateIndirect(int ResID, HWND Parent=NULL, HINSTANCE Instance=NULL, bool RightToLeft=false);
	static INT_PTR CALLBACK InternalDlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	int DoModal();

	CWinWindow* RegisterControl(int ControlID);
	CWinButton* RegisterButton(int ControlID);
	CWinStatic* RegisterStatic(int ControlID);
	CWinCombo* RegisterCombo(int ControlID);
	CWinEditor* RegisterEditor(int ControlID);
	CWinTabCtrl* RegisterTabCtrl(int ControlID);
	
	void RegisterEvent(int ControlID, int MessageID, EVENT_HANDLER_PROC HandlerProc);
	HRESULT InvokeEvent(int ControlID, int MessageID);
	
	// overrides
	virtual HRESULT RegisterControls();
	virtual HRESULT OnInitDialog();
	virtual HRESULT OnKeyDown(UINT KeyCode, UINT Flags);
	virtual HRESULT OnSysKeyDown(UINT KeyCode, UINT Flags);
	virtual HRESULT UnhandledEvent(int ControlID, int MessageID);
	virtual HRESULT OnEnterIdle();
	virtual HRESULT OnPaint();

	// methods
	void EndDialog(int Result);
	
protected:
	// the real message handler
	virtual INT_PTR CALLBACK DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	HRESULT InternalInitDialog(HWND Hwnd);

private:
	bool m_InModalLoop;
	int m_ModalResult;
	HWND m_Parent;

	class CWinEventHandler
	{
	public:
		int m_ControlID;
		int m_MessageID;
		EVENT_HANDLER_PROC m_HandlerProc;

		CWinEventHandler(int ControlID, int MessageID, EVENT_HANDLER_PROC HandlerProc)
		{
			m_ControlID = ControlID;
			m_MessageID = MessageID;
			m_HandlerProc = HandlerProc;
		}

		HRESULT Invoke(CWinWindow* Object)
		{
			if(m_HandlerProc && Object)
			{
				(Object->*(m_HandlerProc))();
				return S_OK;
			}
			else return E_FAIL;
		}
	};

	CBArray<CWinWindow*, CWinWindow*> m_Controls;
	CBArray<CWinEventHandler*, CWinEventHandler*> m_EventHandlers;
};
