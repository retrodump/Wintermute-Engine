// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BSaveThumbHelper.h"


//////////////////////////////////////////////////////////////////////////
CBSaveThumbHelper::CBSaveThumbHelper(CBGame* inGame):CBBase(inGame)
{
	m_Thumbnail = NULL;
	m_RichThumbnail = NULL;
}

//////////////////////////////////////////////////////////////////////////
CBSaveThumbHelper::~CBSaveThumbHelper(void)
{
	SAFE_DELETE(m_Thumbnail);
	SAFE_DELETE(m_RichThumbnail);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBSaveThumbHelper::StoreThumbnail(bool DoFlip)
{
	SAFE_DELETE(m_Thumbnail);
	SAFE_DELETE(m_RichThumbnail);

	if(Game->m_RichSavedGames || (Game->m_ThumbnailWidth > 0 && Game->m_ThumbnailHeight > 0))
	{
		if(DoFlip)
		{
			Game->DisplayContent(false);
			Game->m_Renderer->Flip();
		}

		CBImage* Screenshot = Game->m_Renderer->TakeScreenshot();
		if(!Screenshot) return E_FAIL;

		// normal thumbnail
		if(Game->m_ThumbnailWidth > 0 && Game->m_ThumbnailHeight > 0)
		{
			m_Thumbnail = new CBImage(Game);
			m_Thumbnail->CopyFrom(Screenshot, Game->m_ThumbnailWidth, Game->m_ThumbnailHeight);
		}

		// rich saved game thumbnail
		if(Game->m_RichSavedGames)
		{
			int Width = 256;
			int Height = Width * ((float)Game->m_Renderer->m_Height / (float)Game->m_Renderer->m_Width);

			m_RichThumbnail = new CBImage(Game);
			m_RichThumbnail->CopyFrom(Screenshot, Width, Height);
		}

		SAFE_DELETE(Screenshot);
	}
	return S_OK;
}