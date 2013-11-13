// AnimDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SceneEdit.h"
#include "AnimDlg.h"


// CAnimDlg dialog

IMPLEMENT_DYNAMIC(CAnimDlg, CDialog)

//////////////////////////////////////////////////////////////////////////
CAnimDlg::CAnimDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimDlg::IDD, pParent)
{
	m_Actor = NULL;
}

//////////////////////////////////////////////////////////////////////////
CAnimDlg::~CAnimDlg()
{
}

//////////////////////////////////////////////////////////////////////////
void CAnimDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ANIM_LIST, m_AnimList);
	DDX_Control(pDX, IDOK, m_BtnOK);
}


BEGIN_MESSAGE_MAP(CAnimDlg, CDialog)
	ON_LBN_SELCHANGE(IDC_ANIM_LIST, &CAnimDlg::OnAnimSelChanged)
	ON_LBN_DBLCLK(IDC_ANIM_LIST, &CAnimDlg::OnAnimSelDblClick)
END_MESSAGE_MAP()


// CAnimDlg message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CAnimDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_AnimList.ResetContent();
	if(m_Actor && m_Actor->m_ModelX)
	{
		int SelIndex = -1;
		for(int i=0; i < m_Actor->m_ModelX->m_AnimationSets.GetSize(); i++)
		{
			CXAnimationSet* AnimSet = m_Actor->m_ModelX->m_AnimationSets[i];
			if(AnimSet->m_Name)
			{
				AnimSet->m_Looping = true;
				m_AnimList.AddString(AnimSet->m_Name);

				if(m_Actor->m_IdleAnimName)
				{
					if(stricmp(m_Actor->m_IdleAnimName, AnimSet->m_Name)==0) SelIndex = i;
				}
			}
		}
		if(SelIndex>=0) m_AnimList.SetCurSel(SelIndex);
	}
	SetState();

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////
void CAnimDlg::SetState()
{
	if(m_AnimList.GetCurSel()!=LB_ERR) m_BtnOK.EnableWindow(TRUE);
	else m_BtnOK.EnableWindow(FALSE);
}

//////////////////////////////////////////////////////////////////////////
void CAnimDlg::OnAnimSelChanged()
{
	SetState();
}

//////////////////////////////////////////////////////////////////////////
void CAnimDlg::OnOK()
{
	CString AnimName;
	m_AnimList.GetText(m_AnimList.GetCurSel(), AnimName);

	CBUtils::SetString(&m_Actor->m_IdleAnimName, (char*)LPCSTR(AnimName));

	CDialog::OnOK();
}

//////////////////////////////////////////////////////////////////////////
void CAnimDlg::OnAnimSelDblClick()
{
	if(m_BtnOK.IsWindowEnabled()) OnOK();
}
