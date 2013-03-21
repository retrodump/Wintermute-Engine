  // ViewProps.cpp : implementation file
//

#include "stdafx.h"
#include "ProjectMan.h"
#include "ViewProps.h"
#include "ViewHint.h"
#include "resource.h"
#include "../MFCExt/utils_mfc.h"
#include "../MFCExt/ScriptDlg.h"
#include "ViewportDlg.h"
#include "SaveLoadDlg.h"
#include "GuidDlg.h"
#include "CompatDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewProps

IMPLEMENT_DYNCREATE(CViewProps, CFormViewEx)


//////////////////////////////////////////////////////////////////////////
CViewProps::CViewProps()
	: CFormViewEx(CViewProps::IDD)
{
	//{{AFX_DATA_INIT(CViewProps)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_View = NULL;
	m_CurrentPackage = NULL;
}


//////////////////////////////////////////////////////////////////////////
CViewProps::~CViewProps()
{
	m_View = NULL;
	m_CurrentPackage = NULL;
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::DoDataExchange(CDataExchange* pDX)
{
	CFormViewEx::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewProps)
	DDX_Control(pDX, IDC_PROP_LIST, m_PropList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewProps, CFormViewEx)
	//{{AFX_MSG_MAP(CViewProps)
	ON_WM_SIZE()
	ON_COMMAND(ID_POPUP_FILE_BROWSE, OnPopupFileBrowse)
	ON_COMMAND(ID_POPUP_FILE_EDIT, OnPopupFileEdit)
	ON_COMMAND(ID_POPUP_FILE_REMOVE, OnPopupFileRemove)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FILE_EDIT, OnUpdatePopupFileEdit)
	ON_UPDATE_COMMAND_UI(ID_POPUP_FILE_REMOVE, OnUpdatePopupFileRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewProps diagnostics

#ifdef _DEBUG
void CViewProps::AssertValid() const
{
	CFormViewEx::AssertValid();
}

void CViewProps::Dump(CDumpContext& dc) const
{
	CFormViewEx::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewProps message handlers


//////////////////////////////////////////////////////////////////////////
BOOL CViewProps::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
	if(m_View && m_View->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo)) return TRUE;
	else return CFormViewEx::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



//////////////////////////////////////////////////////////////////////////
HRESULT CViewProps::Redraw()
{
	m_PropList.Cleanup();

	DisplayProps();
	
	m_PropList.Populate(true);
	m_PropList.Redraw(true);


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CViewProps::DisplayProps()
{
	if(!m_View) return E_FAIL;

	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->m_Settings) return E_FAIL;


	CPLPropertyEdit* pEdit;
	CPLPropertyCombo* pCombo;
	CPLPropertyEditEx* pEditEx;
	CPLCategory* cat;
	CPLPropertyColor* pColor;

	CString str;


	//////////////////////////////////////////////////////////////////////////
	cat = m_PropList.AddCategory("startup", LOC("/str0001/Startup settings"));
	
	pEdit = new CPLPropertyEdit("resolution-x", LOC("/str0002/Resolution - width"));
	pEdit->m_HelpFile = "resolution";
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 320, 3000);
	str.Format("%d", Doc->m_Settings->m_ResolutionWidth);
	pEdit->SetValue(str);

	pEdit = new CPLPropertyEdit("resolution-y", LOC("/str0003/Resolution - height"));
	pEdit->m_HelpFile = "resolution";
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 200, 3000);
	str.Format("%d", Doc->m_Settings->m_ResolutionHeight);
	pEdit->SetValue(str);

	pEdit = new CPLPropertyEdit("reg-path", LOC("/str0005/Registry path"));
	m_PropList.AddProperty(cat, pEdit);
	if(Doc->m_Settings->m_RegPath) pEdit->SetValue(Doc->m_Settings->m_RegPath);
	else pEdit->SetValue("");
/*
	pCombo = new CPLPropertyCombo("require-accel", LOC("/str0006/Require 3D accel."));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_RequireAcceleration?LOC("/str1000/Yes"):LOC("/str1001/No"));
*/
	pCombo = new CPLPropertyCombo("require-sound", LOC("/str1144/Require sound"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_RequireSound?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("hwtl-mode", LOC("/str1145/Hardware T&&L"));
	pCombo->AddValue(LOC("/str1146/don't use"), "0");
	pCombo->AddValue(LOC("/str1147/use when available"), "1");
	pCombo->AddValue(LOC("/str1148/ask user"), "2");
	m_PropList.AddProperty(cat,  pCombo);
	switch(Doc->m_Settings->m_TLMode)
	{
	case 0:
		pCombo->SetValue("0");
		break;
	case 2:
		pCombo->SetValue("2");
		break;
	default:
		pCombo->SetValue("1");
	}	

	pCombo = new CPLPropertyCombo("use-d3d9", LOC("/str1149/Use Direct3D 9"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_D3D9?LOC("/str1000/Yes"):LOC("/str1001/No"));


	pCombo = new CPLPropertyCombo("allow-windowed", LOC("/str0007/Allow windowed"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_AllowWindowed?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("allow-advanced", LOC("/str1115/Allow 3D settings"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_AllowAdvanced?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("allow-desktop-res", LOC("/str1186/Allow desktop resolution"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_AllowDesktopRes?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("allow-access-tab", LOC("/str1126/Allow accessibility"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_AllowAccessTab?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("allow-about-tab", LOC("/str1127/Allow about tab"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_AllowAboutTab?LOC("/str1000/Yes"):LOC("/str1001/No"));


	pEditEx = new CPLPropertyEditEx("string-table", LOC("/str0008/String table"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	pEditEx->SetValue(Doc->m_Settings->m_StringTableFilename);


	//////////////////////////////////////////////////////////////////////////
	cat = m_PropList.AddCategory("game", LOC("/str0009/Game settings"));

	pEdit = new CPLPropertyEdit("caption", LOC("/str0010/Caption"));
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValue(Doc->Game->m_Caption[0]);

	pEdit = new CPLPropertyEdit("name", LOC("/str0011/Name"));
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValue(Doc->Game->m_Name);

	pEditEx = new CPLPropertyEditEx("startup-scene", LOC("/str1087/Startup scene"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_StartupScene)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_StartupScene);

	pEditEx = new CPLPropertyEditEx("scripts", LOC("/str0012/Scripts"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonScripts, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->Game->m_Scripts.GetSize()==0) str = LOC("/str0013/[NO SCRIPTS]");
	else{
		str.Format("[%d %s]", Doc->Game->m_Scripts.GetSize(), Doc->Game->m_Scripts.GetSize()>1?LOC("/str0014/scripts"):LOC("/str0015/script"));
	}
	pEditEx->SetValue(str);


	pCombo = new CPLPropertyCombo("subtitles", LOC("/str0017/Speech subtitles"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_EdGame->m_Subtitles?LOC("/str1000/Yes"):LOC("/str1001/No"));


	pCombo = new CPLPropertyCombo("video-subtitles", LOC("/str0018/Video subtitles"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_EdGame->m_VideoSubtitles?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pEdit = new CPLPropertyEdit("subtitles-speed", LOC("/str0019/Subtitles speed"));
	m_PropList.AddProperty(cat, pEdit);
	CString val;
	val.Format("%d", Doc->m_Settings->m_EdGame->m_SubtitlesSpeed);
	pEdit->SetValue(val);

	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 10, 1000);


	pEditEx = new CPLPropertyEditEx("system-font", LOC("/str0020/System font"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_SystemFont)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_SystemFont);

	pEditEx = new CPLPropertyEditEx("video-font", LOC("/str0021/Video font"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_VideoFont)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_VideoFont);

	pEditEx = new CPLPropertyEditEx("cursor", LOC("/str0022/Cursor"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_Cursor)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_Cursor);

	pEditEx = new CPLPropertyEditEx("active-cursor", LOC("/str0023/Active cursor"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_ActiveCursor)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_ActiveCursor);

	pEditEx = new CPLPropertyEditEx("nonint-cursor", LOC("/str0024/Nonint.cursor"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_NoninteractiveCursor)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_NoninteractiveCursor);


	pEditEx = new CPLPropertyEditEx("inventory", LOC("/str0025/Inventory window"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_InventoryFile)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_InventoryFile);


	pEditEx = new CPLPropertyEditEx("response", LOC("/str0026/Response window"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_ResponseFile)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_ResponseFile);


	pEditEx = new CPLPropertyEditEx("items", LOC("/str0027/Items definition"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_ItemsFile)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_ItemsFile);


	pCombo = new CPLPropertyCombo("talk-skip", LOC("/str0028/Talk lines skipped by"));
	pCombo->AddValue(LOC("/str0029/left mouse button"), "left");
	pCombo->AddValue(LOC("/str0030/right mouse button"), "right");
	pCombo->AddValue(LOC("/str0031/both mouse buttons"), "both");
	m_PropList.AddProperty(cat,  pCombo);

	if(Doc->m_Settings->m_EdGame->m_TalkSkipButton==TALK_SKIP_RIGHT)
		pCombo->SetValue("right");
	else if(Doc->m_Settings->m_EdGame->m_TalkSkipButton==TALK_SKIP_BOTH)
		pCombo->SetValue("both");
	else
		pCombo->SetValue("left");


	pEditEx = new CPLPropertyEditEx("viewport", LOC("/str0032/Scene viewport"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonViewport, this);
	m_PropList.AddProperty(cat, pEditEx);

	RECT* rc = &Doc->m_Settings->m_EdGame->m_SceneViewport;
	if(IsRectEmpty(rc)) str = LOC("/str0033/[fullscreen]");
	else{		
		str.Format(CString("[") + LOC("/str0034/Pos:") + "%d,%d  " + LOC("/str0035/Size:") + "%dx%d]", rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top);
	}
	pEditEx->SetValue(str);


	pEditEx = new CPLPropertyEditEx("guid", LOC("/str1151/GUID"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonGUID, this);
	m_PropList.AddProperty(cat, pEditEx);

	if(Doc->m_Settings->m_GameGUID==GUID_NULL) str = "";
	else
	{
		LPOLESTR  olestr;
		StringFromIID(Doc->m_Settings->m_GameGUID, &olestr);
		str = CString(olestr);
		CoTaskMemFree(olestr);
	}
	pEditEx->SetValue(str);


	pEditEx = new CPLPropertyEditEx("compat", LOC("/str1177/Compatibility"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonCompat, this);
	m_PropList.AddProperty(cat, pEditEx);


	//////////////////////////////////////////////////////////////////////////
	cat = m_PropList.AddCategory("save", LOC("/str1091/Saved games"));
	cat->m_Expanded = false;

	pCombo = new CPLPropertyCombo("personal-save", LOC("/str0016/Personal savegames"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_EdGame->m_PersonalizedSave?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pEdit = new CPLPropertyEdit("save-path", LOC("/str1116/Save directory"));
	m_PropList.AddProperty(cat, pEdit);
	if(Doc->m_Settings->m_EdGame->m_LocalSaveDir) pEdit->SetValue(Doc->m_Settings->m_EdGame->m_LocalSaveDir);
	else pEdit->SetValue("");


	pEdit = new CPLPropertyEdit("thumbnail-width", LOC("/str1092/Thumbnail width"));
	pEdit->m_HelpFile = "thumbnail";
	m_PropList.AddProperty(cat, pEdit);	
	val.Format("%d", Doc->m_Settings->m_EdGame->m_ThumbnailWidth);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 0, 400);


	pEdit = new CPLPropertyEdit("thumbnail-height", LOC("/str1093/Thumbnail height"));
	pEdit->m_HelpFile = "thumbnail";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", Doc->m_Settings->m_EdGame->m_ThumbnailHeight);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 0, 400);

	pEdit = new CPLPropertyEdit("indicator-x", LOC("/str1097/Indicator X"));
	pEdit->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", Doc->m_Settings->m_EdGame->m_IndicatorX);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_SIGNED, false);

	pEdit = new CPLPropertyEdit("indicator-y", LOC("/str1098/Indicator Y"));
	pEdit->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", Doc->m_Settings->m_EdGame->m_IndicatorY);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_SIGNED, false);

	pEdit = new CPLPropertyEdit("indicator-width", LOC("/str1099/Indicator Width"));
	pEdit->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", Doc->m_Settings->m_EdGame->m_IndicatorWidth);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_SIGNED, false);

	pEdit = new CPLPropertyEdit("indicator-height", LOC("/str1100/Indicator Height"));
	pEdit->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", Doc->m_Settings->m_EdGame->m_IndicatorHeight);
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_SIGNED, false);

	pColor = new CPLPropertyColor("indicator-color", LOC("/str1101/Indicator Color"));
	pColor->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pColor);
	pColor->SetRGB(D3DCOLGetR(Doc->m_Settings->m_EdGame->m_IndicatorColor), D3DCOLGetG(Doc->m_Settings->m_EdGame->m_IndicatorColor), D3DCOLGetB(Doc->m_Settings->m_EdGame->m_IndicatorColor));


	pEdit = new CPLPropertyEdit("indicator-alpha", LOC("/str1102/Indicator Transparency"));
	pEdit->m_HelpFile = "indicator";
	m_PropList.AddProperty(cat, pEdit);
	val.Format("%d", D3DCOLGetA(Doc->m_Settings->m_EdGame->m_IndicatorColor));
	pEdit->SetValue(val);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 0, 255);


	pEditEx = new CPLPropertyEditEx("save-load-image", LOC("/str1104/Save/load screen"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonSaveLoad, this);
	m_PropList.AddProperty(cat, pEditEx);

	if(Doc->m_Settings->m_EdGame->m_SaveImageName || Doc->m_Settings->m_EdGame->m_LoadImageName)
		str = LOC("/str1105/<set>");
	else
		str = LOC("/str1106/<not set>");
	pEditEx->SetValue(str);

	pEdit = new CPLPropertyEdit("save-ext", LOC("/str1158/File extension"));
	m_PropList.AddProperty(cat, pEdit);
	if(Doc->m_Settings->m_SavedGameExt) pEdit->SetValue(Doc->m_Settings->m_SavedGameExt);
	else pEdit->SetValue("");


	pCombo = new CPLPropertyCombo("rich-save", LOC("/str1157/Rich saved games"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_Settings->m_RichSavedGames?LOC("/str1000/Yes"):LOC("/str1001/No"));


	//////////////////////////////////////////////////////////////////////////
	cat = m_PropList.AddCategory("debug", LOC("/str0036/Debugging settings"));

	pCombo = new CPLPropertyCombo("debug-mode", LOC("/str0037/Debug mode"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_DebugMode?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("show-fps", LOC("/str0038/Show FPS"));
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_ShowFPS?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("allow-windowed", LOC("/str0039/Allow windowed"));
	pCombo->m_HelpFile = "allow-windowed-d";
	pCombo->SetupBool();
	m_PropList.AddProperty(cat,  pCombo);
	pCombo->SetValue(Doc->m_AllowWindowed?LOC("/str1000/Yes"):LOC("/str1001/No"));

	pCombo = new CPLPropertyCombo("console-mode", LOC("/str1129/Debugging console"));
	pCombo->AddValue(LOC("/str1130/Never"), "0");
	pCombo->AddValue(LOC("/str1131/In windowed mode"), "1");
	pCombo->AddValue(LOC("/str1132/Always"), "2");
	m_PropList.AddProperty(cat,  pCombo);
	switch(Doc->m_ConsoleMode)
	{
		case 0:
			pCombo->SetValue("0");
			break;
		case 2:
			pCombo->SetValue("2");
			break;
		default:
			pCombo->SetValue("1");
	}	

	pEditEx = new CPLPropertyEditEx("startup-scene-d", LOC("/str1087/Startup scene"));
	pEditEx->m_ReadOnly = true;
	pEditEx->SetButtonPressCallback(HookButtonFile, this);
	m_PropList.AddProperty(cat, pEditEx);
	if(Doc->m_Settings->m_EdGame->m_DebugStartupScene)
		pEditEx->SetValue(Doc->m_Settings->m_EdGame->m_DebugStartupScene);


	//////////////////////////////////////////////////////////////////////////
	cat = m_PropList.AddCategory("package", LOC("/str0040/Selected package"));

	pEdit = new CPLPropertyEdit("description", LOC("/str0041/Description"));
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_ANY, true, 0, 100);

	/*
	pEdit = new CPLPropertyEdit("cd", LOC("/str0042/CD"));
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 0, 99);
	*/

	pEdit = new CPLPropertyEdit("priority", LOC("/str0043/Priority"));
	m_PropList.AddProperty(cat, pEdit);
	pEdit->SetValidation(CPLPropertyEdit::EDIT_UNSIGNED, true, 0, 200);

	m_PropList.ShowCategory("package", false);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnSize(UINT nType, int cx, int cy) 
{
	SetScrollSizes(MM_TEXT, CSize(cx, cy));
	CFormViewEx::OnSize(nType, cx, cy);
	SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOMOVE|SWP_NOZORDER|SWP_NOACTIVATE);
		
	if(m_PropList.GetSafeHwnd()){
		m_PropList.SetWindowPos(NULL, 0, 0, cx, cy, SWP_NOZORDER|SWP_NOACTIVATE);
		m_PropList.Redraw();
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnInitialUpdate() 
{
	CFormViewEx::OnInitialUpdate();
	
	m_PropList.SetPropChangeCallback(HookPropChange, this);
	m_PropList.SetPropFocusCallback(HookPropFocus, this);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::HookPropChange(CPLProperty *Prop, CPLCategory *Cat, void *Data)
{
	((CViewProps*)Data)->OnPropChange(Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::HookPropFocus(CPLProperty *Prop, CPLCategory *Cat, void *Data)
{
	((CViewProps*)Data)->OnPropFocus(Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonScripts(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonScripts(Button, Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonViewport(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonViewport(Button, Prop, Cat);
}

//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonSaveLoad(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonSaveLoad(Button, Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonFile(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonFile(Button, Prop, Cat);
}

//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonGUID(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonGUID(Button, Prop, Cat);
}

//////////////////////////////////////////////////////////////////////////
bool CViewProps::HookButtonCompat(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat, void *Data)
{
	return ((CViewProps*)Data)->OnButtonCompat(Button, Prop, Cat);
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonScripts(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	if(!m_View) return false;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->Game) return false;
	
	CScriptDlg dlg;
	dlg.m_Object = (CBObject*)Doc->Game;
	dlg.m_View = m_View;
	if(dlg.DoModal()==IDOK){
		CString str;
		if(dlg.m_ScriptNames.GetSize()==0) str = LOC("/str0013/[NO SCRIPTS]");
		else str.Format("[%d %s]", dlg.m_ScriptNames.GetSize(), dlg.m_ScriptNames.GetSize()>1?LOC("/str0014/scripts"):LOC("/str0015/script"));
		Prop->SetValue(str);
		m_PropList.Redraw();
	}
	return false;
}



//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonViewport(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	if(!m_View) return false;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->Game) return false;

	CViewportDlg dlg;
	dlg.m_Rect = Doc->m_Settings->m_EdGame->m_SceneViewport;
	if(dlg.DoModal()==IDOK){
		Doc->m_Settings->m_EdGame->m_SceneViewport = dlg.m_Rect;

		CString str;
		RECT* rc = &Doc->m_Settings->m_EdGame->m_SceneViewport;
		if(IsRectEmpty(rc)) str = LOC("/str0033/[fullscreen]");
		else{		
			str.Format(CString("[") + LOC("/str0034/Pos:") + "%d,%d  " + LOC("/str0035/Size:") + "%dx%d]", rc->left, rc->top, rc->right - rc->left, rc->bottom - rc->top);
		}
		Prop->SetValue(str);
		
		m_PropList.Redraw();
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonSaveLoad(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	if(!m_View) return false;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->Game) return false;

	CSaveLoadDlg dlg;
	dlg.m_SaveX = Doc->m_Settings->m_EdGame->m_SaveImageX;
	dlg.m_SaveY = Doc->m_Settings->m_EdGame->m_SaveImageY;

	dlg.m_LoadX = Doc->m_Settings->m_EdGame->m_LoadImageX;
	dlg.m_LoadY = Doc->m_Settings->m_EdGame->m_LoadImageY;

	if(Doc->m_Settings->m_EdGame->m_SaveImageName) dlg.m_SaveFile = Doc->m_Settings->m_EdGame->m_SaveImageName;
	if(Doc->m_Settings->m_EdGame->m_LoadImageName) dlg.m_LoadFile = Doc->m_Settings->m_EdGame->m_LoadImageName;

	dlg.Game = Doc->Game;

	if(dlg.DoModal()==IDOK){
		Doc->m_Settings->m_EdGame->m_SaveImageX = dlg.m_SaveX;
		Doc->m_Settings->m_EdGame->m_SaveImageY = dlg.m_SaveY;

		Doc->m_Settings->m_EdGame->m_LoadImageX = dlg.m_LoadX;
		Doc->m_Settings->m_EdGame->m_LoadImageY = dlg.m_LoadY;

		if(dlg.m_SaveFile.Trim()!="") CBUtils::SetString(&Doc->m_Settings->m_EdGame->m_SaveImageName, (char*)LPCSTR(dlg.m_SaveFile.Trim()));
		else SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_SaveImageName);

		if(dlg.m_LoadFile.Trim()!="") CBUtils::SetString(&Doc->m_Settings->m_EdGame->m_LoadImageName, (char*)LPCSTR(dlg.m_LoadFile.Trim()));
		else SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_LoadImageName);


		CString str;
		if(Doc->m_Settings->m_EdGame->m_SaveImageName || Doc->m_Settings->m_EdGame->m_LoadImageName)
			str = LOC("/str1105/<set>");
		else
			str = LOC("/str1106/<not set>");

		Prop->SetValue(str);
		m_PropList.Redraw();
		Doc->SetModifiedFlag(TRUE);
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonFile(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(!prop) return false;

	if(prop->GetValue()=="") OnPopupFileBrowse();
	else{
		RECT rc;
		Button->GetWindowRect(&rc);
		ShowPopupMenu(IDR_POPUP_FILE, IDR_POPUP_FILE, &rc);
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonGUID(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	if(!m_View) return false;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->Game) return false;

	CGuidDlg dlg;
	dlg.m_Guid = Doc->m_Settings->m_GameGUID;
	if(dlg.DoModal()==IDOK)
	{
		Doc->m_Settings->m_GameGUID = dlg.m_Guid;

		CString str;
		LPOLESTR  olestr;
		StringFromIID(Doc->m_Settings->m_GameGUID, &olestr);
		str = CString(olestr);
		CoTaskMemFree(olestr);

		Prop->SetValue(str);
		m_PropList.Redraw();
	}

	return false;
}

//////////////////////////////////////////////////////////////////////////
bool CViewProps::OnButtonCompat(CButton* Button, CPLPropertyEditEx *Prop, CPLCategory *Cat)
{
	if(!m_View) return false;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc || !Doc->Game) return false;

	CCompatDlg dlg;
	dlg.m_CompatKillMethodThreads = Doc->m_Settings->m_EdGame->m_CompatKillMethodThreads;
	if(dlg.DoModal()==IDOK)
	{
		Doc->m_Settings->m_EdGame->m_CompatKillMethodThreads = (dlg.m_CompatKillMethodThreads!=FALSE);
		return true;
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
void CViewProps::OnPopupFileBrowse() 
{

	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(!prop) return;

	if(!m_View) return;
	CProjectDoc* Doc = m_View->GetDocument();
	
	if(prop->GetID()=="string-table"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_StringTableFilename, CString(LOC("/str1074/String tables")) + " (*.tab; *.txt)|*.tab;*.txt");
	}		
	else if(prop->GetID()=="system-font"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_SystemFont, CString(LOC("/str1075/Font files")) + " (*.fnt; *.font)|*.fnt;*.font");
	}
	else if(prop->GetID()=="video-font"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_VideoFont, CString(LOC("/str1075/Font files")) + " (*.fnt; *.font)|*.fnt;*.font");
	}
	else if(prop->GetID()=="cursor"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_Cursor, CString(LOC("/str1076/Sprite files")) + " (*.sprite; *.bmp; *.tga; *.png; *.jpg)|*.sprite;*.bmp;*.tga;*.png;*.jpg");
	}
	else if(prop->GetID()=="active-cursor"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_ActiveCursor, CString(LOC("/str1076/Sprite files")) + " (*.sprite; *.bmp; *.tga; *.png; *.jpg)|*.sprite;*.bmp;*.tga;*.png;*.jpg");
	}
	else if(prop->GetID()=="nonint-cursor"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_NoninteractiveCursor, CString(LOC("/str1076/Sprite files")) + " (*.sprite; *.bmp; *.tga; *.png; *.jpg)|*.sprite;*.bmp;*.tga;*.png;*.jpg");
	}
	else if(prop->GetID()=="inventory"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_InventoryFile, CString(LOC("/str1077/Definition files")) + " (*.def)|*.def");
	}
	else if(prop->GetID()=="response"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_ResponseFile, CString(LOC("/str1077/Definition files")) + " (*.def)|*.def");
	}
	else if(prop->GetID()=="items"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_ItemsFile, CString(LOC("/str1078/Items files")) + " (*.items)|*.items");
	}
	else if(prop->GetID()=="startup-scene"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_StartupScene, CString(LOC("/str1088/Scene files")) + " (*.scene)|*.scene");
	}
	else if(prop->GetID()=="startup-scene-d"){
		BrowseFile(Doc, prop, &Doc->m_Settings->m_EdGame->m_DebugStartupScene, CString(LOC("/str1088/Scene files")) + " (*.scene)|*.scene");
	}

	m_PropList.Redraw();
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnPopupFileEdit() 
{
	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(!prop) return;

	EditFile(prop->GetValue(), m_View->Game, this);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnPopupFileRemove() 
{
	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(!prop) return;

	CProjectDoc* Doc = m_View->GetDocument();


	if(MessageBox(LOC("/str0048/Do you really want to remove the currently assigned file?"), NULL, MB_ICONQUESTION|MB_YESNO)==IDYES){

		if(prop->GetID()=="string-table"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_StringTableFilename);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}		
		else if(prop->GetID()=="system-font"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_SystemFont);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="video-font"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_VideoFont);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="cursor"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_Cursor);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="active-cursor"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_ActiveCursor);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="nonint-cursor"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_NoninteractiveCursor);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="inventory"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_InventoryFile);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="response"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_ResponseFile);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="items"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_ItemsFile);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="startup-scene"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_StartupScene);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}
		else if(prop->GetID()=="startup-scene-d"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_EdGame->m_DebugStartupScene);
			Doc->SetModifiedFlag();
			prop->SetValue("");			
		}

		m_PropList.Redraw();
	}
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnUpdatePopupFileEdit(CCmdUI* pCmdUI) 
{
	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(prop && prop->GetValue()!="")pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::OnUpdatePopupFileRemove(CCmdUI* pCmdUI) 
{
	CPLProperty* prop = m_PropList.GetProperty(m_PropList.GetCurSel());
	if(prop && prop->GetValue()!="")pCmdUI->Enable();
	else pCmdUI->Enable(FALSE);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::BrowseFile(CProjectDoc* Doc, CPLProperty* prop, char** pFilename, CString Mask, CString DefaultDir)
{
	CString OrigFile;
	if(*pFilename) OrigFile = CString(*pFilename);
	CString NewFile = GetRelativeFilename(Doc->Game, OrigFile, Mask, DefaultDir, this);
	if(NewFile!=""){
		Doc->SetModifiedFlag();
		SAFE_DELETE_ARRAY(*pFilename);
		*pFilename = new char[NewFile.GetLength()+1];
		strcpy(*pFilename, LPCSTR(NewFile));

		prop->SetValue(NewFile);
	}
	
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CViewProps::OnPropChange(CPLProperty *Prop, CPLCategory *Cat)
{
	if(!m_View) return;
	CProjectDoc* Doc = m_View->GetDocument();
	if(!Doc) return;

	Doc->SetModifiedFlag();

	//////////////////////////////////////////////////////////////////////////
	if(Cat->GetID()=="startup"){

		if(Prop->GetID()=="resolution-x"){
			Doc->m_Settings->m_ResolutionWidth = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="resolution-y"){
			Doc->m_Settings->m_ResolutionHeight = atoi(Prop->GetValue());
		}
		
		else if(Prop->GetID()=="reg-path"){
			SAFE_DELETE_ARRAY(Doc->m_Settings->m_RegPath);
			Doc->m_Settings->m_RegPath = new char[strlen(Prop->GetValue())+1];
			strcpy(Doc->m_Settings->m_RegPath, Prop->GetValue());
			//CopyStr(Doc->m_Settings->m_RegPath, Prop->GetValue());
		}

		else if(Prop->GetID()=="require-accel"){
			Doc->m_Settings->m_RequireAcceleration = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="require-sound"){
			Doc->m_Settings->m_RequireSound = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="hwtl-mode"){
			Doc->m_Settings->m_TLMode = ((CPLPropertyCombo*)Prop)->GetValueInt();
		}

		else if(Prop->GetID()=="use-d3d9"){
			Doc->m_D3D9 = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="allow-windowed"){
			Doc->m_Settings->m_AllowWindowed = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="allow-desktop-res"){
			Doc->m_Settings->m_AllowDesktopRes = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="allow-advanced"){
			Doc->m_Settings->m_AllowAdvanced = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}
		
		else if(Prop->GetID()=="allow-access-tab"){
			Doc->m_Settings->m_AllowAccessTab = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="allow-about-tab"){
			Doc->m_Settings->m_AllowAboutTab = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	else if(Cat->GetID()=="game"){

		if(Prop->GetID()=="caption"){
			Doc->Game->SetCaption((char*)LPCSTR(Prop->GetValue()));
		}

		else if(Prop->GetID()=="name"){
			Doc->Game->SetName((char*)LPCSTR(Prop->GetValue()));
		}

		else if(Prop->GetID()=="subtitles"){
			Doc->m_Settings->m_EdGame->m_Subtitles = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="video-subtitles"){
			Doc->m_Settings->m_EdGame->m_VideoSubtitles = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="subtitles-speed"){
			Doc->m_Settings->m_EdGame->m_SubtitlesSpeed = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="talk-skip"){
			if(Prop->GetValue()=="right") Doc->m_Settings->m_EdGame->m_TalkSkipButton = TALK_SKIP_RIGHT;
			else if(Prop->GetValue()=="both") Doc->m_Settings->m_EdGame->m_TalkSkipButton = TALK_SKIP_BOTH;
			else Doc->m_Settings->m_EdGame->m_TalkSkipButton = TALK_SKIP_LEFT;
		}
	
	}

	
	//////////////////////////////////////////////////////////////////////////
	else if(Cat->GetID()=="save"){

		if(Prop->GetID()=="personal-save"){
			Doc->m_Settings->m_EdGame->m_PersonalizedSave = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="save-path"){
			CBUtils::SetString(&Doc->m_Settings->m_EdGame->m_LocalSaveDir, (char*)LPCSTR(Prop->GetValue()));
		}

		else if(Prop->GetID()=="thumbnail-width"){
			Doc->m_Settings->m_EdGame->m_ThumbnailWidth = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="thumbnail-height"){
			Doc->m_Settings->m_EdGame->m_ThumbnailHeight = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="indicator-x"){
			Doc->m_Settings->m_EdGame->m_IndicatorX = atoi(Prop->GetValue());
		}
		
		else if(Prop->GetID()=="indicator-y"){
			Doc->m_Settings->m_EdGame->m_IndicatorY = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="indicator-width"){
			Doc->m_Settings->m_EdGame->m_IndicatorWidth = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="indicator-height"){
			Doc->m_Settings->m_EdGame->m_IndicatorHeight = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="indicator-color"){
			BYTE r, g, b;
			CPLPropertyColor* ColProp = (CPLPropertyColor*)Prop;
			ColProp->GetRGB(r, g, b);
			Doc->m_Settings->m_EdGame->m_IndicatorColor = DRGBA(r, g, b, D3DCOLGetA(Doc->m_Settings->m_EdGame->m_IndicatorColor));
		}

		else if(Prop->GetID()=="indicator-alpha"){
			Doc->m_Settings->m_EdGame->m_IndicatorColor = DRGBA(D3DCOLGetR(Doc->m_Settings->m_EdGame->m_IndicatorColor), D3DCOLGetG(Doc->m_Settings->m_EdGame->m_IndicatorColor), D3DCOLGetB(Doc->m_Settings->m_EdGame->m_IndicatorColor), atoi(Prop->GetValue()));
		}

		else if(Prop->GetID()=="save-ext"){
			CBUtils::SetString(&Doc->m_Settings->m_SavedGameExt, (char*)LPCSTR(Prop->GetValue()));
		}

		else if(Prop->GetID()=="rich-save"){
			Doc->m_Settings->m_RichSavedGames = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

	}


	//////////////////////////////////////////////////////////////////////////
	else if(Cat->GetID()=="debug"){

		if(Prop->GetID()=="debug-mode"){
			Doc->m_DebugMode = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="show-fps"){
			Doc->m_ShowFPS = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="allow-windowed"){
			Doc->m_AllowWindowed = (((CPLPropertyCombo*)Prop)->GetValueInt()!=0);
		}

		else if(Prop->GetID()=="console-mode"){
			Doc->m_ConsoleMode = ((CPLPropertyCombo*)Prop)->GetValueInt();
		}

	}


	//////////////////////////////////////////////////////////////////////////
	else if(Cat->GetID()=="package" && m_CurrentPackage){

		if(Prop->GetID()=="description"){
			m_CurrentPackage->m_Description = Prop->GetValue();
		}

		else if(Prop->GetID()=="cd"){
			m_CurrentPackage->m_CD = atoi(Prop->GetValue());
		}

		else if(Prop->GetID()=="priority"){
			m_CurrentPackage->m_Priority = atoi(Prop->GetValue());
		}

	}

}


//////////////////////////////////////////////////////////////////////////
void CViewProps::CopyStr(char *Dest, const char *Src)
{
	// BUGBUG
	SAFE_DELETE_ARRAY(Dest);
	Dest = new char[strlen(Src)+1];
	strcpy(Dest, Src);
}


//////////////////////////////////////////////////////////////////////////
void CViewProps::SetCurrentPackage(CPackage *Package)
{
	if(true || Package!=m_CurrentPackage){
		m_CurrentPackage = Package;

		if(m_CurrentPackage==NULL) m_PropList.ShowCategory("package", false);
		else{
			CString val;
			CPLPropertyEdit* prop;

			prop =  m_PropList.GetPropertyEdit("description", "package");
			prop->SetValue(m_CurrentPackage->m_Description);

			/*
			prop =  m_PropList.GetPropertyEdit("cd", "package");
			val.Format("%d", m_CurrentPackage->m_CD);
			prop->SetValue(val);
			*/

			prop =  m_PropList.GetPropertyEdit("priority", "package");
			val.Format("%d", m_CurrentPackage->m_Priority);
			prop->SetValue(val);
			
			m_PropList.ShowCategory("package", true, false);
			m_PropList.Populate();
			m_PropList.Redraw(true);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
void CViewProps::OnPropFocus(CPLProperty *Prop, CPLCategory *Cat)
{
	if(!m_View) return;

	if(m_View && m_View->m_ViewHint){
		CString HintFilename;
		if(Prop->m_HelpFile!="") HintFilename = Prop->m_HelpFile;
		else HintFilename = Prop->m_ID;

		HintFilename += ".html";

		m_View->m_ViewHint->DisplayHint(HintFilename);
	}
}