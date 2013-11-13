// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__0064FE10_682B_4375_916F_78CA12CDBDF9__INCLUDED_)
#define AFX_STDAFX_H__0064FE10_682B_4375_916F_78CA12CDBDF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#pragma warning (disable: 4786) // symbols longer than 255 chars (STL)

#ifndef WINVER
#define WINVER 0x0500
#endif


#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#include <htmlhelp.h>
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxdlgs.h>


//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__0064FE10_682B_4375_916F_78CA12CDBDF9__INCLUDED_)
