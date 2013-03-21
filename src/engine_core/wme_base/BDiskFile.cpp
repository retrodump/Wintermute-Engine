// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BDiskFile.h"
#include "RichSavedGame.h"


//////////////////////////////////////////////////////////////////////////
CBDiskFile::CBDiskFile(CBGame* inGame):CBFile(inGame)
{
	m_File = NULL;
	m_Data = NULL;
	m_Compressed = false;
	m_PrefixSize = 0;
}


//////////////////////////////////////////////////////////////////////////
CBDiskFile::~CBDiskFile()
{
	Close();	
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDiskFile::Open(char* Filename)
{
	Close();

	char FullPath[MAX_PATH];

	for(int i=0; i<Game->m_FileManager->m_SinglePaths.GetSize(); i++)
	{
		sprintf(FullPath, "%s%s", Game->m_FileManager->m_SinglePaths[i], Filename);
		m_File = fopen(FullPath, "rb");
		if(m_File!=NULL) break;
	}

	// if we didn't find it in search paths, try to open directly
	if(!m_File) m_File = fopen(Filename, "rb");

	if(m_File)
	{
		DWORD magic1, magic2;
		fread(&magic1, sizeof(DWORD), 1, m_File);
		fread(&magic2, sizeof(DWORD), 1, m_File);

		// hack - rich saved games
		if(magic1==RM_MAGICNUMBER)
		{
			fseek(m_File, 0, SEEK_SET);
			RICH_GAME_MEDIA_HEADER hdr;
			fread(&hdr, sizeof(RICH_GAME_MEDIA_HEADER), 1, m_File);
			m_PrefixSize = hdr.dwHeaderSize + hdr.dwThumbnailSize;

			fseek(m_File, m_PrefixSize, SEEK_SET);
			fread(&magic1, sizeof(DWORD), 1, m_File);
			fread(&magic2, sizeof(DWORD), 1, m_File);
		}


		if(magic1==DCGF_MAGIC && magic2==COMPRESSED_FILE_MAGIC) m_Compressed = true;

		if(m_Compressed)
		{
			DWORD DataOffset, CompSize, UncompSize;
			fread(&DataOffset, sizeof(DWORD), 1, m_File);
			fread(&CompSize, sizeof(DWORD), 1, m_File);
			fread(&UncompSize, sizeof(DWORD), 1, m_File);

			BYTE* CompBuffer = new BYTE[CompSize];
			if(!CompBuffer)
			{
				Game->LOG(0, "Error allocating memory for compressed file '%s'", Filename);
				Close();
				return E_FAIL;
			}

			m_Data = new BYTE[UncompSize];
			if(!m_Data)
			{
				Game->LOG(0, "Error allocating buffer for file '%s'", Filename);
				delete [] CompBuffer;
				Close();
				return E_FAIL;
			}

			fseek(m_File, DataOffset + m_PrefixSize, SEEK_SET);
			fread(CompBuffer, CompSize, 1, m_File);

			if(uncompress(m_Data, &UncompSize, CompBuffer, CompSize)!=Z_OK)
			{
				Game->LOG(0, "Error uncompressing file '%s'", Filename);
				delete [] CompBuffer;
				Close();
				return E_FAIL;
			}

			delete [] CompBuffer;
			m_Size = UncompSize;
			m_Pos = 0;
			fclose(m_File);
			m_File = NULL;
		}
		else
		{
			m_Pos = 0;
			fseek(m_File, 0, SEEK_END);
			m_Size = ftell(m_File) - m_PrefixSize;
			fseek(m_File, m_PrefixSize, SEEK_SET);
		}

		return S_OK;
	}
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDiskFile::Close()
{
	if(m_File) fclose(m_File);
	m_File = NULL;
	m_Pos = 0;
	m_Size = 0;

	SAFE_DELETE_ARRAY(m_Data);
	m_Compressed = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDiskFile::Read(void *Buffer, DWORD Size)
{
	if(m_Compressed)
	{
		memcpy(Buffer, m_Data+m_Pos, Size);
		m_Pos+=Size;
		return S_OK;
	}
	else
	{
		if(m_File && fread(Buffer, Size, 1, m_File)==1)
		{
			m_Pos+=Size;
			return S_OK;
		}
		else return E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBDiskFile::Seek(DWORD Pos, TSeek Origin)
{
	if(m_Compressed)
	{
		DWORD NewPos=0;

		switch(Origin)
		{
			case SEEK_TO_BEGIN:   NewPos = Pos;          break;
			case SEEK_TO_END:     NewPos = m_Size + Pos; break;
			case SEEK_TO_CURRENT: NewPos = m_Pos + Pos;  break;
		}

		if(NewPos<0 || NewPos > m_Size) return E_FAIL;
		else m_Pos = NewPos;
		return S_OK;
	}
	else
	{
		if(!m_File) return E_FAIL;

		int ret=1;

		switch(Origin)
		{
			case SEEK_TO_BEGIN:   ret = fseek(m_File, m_PrefixSize + Pos, SEEK_SET); break;
			case SEEK_TO_END:     ret = fseek(m_File, Pos, SEEK_END); break;
			case SEEK_TO_CURRENT: ret = fseek(m_File, Pos, SEEK_CUR); break;
		}
		if(ret==0)
		{
			m_Pos = ftell(m_File - m_PrefixSize);
			return S_OK;
		}
		else return E_FAIL;
	}
}
