// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "BRenderer.h"
#include "XModel.h"


//////////////////////////////////////////////////////////////////////
CBRenderer::CBRenderer(CBGame* inGame):CBBase(inGame)
{
	m_Instance = NULL;
	m_Window = NULL;
	m_ClipperWindow = NULL;
	m_Active = false;
	m_Ready = false;
	m_Windowed = false;
	m_ForceAlphaColor = 0x00;

	m_Width = m_Height = m_BPP = 0;
	CBPlatform::SetRectEmpty(&m_MonitorRect);

	m_RealWidth = m_RealHeight = 0;
	m_DrawOffsetX = m_DrawOffsetY = 0;
}


//////////////////////////////////////////////////////////////////////
CBRenderer::~CBRenderer()
{
	DeleteRectList();
	UnclipCursor();
}


//////////////////////////////////////////////////////////////////////
void CBRenderer::InitLoop()
{
	DeleteRectList();
}


//////////////////////////////////////////////////////////////////////
CBObject* CBRenderer::GetObjectAt(int X, int Y)
{
	POINT point;
	point.x = X;
	point.y = Y;

	for(int i=m_RectList.GetSize()-1; i>=0; i--)
	{
		if(PtInRect(&m_RectList[i]->m_Rect, point))
		{
			if(m_RectList[i]->m_Precise)
			{
				// frame
				if(m_RectList[i]->m_Frame)
				{
					int XX = (int)((m_RectList[i]->m_Frame->m_Rect.left + X - m_RectList[i]->m_Rect.left + m_RectList[i]->m_OffsetX)/(float)((float)m_RectList[i]->m_ZoomX/(float)100));
					int YY = (int)((m_RectList[i]->m_Frame->m_Rect.top  + Y - m_RectList[i]->m_Rect.top  + m_RectList[i]->m_OffsetY) /(float)((float)m_RectList[i]->m_ZoomY/(float)100));

					if(m_RectList[i]->m_Frame->m_MirrorX)
					{
						int Width = m_RectList[i]->m_Frame->m_Rect.right - m_RectList[i]->m_Frame->m_Rect.left;
						XX = Width - XX;
					}

					if(m_RectList[i]->m_Frame->m_MirrorY)
					{
						int Height = m_RectList[i]->m_Frame->m_Rect.bottom - m_RectList[i]->m_Frame->m_Rect.top;
						YY = Height - YY;
					}

					if(!m_RectList[i]->m_Frame->m_Surface->IsTransparentAt(XX, YY)) return m_RectList[i]->m_Owner;
				}
				// model
				else if(m_RectList[i]->m_Model)
				{
					if(!m_RectList[i]->m_Model->IsTransparentAt(X, Y)) return m_RectList[i]->m_Owner;
				}
				// modelX
				else if(m_RectList[i]->m_ModelX)
				{
					if(!m_RectList[i]->m_ModelX->IsTransparentAt(X, Y)) return m_RectList[i]->m_Owner;
				}
				// region
				else if(m_RectList[i]->m_Region)
				{
					if(m_RectList[i]->m_Region->PointInRegion(X+m_RectList[i]->m_OffsetX, Y+m_RectList[i]->m_OffsetY)) return m_RectList[i]->m_Owner;
				}
			}
			else return m_RectList[i]->m_Owner;
		}
	}
	
	return (CBObject*)NULL;
}


//////////////////////////////////////////////////////////////////////////
void CBRenderer::DeleteRectList()
{
	for(int i=0; i<m_RectList.GetSize(); i++)
	{
		delete m_RectList[i];
	}
	m_RectList.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::SwitchFullscreen()
{
	    // In case we're coming from a fullscreen mode, restore the window size
    if (m_Windowed)
	{
		Game->LOG(0, "Switching to windowed mode");
		SetWindowLong(m_Window, GWL_STYLE, WS_OVERLAPPEDWINDOW);
		SetWindowLong(m_Window, GWL_EXSTYLE, 0);

		Game->LOG(0, "%d", m_WindowRect.bottom - m_WindowRect.top);
        SetWindowPos(m_Window, HWND_NOTOPMOST, m_WindowRect.left, m_WindowRect.top,
                     (m_WindowRect.right - m_WindowRect.left), 
                     (m_WindowRect.bottom - m_WindowRect.top), SWP_SHOWWINDOW );
    }
	else
	{
		Game->LOG(0, "Switching to full-screen mode");
		SetWindowLong(m_Window, GWL_STYLE, WS_POPUP);
		SetWindowLong(m_Window, GWL_EXSTYLE, WS_EX_TOPMOST);
	}

	return S_OK;;
}

//////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Flip()
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::InitRenderer(CHWManager* hwManager)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////
void CBRenderer::OnWindowChange()
{
}


//////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Fill(BYTE r, BYTE g, BYTE b, RECT* rect)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::WindowedBlt()
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Setup2D(bool Force)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::SetupLines()
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Setup3D(C3DCamera* Camera, bool Force)
{
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Setup3DCustom(D3DXMATRIX* ViewMat, D3DXMATRIX* ProjMat)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::DrawLine(int X1, int Y1, int X2, int Y2, DWORD Color)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::DrawRect(int X1, int Y1, int X2, int Y2, DWORD Color, int Width)
{
	for(int i=0; i<Width; i++)
	{
		DrawLine(X1+i, Y1+i, X2-i,   Y1+i,   Color); // up
		DrawLine(X1+i, Y2-i, X2-i+1, Y2-i, Color); // down

		DrawLine(X1+i, Y1+i, X1+i, Y2-i,   Color); // left
		DrawLine(X2-i, Y1+i, X2-i, Y2-i+1, Color); // right
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::Fade(WORD Alpha)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::FadeToColor(DWORD Color, RECT* rect)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::SetViewport(int left, int top, int right, int bottom)
{
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::SetScreenViewport()
{
	return SetViewport(m_DrawOffsetX, m_DrawOffsetY, m_Width + m_DrawOffsetX, m_Height + m_DrawOffsetY);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::SetViewport(RECT* Rect)
{
	return SetViewport(	Rect->left + m_DrawOffsetX,
						Rect->top + m_DrawOffsetY,
						Rect->right + m_DrawOffsetX,
						Rect->bottom + m_DrawOffsetY);
}


//////////////////////////////////////////////////////////////////////////
CBImage* CBRenderer::TakeScreenshot()
{
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::ClipCursor()
{
	if(!m_Windowed)
	{
		RECT rc;
		GetWindowRect(m_Window, &rc);

		// if "maintain aspect ratio" is in effect, lock mouse to visible area
		rc.left = m_DrawOffsetX;
		rc.top = m_DrawOffsetY;
		rc.right = rc.left + m_Width;
		rc.bottom = rc.top + m_Height;

		::ClipCursor(&rc);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CBRenderer::UnclipCursor()
{
	if(!m_Windowed) ::ClipCursor(NULL);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
bool CBRenderer::PointInViewport(POINT* P)
{
	if(P->x < m_DrawOffsetX) return false;
	if(P->y < m_DrawOffsetY) return false;
	if(P->x > m_DrawOffsetX + m_Width) return false;
	if(P->y > m_DrawOffsetY + m_Height) return false;

	return true;
}