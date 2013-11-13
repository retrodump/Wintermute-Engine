#if !defined(AFX_WAYPOINTSPAGE_H__3C1A01CD_9418_477E_9EFB_D53B1F3AE341__INCLUDED_)
#define AFX_WAYPOINTSPAGE_H__3C1A01CD_9418_477E_9EFB_D53B1F3AE341__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WaypointsPage.h : header file
//

#include "dcgf_ad.h"
#include "SceneView.h"

/////////////////////////////////////////////////////////////////////////////
// CWaypointsPage dialog

class CWaypointsPage : public CDialog
{
// Construction
public:
	void ShowWaypoints(CAdWaypointGroup* Group, CSceneView* View, bool Force=false);
	CAdWaypointGroup* m_WptGroup;
	CWaypointsPage(CWnd* pParent = NULL);   // standard constructor
	CSceneView* m_View;

// Dialog Data
	//{{AFX_DATA(CWaypointsPage)
	enum { IDD = IDD_WAYPOINTS_PAGE };
	CEdit	m_Name;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWaypointsPage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();
	virtual void OnOK();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWaypointsPage)
	afx_msg void OnChangeName();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WAYPOINTSPAGE_H__3C1A01CD_9418_477E_9EFB_D53B1F3AE341__INCLUDED_)
