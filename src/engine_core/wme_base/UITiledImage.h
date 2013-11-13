// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_UITILEDIMAGE_H__380394F0_F7C0_4E6C_AE77_B783E36A7555__INCLUDED_)
#define AFX_UITILEDIMAGE_H__380394F0_F7C0_4E6C_AE77_B783E36A7555__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UIObject.h"

class CUITiledImage : public CBObject  
{
public:
	DECLARE_PERSISTENT(CUITiledImage, CBObject);
	void CorrectSize(int* Width, int* Height);
	HRESULT LoadFile(char * Filename);
	HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);

	HRESULT Display(int X, int Y, int Width, int Height);
	CUITiledImage(CBGame* inGame=NULL);
	virtual ~CUITiledImage();
	CBSubFrame* m_Image;
	RECT m_UpLeft;
	RECT m_UpMiddle;
	RECT m_UpRight;
	RECT m_MiddleLeft;
	RECT m_MiddleMiddle;
	RECT m_MiddleRight;
	RECT m_DownLeft;
	RECT m_DownMiddle;
	RECT m_DownRight;
};

#endif // !defined(AFX_UITILEDIMAGE_H__380394F0_F7C0_4E6C_AE77_B783E36A7555__INCLUDED_)
