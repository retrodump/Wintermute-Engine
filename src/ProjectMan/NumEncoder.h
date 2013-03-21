#pragma once

class CNumEncoder
{
public:
	CNumEncoder(void);
	virtual ~CNumEncoder(void);

	bool DecodeFromString(CString Str);
	CString EncodeToString();

	void Clear();
	bool AddNumber(int Num);
	bool SetNumber(int Index, int Num);
	int GetNumber(int Index, int InitVal=0);

private:
	char GetChar(int Index, CString Str);
	void AddChar(CString& Str, char Ch);
	CArray<int, int> m_Numbers;
};
