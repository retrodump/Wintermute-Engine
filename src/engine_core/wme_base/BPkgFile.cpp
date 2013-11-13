// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BPkgFile.h"


#if _DEBUG
	#pragma comment(lib, "zlib_d.lib")
#else
	#pragma comment(lib, "zlib.lib")
#endif


extern "C"{
	#include "zlib.h"
}



//////////////////////////////////////////////////////////////////////////
CBPkgFile::CBPkgFile(CBGame* inGame):CBFile(inGame)
{
	m_FileEntry = NULL;
	m_Compressed = false;

    m_Stream.zalloc = (alloc_func)0;
    m_Stream.zfree = (free_func)0;
    m_Stream.opaque = (voidpf)0;

	m_InflateInit = false;
}


//////////////////////////////////////////////////////////////////////////
CBPkgFile::~CBPkgFile()
{
	Close();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPkgFile::Open(char* Filename)
{
	Close();

	// correct slashes
	for(int i=0; i<strlen(Filename); i++){
		if(Filename[i]=='/') Filename[i] = '\\';
	}

	m_FileEntry = Game->m_FileManager->GetPackageEntry(Filename);
	if(!m_FileEntry) return E_FAIL;

	m_Compressed = (m_FileEntry->m_CompressedLength != 0);
	m_Size = m_FileEntry->m_Length;

	SeekToPos(0);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPkgFile::Close()
{
	m_FileEntry = NULL;
	m_Pos = 0;
	m_Size = 0;

	if(m_InflateInit) inflateEnd(&m_Stream);
	m_InflateInit = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPkgFile::Read(void *Buffer, DWORD Size)
{
	if(!m_FileEntry || m_Pos+Size>m_Size) return E_FAIL;

	HRESULT ret=S_OK;

	if(m_Compressed){
		DWORD InitOut = m_Stream.total_out;

		m_Stream.avail_out = Size;
		m_Stream.next_out = (BYTE*)Buffer;

		while (m_Stream.total_out - InitOut < Size && m_Stream.total_in < m_FileEntry->m_CompressedLength){
			// needs to read more data?
			if(m_Stream.avail_in==0){
				m_Stream.avail_in = min(COMPRESSED_BUFFER_SIZE, m_FileEntry->m_CompressedLength - m_Stream.total_in);
				m_FileEntry->m_Package->Read(m_FileEntry->m_Offset + m_Stream.total_in, m_CompBuffer, m_Stream.avail_in);
				m_Stream.next_in = m_CompBuffer;
			}

			int res = inflate(&m_Stream, Z_SYNC_FLUSH);
			if(res!=Z_OK && res!=Z_STREAM_END){
				Game->LOG(0, "zlib error: %d", res);
				ret = E_FAIL;
				break;
			}
		}

	
	}
	else{
		ret = m_FileEntry->m_Package->Read(m_FileEntry->m_Offset+m_Pos, (BYTE*)Buffer, Size);		
	}

	m_Pos+=Size;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBPkgFile::Seek(DWORD Pos, TSeek Origin)
{
	if(!m_FileEntry) return E_FAIL;

	DWORD NewPos=0;

	switch(Origin){
		case SEEK_TO_BEGIN:   NewPos = Pos;          break;
		case SEEK_TO_END:     NewPos = m_Size + Pos; break;
		case SEEK_TO_CURRENT: NewPos = m_Pos + Pos;  break;
	}

	if(NewPos<0 || NewPos > m_Size) return E_FAIL;

	return SeekToPos(NewPos);
}


#define STREAM_BUFFER_SIZE 4096
//////////////////////////////////////////////////////////////////////////
HRESULT CBPkgFile::SeekToPos(DWORD NewPos)
{
	HRESULT ret = S_OK;

	// seek compressed stream to NewPos
	if(m_Compressed){
		BYTE StreamBuffer[STREAM_BUFFER_SIZE];
		if(m_InflateInit) inflateEnd(&m_Stream);
		m_InflateInit = false;
		
		m_Stream.avail_in = 0;
		m_Stream.next_in = m_CompBuffer;
		m_Stream.avail_out = min(STREAM_BUFFER_SIZE, NewPos);
		m_Stream.next_out = StreamBuffer;
		inflateInit(&m_Stream);
		m_InflateInit = true;

		while (m_Stream.total_out < NewPos && m_Stream.total_in < m_FileEntry->m_CompressedLength){
			// needs to read more data?
			if(m_Stream.avail_in==0){
				m_Stream.avail_in = min(COMPRESSED_BUFFER_SIZE, m_FileEntry->m_CompressedLength - m_Stream.total_in);
				m_FileEntry->m_Package->Read(m_FileEntry->m_Offset + m_Stream.total_in, m_CompBuffer, m_Stream.avail_in);
				m_Stream.next_in = m_CompBuffer;
			}

			// needs more space?
			if(m_Stream.avail_out==0){
				m_Stream.next_out = StreamBuffer;
				m_Stream.avail_out = min(STREAM_BUFFER_SIZE, NewPos - m_Stream.total_out);
			}

			// stream on!
			int res = inflate(&m_Stream, Z_SYNC_FLUSH);
			if(res!=Z_OK && res!=Z_STREAM_END){
				ret = E_FAIL;
				break;
			}
		}
		
	}

	m_Pos = NewPos;
	return ret;
}
