///////////////////////////////////////////////////////////////////////////////
//
//  Module: zlibcpp.h
//
//    Desc: Basic class wrapper for the zlib dll
//
// Copyright (c) 2003 Automatic Data Processing, Inc. All Rights Reserved.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "zip.h"

class CZLib  
{
public:
	CZLib();
	virtual ~CZLib();

	BOOL Open(CString f_file, int f_nAppend = 0);
   BOOL AddFile(CString f_file);
	void Close();
	FILETIME getLastWriteFileTime(CString sFile);
protected:
	zipFile m_zf;
};
