// amsEdit.cpp : implementation file for CEdit-derived classes
// Created by: Alvaro Mendez - 07/17/2000
//

#include "stdafx.h"
#include "amsEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4355)  // disables: 'this': used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit

CAMSEdit::CAMSEdit() :
	m_rgbText(0),
	m_uInternalFlags(None)
{
}

// Destruction (virtual)
CAMSEdit::~CAMSEdit()
{
}

BEGIN_MESSAGE_MAP(CAMSEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSEdit)
	ON_WM_GETDLGCODE()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the current window's text.
CString CAMSEdit::GetText() const
{
	CString strWindowText;
	GetWindowText(strWindowText);
	return strWindowText;
}

// Sets the window's text to strText.
void CAMSEdit::SetText(const CString& strText)
{
	SetWindowText(strText);
	Redraw();
}

// Sets the background color to the given rgb.
void CAMSEdit::SetBackgroundColor(COLORREF rgb)
{
	m_brushBackground.DeleteObject();
	m_brushBackground.CreateSolidBrush(rgb);
	Invalidate();
}

// Returns the RGB for the background color.
COLORREF CAMSEdit::GetBackgroundColor() const
{
	CAMSEdit* pThis = (CAMSEdit*)this;

	if (!m_brushBackground.GetSafeHandle())
	{
		COLORREF rgb = pThis->GetDC()->GetBkColor();
		pThis->m_brushBackground.CreateSolidBrush(rgb);
		return rgb;
	}

	LOGBRUSH lb;
	pThis->m_brushBackground.GetLogBrush(&lb);
	return lb.lbColor;
}

// Sets the text color to the given rgb.
void CAMSEdit::SetTextColor(COLORREF rgb)
{
	m_rgbText = rgb;
	m_uInternalFlags |= TextColorHasBeenSet;
	Invalidate();
}

// Returns the RGB for the text color.
COLORREF CAMSEdit::GetTextColor() const
{
	if (!(m_uInternalFlags & TextColorHasBeenSet))
	{
		CAMSEdit* pThis = (CAMSEdit*)this;
		pThis->m_rgbText = pThis->GetDC()->GetTextColor();
		pThis->m_uInternalFlags |= TextColorHasBeenSet;
	}
	return m_rgbText;
}

// Returns the current window's text in a valid format.
CString CAMSEdit::GetValidText() const
{
	return GetText();
}

// Redraws the window's text.
void CAMSEdit::Redraw()
{
	if (!::IsWindow(m_hWnd))
		return;

	CString strText = GetValidText();
	if (strText != GetText())
		SetWindowText(strText);
}

// Returns true if the given text should be entered.
bool CAMSEdit::ShouldEnter(TCHAR c) const
{
	return true;
}

// Cuts the current selection into the clipboard.
LONG CAMSEdit::OnCut(UINT, LONG)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nStart < nEnd)
	{
		SendMessage(WM_COPY);				// copy the selection and...
		SendMessage(WM_KEYDOWN, VK_DELETE); // delete it
	}
	
	return 0;
}
	
// Clears the current selection.
LONG CAMSEdit::OnClear(UINT wParam, LONG lParam)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nStart < nEnd)
		SendMessage(WM_KEYDOWN, VK_DELETE); // delete the selection
	
	return 0;
}

// Pastes the text from the clipboard onto the current selection.
LONG CAMSEdit::OnPaste(UINT, LONG)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	CEdit::Default();
	CString strText = GetValidText();

	if (strText != GetText())
	{
		SetWindowText(strText);
		SetSel(nStart, nEnd);
	}

	return 0;
}

// Handles drawing the text and background using the designated colors
BOOL CAMSEdit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if ((message == WM_CTLCOLOREDIT || message == WM_CTLCOLORSTATIC) && (m_brushBackground.GetSafeHandle() || m_uInternalFlags & TextColorHasBeenSet))
	{
		CDC* pDC = CDC::FromHandle((HDC)wParam);

		if (m_rgbText)
			pDC->SetTextColor(m_rgbText);

		// Set the text background to the requested background color
		pDC->SetBkColor(GetBackgroundColor());

		*pLResult = (LRESULT)m_brushBackground.GetSafeHandle();
		return TRUE;
	}

	return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}

// Insures that text set via SetWindowText is valid.
LONG CAMSEdit::OnSetText(UINT wParam, LONG lParam)
{
	LONG nResult = CEdit::Default();

	CString strText = GetValidText();
	if (strText != (LPCTSTR)lParam)
		SetWindowText(strText);

	return nResult;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::SelectionSaver

// Constructs the selection saver object for the given edit control.
// It then saves the edit control's current selection.
CAMSEdit::SelectionSaver::SelectionSaver(CEdit* pEdit) :
	m_pEdit(pEdit)
{
	ASSERT(pEdit);
	pEdit->GetSel(m_nStart, m_nEnd);
}

// Constructs the selection saver object for the given edit control.
// It then saves the given nStart and nEnd values.
CAMSEdit::SelectionSaver::SelectionSaver(CEdit* pEdit, int nStart, int nEnd) :
	m_pEdit(pEdit),
	m_nStart(nStart),
	m_nEnd(nEnd)
{
	ASSERT(pEdit);
	ASSERT(nStart <= nEnd);
}

// Destroys the object and restores the selection to the saved start and end values.
CAMSEdit::SelectionSaver::~SelectionSaver()
{
	m_pEdit->SetSel(m_nStart, m_nEnd, TRUE);
}

// Changes the start and end values to nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveTo(int nStart, int nEnd)
{
	ASSERT(nStart <= nEnd);

	m_nStart = nStart;
	m_nEnd = nEnd;
}

// Changes the start and end values by nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveBy(int nStart, int nEnd)
{
	m_nStart += nStart;
	m_nEnd += nEnd;

	ASSERT(m_nStart <= m_nEnd);
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::MoveBy(int nPos)
{
	m_nStart += nPos;
	m_nEnd += nPos;
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::operator+=(int nPos)
{
	MoveBy(nPos);
}

// Returns the value for the selection's start.
int CAMSEdit::SelectionSaver::GetStart() const
{
	return m_nStart;
}

// Returns the value for the selection's end.
int CAMSEdit::SelectionSaver::GetEnd() const
{
	return m_nEnd;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::Behavior

CAMSEdit::Behavior::Behavior(CAMSEdit* pEdit) :
	m_pEdit(pEdit)
{
	ASSERT(m_pEdit);
}

void CAMSEdit::Behavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	m_pEdit->OnChar(uChar, nRepCnt, nFlags);
}

void CAMSEdit::Behavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	m_pEdit->OnKeyDown(uChar, nRepCnt, nFlags);
}

LONG CAMSEdit::Behavior::_OnPaste(UINT wParam, LONG lParam)
{
	return m_pEdit->OnPaste(wParam, lParam);
}

LRESULT CAMSEdit::Behavior::_Default()
{
	return m_pEdit->Default();
}

void CAMSEdit::Behavior::_Redraw()
{
	m_pEdit->Redraw();
}

bool CAMSEdit::Behavior::_ShouldEnter(TCHAR c) const
{
	return m_pEdit->ShouldEnter(c);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::AlphanumericBehavior

// Constructs the object using the given set of strInvalidChars
CAMSEdit::AlphanumericBehavior::AlphanumericBehavior(CAMSEdit* pEdit, int nMaxChars /*= 0*/, const CString& strInvalidChars /*= _T("%'*\"+?><:\\"")*/) :
	Behavior(pEdit),
	m_nMaxChars(nMaxChars),
	m_strInvalidChars(strInvalidChars)
{
	ASSERT(m_nMaxChars >= 0);
}

// Sets the characters to be considered invalid for text input.
// See also: GetInvalidCharacters
void CAMSEdit::AlphanumericBehavior::SetInvalidCharacters(const CString& strInvalidChars)
{
	if (m_strInvalidChars == strInvalidChars)
		return;

	m_strInvalidChars = strInvalidChars;
	_Redraw();
}

// Returns the characters considered invalid for text input.
// See also: SetInvalidCharacters
const CString& CAMSEdit::AlphanumericBehavior::GetInvalidCharacters() const
{
	return m_strInvalidChars;
}

// Sets the maximum number of characters to allow for input.
void CAMSEdit::AlphanumericBehavior::SetMaxCharacters(int nMaxChars)
{
	if (m_nMaxChars == nMaxChars)
		return;

	m_nMaxChars = nMaxChars;
	_Redraw();
}

// Returns the characters considered invalid for text input.
// See also: SetInvalidCharacters
int CAMSEdit::AlphanumericBehavior::GetMaxCharacters() const
{
	return m_nMaxChars;
}

// Returns the current window's text in a valid format
CString CAMSEdit::AlphanumericBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	CString strNewText = strText.Left(m_nMaxChars ? m_nMaxChars : strText.GetLength());

	// Remove any invalid characters from the current window text
	for (int iPos = strNewText.GetLength() - 1; iPos >= 0; iPos--)
	{
		if (m_strInvalidChars.Find(strNewText[iPos]) >= 0)
			strNewText = strNewText.Left(iPos) + strNewText.Mid(iPos + 1);
	}

	return strNewText;
}

void CAMSEdit::AlphanumericBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	if (!m_strInvalidChars.IsEmpty())
	{
		// Check if the character is invalid
		if (m_strInvalidChars.Find((TCHAR)uChar) >= 0)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}
	}

	TCHAR c = (TCHAR)uChar;
	
	// If the number of characters is already at Max, overwrite
	CString strText = m_pEdit->GetText();
	if (strText.GetLength() == m_nMaxChars && m_nMaxChars && _istprint(c))
	{
		int nStart, nEnd;
		m_pEdit->GetSel(nStart, nEnd);

		if (nStart < m_nMaxChars && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nStart + 1);
			m_pEdit->ReplaceSel(CString(c), TRUE);
		}
		return;
	}

	if (_ShouldEnter(c))
		Behavior::_OnChar(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::MaskedBehavior

CAMSEdit::MaskedBehavior::MaskedBehavior(CAMSEdit* pEdit, const CString& strMask /*= _T("")*/) :
	Behavior(pEdit),
	m_strMask(strMask)
{
}

const CString& CAMSEdit::MaskedBehavior::GetMask() const
{
	return m_strMask;
}

void CAMSEdit::MaskedBehavior::SetMask(const CString& strMask)
{
	if (m_strMask == strMask)
		return;

	m_strMask = strMask;
	_Redraw();
}

CString CAMSEdit::MaskedBehavior::GetNumericText() const
{
	CString strText = m_pEdit->GetText();
	CString strResult;

	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];
		if (_istdigit(c))
			strResult += c;
	}

	return strResult;	
}

CString CAMSEdit::MaskedBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	CString strNewText;
	int nMaskLen = m_strMask.GetLength();
	
	// If the mask is empty, allow anything
	if (!nMaskLen)
		strNewText = strText;
	else
	{
		// Remove any characters not in the mask
		for (int iPos = 0, iMaskPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++, iMaskPos++)
		{
			TCHAR c = strText[iPos];
			TCHAR cMask = (iMaskPos < nMaskLen ? m_strMask[iMaskPos] : 0);

			// If we've reached the end of the mask, break
			if (!cMask)
				break;

			// If it's a digit, look for the next digit placeholder (#) in the mask 
			else if (_istdigit(c))
			{
				for (; iMaskPos < nMaskLen; iMaskPos++)
				{
					cMask = m_strMask[iMaskPos];
					if (cMask == '#')
					{
						strNewText += c;
						break;
					}

					strNewText += cMask;
				}
			}

			// If it's the same character as the mask's allow it.
			else if (c == cMask)
				strNewText += c;
			else
				break;
		}
	}
	return strNewText;
}

void CAMSEdit::MaskedBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	TCHAR c = (TCHAR)uChar;

	// If the mask is empty, allow anything
	int nMaskLen = m_strMask.GetLength();
	if (!nMaskLen)
	{
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	// Check that we haven't gone past the mask's length
	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);
	if (nStart >= nMaskLen && c != VK_BACK)
		return;

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	// Check for a non-printable character (such as Ctrl+C)
	if (!_istprint(c))
	{
		if (c == VK_BACK && nStart != nLen)
		{
			m_pEdit->SendMessage(WM_KEYDOWN, VK_LEFT); // move the cursor left
			return;
		}
		
		// Allow backspace only if the cursor is all the way to the right
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	TCHAR cMask = m_strMask[nStart];

	// Check if we're currently on a number mask
	if (cMask == '#')
	{
		if (_istdigit(c) && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
	}
	else	
	{
		// Check if it's the same character as the mask.
		if (cMask == c && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		// If it's a digit, then find the next number placeholder on the mask.
		else if (_istdigit(c))
		{
			CString strMaskPortion = m_strMask.Mid(nStart);
			int nPound = strMaskPortion.Find('#');
			if (nPound >= 0 && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + nPound);
				m_pEdit->ReplaceSel(strMaskPortion.Left(nPound), TRUE);

				keybd_event((BYTE)c, 0, 0, 0);
			}
		}
	}
}

void CAMSEdit::MaskedBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			// If deleting make sure it's the last character or that
			// the selection goes all the way to the end of the text

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			if (nEnd != nLen)
			{
				if (!(nEnd == nStart && nEnd == nLen - 1))
					return;
			}
			break;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::NumericBehavior

// Constructs the object using the given nMaxWholeDigits and nMaxDecimalPlaces.
CAMSEdit::NumericBehavior::NumericBehavior(CAMSEdit* pEdit, int nMaxWholeDigits /*= 9*/, int nMaxDecimalPlaces /*= 4*/) :
	Behavior(pEdit),
	m_nMaxWholeDigits(nMaxWholeDigits >= 0 ? nMaxWholeDigits : -nMaxWholeDigits),
	m_nMaxDecimalPlaces(nMaxDecimalPlaces),
	m_bAllowNegative(nMaxWholeDigits >= 0),
	m_cNegativeSign('-'),
	m_cDecimalPoint('.'),
	m_cGroupSeparator(','),
	m_nDigitsInGroup(0)
{
	ASSERT(m_nMaxWholeDigits);			// must have at least 1 digit to the left of the decimal
	ASSERT(m_nMaxDecimalPlaces >= 0);	// decimal places must be positive

	// Get the system's decimal point	
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, &m_cNegativeSign, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, &m_cNegativeSign, sizeof(m_cNegativeSign));

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &m_cDecimalPoint, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &m_cDecimalPoint, sizeof(m_cDecimalPoint));

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, &m_cGroupSeparator, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, &m_cGroupSeparator, sizeof(m_cGroupSeparator));
}

void CAMSEdit::NumericBehavior::SetMaxWholeDigits(int nMaxWholeDigits)
{
	ASSERT(nMaxWholeDigits);

	// If nMaxWholeDigits is negative, don't allow negatives
	bool bAllowNegative = (nMaxWholeDigits >= 0);
	if (nMaxWholeDigits < 0)
		nMaxWholeDigits = -nMaxWholeDigits;
		
	if (m_nMaxWholeDigits == nMaxWholeDigits && m_bAllowNegative == bAllowNegative)
		return;

	m_nMaxWholeDigits = nMaxWholeDigits;
	m_bAllowNegative = bAllowNegative;
	_Redraw();
}

int CAMSEdit::NumericBehavior::GetMaxWholeDigits() const
{
	return m_nMaxWholeDigits;
}

void CAMSEdit::NumericBehavior::SetMaxDecimalPlaces(int nMaxDecimalPlaces)
{
	ASSERT(nMaxDecimalPlaces >= 0);
	if (m_nMaxDecimalPlaces == nMaxDecimalPlaces)
		return;

	m_nMaxDecimalPlaces = nMaxDecimalPlaces;
	_Redraw();
}

int CAMSEdit::NumericBehavior::GetMaxDecimalPlaces() const
{
	return m_nMaxDecimalPlaces;
}

// Sets whether the negative sign is allowed in the number or not.
void CAMSEdit::NumericBehavior::AllowNegative(bool bAllowNegative /*= true*/)
{
	if (m_bAllowNegative == bAllowNegative)
		return;

	m_bAllowNegative = bAllowNegative;
	_Redraw();
}

// Returns true if the negative sign is allowed in the number.
bool CAMSEdit::NumericBehavior::IsNegativeAllowed() const
{
	return m_bAllowNegative;
}

// Sets the number of digits to be grouped together (if any).
void CAMSEdit::NumericBehavior::SetDigitsInGroup(int nDigitsInGroup)
{
	ASSERT(nDigitsInGroup >= 0);
	if (m_nDigitsInGroup == nDigitsInGroup)
		return;

	m_nDigitsInGroup = nDigitsInGroup;
	_Redraw();
}

// Returns the number of digits to be grouped together (if any).
int CAMSEdit::NumericBehavior::GetDigitsInGroup() const
{
	return m_nDigitsInGroup;
}

void CAMSEdit::NumericBehavior::SetPrefix(const CString& strPrefix)
{
	if (m_strPrefix == strPrefix)
		return;

	m_strPrefix = strPrefix;
	_Redraw();
}

const CString& CAMSEdit::NumericBehavior::GetPrefix() const
{
	return m_strPrefix;
}

// Parses the given strMask to set the control's configuration.
void CAMSEdit::NumericBehavior::SetMask(const CString& strMask)
{
	int nDecimalPos = -1;
	int nGroupingPos = -1;
	int nLen = strMask.GetLength();

	m_nMaxWholeDigits = 0;
	m_nMaxDecimalPlaces = 0;
	m_nDigitsInGroup = 0;
	m_bAllowNegative = true;
	m_strPrefix = _T("");

	for (int iPos = nLen - 1; iPos >= 0; iPos--)
	{
		TCHAR c = strMask[iPos];
		if (c == '#')
		{
			if (nDecimalPos >= 0)
				m_nMaxWholeDigits++;
			else
				m_nMaxDecimalPlaces++;
		}
		else if ((c == '.' || c == m_cDecimalPoint) && nDecimalPos < 0)
		{
			nDecimalPos = iPos;
			m_cDecimalPoint = c;
		}
		else if (c == ',' || c == m_cGroupSeparator)
		{
			if (!m_nDigitsInGroup)
			{
				m_nDigitsInGroup = (((nDecimalPos >= 0) ? nDecimalPos : nLen) - iPos) - 1;
				m_cGroupSeparator = c;
			}
		}
		else
		{
			m_strPrefix = strMask.Left(iPos + 1);
			break;
		}
	}

	if (nDecimalPos < 0)
	{
		m_nMaxWholeDigits = m_nMaxDecimalPlaces;
		m_nMaxDecimalPlaces = 0;
	}

	ASSERT(m_nMaxWholeDigits > 0);	// must have at least one digit on left side of decimal point
	_Redraw();
}

// Gets the mask corresponding to the maximum number than can be entered into the control
CString CAMSEdit::NumericBehavior::GetMask() const
{
	CString strMask;

	for (int iDigit = 0; iDigit < m_nMaxWholeDigits; iDigit++)
		strMask += '0';

	if (m_nMaxDecimalPlaces)
		strMask += m_cDecimalPoint;

	for (int iDigit = 0; iDigit < m_nMaxDecimalPlaces; iDigit++)
		strMask += '0';

	strMask = GetSeparatedText(strMask);

	for (int iPos = 0, nLen = strMask.GetLength(); iPos < nLen; iPos++)
	{
		if (strMask[iPos] == '0')
			strMask.SetAt(iPos, '#');
	}

	return strMask;
}

// Returns the number of group separator characters in the given strText.
int CAMSEdit::NumericBehavior::GetGroupSeparatorCount(const CString& strText) const
{
	int nSepCount = 0;
	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		if (strText[iPos] == m_cGroupSeparator)
			nSepCount++;
	}

	return nSepCount;
}

// Returns the given strText as a numeric string.
CString CAMSEdit::NumericBehavior::GetNumericText(const CString& strText) const
{
	CString strNewText;

	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];
		if (_istdigit(c) || c == m_cNegativeSign || c == m_cDecimalPoint)
			strNewText += c;
	}

	return strNewText;
}

// Returns the current text as a double value.
double CAMSEdit::NumericBehavior::GetTextAsDouble() const
{
	return _tcstod(GetNumericText(m_pEdit->GetText()), NULL);
}

// Adjusts the location of separators based on the nCurrentSeparatorCount.
void CAMSEdit::NumericBehavior::AdjustSeparators(int nCurrentSeparatorCount)
{
	SelectionSaver selection = m_pEdit;

	CString strText = _GetValidText();
	if (strText.IsEmpty() && selection.GetStart() <= m_strPrefix.GetLength())
		m_pEdit->SetWindowText(m_strPrefix.Mid(0, selection.GetStart()));
	else
		m_pEdit->SetWindowText(strText);

	// Adjust the current selection if separators were added/removed
	int nNewSeparatorCount = GetGroupSeparatorCount(strText);
	if (nCurrentSeparatorCount != nNewSeparatorCount && selection.GetStart() > m_strPrefix.GetLength())
		selection += (nNewSeparatorCount - nCurrentSeparatorCount);
}

// Returns the given text with the group separator characters inserted in the proper places.
CString CAMSEdit::NumericBehavior::GetSeparatedText(const CString& strText) const
{
	CString strNumericText = GetNumericText(strText);
	CString strNewText = strNumericText;
	
	// Retrieve the number without the decimal point
	int nDecimalPos = strNumericText.Find(m_cDecimalPoint);
	if (nDecimalPos >= 0)
		strNewText = strNewText.Left(nDecimalPos);

	if (m_nDigitsInGroup > 0)
	{
		int nLen = strNewText.GetLength();
		BOOL bIsNegative = (!strNewText.IsEmpty() && strNewText[0] == m_cNegativeSign);

		// Loop in reverse and stick the separator every m_nDigitsInGroup digits.
		for (int iPos = nLen - (m_nDigitsInGroup + 1); iPos >= bIsNegative; iPos -= m_nDigitsInGroup)
			strNewText = strNewText.Left(iPos + 1) + m_cGroupSeparator + strNewText.Mid(iPos + 1);
	}

	// Prepend the prefix, if the number is not empty.
	if (!strNewText.IsEmpty() || nDecimalPos >= 0)
	{
		strNewText = m_strPrefix + strNewText;

		if (nDecimalPos >= 0)
			strNewText += strNumericText.Mid(nDecimalPos);
	}

	return strNewText;
}

CString CAMSEdit::NumericBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	CString strNewText;
	BOOL bIsNegative = FALSE;
	int nPrefixLen = m_strPrefix.GetLength();

	// Remove any invalid characters from the number
	for (int iPos = 0, nDecimalPos = -1, nNewLen = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];

		// Check for a negative sign
		if (iPos == nPrefixLen && c == m_cNegativeSign && m_bAllowNegative)
		{
			strNewText += c;
			nNewLen++;
			bIsNegative = TRUE;
		}
		// Check for a digit
		else if (_istdigit(c))
		{
			// Make sure it doesn't go beyond the limits
			if (nDecimalPos < 0 && nNewLen == m_nMaxWholeDigits + bIsNegative)
				continue;

			if (nDecimalPos >= 0 && nNewLen > nDecimalPos + m_nMaxDecimalPlaces)
				break;

			strNewText += c;
			nNewLen++;
		}
		// Check for a decimal point
		else if (c == m_cDecimalPoint && nDecimalPos < 0)
		{
			if (m_nMaxDecimalPlaces == 0)
				break;

			strNewText += c;
			nDecimalPos = nNewLen;
			nNewLen++;
		}
	}

	return GetSeparatedText(strNewText);
}

void CAMSEdit::NumericBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	TCHAR c = (TCHAR)uChar;

	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);

	CString strText = m_pEdit->GetText();
	CString strNumericText = GetNumericText(strText);
	int nDecimalPos = strText.Find(m_cDecimalPoint);
	int nNumericDecimalPos = strNumericText.Find(m_cDecimalPoint);
	int nLen = strText.GetLength();
	int nNumericLen = strNumericText.GetLength();
	int nPrefixLen = m_strPrefix.GetLength();
	int nSepCount = GetGroupSeparatorCount(strText);
	bool bNeedAdjustment = false;

	// Check if we're in the prefix's location
	if (nStart < nPrefixLen && c != VK_BACK)
	{
		TCHAR cPrefix = m_strPrefix[nStart];

		// Check if it's the same character as the prefix.
		if (cPrefix == c && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		// If it's a part of the number, enter the prefix
		else if ((_istdigit(c) || c == m_cNegativeSign || c == m_cDecimalPoint) && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nPrefixLen);
			m_pEdit->ReplaceSel(m_strPrefix.Mid(nStart), TRUE);

			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		
		return;
	}

	// Check if it's a negative sign
	if (c == m_cNegativeSign && m_bAllowNegative)
	{
		// If it's at the beginning, determine if it should overwritten
		if (nStart == nPrefixLen)
		{
			if (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(m_cNegativeSign), TRUE);
				return;
			}
		}
		// If we're not at the beginning, toggle the sign
		else if (_ShouldEnter(c))
		{
			if (strNumericText[0] == m_cNegativeSign)
			{
				m_pEdit->SetSel(nPrefixLen, nPrefixLen + 1);
				m_pEdit->ReplaceSel(_T(""), TRUE);
				m_pEdit->SetSel(nStart - 1, nEnd - 1);
			}
			else
			{
				m_pEdit->SetSel(nPrefixLen, nPrefixLen);
				m_pEdit->ReplaceSel(CString(m_cNegativeSign), TRUE);
				m_pEdit->SetSel(nStart + 1, nEnd + 1);
			}

			return;
		}
	}

	// Check if it's a decimal point (only one is allowed).
	else if (c == m_cDecimalPoint && m_nMaxDecimalPlaces > 0)
	{
		if (nDecimalPos >= 0)
		{
			if (_ShouldEnter(c))
				 m_pEdit->SetSel(nDecimalPos + 1, nDecimalPos + 1);
			return;
		}
	}

	// Check if it's a digit
	else if (_istdigit(c))
	{
		// Check if we're on the right of the decimal point.
		if (nDecimalPos >= 0 && nDecimalPos < nStart)
		{
			if (strNumericText.Mid(nNumericDecimalPos + 1).GetLength() == m_nMaxDecimalPlaces)
			{
				if (nStart <= nDecimalPos + m_nMaxDecimalPlaces && _ShouldEnter(c))
				{
					m_pEdit->SetSel(nStart, nStart + 1);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				return;
			}
		}

		// We're on the left side of the decimal point
		else 
		{
			BOOL bIsNegative = (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign);

			// Make sure we can still enter digits.
			if (nStart == m_nMaxWholeDigits + bIsNegative + nSepCount + nPrefixLen)
				return;

			if (strNumericText.Mid(0, nNumericDecimalPos >= 0 ? nNumericDecimalPos : nNumericLen).GetLength() == m_nMaxWholeDigits + bIsNegative)
			{
				if (_ShouldEnter(c))
				{
					if (strText[nStart] == m_cGroupSeparator)
						nStart++;

					m_pEdit->SetSel(nStart, nStart + 1);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				return;
			}

			bNeedAdjustment = true;
		}
	}

	// Check if it's a non-printable character, such as Backspace or Ctrl+C
	else if (!_istprint(c))
		bNeedAdjustment = true;
	else
		return;

	// Check if the character should be entered
	if (!_ShouldEnter(c))
		return;
	
	Behavior::_OnChar(uChar, nRepCnt, nFlags);

	// If the decimal point was added/removed or a separator needs adding/removing, adjust the text
	if (bNeedAdjustment || nNumericDecimalPos != GetNumericText(m_pEdit->GetText()).Find(m_cDecimalPoint))
		AdjustSeparators(nSepCount);
}

void CAMSEdit::NumericBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			// If deleting the prefix, don't allow it if there's a number after it.
			int nPrefixLen = m_strPrefix.GetLength();
			if (nStart < nPrefixLen && nLen > nPrefixLen)
			{
				if (nEnd == nLen)
					break;
				return;
			}

			// Allow the deletion and then adjust the value
			int nSepCount = GetGroupSeparatorCount(strText);
			Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);

			AdjustSeparators(nSepCount);

			// If everything on the right was deleted, put the selection on the right
			if (nEnd == nLen)
				m_pEdit->SetSel(nStart, nStart);

			return;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::DateBehavior

#define AMS_MIN_CTIME			CTime(1970, 1, 1, 0, 0, 0)
#define AMS_MAX_CTIME			CTime(2037, 12, 31, 23, 59, 59)
#define AMS_MIN_OLEDATETIME		COleDateTime(1900, 1, 1, 0, 0, 0)
#define AMS_MAX_OLEDATETIME		COleDateTime(9999, 12, 31, 23, 59, 59)
#define AMS_DATE_SEP			'/'
#define AMS_DATE_SEP_STR		_T("/")


CAMSEdit::DateBehavior::DateBehavior(CAMSEdit* pEdit) :
	Behavior(pEdit),
	m_dateCurrent(COleDateTime::GetCurrentTime()),
	m_dateMin(AMS_MIN_OLEDATETIME),
	m_dateMax(AMS_MAX_OLEDATETIME),
	m_cSep(AMS_DATE_SEP),
	m_uFlags(None)
{
	// Get the system's date separator
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, &m_cSep, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, &m_cSep, sizeof(m_cSep));

	// Determine if the day should go before the month
	TCHAR szShortDate[MAX_PATH];
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, szShortDate, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, szShortDate, sizeof(szShortDate));

		for (int iPos = 0; szShortDate[iPos]; iPos++)
		{
			TCHAR c = _totupper(szShortDate[iPos]);
			if (c == 'M')	// see if the month is first
				break;
			if (c == 'D')	// see if the day is first, and then set the flag
			{
				m_uFlags |= DayBeforeMonth;
				break;
			}
		}
	}
}

void CAMSEdit::DateBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	TCHAR c = (TCHAR)uChar;
	int nDigit = _ttoi(CString(c));

	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	// Check for a non-printable character (such as Ctrl+C)
	if (!_istprint(c))
	{
		if (c == VK_BACK && nStart != nLen)
		{
			m_pEdit->SendMessage(WM_KEYDOWN, VK_LEFT); // move the cursor left
			return;
		}
		
		// Allow backspace only if the cursor is all the way to the right
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	// Add the digit depending on its location
	switch (nStart)
	{
		case 0:		// FIRST DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidDayDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidDay(GetDay()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinDayDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			else
			{
				if (IsValidMonthDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidMonth(GetMonth()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinMonthDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
						AdjustMaxDay();
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			break;
		}
		case 1:		// SECOND DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			else
			{
				if (IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (GetDay() > 0 && AdjustMaxDay())
							m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 2);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			break;
		}
		
		case 2:		// FIRST SLASH
		{
			int nSlash = 0;
			if (c == m_cSep)
				nSlash = 1;
			else
			{
				if (m_uFlags & DayBeforeMonth)
					nSlash = (IsValidMonthDigit(c, 0) ? 2 : 0);
				else
					nSlash = (IsValidDayDigit(c, 0) ? 2 : 0);
			}

			// If we need the slash, enter it
			if (nSlash && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
			}

			// If the slash is to be preceded by a valid digit, "type" it in.
			if (nSlash == 2)
				keybd_event((BYTE)c, 0, 0, 0);
			break;
		}

		case 3:		// THIRD DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidMonthDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidMonth(GetMonth()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinMonthDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
						AdjustMaxDay();
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			else
			{
				if (IsValidDayDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidDay(GetDay()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinDayDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			break;			
		}

		case 4:		// FOURTH DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (GetDay() > 0 && AdjustMaxDay())
							m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 2);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			else
			{
				if (IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
			}
			break;
		}

		case 5:		// SECOND SLASH	(year's first digit)
		{
			int nSlash = 0;
			if (c == m_cSep)
				nSlash = 1;
			else
				nSlash = (IsValidYearDigit(c, 0) ? 2 : 0);

			// If we need the slash, enter it
			if (nSlash && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
			}

			// If the slash is to be preceded by a valid digit, "type" it in.
			if (nSlash == 2)
				keybd_event((BYTE)c, 0, 0, 0);
			break;			
		}

		case 6:		// YEAR (all 4 digits)
		case 7:
		case 8:
		case 9:
		{
			if (IsValidYearDigit(c, nStart - GetYearStartPosition()) && _ShouldEnter(c))
			{
				if (nLen > nStart)
				{
					m_pEdit->SetSel(nStart, nStart + 1, FALSE);
					m_pEdit->ReplaceSel(CString(c), TRUE);

					if (nLen > nStart + 1)
					{
						if (!IsValidYearDigit(strText[nStart + 1], nStart - (GetYearStartPosition() - 1)))
						{
							m_pEdit->SetSel(nStart + 1, 10, FALSE);
							CString strPortion;
							for (int iPos = nStart + 1; iPos < 10; iPos++)
								strPortion += GetMinYearDigit(iPos - GetYearStartPosition(), true);
							
							m_pEdit->ReplaceSel(strPortion, TRUE);
							m_pEdit->SetSel(nStart + 1, 10, FALSE);
						}
					}
				}
				else
					Behavior::_OnChar(uChar, nRepCnt, nFlags);

				if (IsValidYear(GetYear()))
				{
					AdjustMaxDay();			// adjust the day first
					AdjustMaxMonthAndDay();	// then adjust the month and the day, if necessary
				}
			}
			break;
		}
	}
}

void CAMSEdit::DateBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			// If deleting make sure it's the last character or that
			// the selection goes all the way to the end of the text

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			if (nEnd != nLen)
			{
				if (!(nEnd == nStart && nEnd == nLen - 1))
					return;
			}
			break;
		}

		case VK_UP:
		{
			// If pressing the UP arrow, increment the corresponding value.

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			if (nStart >= GetYearStartPosition() && nStart <= GetYearStartPosition() + 4)
			{
				int nYear = GetYear();
				if (nYear >= m_dateMin.GetYear() && nYear < m_dateMax.GetYear())
					SetYear(++nYear);
			}

			else if (nStart >= GetMonthStartPosition() && nStart <= GetMonthStartPosition() + 2)
			{
				int nMonth = GetMonth();
				if (nMonth >= GetMinMonth() && nMonth < GetMaxMonth())
					SetMonth(++nMonth);
			}

			else if (nStart >= GetDayStartPosition() && nStart <= GetDayStartPosition() + 2)
			{
				int nDay = GetDay();
				if (nDay >= GetMinDay() && nDay < GetMaxDay())
					SetDay(++nDay);
			}
			
			return;
		}

		case VK_DOWN:
		{
			// If pressing the DOWN arrow, decrement the corresponding value.

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			if (nStart >= GetYearStartPosition() && nStart <= GetYearStartPosition() + 4)
			{
				int nYear = GetYear();
				if (nYear > m_dateMin.GetYear())
					SetYear(--nYear);
			}

			else if (nStart >= GetMonthStartPosition() && nStart <= GetMonthStartPosition() + 2)
			{
				int nMonth = GetMonth();
				if (nMonth > GetMinMonth())
					SetMonth(--nMonth);
			}

			else if (nStart >= GetDayStartPosition() && nStart <= GetDayStartPosition() + 2)
			{
				int nDay = GetDay();
				if (nDay > GetMinDay())
					SetDay(--nDay);
			}
			
			return;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

CString CAMSEdit::DateBehavior::GetString(int nValue, bool bTwoDigitWithLeadingZero /*= true*/)
{
	CString strValue;
	if (bTwoDigitWithLeadingZero)
		strValue.Format(_T("%02d"), nValue);
	else
		strValue.Format(_T("%d"), nValue);
	return strValue;
}

int CAMSEdit::DateBehavior::GetMonthStartPosition() const
{
	return ((m_uFlags & DayBeforeMonth) ? 3 : 0);
}

int CAMSEdit::DateBehavior::GetDayStartPosition() const
{
	return ((m_uFlags & DayBeforeMonth) ? 0 : 3);
}

int CAMSEdit::DateBehavior::GetYearStartPosition() const
{
	return 6;
}

int CAMSEdit::DateBehavior::GetMaxMonth() const
{
	if (GetValidYear() == m_dateMax.GetYear())
		return m_dateMax.GetMonth();
	return 12;
}

int CAMSEdit::DateBehavior::GetMinMonth() const
{
	if (GetValidYear() == m_dateMin.GetYear())
		return m_dateMin.GetMonth();
	return 1;
}

int CAMSEdit::DateBehavior::GetMaxDay() const
{
	int nYear = GetValidYear();
	int nMonth = GetValidMonth();

	if (nYear == m_dateMax.GetYear() && nMonth == m_dateMax.GetMonth())
		return m_dateMax.GetDay();

	return GetMaxDayOfMonth(nMonth, nYear);
}

int CAMSEdit::DateBehavior::GetMinDay() const
{
	int nYear = GetValidYear();
	int nMonth = GetValidMonth();

	if (nYear == m_dateMin.GetYear() && nMonth == m_dateMin.GetMonth())
		return m_dateMin.GetDay();

	return 1;
}

int CAMSEdit::DateBehavior::GetMaxDayOfMonth(int nMonth, int nYear)
{
	ASSERT(nMonth >= 1 && nMonth <= 12);

	switch (nMonth)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;

		case 2:
			return IsLeapYear(nYear) ? 29 : 28;
	}
	return 31;
}

TCHAR CAMSEdit::DateBehavior::GetMaxMonthDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nYear = GetValidYear();
	int nMaxMonth = m_dateMax.GetMonth();
	int nMaxYear = m_dateMax.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year is at the max, then use the first digit of the max month
		if (nYear == nMaxYear)
			return GetString(nMaxMonth)[0];

		// Otherwise, it's always '1'
		return '1';
	}

	// Second digit
	CString strWindowText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strWindowText.GetLength() > GetMonthStartPosition()) ? strWindowText[GetMonthStartPosition()] : 0;
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// If the year is at the max, then check if the first digits match
	if (nYear == nMaxYear && IsValidYear(GetYear()))
	{
		// If the first digits match, then use the second digit of the max month
		if (GetString(nMaxMonth)[0] == cFirstDigit)
			return GetString(nMaxMonth)[1];

		// Assuming the logic for the first digit is correct, then it must be '0'
		ASSERT(cFirstDigit == '0');
		return '9';  
	}

	// Use the first digit to determine the second digit's max
	return (cFirstDigit == '1' ? '2' : '9');
}

TCHAR CAMSEdit::DateBehavior::GetMinMonthDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nYear = GetValidYear();
	int nMinMonth = m_dateMin.GetMonth();
	int nMinYear = m_dateMin.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year is at the min, then use the first digit of the min month
		if (nYear == nMinYear)
			return GetString(nMinMonth)[0];

		// Otherwise, it's always '0'
		return '0';
	}

	// Second digit
	CString strWindowText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strWindowText.GetLength() > GetMonthStartPosition()) ? strWindowText[GetMonthStartPosition()] : 0;
	if (!cFirstDigit)
		return '1';

	// If the year is at the max, then check if the first digits match
	if (nYear == nMinYear && IsValidYear(GetYear()))
	{
		// If the first digits match, then use the second digit of the max month
		if (GetString(nMinMonth)[0] == cFirstDigit)
			return GetString(nMinMonth)[1];

		return '0';  
	}

	// Use the first digit to determine the second digit's min
	return (cFirstDigit == '1' ? '0' : '1');
}

bool CAMSEdit::DateBehavior::IsValidMonthDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinMonthDigit(nPos) && c <= GetMaxMonthDigit(nPos));
}

bool CAMSEdit::DateBehavior::IsValid(const COleDateTime& date) const
{
	return (date.GetStatus() == COleDateTime::valid && date >= m_dateMin && date <= m_dateMax);
}

bool CAMSEdit::DateBehavior::IsValidMonth(int nMonth) const
{
	int nYear = GetValidYear();
	return IsValid(COleDateTime(nYear, nMonth, 1, 0, 0, 0));
}

TCHAR CAMSEdit::DateBehavior::GetMaxDayDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nMonth = GetValidMonth();
	int nYear = GetValidYear();
	int nMaxDay = m_dateMax.GetDay();
	int nMaxMonth = m_dateMax.GetMonth();
	int nMaxYear = m_dateMax.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year and month are at the max, then use the first digit of the max day
		if (nYear == nMaxYear && nMonth == nMaxMonth)
			return GetString(nMaxDay)[0];
		return GetString(GetMaxDayOfMonth(nMonth, nYear))[0];
	}

	// Second digit
	CString strWindowText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strWindowText.GetLength() > GetDayStartPosition()) ? strWindowText[GetDayStartPosition()] : 0;
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// If the year and month are at the max, then use the second digit of the max day
	if (nYear == nMaxYear && nMonth == nMaxMonth && GetString(nMaxDay)[0] == cFirstDigit)
		return GetString(nMaxDay)[1];

	if (cFirstDigit == '0' || 
		cFirstDigit == '1' || 
		(cFirstDigit == '2' && nMonth != 2) || 
		(nMonth == 2 && !IsValidYear(GetYear())))
		return '9';
	return GetString(GetMaxDayOfMonth(nMonth, nYear))[1];
}

TCHAR CAMSEdit::DateBehavior::GetMinDayDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nMonth = GetValidMonth();
	int nYear = GetValidYear();
	int nMinDay = m_dateMin.GetDay();
	int nMinMonth = m_dateMin.GetMonth();
	int nMinYear = m_dateMin.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year and month are at the min, then use the first digit of the min day
		if (nYear == nMinYear && nMonth == nMinMonth)
			return GetString(nMinDay)[0];
		return '0';
	}

	// Second digit
	CString strWindowText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strWindowText.GetLength() > GetDayStartPosition()) ? strWindowText[GetDayStartPosition()] : 0;
	if (!cFirstDigit)  // must have a valid first digit at this point
		return '1';

	// If the year and month are at the max, then use the first second of the max day
	if (nYear == nMinYear && nMonth == nMinMonth && GetString(nMinDay)[0] == cFirstDigit)
		return GetString(nMinDay)[1];

	// Use the first digit to determine the second digit's min
	return (cFirstDigit == '0' ? '1' : '0');
}

bool CAMSEdit::DateBehavior::IsValidDayDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinDayDigit(nPos) && c <= GetMaxDayDigit(nPos));
}

bool CAMSEdit::DateBehavior::IsValidDay(int nDay) const
{
	return IsValid(COleDateTime(GetValidYear(), GetValidMonth(), nDay, 0, 0, 0));
}

bool CAMSEdit::DateBehavior::IsValidYear(int nYear) const
{
	return (nYear >= m_dateMin.GetYear() && nYear <= m_dateMax.GetYear());
}

bool CAMSEdit::DateBehavior::AdjustMaxMonthAndDay()
{
	int nMonth = GetMonth();	
	if (nMonth && !IsValidMonth(nMonth))
	{
		SetMonth(GetMinMonth());  // this adjusts the day automatically
		return true;
	}

	return AdjustMaxDay();
}

bool CAMSEdit::DateBehavior::AdjustMaxDay()
{
	int nDay = GetDay();
	if (nDay && !IsValidDay(nDay))
	{
		SetDay(GetMaxDay());
		return true;
	}
	
	return false;	// nothing had to be adjusted
}

TCHAR CAMSEdit::DateBehavior::GetMaxYearDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 3);

	CString strYear = GetString(GetYear(), false);
	CString strMaxYear = GetString(m_dateMax.GetYear(), false);

	if (nPos == 0 || strMaxYear.Left(nPos) == strYear.Left(nPos))
		return strMaxYear[nPos];
	return '9';
}

TCHAR CAMSEdit::DateBehavior::GetMinYearDigit(int nPos, bool bValidYear /*= false*/) const
{
	ASSERT(nPos >= 0 && nPos <= 3);

	int nYear = GetYear();
	if (!IsValidYear(nYear) && bValidYear)
		nYear = GetValidYear();

	CString strYear = GetString(nYear, false);
	CString strMinYear = GetString(m_dateMin.GetYear(), false);

	if (nPos == 0 || strMinYear.Left(nPos) == strYear.Left(nPos))
		return strMinYear[nPos];
	return '0';
}

bool CAMSEdit::DateBehavior::IsValidYearDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinYearDigit(nPos) && c <= GetMaxYearDigit(nPos));
}

int CAMSEdit::DateBehavior::GetValidMonth() const
{
	int nMonth = GetMonth();
	
	// It it's outside the range, fix it
	if (nMonth < GetMinMonth())
		nMonth = GetMinMonth();
	else if (nMonth > GetMaxMonth())
		nMonth = GetMaxMonth();

	return nMonth;
}

int CAMSEdit::DateBehavior::GetMonth() const
{
	CString strText = m_pEdit->GetText();
	if (strText.GetLength() >= 2)
		return _ttoi(strText.Mid(0, 2));
	return 0;
}

int CAMSEdit::DateBehavior::GetValidYear() const
{
	int nYear = GetYear();
	if (nYear < m_dateMin.GetYear())
		nYear = m_dateMin.GetYear();
	else if (nYear > m_dateMax.GetYear())
		nYear = m_dateMax.GetYear();

	return nYear;
}

int CAMSEdit::DateBehavior::GetYear() const
{
	CString strText = m_pEdit->GetText();

	int nSlash = strText.ReverseFind(m_cSep);
	if (nSlash > 0)
		return _ttoi(strText.Mid(nSlash + 1));
	return 0;
}

int CAMSEdit::DateBehavior::GetDay() const
{
	CString strText = m_pEdit->GetText();

	int nSlash = strText.Find(m_cSep);
	if (nSlash > 0)
		return _ttoi(strText.Mid(nSlash + 1, 2));
	return 0;
}

int CAMSEdit::DateBehavior::GetValidDay() const
{
	int nDay = GetDay();

	// It it's outside the range, fix it
	if (nDay < GetMinDay())
		nDay = GetMinDay();
	else if (nDay > GetMaxDay())
		nDay = GetMaxDay();

	return nDay;
}

void CAMSEdit::DateBehavior::SetYear(int nYear)
{
	ASSERT(IsValidYear(nYear));

	SelectionSaver selection = m_pEdit;	// remember the current selection

	if (GetYear() > 0)		// see if there's already a year
		m_pEdit->SetSel(GetYearStartPosition(), GetYearStartPosition() + 4);

	CString strText;
	strText.Format(_T("%4d"), nYear);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the year
	
	AdjustMaxMonthAndDay();	// adjust the month and/or day if they're out of range
}

void CAMSEdit::DateBehavior::SetMonth(int nMonth)
{
	ASSERT(IsValidMonth(nMonth));

	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetMonth() > 0)		// see if there's already a month
		m_pEdit->SetSel(GetMonthStartPosition(), GetMonthStartPosition() + 3);

	CString strText;
	strText.Format(_T("%02d%c"), nMonth, m_cSep);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the month

	AdjustMaxDay();	// adjust the day if it's out of range
}

void CAMSEdit::DateBehavior::SetDay(int nDay)
{
	ASSERT(IsValidDay(nDay));

	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetDay() > 0)		// see if there's already a day
		m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 3);

	CString strText;
	strText.Format(_T("%02d%c"), nDay, m_cSep);

	m_pEdit->ReplaceSel(strText, TRUE);	// set the day
}

void CAMSEdit::DateBehavior::ShowErrorMessage() const
{
	CString strMessage;
	strMessage.Format(_T("Please specify a date between %s and %s."), m_dateMin.Format(_T("%m/%d/%Y")), m_dateMax.Format(_T("%m/%d/%Y")));
	AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
}

CTime CAMSEdit::DateBehavior::GetDate() const
{
	return CTime(GetYear(), GetMonth(), GetDay(), 0, 0, 0);
}

COleDateTime CAMSEdit::DateBehavior::GetOleDate() const
{
	return COleDateTime(GetYear(), GetMonth(), GetDay(), 0, 0, 0);
}

void CAMSEdit::DateBehavior::SetDate(int nYear, int nMonth, int nDay)
{
#ifdef _DEBUG	// verify it's within the range (when debugging)
	COleDateTime date(nYear, nMonth, nDay, 0, 0, 0);
	ASSERT(date >= m_dateMin); 
	ASSERT(date <= m_dateMax); 
#endif
	CString strText;

	if (m_uFlags & DayBeforeMonth)
		strText.Format(_T("%02d%c%02d%c%4d"), nDay, m_cSep, nMonth, m_cSep, nYear);
	else
		strText.Format(_T("%02d%c%02d%c%4d"), nMonth, m_cSep, nDay, m_cSep, nYear);

	m_pEdit->SetWindowText(strText);
}

void CAMSEdit::DateBehavior::SetDate(const CTime& date)
{
	SetDate(date.GetYear(), date.GetMonth(), date.GetDay());
}

void CAMSEdit::DateBehavior::SetDate(const COleDateTime& date)
{
	SetDate(date.GetYear(), date.GetMonth(), date.GetDay());
}

void CAMSEdit::DateBehavior::SetDateToToday()
{
	SetDate(COleDateTime::GetCurrentTime());
}

bool CAMSEdit::DateBehavior::IsLeapYear(int nYear)
{
	return (nYear & 3) == 0 && (nYear % 100 != 0 || nYear % 400 == 0); 
}

bool CAMSEdit::DateBehavior::IsValid() const
{
	COleDateTime date(GetYear(), GetMonth(), GetDay(), 0, 0, 0);
	return (date.GetStatus() == COleDateTime::valid && date >= m_dateMin && date <= m_dateMax);
}

bool CAMSEdit::DateBehavior::CheckIfValid(bool bShowErrorIfNotValid /*= true*/)
{
	if (!m_pEdit->IsWindowEnabled())
		return true;

	bool bValid = IsValid();
	if (!bValid && bShowErrorIfNotValid)
	{
		ShowErrorMessage();
		m_pEdit->SetFocus();
	}
	
	return bValid;
}

LONG CAMSEdit::DateBehavior::_OnPaste(UINT, LONG)
{
	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);
	CString strTextBefore = m_pEdit->GetText();

	_Default();

	if (!IsValid())
	{
		MessageBeep(MB_ICONEXCLAMATION);
		m_pEdit->SetWindowText(strTextBefore);
		m_pEdit->SetSel(nStart, nEnd);
	}

	return 0;
}

void CAMSEdit::DateBehavior::SetRange(const CTime& dateMin, const CTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_CTIME);
	ASSERT(dateMax <= AMS_MAX_CTIME);
	ASSERT(dateMin <= dateMax);

	m_dateMin.SetDate(dateMin.GetYear(), dateMin.GetMonth(), dateMin.GetDay());
	m_dateMax.SetDate(dateMax.GetYear(), dateMax.GetMonth(), dateMax.GetDay());

	_Redraw();
}

void CAMSEdit::DateBehavior::SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_OLEDATETIME);
	ASSERT(dateMax <= AMS_MAX_OLEDATETIME);
	ASSERT(dateMin <= dateMax);

	m_dateMin = dateMin;
	m_dateMax = dateMax;
	_Redraw();
}

void CAMSEdit::DateBehavior::GetRange(CTime* pDateMin, CTime* pDateMax) const
{
	ASSERT(pDateMin);
	ASSERT(pDateMax);

	*pDateMin = CTime(m_dateMin.GetYear(), m_dateMin.GetMonth(), m_dateMin.GetDay(), 0, 0, 0);
	*pDateMax = CTime(m_dateMax.GetYear(), m_dateMax.GetMonth(), m_dateMax.GetDay(), 0, 0, 0);
}

void CAMSEdit::DateBehavior::GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const
{
	ASSERT(pDateMin);
	ASSERT(pDateMax);

	*pDateMin = m_dateMin;
	*pDateMax = m_dateMax;
}

void CAMSEdit::DateBehavior::SetSeparator(TCHAR cSep)
{
	ASSERT(cSep);
	ASSERT(!_istdigit(cSep));

	if (m_cSep != cSep)
	{
		m_cSep = cSep;
		_Redraw();
	}
}

TCHAR CAMSEdit::DateBehavior::GetSeparator() const
{
	return m_cSep;
}

void CAMSEdit::DateBehavior::ShowDayBeforeMonth(bool bDayBeforeMonth /*= true*/)
{
	ModifyFlags(bDayBeforeMonth ? DayBeforeMonth : 0, bDayBeforeMonth ? 0 : DayBeforeMonth);
}

bool CAMSEdit::DateBehavior::IsDayShownBeforeMonth() const
{
	return (m_uFlags & DayBeforeMonth) ? true : false;
}

void CAMSEdit::DateBehavior::ModifyFlags(UINT uAdd, UINT uRemove)
{
	UINT uFlags = (m_uFlags & ~uRemove) | uAdd;

	if (m_uFlags != uFlags)
	{
		m_uFlags = uFlags;
		_Redraw();
	}
}

UINT CAMSEdit::DateBehavior::GetFlags() const
{
	return m_uFlags;
}

CString CAMSEdit::DateBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();

	if (strText.IsEmpty() || IsValid())
		return strText;

	// If the date is empty, try using today
	if (GetYear() == 0 && GetMonth() == 0 && GetDay() == 0)
		((CAMSEdit::DateBehavior*)this)->SetDateToToday();

	int nYear = GetValidYear();
	int nMonth = GetValidMonth();
	int nDay = GetValidDay();

	if (!IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0)))
		nMonth = GetMinMonth();

	if (!IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0)))
		nDay = GetMaxDay();

	CString strNewText;
	
	if (m_uFlags & DayBeforeMonth)
		strNewText.Format(_T("%02d%c%02d%c%4d"), nDay, m_cSep, nMonth, m_cSep, nYear);
	else
		strNewText.Format(_T("%02d%c%02d%c%4d"), nMonth, m_cSep, nDay, m_cSep, nYear);

	return strNewText;
}



/////////////////////////////////////////////////////////////////////////////
// CAMSAlphanumericEdit window

// Constructs the object using the given set of strInvalidChars
CAMSAlphanumericEdit::CAMSAlphanumericEdit(int nMaxChars /*= 0*/, const CString& strInvalidChars /*= _T("%'*\"+?><:\\"")*/) :
	AlphanumericBehavior(this, nMaxChars, strInvalidChars)
{
}

BEGIN_MESSAGE_MAP(CAMSAlphanumericEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSAlphanumericEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the current window's text in a valid format
CString CAMSAlphanumericEdit::GetValidText() const
{
	return _GetValidText();
}

void CAMSAlphanumericEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSMaskedEdit window

// Constructs the object using the given numeric strMask.
CAMSMaskedEdit::CAMSMaskedEdit(const CString& strMask /*= _T("")*/) :
	MaskedBehavior(this, strMask)
{
}

BEGIN_MESSAGE_MAP(CAMSMaskedEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSMaskedEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


// Returns the current window's text in a valid format
CString CAMSMaskedEdit::GetValidText() const
{
	return _GetValidText();
}

void CAMSMaskedEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

void CAMSMaskedEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// Constructs the object using the given nMaxWholeDigits and nMaxDecimalPlaces.
CAMSNumericEdit::CAMSNumericEdit(int nMaxWholeDigits /*= 9*/, int nMaxDecimalPlaces /*= 4*/) :
	NumericBehavior(this, nMaxWholeDigits, nMaxDecimalPlaces)
{
}

BEGIN_MESSAGE_MAP(CAMSNumericEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSNumericEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

CString CAMSNumericEdit::GetValidText() const
{
	return _GetValidText();
}

void CAMSNumericEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

void CAMSNumericEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSIntegerEdit window

// Constructs the object allowing the negative sign or not.
CAMSIntegerEdit::CAMSIntegerEdit(int nMaxWholeDigits /*= 9*/) :
	CAMSNumericEdit(nMaxWholeDigits, 0)
{
}

BEGIN_MESSAGE_MAP(CAMSIntegerEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSIntegerEdit)
	ON_WM_CHAR()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

long CAMSIntegerEdit::GetTextAsLong() const
{
	return _ttol(GetText());
}


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// Constructs the object using the given numeric strMask.
CAMSCurrencyEdit::CAMSCurrencyEdit()
{
	m_nDigitsInGroup = 3;
	m_nMaxDecimalPlaces = 2;
	m_strPrefix = _T("$");
	
	// Get the system's current settings
	TCHAR szValue[10];
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, szValue, sizeof(szValue));
		m_strPrefix = szValue;
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONGROUPING, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONGROUPING, szValue, sizeof(szValue));
		m_nDigitsInGroup = _ttoi(szValue);
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRDIGITS, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRDIGITS, szValue, sizeof(szValue));
		m_nMaxDecimalPlaces = _ttoi(szValue);
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, &m_cDecimalPoint, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, &m_cDecimalPoint, sizeof(m_cDecimalPoint));

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, &m_cGroupSeparator, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, &m_cGroupSeparator, sizeof(m_cGroupSeparator));
}
	
BEGIN_MESSAGE_MAP(CAMSCurrencyEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSCurrencyEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAMSDateEdit

CAMSDateEdit::CAMSDateEdit() :
	DateBehavior(this)
{
}


BEGIN_MESSAGE_MAP(CAMSDateEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSDateEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_GETDLGCODE()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

CString CAMSDateEdit::GetValidText() const
{
	return _GetValidText();
}

void CAMSDateEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

void CAMSDateEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}

void CAMSDateEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);

	if (m_uFlags & OnKillFocus_LeaveAlone)
		return;

	CString strText = GetText();

	// If it's empty, take action based on the flag
	if (strText.IsEmpty())
	{
		if (m_uFlags & OnKillFocus_Beep_IfEmpty)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfEmpty)
			SetWindowText(_T(" "));

		if ((m_uFlags & OnKillFocus_ShowMessage_IfEmpty) == OnKillFocus_ShowMessage_IfEmpty)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfEmpty)
			SetFocus();

		return;
	}
		
	if (!IsValid())
	{
		if (m_uFlags & OnKillFocus_Beep_IfInvalid)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfInvalid)
			Redraw();
		
		if ((m_uFlags & OnKillFocus_ShowMessage_IfInvalid) == OnKillFocus_ShowMessage_IfInvalid)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfInvalid)
			SetFocus();
	}
}

LONG CAMSDateEdit::OnPaste(UINT, LONG)
{
	return _OnPaste(0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSMultiMaskedEdit

CAMSMultiMaskedEdit::CAMSMultiMaskedEdit() :
	AlphanumericBehavior(this),
	NumericBehavior(this),
	MaskedBehavior(this),
	DateBehavior(this),
	m_pCurrentBehavior((AlphanumericBehavior*)this)
{
}

// Returns the mask
const CString& CAMSMultiMaskedEdit::GetMask() const
{
	return MaskedBehavior::m_strMask;
}

// Sets the mask and thereby the edit box's behavior
void CAMSMultiMaskedEdit::SetMask(const CString& strMask)
{
	int nLen = strMask.GetLength();
	MaskedBehavior::m_strMask = strMask;

	// If it doesn't have numeric place holders then it's alphanumeric
	int nPos = strMask.Find('#');
	if (nPos < 0)
	{
		m_pCurrentBehavior = (AlphanumericBehavior*)this;
		SetMaxCharacters(nLen);
		return;
	}

	// If it's exactly like the date mask, then it's a date
	if (nLen == 10 && strMask == _T("##/##/####"))
	{
		m_pCurrentBehavior = (DateBehavior*)this;
		Redraw();
		return;
	}

	// If after the first numeric placeholder, we don't find any foreign characters,
	// then it's numeric, otherwise it's masked numeric.
	CString strSmallMask = strMask.Mid(nPos + 1);
	int nSmallLen = strSmallMask.GetLength();
	for (int iPos = 0; iPos < nSmallLen; iPos++)
	{
		TCHAR c = strSmallMask[iPos];
		if (c != '#' && c != m_cDecimalPoint && c != m_cGroupSeparator)
		{
			m_pCurrentBehavior = (MaskedBehavior*)this;
			MaskedBehavior::m_strMask = _T("");
			MaskedBehavior::SetMask(strMask);
			return;
		}
	}

	// Verify that it ends in a number; otherwise it's a masked numeric
	if (nSmallLen && strSmallMask[nSmallLen - 1] != '#')
	{
		m_pCurrentBehavior = (MaskedBehavior*)this;
		MaskedBehavior::m_strMask = _T("");
		MaskedBehavior::SetMask(strMask);
	}
	else
	{
		m_pCurrentBehavior = (NumericBehavior*)this;
		NumericBehavior::SetMask(strMask);
	}
}

CString CAMSMultiMaskedEdit::GetValidText() const
{
	ASSERT(m_pCurrentBehavior);
	return m_pCurrentBehavior->_GetValidText();
}

BEGIN_MESSAGE_MAP(CAMSMultiMaskedEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSMultiMaskedEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

void CAMSMultiMaskedEdit::OnChar(UINT uChar, UINT nRepCnt, UINT c)
{
	ASSERT(m_pCurrentBehavior);
	m_pCurrentBehavior->_OnChar(uChar, nRepCnt, nRepCnt);
}

void CAMSMultiMaskedEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	ASSERT(m_pCurrentBehavior);
	m_pCurrentBehavior->_OnKeyDown(uChar, nRepCnt, nRepCnt);
}

LONG CAMSMultiMaskedEdit::OnPaste(UINT wParam, LONG lParam)
{
	ASSERT(m_pCurrentBehavior);
	return m_pCurrentBehavior->_OnPaste(wParam, lParam);
}
