// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifndef UTILS_H
#define UTILS_H

class CBGame;

class CBUtils
{
public:
	static int ClosestColor (BYTE r, BYTE g, BYTE b, PALETTEENTRY* entries, int num_entries);
	static void Clip(int *DestX, int *DestY, RECT *SrcRect, RECT *DestRect);
	static void Swap(int *a, int *b);
	static DWORD RGBtoNative(DDPIXELFORMAT pf, BYTE red, BYTE green, BYTE blue, DWORD* num_bytes=NULL);
	static bool StrBeginsI(const char* String, const char* Fragment);
	static float NormalizeAngle(float Angle);
	static DWORD COLORREF2D3D(COLORREF Color);
	static COLORREF D3D2COLORREF(DWORD Color);
	static void GetWindowsVersion(char* str);

	static void CreatePath(char* Path, bool PathOnly=false);
	static HRESULT GetDXVersion(DWORD* pdwDirectXVersion, TCHAR* strDirectXVersion, int cchDirectXVersion);

	static void DebugMessage(HWND hWnd, const char* Text);
	static char* SetString(char** String, char* Value);

	static int StrNumEntries(const char* Str, const char Delim=',');
	static char* StrEntry(int Entry, const char* Str, const char Delim=',');

	static int RandomInt(int From, int To);
	static float RandomFloat(float From, float To);
	static float RandomAngle(float From, float To);

	static bool IsWinNT();
	static bool IsKeyDown(int VKey);

	static int GetArgCount(const char* CmdLine);
	static bool GetArg(int Num, const char* CmdLine, char* Buf);

	static bool MatchesPattern(const char* pattern, const char* string);

	static int GetUsedMemMB();
	static char* GetPath(char* Filename);
	static char* GetFilename(char* Filename);

	static void RGBtoHSL(DWORD RGBColor, BYTE* OutH, BYTE* OutS, BYTE* OutL);
	static DWORD HSLtoRGB(BYTE H, BYTE S, BYTE L);

private:
	static float Hue2RGB(float v1, float v2, float vH);
};

#endif
