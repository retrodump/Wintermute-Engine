// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\windialog.h"
#include "commctrl.h"

#pragma comment(lib, "comctl32.lib")


//////////////////////////////////////////////////////////////////////////
CWinDialog::CWinDialog(void) : CWinWindow()
{
	::InitCommonControls();
	m_Hwnd = NULL;

	m_InModalLoop = false;
	m_ModalResult = 0;
	m_Parent = NULL;
}


//////////////////////////////////////////////////////////////////////////
CWinDialog::~CWinDialog(void)
{
	for(int i=0; i<m_Controls.GetSize(); i++)
		delete m_Controls[i];
	m_Controls.RemoveAll();


	for(int i=0; i<m_EventHandlers.GetSize(); i++)
		delete m_EventHandlers[i];
	m_EventHandlers.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::Create(int ResID, HWND Parent, HINSTANCE Instance)
{
	if(Instance==NULL) Instance = GetModuleHandle(NULL);
	if(DialogBoxParam(Instance, MAKEINTRESOURCE(ResID), Parent, CWinDialog::InternalDlgMsgHandler, (LPARAM)this) != IDOK)
	{
		return E_FAIL;
	}
	else
	{
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::CreateIndirect(int ResID, HWND Parent, HINSTANCE Instance, bool RightToLeft)
{
	if(Instance==NULL) Instance = GetModuleHandle(NULL);

	HRSRC hResource = ::FindResource(Instance, MAKEINTRESOURCE(ResID), RT_DIALOG);
	HGLOBAL hTemplate = ::LoadResource(Instance, hResource);
	LPCDLGTEMPLATE lpDialogTemplate = (LPCDLGTEMPLATE)::LockResource(hTemplate);

	DWORD ResSize = ::SizeofResource(Instance, hResource);
	DLGTEMPLATEEX* DlgTemplateEx = (DLGTEMPLATEEX*)new BYTE[ResSize];
	memcpy(DlgTemplateEx, lpDialogTemplate, ResSize);

	if(RightToLeft) DlgTemplateEx->exStyle |= WS_EX_LAYOUTRTL;


	//m_Hwnd = CreateDialogIndirectParam(Instance, lpDialogTemplate, Parent, CWinDialog::InternalDlgMsgHandler, (LPARAM)this);
	m_Hwnd = CreateDialogIndirectParam(Instance, (DLGTEMPLATE*)DlgTemplateEx, Parent, CWinDialog::InternalDlgMsgHandler, (LPARAM)this);

	UnlockResource(hTemplate);
	::FreeResource(hTemplate);
	delete [] (BYTE*)DlgTemplateEx;

	m_Parent = Parent;

	return m_Hwnd!=NULL?S_OK:E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK CWinDialog::InternalDlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWinDialog* Dlg;

	if (uMsg == WM_INITDIALOG)
	{
		// get the pointer to the window from lpCreateParams which was set in CreateWindow
		::SetWindowLong(hwnd, GWL_USERDATA, (long)(lParam));
	}

	// get the pointer to the window
	Dlg = (CWinDialog*)GetObjectFromWindow(hwnd);

	// if we have the pointer, go to the message handler of the window
	// else, use DefWindowProc
	if (Dlg)
	{
		return Dlg->DlgMsgHandler(hwnd, uMsg, wParam, lParam);
	}
	else
		return FALSE;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::InternalInitDialog(HWND Hwnd)
{
	// remember the window handle on init
	m_Hwnd = Hwnd;
	
	HRESULT Res;
	Res = RegisterControls();
	if(FAILED(Res)) return Res;

	Res = OnInitDialog();
	if(FAILED(Res)) return Res;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CWinDialog::DoModal()
{
	MSG msg;

	// disable parent window
	bool EnableParent = false;
	if(m_Parent!=NULL && m_Parent != ::GetDesktopWindow() && ::IsWindowEnabled(m_Parent))
	{
		::EnableWindow(m_Parent, FALSE);
		EnableParent = true;
	}

	ShowWindow();


	m_InModalLoop = true;
	
	for(bool Done=false; !Done; ::WaitMessage())
	{
		while(::PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				Done = true;
				::PostMessage(NULL,WM_QUIT,0,0);
				break;
			}
			else if(msg.message == WM_KEYDOWN)
			{
				if(SUCCEEDED(OnKeyDown((UINT)msg.wParam, (UINT)msg.lParam)))
				{
					continue;
				}
			}
			else if(msg.message==WM_SYSKEYDOWN)
			{
				if(SUCCEEDED(OnSysKeyDown((UINT)msg.wParam, (UINT)msg.lParam)))
				{
					continue;
				}
			}


			if(!::IsDialogMessage(m_Hwnd, &msg))
			{
				::TranslateMessage(&msg);
				::DispatchMessage(&msg);
			}
		}
		if(!m_InModalLoop) break;
		OnEnterIdle();
	}

	if (m_Hwnd != NULL)	SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW|SWP_NOSIZE|SWP_NOMOVE|SWP_NOACTIVATE|SWP_NOZORDER);


	if(EnableParent) ::EnableWindow(m_Parent, TRUE);
	if(m_Parent != NULL && ::GetActiveWindow() == m_Hwnd) ::SetActiveWindow(m_Parent);

	DestroyWindow();
	return m_ModalResult;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::RegisterControls()
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::OnInitDialog()
{
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::OnKeyDown(UINT KeyCode, UINT Flags)
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::OnSysKeyDown(UINT KeyCode, UINT Flags)
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::OnEnterIdle()
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::OnPaint()
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
CWinWindow* CWinDialog::RegisterControl(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinWindow* Win = new CWinWindow();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
CWinButton* CWinDialog::RegisterButton(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinButton* Win = new CWinButton();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
CWinStatic* CWinDialog::RegisterStatic(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinStatic* Win = new CWinStatic();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
CWinCombo* CWinDialog::RegisterCombo(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinCombo* Win = new CWinCombo();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
CWinEditor* CWinDialog::RegisterEditor(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinEditor* Win = new CWinEditor();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
CWinTabCtrl* CWinDialog::RegisterTabCtrl(int ControlID)
{
	HWND Hwnd = ::GetDlgItem(GetHwnd(), ControlID);
	if(!Hwnd) return NULL;

	CWinTabCtrl* Win = new CWinTabCtrl();
	Win->BindWindow(Hwnd);
	m_Controls.Add(Win);

	return Win;
}

//////////////////////////////////////////////////////////////////////////
void CWinDialog::RegisterEvent(int ControlID, int MessageID, EVENT_HANDLER_PROC HandlerProc)
{
	for(int i=0; i<m_EventHandlers.GetSize(); i++)
	{
		CWinEventHandler* Handler = m_EventHandlers[i];
		if(Handler->m_ControlID==ControlID && Handler->m_MessageID==MessageID)
		{
			if(!HandlerProc)
			{
				delete m_EventHandlers[i];
				m_EventHandlers.RemoveAt(i);
			}
			else Handler->m_HandlerProc = HandlerProc;

			return;
		}
	}
	CWinEventHandler* Handler = new CWinEventHandler(ControlID, MessageID, HandlerProc);
	if(Handler) m_EventHandlers.Add(Handler);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::InvokeEvent(int ControlID, int MessageID)
{
	for(int i=0; i<m_EventHandlers.GetSize(); i++)
	{
		CWinEventHandler* Handler = m_EventHandlers[i];
		if(Handler->m_ControlID==ControlID && Handler->m_MessageID==MessageID)
		{
			return Handler->Invoke(this);
		}
	}
	return UnhandledEvent(ControlID, MessageID); // no handler
}

//////////////////////////////////////////////////////////////////////////
HRESULT CWinDialog::UnhandledEvent(int ControlID, int MessageID)
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
INT_PTR CALLBACK CWinDialog::DlgMsgHandler(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
		case WM_INITDIALOG:
			return SUCCEEDED(InternalInitDialog(hwnd));
		break;

		case WM_PAINT:
			return SUCCEEDED(OnPaint());
		break;

		case WM_COMMAND:
		{
			int ControlID = LOWORD(wParam);
			int MessageID = HIWORD(wParam);

			return SUCCEEDED(InvokeEvent(ControlID, MessageID));
		}
		break;

		case WM_NOTIFY:
		{
			int ControlID = (int)wParam;
			LPNMHDR pnmh = (LPNMHDR)lParam;

			return SUCCEEDED(InvokeEvent(ControlID, pnmh->code));
		}
		break;
	}
	return FALSE;
}

//////////////////////////////////////////////////////////////////////////
// methods
//////////////////////////////////////////////////////////////////////////
void CWinDialog::EndDialog(int Result)
{
	m_ModalResult = Result;
	m_InModalLoop = false;
	if(m_Hwnd) ::EndDialog(m_Hwnd, Result);
}