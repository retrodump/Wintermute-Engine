// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BACTIVERECT_H__63461149_54E4_11D4_B319_80A6438CA47C__INCLUDED_)
#define AFX_BACTIVERECT_H__63461149_54E4_11D4_B319_80A6438CA47C__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class C3DModel;
class CXModel;

class CBActiveRect:CBBase
{
public:	
	void ClipRect();
	bool m_Precise;
	float m_ZoomX;
	float m_ZoomY;
	CBSubFrame* m_Frame;
	C3DModel* m_Model;
	CXModel* m_ModelX;
	CBObject* m_Owner;
	CBRegion* m_Region;
	int m_OffsetX;
	int m_OffsetY;
	RECT m_Rect;
	CBActiveRect(CBGame* inGameOwner=NULL);
	CBActiveRect(CBGame* inGameOwner, CBObject* Owner, CBSubFrame* Frame, int X, int Y, int Width, int Height, float ZoomX=100, float ZoomY=100, bool Precise=true);
	CBActiveRect(CBGame* inGameOwner, CBObject* Owner, C3DModel* Model, int X, int Y, int Width, int Height, bool Precise=true);
	CBActiveRect(CBGame* inGameOwner, CBObject* Owner, CXModel* Model, int X, int Y, int Width, int Height, bool Precise=true);
	CBActiveRect(CBGame* inGame, CBObject* Owner, CBRegion* Region, int OffsetX, int OffsetY);
	virtual ~CBActiveRect();

};

#endif // !defined(AFX_BACTIVERECT_H__63461149_54E4_11D4_B319_80A6438CA47C__INCLUDED_)
