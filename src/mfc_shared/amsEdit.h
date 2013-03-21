#if !defined(AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_)
#define AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Edit.h : header file
// Created by: Alvaro Mendez - 07/17/2000
//

#include <afxwin.h>

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit window

// Class CAMSEdit is the base class for all the other AMS CEdit classes.  
// It provides some base functionality to set and get the text and change
// its text and background color.
//
class CAMSEdit : public CEdit
{
public:
	// Construction/destruction
	CAMSEdit();
	virtual ~CAMSEdit();

	// Operations
	void SetText(const CString& strText);
	CString GetText() const;

	void SetBackgroundColor(COLORREF rgb);
	COLORREF GetBackgroundColor() const;

	void SetTextColor(COLORREF rgb);
	COLORREF GetTextColor() const;

protected:
	virtual void Redraw();
	virtual CString GetValidText() const;
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual bool ShouldEnter(TCHAR c) const;

protected:
	CBrush	m_brushBackground;
	COLORREF m_rgbText;

private:
	enum InternalFlags
	{
		None				= 0x0000,
		TextColorHasBeenSet = 0x0001
	};
	UINT m_uInternalFlags;

public:
	// Class SelectionSaver is used to save an edit box's current
	// selection and then restore it on destruction.
	class SelectionSaver
	{
	public:
		SelectionSaver(CEdit* pEdit);
		SelectionSaver(CEdit* pEdit, int nStart, int nEnd);
		~SelectionSaver();

		void MoveTo(int nStart, int nEnd);
		void MoveBy(int nStart, int nEnd);
		void MoveBy(int nPos);
		void operator+=(int nPos);

		int GetStart() const;
		int GetEnd() const;

	protected:
		CEdit* m_pEdit;
		int m_nStart, m_nEnd;
	};

	// Class Behavior is an abstract base class used to define how an edit
	// box will behave when it is used. 
	class Behavior
	{
	protected:
		Behavior(CAMSEdit* pEdit);

	public:
		virtual CString _GetValidText() const = 0;

		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) = 0;
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual LONG _OnPaste(UINT wParam, LONG lParam);

	protected:
		virtual LRESULT _Default();		
		void _Redraw();
		bool _ShouldEnter(TCHAR c) const;

	protected:
		CAMSEdit* m_pEdit;
	};
	friend class Behavior;

	// The AlphanumericBehavior class is used to allow entry of alphanumeric
	// characters.  It can be restricted in terms of what characters cannot 
	// be inputed as well as how many are allowed altogether.
	class AlphanumericBehavior : public Behavior
	{
	public:
		AlphanumericBehavior(CAMSEdit* pEdit, int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

		// Operations
		void SetInvalidCharacters(const CString& strInvalidChars);
		const CString& GetInvalidCharacters() const;

		void SetMaxCharacters(int nMaxChars);
		int GetMaxCharacters() const;

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		int m_nMaxChars;
		CString m_strInvalidChars;
	};

	// The MaskedBehavior class is used to allow entry of numeric characters
	// based on a given mask containing '#' characters to hold digits.
	class MaskedBehavior : public Behavior
	{
	public:
		// Construction
		MaskedBehavior(CAMSEdit* pEdit, const CString& strMask = _T(""));

	public:
		// Operations
		void SetMask(const CString& strMask);
		const CString& GetMask() const;

		CString GetNumericText() const;
	
	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		// Attributes
		CString m_strMask;
	};

	// The NumericBehavior class is used to allow the entry of an actual numeric
	// value into the edit control.  It may be restricted by the number of digits
	// before or after the decimal point (if any).  If can also be set to use
	// commas to separate and group thousands.
	class NumericBehavior : public Behavior
	{
	public:
		// Construction
		NumericBehavior(CAMSEdit* pEdit, int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

	public:
		// Operations
		void SetMaxWholeDigits(int nMaxWholeDigits);
		int GetMaxWholeDigits() const;
		
		void SetMaxDecimalPlaces(int nMaxDecimalPlaces);
		int GetMaxDecimalPlaces() const;
		
		void AllowNegative(bool bAllowNegative = true);
		bool IsNegativeAllowed() const;
		
		void SetDigitsInGroup(int nDigitsInGroup);
		int GetDigitsInGroup() const;
		
		void SetPrefix(const CString& strPrefix);
		const CString& GetPrefix() const;

		void SetMask(const CString& strMask);
		CString GetMask() const;

		double GetTextAsDouble() const;

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);

		int GetGroupSeparatorCount(const CString& strText) const;
		
		CString GetNumericText(const CString& strText) const;
		CString GetSeparatedText(const CString& strText) const;
		void AdjustSeparators(int nCurrentSeparatorCount);

	protected:
		// Attributes
		int m_nMaxWholeDigits;
		int m_nMaxDecimalPlaces;
		bool m_bAllowNegative;
		TCHAR m_cNegativeSign;
		TCHAR m_cDecimalPoint;
		TCHAR m_cGroupSeparator;
		int m_nDigitsInGroup;
		CString m_strPrefix;
	};

	// The DateBehavior class is used to allow the entry of date values.
	class DateBehavior : public Behavior
	{
	public:
		// Construction
		DateBehavior(CAMSEdit* pEdit);

	public:
		// Operations
		void SetDate(int nYear, int nMonth, int nDay);
		void SetDate(const CTime& date);
		void SetDate(const COleDateTime& date);
		void SetDateToToday();

		CTime GetDate() const;
		COleDateTime GetOleDate() const;

		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		void SetYear(int nYear);
		void SetMonth(int nMonth);
		void SetDay(int nDay);
		bool IsValid() const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
		void SetSeparator(TCHAR cSep);
		TCHAR GetSeparator() const;

		void ShowDayBeforeMonth(bool bDayBeforeMonth = true);
		bool IsDayShownBeforeMonth() const;

		enum Flags
		{
			None				= 0x0000,
			DayBeforeMonth		= 0x1000
		};

		void ModifyFlags(UINT uAdd, UINT uRemove);
		UINT GetFlags() const;

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual LONG _OnPaste(UINT wParam, LONG lParam);

	protected:
		// Helpers
		bool AdjustMaxMonthAndDay();
		bool AdjustMaxDay();
		bool IsValid(const COleDateTime& date) const;
		void ShowErrorMessage() const;

		int GetValidMonth() const;
		int GetMaxMonth() const;
		int GetMinMonth() const;
		int GetMonthStartPosition() const;
		TCHAR GetMaxMonthDigit(int nPos) const;
		TCHAR GetMinMonthDigit(int nPos) const;
		bool IsValidMonthDigit(TCHAR c, int nPos) const;
		bool IsValidMonth(int nMonth) const;

		int GetValidDay() const;
		int GetMaxDay() const;
		int GetMinDay() const;
		int GetDayStartPosition() const;
		TCHAR GetMaxDayDigit(int nPos) const;
		TCHAR GetMinDayDigit(int nPos) const;
		bool IsValidDayDigit(TCHAR c, int nPos) const;
		bool IsValidDay(int nDay) const;
		
		int GetValidYear() const;
		int GetYearStartPosition() const;
		TCHAR GetMaxYearDigit(int nPos) const;
		TCHAR GetMinYearDigit(int nPos, bool bValidYear = false) const;
		bool IsValidYearDigit(TCHAR c, int nPos) const;
		bool IsValidYear(int nYear) const;

	public:
		static bool IsLeapYear(int nYear);
		static CString GetString(int nValue, bool bTwoDigitWithLeadingZero = true);
		static int GetMaxDayOfMonth(int nMonth, int nYear);

	protected:
		// Attributes
		COleDateTime m_dateCurrent;
		COleDateTime m_dateMin;
		COleDateTime m_dateMax;
		TCHAR m_cSep;
		UINT m_uFlags;
	};

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSEdit)
	//}}AFX_MSG
	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSAlphanumericEdit window

// The CAMSAlphanumericEdit is a CAMSEdit control which supports the AlphanumericBehavior class.
//
class CAMSAlphanumericEdit : public CAMSEdit, 
                             public CAMSEdit::AlphanumericBehavior
{
public:
	// Construction
	CAMSAlphanumericEdit(int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSAlphanumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSAlphanumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSMaskedEdit window

// The CAMSMaskedEdit is a CAMSEdit control which supports the MaskedBehavior class.
//
class CAMSMaskedEdit : public CAMSEdit, 
                       public CAMSEdit::MaskedBehavior
{
public:
	// Construction
	CAMSMaskedEdit(const CString& strMask = _T(""));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class.
//
class CAMSNumericEdit : public CAMSEdit, 
                        public CAMSEdit::NumericBehavior
{
public:
	CAMSNumericEdit(int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSNumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSNumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSIntegerEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class
// restricted to only allow integer values.
//
class CAMSIntegerEdit : public CAMSNumericEdit
{
public:
	// Construction
	CAMSIntegerEdit(int nMaxWholeDigits = 9);

public:
	// Operations
	long GetTextAsLong() const;

private:
	void SetMaxDecimalPlaces(int nMaxDecimalPlaces);	// can't be set; always 0

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSIntegerEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSIntegerEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSCurrencyEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class
// modified to put the '$' character in front of the value and use commas to separate the thousands.
//
class CAMSCurrencyEdit : public CAMSNumericEdit
{
public:
	// Construction
	CAMSCurrencyEdit();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSCurrencyEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSCurrencyEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSDateEdit window

// The CAMSDateEdit is a CAMSEdit control which supports the DateBehavior class.
//
class CAMSDateEdit : public CAMSEdit, 
                     public CAMSEdit::DateBehavior
{
public:
	// Construction
	CAMSDateEdit();

public:
	// Operations
	enum OnKillFocusFlags
	{
		None								= 0x0000,
		OnKillFocus_LeaveAlone				= 0x0800,
		OnKillFocus_Beep_IfInvalid			= 0x0001,
		OnKillFocus_Beep_IfEmpty			= 0x0002,
		OnKillFocus_Beep					= 0x0003,
		OnKillFocus_SetValid_IfInvalid		= 0x0004,
		OnKillFocus_SetValid_IfEmpty		= 0x0008,
		OnKillFocus_SetValid				= 0x000C,
		OnKillFocus_SetFocus_IfInvalid		= 0x0010,
		OnKillFocus_SetFocus_IfEmpty		= 0x0020,
		OnKillFocus_SetFocus				= 0x0030,
		OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
		OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
		OnKillFocus_ShowMessage				= 0x00F0
	};

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSDateEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSDateEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSMultiMaskedEdit window

// The CAMSMultiMaskedEdit class is a CAMSEdit control which can support the 
// AlphanumericBehavior, NumericBehavior, MaskedBehavior, or 
// DateBehavior behavior classes.  It uses the mask to determine the current behavior.
//
class CAMSMultiMaskedEdit : public CAMSEdit, 
                            public CAMSEdit::AlphanumericBehavior,
                            public CAMSEdit::NumericBehavior,
                            public CAMSEdit::MaskedBehavior,
                            public CAMSEdit::DateBehavior
{
public:
	CAMSMultiMaskedEdit();

	const CString& GetMask() const;
	void SetMask(const CString& strMask);
	
protected:
	virtual CString GetValidText() const;

	// Attributes
	Behavior* m_pCurrentBehavior;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSMultiMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSMultiMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_)
