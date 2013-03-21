// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "UIObject.h"



IMPLEMENT_PERSISTENT(CUIObject, false);

//////////////////////////////////////////////////////////////////////////
CUIObject::CUIObject(CBGame* inGame):CBObject(inGame)
{
	m_Back = NULL;
	m_Image = NULL;
	m_Font = NULL;
	m_Text = NULL;
	m_SharedFonts = m_SharedImages = false;

	m_Width = m_Height = 0;

	m_ListenerObject = NULL;
	m_ListenerParamObject = NULL;
	m_ListenerParamDWORD = 0;

	m_Disable = false;
	m_Visible = true;

	m_Type = UI_UNKNOWN;
	m_Parent = NULL;

	m_ParentNotify = false;

	m_FocusedWidget = NULL;

	m_CanFocus = false;
	m_NonIntMouseEvents = true;
}


//////////////////////////////////////////////////////////////////////////
CUIObject::~CUIObject()
{
	if(!Game->m_LoadInProgress) CSysClassRegistry::GetInstance()->EnumInstances(CBGame::InvalidateValues, "CScValue", (void*)this);

	if(m_Back) delete m_Back;
	if(m_Font && !m_SharedFonts) Game->m_FontStorage->RemoveFont(m_Font);

	if(m_Image && !m_SharedImages) delete m_Image;

	if(m_Text) delete [] m_Text;

	m_FocusedWidget = NULL; // ref only
}


//////////////////////////////////////////////////////////////////////////
void CUIObject::SetText(char *Text)
{
	if(m_Text) delete [] m_Text;
	m_Text = new char [strlen(Text)+1];
	if(m_Text){
		strcpy(m_Text, Text);
		for(int i=0; i<strlen(m_Text); i++){
			if(m_Text[i]=='|') m_Text[i]='\n';
		}
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::Display(int OffsetX, int OffsetY)
{
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CUIObject::SetListener(CBScriptHolder *Object, CBScriptHolder* ListenerObject, DWORD ListenerParam)
{
	m_ListenerObject = Object;
	m_ListenerParamObject = ListenerObject;
	m_ListenerParamDWORD = ListenerParam;
}


//////////////////////////////////////////////////////////////////////////
void CUIObject::CorrectSize()
{
	RECT rect;

	if(m_Width<=0){
		if(m_Image){
			m_Image->GetBoundingRect(&rect, 0, 0);
			m_Width = rect.right - rect.left;
		}
		else m_Width = 100;
	}

	if(m_Height<=0){
		if(m_Image){
			m_Image->GetBoundingRect(&rect, 0, 0);
			m_Height = rect.bottom - rect.top;
		}		
	}

	if(m_Back) m_Back->CorrectSize(&m_Width, &m_Height);
}



//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// SetFont
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "SetFont")==0){
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();

		if(m_Font) Game->m_FontStorage->RemoveFont(m_Font);
		if(Val->IsNULL())
		{
			m_Font = NULL;
			Stack->PushBool(true);
		}
		else
		{
			m_Font = Game->m_FontStorage->AddFont(Val->GetString());
			Stack->PushBool(m_Font!=NULL);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetImage
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetImage")==0){
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();

		char* Filename = Val->GetString();

		SAFE_DELETE(m_Image);		
		if(Val->IsNULL()){
			Stack->PushBool(true);
			return S_OK;
		}
		
		m_Image = new CBSprite(Game);
		if(!m_Image || FAILED(m_Image->LoadFile(Val->GetString()))){
			SAFE_DELETE(m_Image);
			Stack->PushBool(false);			
		}
		else Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetImage
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetImage")==0){
		Stack->CorrectParams(0);
		if(!m_Image || !m_Image->m_Filename) Stack->PushNULL();
		else Stack->PushString(m_Image->m_Filename);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetImageObject
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetImageObject")==0){
		Stack->CorrectParams(0);
		if(!m_Image) Stack->PushNULL();
		else Stack->PushNative(m_Image, true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Focus
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Focus")==0){
		Stack->CorrectParams(0);
		Focus();
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MoveAfter / MoveBefore
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MoveAfter")==0 || strcmp(Name, "MoveBefore")==0){
		Stack->CorrectParams(1);

		if(m_Parent && m_Parent->m_Type == UI_WINDOW){
			CUIWindow* win = (CUIWindow*)m_Parent;

			int i;
			bool found = false;			
			CScValue* val = Stack->Pop();
			// find directly
			if(val->IsNative()){
				CUIObject* widget = (CUIObject*)val->GetNative();
				for(i=0; i<win->m_Widgets.GetSize(); i++){
					if(win->m_Widgets[i]==widget){
						found = true;
						break;
					}
				}
			}
			// find by name
			else{
				char* name = val->GetString();
				for(i=0; i<win->m_Widgets.GetSize(); i++){
					if(stricmp(win->m_Widgets[i]->m_Name, name)==0){
						found = true;
						break;
					}
				}
			}

			if(found){
				bool done=false;
				for(int j=0; j<win->m_Widgets.GetSize(); j++){
					if(win->m_Widgets[j]==this){
						if(strcmp(Name, "MoveAfter")==0) i++;
						if(j>=i) j++;

						win->m_Widgets.InsertAt(i, this);
						win->m_Widgets.RemoveAt(j);						

						done = true;
						Stack->PushBool(true);
						break;
					}
				}
				if(!done) Stack->PushBool(false);
			}
			else Stack->PushBool(false);

		}
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MoveToBottom
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MoveToBottom")==0){
		Stack->CorrectParams(0);

		if(m_Parent && m_Parent->m_Type == UI_WINDOW){
			CUIWindow* win = (CUIWindow*)m_Parent;
			for(int i=0; i<win->m_Widgets.GetSize(); i++){
				if(win->m_Widgets[i]==this){
					win->m_Widgets.RemoveAt(i);
					win->m_Widgets.InsertAt(0, this);
					break;
				}
			}
			Stack->PushBool(true);
		}
		else Stack->PushBool(false);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MoveToTop
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MoveToTop")==0){
		Stack->CorrectParams(0);

		if(m_Parent && m_Parent->m_Type == UI_WINDOW){
			CUIWindow* win = (CUIWindow*)m_Parent;
			for(int i=0; i<win->m_Widgets.GetSize(); i++){
				if(win->m_Widgets[i]==this){
					win->m_Widgets.RemoveAt(i);
					win->m_Widgets.Add(this);
					break;
				}
			}
			Stack->PushBool(true);
		}
		else Stack->PushBool(false);

		return S_OK;
	}

	else return CBObject::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CUIObject::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("ui_object");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Name")==0){
		m_ScValue->SetString(m_Name);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Parent (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Parent")==0){
		m_ScValue->SetNative(m_Parent, true);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// ParentNotify
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ParentNotify")==0){
		m_ScValue->SetBool(m_ParentNotify);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Width
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Width")==0){
		m_ScValue->SetInt(m_Width);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Height
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Height")==0){
		m_ScValue->SetInt(m_Height);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Visible
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Visible")==0){
		m_ScValue->SetBool(m_Visible);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Disabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Disabled")==0){
		m_ScValue->SetBool(m_Disable);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Text
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Text")==0){
		m_ScValue->SetString(m_Text);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// NextSibling (RO) / PrevSibling (RO)
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "NextSibling")==0 || strcmp(Name, "PrevSibling")==0){
		m_ScValue->SetNULL();
		if(m_Parent && m_Parent->m_Type == UI_WINDOW){
			CUIWindow* win = (CUIWindow*)m_Parent;
			for(int i=0; i<win->m_Widgets.GetSize(); i++){
				if(win->m_Widgets[i] == this){
					if(strcmp(Name, "NextSibling")==0){
						if(i<win->m_Widgets.GetSize()-1) m_ScValue->SetNative(win->m_Widgets[i+1], true);
					}
					else{
						if(i>0) m_ScValue->SetNative(win->m_Widgets[i-1], true);
					}
					break;
				}
			}
		}
		return m_ScValue;
	}

	else return CBObject::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// Name
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Name")==0){
		SetName(Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ParentNotify
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ParentNotify")==0){
		m_ParentNotify = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Width
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Width")==0){
		m_Width = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Height
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Height")==0){
		m_Height = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Visible
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Visible")==0){
		m_Visible = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Disabled
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Disabled")==0){
		m_Disable = Value->GetBool();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Text
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Text")==0){
		SetText(Value->GetString());
		return S_OK;
	}

	else return CBObject::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CUIObject::ScToString()
{
	return "[ui_object]";
}


//////////////////////////////////////////////////////////////////////////
bool CUIObject::IsFocused()
{
	if(!Game->m_FocusedWindow) return false;
	if(Game->m_FocusedWindow==this) return true;

	CUIObject* obj = Game->m_FocusedWindow;
	while(obj){
		if(obj==this) return true;
		else obj = obj->m_FocusedWidget;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::HandleMouse(TMouseEvent Event, TMouseButton Button)
{
	// handle focus change
	if(Event==MOUSE_CLICK && Button==MOUSE_BUTTON_LEFT){
		Focus();
	}
	return CBObject::HandleMouse(Event, Button);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::Focus()
{
	CUIObject* obj = this;
	bool disabled = false;
	while(obj){
		if(obj->m_Disable && obj->m_Type==UI_WINDOW){
			disabled = true;
			break;
		}
		obj = obj->m_Parent;
	}
	if(!disabled){
		obj = this;
		while(obj){
			if(obj->m_Parent){
				if(!obj->m_Disable && obj->m_CanFocus) obj->m_Parent->m_FocusedWidget = obj;
			}
			else{
				if(obj->m_Type==UI_WINDOW) Game->FocusWindow((CUIWindow*)obj);
			}

			obj = obj->m_Parent;
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::GetTotalOffset(int *OffsetX, int *OffsetY)
{
	int OffX=0, OffY=0;

	CUIObject* obj = m_Parent;
	while(obj){
		OffX += obj->m_PosX;
		OffY += obj->m_PosY;

		obj = obj->m_Parent;
	}
	if(OffsetX) *OffsetX = OffX;
	if(OffsetY) *OffsetY = OffY;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::Persist(CBPersistMgr* PersistMgr){

	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Back));
	PersistMgr->Transfer(TMEMBER(m_CanFocus));
	PersistMgr->Transfer(TMEMBER(m_Disable));
	PersistMgr->Transfer(TMEMBER(m_FocusedWidget));
	PersistMgr->Transfer(TMEMBER(m_Font));
	PersistMgr->Transfer(TMEMBER(m_Height));
	PersistMgr->Transfer(TMEMBER(m_Image));
	PersistMgr->Transfer(TMEMBER(m_ListenerObject));
	PersistMgr->Transfer(TMEMBER(m_ListenerParamObject));
	PersistMgr->Transfer(TMEMBER(m_ListenerParamDWORD));
	PersistMgr->Transfer(TMEMBER(m_Parent));
	PersistMgr->Transfer(TMEMBER(m_ParentNotify));
	PersistMgr->Transfer(TMEMBER(m_SharedFonts));
	PersistMgr->Transfer(TMEMBER(m_SharedImages));
	PersistMgr->Transfer(TMEMBER(m_Text));
	PersistMgr->Transfer(TMEMBER_INT(m_Type));
	PersistMgr->Transfer(TMEMBER(m_Visible));
	PersistMgr->Transfer(TMEMBER(m_Width));

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
char* CUIObject::GetAccessCaption()
{
	if(m_AccessCaption) return m_AccessCaption;
	else
	{
		if(m_Text) return m_Text;
		else return GetCaption();
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CUIObject::SaveAsText(CBDynBuffer* Buffer, int Indent)
{	
	return E_FAIL;
}