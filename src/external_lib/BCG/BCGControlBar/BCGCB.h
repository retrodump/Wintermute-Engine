//*******************************************************************************
// COPYRIGHT NOTES
// ---------------
// This is a part of the BCGControlBar Library
// Copyright (C) 1998-2000 BCGSoft Ltd.
// All rights reserved.
//
// This source code can be used, distributed or modified
// only under terms and conditions 
// of the accompanying license agreement.
//*******************************************************************************

#ifndef __BCGCB_H
#define __BCGCB_H

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#endif

#include "bcgcbver.h"	// Library version info.

#ifdef _BCGCB_IN_OTHER_DLL
BCGCONTROLBARDLLEXPORT void BCGControlBarDllInitialize ();	// By Guy Hachlili
#endif // _BCGCB_IN_OTHER_DLL


#if defined _AFXDLL && !defined _BCGCONTROLBAR_STATIC_
	//------------------------------------------
	// MFC shared DLL, BCGControlBar shared DLL
	//------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582UD.lib") 
			#pragma message("Automatically linking with BCGCB582UD.dll")
		#else
			#pragma comment(lib,"BCGCB582d.lib") 
			#pragma message("Automatically linking with BCGCB582d.dll")
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582U.lib") 
			#pragma message("Automatically linking with BCGCB582U.dll") 
		#else
			#pragma comment(lib,"BCGCB582.lib") 
			#pragma message("Automatically linking with BCGCB582.dll") 
		#endif
	#endif
#elif defined _AFXDLL && defined _BCGCONTROLBAR_STATIC_
	//---------------------------------------------
	// MFC shared DLL, BCGControlBar static library
	//---------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582Staticuds.lib") 
			#pragma message("Automatically linking with BCGCB582Staticuds.lib") 
		#else
			#pragma comment(lib,"BCGCB582Staticds.lib") 
			#pragma message("Automatically linking with BCGCB582Staticds.lib") 
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582Staticus.lib") 
			#pragma message("Automatically linking with BCGCB582Staticus.lib") 
		#else
			#pragma comment(lib,"BCGCB582Statics.lib") 
			#pragma message("Automatically linking with BCGCB582Statics.lib") 
		#endif
	#endif
#else
	//--------------------------------------------------
	// MFC static library, BCGControlBar static library
	//--------------------------------------------------
	#ifdef _DEBUG
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582Staticud.lib") 
			#pragma message("Automatically linking with BCGCB582Staticud.lib") 
		#else
			#pragma comment(lib,"BCGCB582Staticd.lib") 
			#pragma message("Automatically linking with BCGCB582Staticd.lib") 
		#endif
	#else
		#ifdef _UNICODE
			#pragma comment(lib,"BCGCB582StaticU.lib") 
			#pragma message("Automatically linking with BCGCB582StaticU.lib") 
		#else
			#pragma comment(lib,"BCGCB582Static.lib") 
			#pragma message("Automatically linking with BCGCB582Static.lib") 
		#endif
	#endif
#endif

//------------------
// BCG control bars:
//------------------
#include "BCGToolBar.h"
#include "BCGStatusBar.h"
#include "BCGMenuBar.h"
#include "BCGDialogBar.h"
#include "BCGOutlookBar.h"
#include "BCGColorBar.h"
#include "BCGCaptionBar.h"

//-------------------------
// BCG control bar buttons:
//-------------------------
#include "BCGToolbarButton.h"
#include "BCGToolbarComboBoxButton.h"
#include "BCGToolbarDateTimeCtrl.h"
#include "BCGToolbarMenuButton.h"
#include "BCGToolbarRegularMenuButton.h"
#include "BCGToolbarEditBoxButton.h"
#include "BCGDropDown.h"
#include "BCGColorMenuButton.h"
#include "BCGToolbarFontCombo.h"

//-------------------------------------------------------------------
// BCG frame windows (replaces CFrameWnd, CMDIFrameWnd, CMDIChildWnd,
// COleIPFrameWnd and OleDocIPFrameWnd):
//-------------------------------------------------------------------
#include "BCGFrameWnd.h"
#include "BCGMDIFrameWnd.h"
#include "BCGMDIChildWnd.h"
#include "BCGOleIPFrameWnd.h"
#include "BCGOleDocIPFrameWnd.h"

//-------------------------
// BCG customization stuff:
//-------------------------
#include "CBCGToolbarCustomize.h"

#include "BCGContextMenuManager.h"
#include "BCGKeyboardManager.h"
#include "BCGMouseManager.h"

#include "BCGUserTool.h"
#include "KeyHelper.h"

//-----------------------
// BCG workspace manager
//-----------------------
#include "BCGWorkspace.h"
#include "BCGRegistry.h"
#include "RebarState.h"

//-------------
// BCG helpers:
//-------------
#include "BCGTearOffManager.h"
#include "BCGDrawManager.h"

//-----------------------
// BCG menu replacements:
//-----------------------
#include "BCGPopupMenu.h"
#include "BCGPopupMenuBar.h"
#include "BCGToolBarImages.h"

//------------------
// BCG docking bars:
//------------------
#include "BCGSizingControlBar.h"
#include "BCGTabWnd.h"

//--------------
// BCG controls:
//--------------
#include "BCGButton.h"
#include "BCGColorButton.h"
#include "BCGMenuButton.h"
#include "BCGURLLinkButton.h"
#include "BCGEditListBox.h"
#include "BCGAnimCtrl.h"
#include "PowerColorPicker.h"
#include "BCGFontComboBox.h"
#include "BCGHeaderCtrl.h"
#include "BCGListCtrl.h"

//-------------
// BCG dialogs:
//-------------
#include "BCGDialog.h"
#include "BCGFileDialog.h"
#include "BCGWindowsManagerDlg.h"
#include "BCGPrintPreviewView.h"
#include "ImageEditDlg.h"
#include "BCGColorDialog.h"
#include "BCGKeyMapDlg.h"

//-----------
// BCG views:
//-----------
#include "BCGTabView.h"

//--------------------
// Visualization stuf:
//--------------------
#include "BCGVisualManager.h"
#include "BCGVisualManagerXP.h"
#include "BCGWinXPVisualManager.h"
#include "BCGSkinManager.h"

//----------------
// Shell controls:
//----------------
#include "BCGShellManager.h"
#include "BCGShellTree.h"
#include "BCGShellList.h"

#endif // __BCGCB_H
