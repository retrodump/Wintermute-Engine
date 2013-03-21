// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADRESPONSE_H__1BE4E980_93F3_47A4_A044_07D4E11DA6BD__INCLUDED_)
#define AFX_ADRESPONSE_H__1BE4E980_93F3_47A4_A044_07D4E11DA6BD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BObject.h"

class CAdResponse : public CBObject
{
public:	
	DECLARE_PERSISTENT(CAdResponse, CBObject);
	HRESULT SetIcon(char* Filename);
	HRESULT SetFont(char* Filename);
	HRESULT SetIconHover(char* Filename);
	HRESULT SetIconPressed(char* Filename);
	void SetText(char* Text);
	int m_ID;
	CBSprite* m_Icon;
	CBSprite* m_IconHover;
	CBSprite* m_IconPressed;
	CBFont* m_Font;
	char* m_Text;
	char* m_TextOrig;
	CAdResponse(CBGame* inGame);
	virtual ~CAdResponse();
	TResponseType m_ResponseType;

};

#endif // !defined(AFX_ADRESPONSE_H__1BE4E980_93F3_47A4_A044_07D4E11DA6BD__INCLUDED_)
