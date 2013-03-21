// Package.cpp: implementation of the CPackage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Package.h"
#include "../../engine_core/wme_base/dcpackage.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


extern "C"{
#include "zlib.h"
}

#if _DEBUG
	#pragma comment(lib, "zlib_d.lib")
#else
	#pragma comment(lib, "zlib.lib")
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
CPackage::CPackage()
{
	m_Name = NULL;
	m_File = NULL;
}


//////////////////////////////////////////////////////////////////////////
CPackage::~CPackage()
{
	for(int i=0; i<m_Files.GetSize(); i++) delete m_Files[i];
	m_Files.RemoveAll();

	delete [] m_Name;

	if(m_File) fclose(m_File);
}


//////////////////////////////////////////////////////////////////////////
bool CPackage::ReadPackage(CString Filename)
{
	m_File = fopen(LPCSTR(Filename), "rb");
	if(!m_File) return false;


	TPackageHeader hdr;
	fread(&hdr, sizeof(TPackageHeader), 1, m_File);
	if(hdr.Magic1 != PACKAGE_MAGIC_1 || hdr.Magic2 != PACKAGE_MAGIC_2 || hdr.PackageVersion != PACKAGE_VERSION){
		fclose(m_File);
		m_File = NULL;
		return false;
	}

	DWORD DirOffset;
	fread(&DirOffset, sizeof(DWORD), 1, m_File);
	fseek(m_File, DirOffset, SEEK_SET);


	// read package info
	BYTE NameLength;
	fread(&NameLength, sizeof(BYTE), 1, m_File);
	m_Name = new char[NameLength];
	fread(m_Name, NameLength, 1, m_File);
	fread(&m_CD, sizeof(BYTE), 1, m_File);

	// read file entries
	DWORD NumFiles;
	fread(&NumFiles, sizeof(DWORD), 1, m_File);

	for(DWORD j=0; j<NumFiles; j++){
		char* Name;
		DWORD Offset, Length, CompLength, Flags, TimeDate1, TimeDate2;

		fread(&NameLength, sizeof(BYTE), 1, m_File);
		Name = new char[NameLength];
		fread(Name, NameLength, 1, m_File);

		for(int k=0; k<NameLength; k++){
			((BYTE*)Name)[k] ^= 'D';
		}


		fread(&Offset, sizeof(DWORD), 1, m_File);
		fread(&Length, sizeof(DWORD), 1, m_File);
		fread(&CompLength, sizeof(DWORD), 1, m_File);
		fread(&Flags, sizeof(DWORD), 1, m_File);

		fread(&TimeDate1, sizeof(DWORD), 1, m_File);
		fread(&TimeDate2, sizeof(DWORD), 1, m_File);



		CBFileEntry* file = new CBFileEntry();
		file->m_Offset = Offset;
		file->m_Length = Length;
		file->m_CompressedLength = CompLength;
		file->m_Flags = Flags;
		file->m_Filename = CString(Name);

		m_Files.Add(file);
		delete [] Name;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
int CPackage::GetNumEntries()
{
	return m_Files.GetSize();
}


//////////////////////////////////////////////////////////////////////////
bool CPackage::ExtractEntry(int Entry, CString TargetDir)
{
	if(Entry<0 || Entry>=m_Files.GetSize()) return false;

	BYTE* buffer = NULL;

	CBFileEntry* FileEntry;
	FileEntry = m_Files[Entry];
	DWORD length = FileEntry->m_Length;

	// read uncompressed file
	if(FileEntry->m_CompressedLength==0){				
		buffer = new BYTE [length];
		if(buffer == NULL) return false;
		
		fseek(m_File, FileEntry->m_Offset, SEEK_SET);
		fread(buffer, length, 1, m_File);
	}
	
	// read compressed file
	else{
		// compressed buffer
		BYTE* comp_buffer = new BYTE [FileEntry->m_CompressedLength];
		if(comp_buffer == NULL) return false;

		// uncompressed buffer
		buffer = new BYTE [length];
		if(buffer == NULL){
			delete [] comp_buffer;
			return false;
		}

		// read data
		fseek(m_File, FileEntry->m_Offset, SEEK_SET);
		fread(comp_buffer, FileEntry->m_CompressedLength, 1, m_File);

		// decompress
		DWORD uncom_length = FileEntry->m_Length;
		if(uncompress(buffer, &uncom_length, comp_buffer, FileEntry->m_CompressedLength)!=Z_OK){			
			delete [] comp_buffer;
			delete [] buffer;
			return false;
		}
		delete [] comp_buffer;
	}

	if(TargetDir[TargetDir.GetLength()-1] != '\\') TargetDir += '\\';
	TargetDir += FileEntry->m_Filename;

	SetFileAttributes(LPCSTR(TargetDir), FILE_ATTRIBUTE_NORMAL);
	FILE* out = fopen(LPCSTR(TargetDir), "wb");
	if(!out){
		delete [] buffer;
		return false;
	}

	fwrite(buffer, length, 1, out);
	fclose(out);

	delete [] buffer;

	return true;
}


//////////////////////////////////////////////////////////////////////////
CString CPackage::GetEntryName(int Entry)
{
	if(Entry<0 || Entry>=m_Files.GetSize()) return "";
	else return m_Files[Entry]->m_Filename;
}
