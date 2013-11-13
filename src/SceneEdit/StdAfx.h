// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0E6B4EA4_0512_4DB8_848F_25356221AF16__INCLUDED_)
#define AFX_STDAFX_H__0E6B4EA4_0512_4DB8_848F_25356221AF16__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#pragma warning(disable: 4786) 

#ifndef WINVER
#define WINVER 0x0500
#endif


#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxadv.h>
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <htmlhelp.h>
#endif // _AFX_NO_AFXCMN_SUPPORT

//#define _BCGCONTROLBAR_STATIC_
#include <bcgcb.h>			// BCG Control Bar



//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0E6B4EA4_0512_4DB8_848F_25356221AF16__INCLUDED_)
