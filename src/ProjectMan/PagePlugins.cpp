// PagePlugins.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "PagePlugins.h"
#include ".\pageplugins.h"
#include "../MFCExt/utils_mfc.h"

// CPagePlugins dialog

//////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CPagePlugins, CPropertyPage)
CPagePlugins::CPagePlugins()
	: CPropertyPage(CPagePlugins::IDD)
{
	m_Game = NULL;
}

//////////////////////////////////////////////////////////////////////////
CPagePlugins::~CPagePlugins()
{
}

//////////////////////////////////////////////////////////////////////////
void CPagePlugins::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGIN_LIST, m_PluginList);
	DDX_Control(pDX, IDC_PLUGIN_INFO, m_PluginInfo);
}


BEGIN_MESSAGE_MAP(CPagePlugins, CPropertyPage)
	ON_LBN_SELCHANGE(IDC_PLUGIN_LIST, OnLbnSelchangePluginList)
	ON_LBN_DBLCLK(IDC_PLUGIN_LIST, OnLbnDblclkPluginList)
END_MESSAGE_MAP()


// CPagePlugins message handlers

//////////////////////////////////////////////////////////////////////////
BOOL CPagePlugins::OnInitDialog()
{
	CPropertyPage::OnInitDialog();
	LOCWIN(this);

	if(!m_Game) return TRUE;

	for(int i=0; i<m_Game->m_PluginMgr->m_Plugins.GetSize(); i++)
	{
		CBPlugin* Plugin = m_Game->m_PluginMgr->m_Plugins[i];
		CString ShortName = GetFName(Plugin->m_DllPath);
		
		m_PluginList.AddString(ShortName);
		
		for(int j=0; j<m_SelectedPlugins.GetSize(); j++)
		{
			if(m_SelectedPlugins[j].CompareNoCase(ShortName)==0)
				m_PluginList.SetCheck(i, 1);
		}
	}

	if(m_PluginList.GetCount() > 0)
	{
		m_PluginList.SetCurSel(0);
		OnLbnSelchangePluginList();
	}


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


//////////////////////////////////////////////////////////////////////////
void CPagePlugins::OnOK()
{
	m_SelectedPlugins.RemoveAll();
	for(int i=0; i<m_PluginList.GetCount(); i++)
	{
		if(m_PluginList.GetCheck(i)==1)
		{
			CString Item;
			m_PluginList.GetText(i, Item);
			m_SelectedPlugins.Add(Item);
		}
	}

	CPropertyPage::OnOK();
}

//////////////////////////////////////////////////////////////////////////
void CPagePlugins::OnLbnSelchangePluginList()
{
	CString Info;
	int Index = m_PluginList.GetCurSel();
	if(Index>=0 && Index<m_Game->m_PluginMgr->m_Plugins.GetSize())
	{
		CBPlugin* Plugin = m_Game->m_PluginMgr->m_Plugins[Index];		
		Info = LOC("/str1119/Description:") + CString(" ") + Plugin->m_PluginInfo.Description;
		Info += "\r\n";
		Info += LOC("/str1120/Version:") + CString(" ") + Plugin->m_PluginInfo.PluginVersion;
		Info += "\r\n";
		Info += LOC("/str1121/WME version:") + CString(" ") + Plugin->m_PluginInfo.WmeVersion;
		Info += "\r\n";
		Info += LOC("/str1122/Full path:") + CString(" ") + Plugin->m_DllPath;
		Info += "\r\n";
		Info += LOC("/str1123/Supported classes:") + CString(" ");
		for(int i=0; i<Plugin->m_Classes.GetSize(); i++)
		{
			Info += CString(Plugin->m_Classes[i]);
			if(i < Plugin->m_Classes.GetSize()-1) Info += ", ";
		}
	}
	else Info = "";

	m_PluginInfo.SetWindowText(Info);
}

//////////////////////////////////////////////////////////////////////////
void CPagePlugins::OnLbnDblclkPluginList()
{
	int Index = m_PluginList.GetCurSel();
	if(Index >= 0)
	{
		if(m_PluginList.GetCheck(Index)==0) m_PluginList.SetCheck(Index, 1);
		else m_PluginList.SetCheck(Index, 0);
	}
}
