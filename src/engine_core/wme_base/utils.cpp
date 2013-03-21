// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "rgbpixel.h"
#include <direct.h>

#ifdef _DEBUG
	#include <Psapi.h>
	#pragma comment(lib, "psapi.lib")
#endif


//////////////////////////////////////////////////////////////////////
static inline unsigned Sqr (int x)
{
  return (x * x);
}


//////////////////////////////////////////////////////////////////////
int CBUtils::ClosestColor (BYTE r, BYTE g, BYTE b, PALETTEENTRY* entries, int num_entries)
{
	int closest_idx = -1;
	unsigned closest_dst = (unsigned)-1;

	for (int idx = 0; idx < num_entries; idx++){
		unsigned dst =	Sqr (r - entries[idx].peRed)   * R_COEF_SQ +
						Sqr (g - entries[idx].peGreen) * G_COEF_SQ +
						Sqr (b - entries[idx].peBlue)  * B_COEF_SQ;
		if (dst == 0) return idx;
		if (dst < closest_dst){
			closest_dst = dst;
			closest_idx = idx;
		}
	}
	return closest_idx;
}


//////////////////////////////////////////////////////////////////////////////////
void CBUtils::Clip(int *DestX, int *DestY, RECT *SrcRect, RECT *DestRect)
{
	// If it's partly off the right side of the screen
	if(*DestX + (SrcRect->right - SrcRect->left) > DestRect->right)
		SrcRect->right -= *DestX + (SrcRect->right-SrcRect->left) - DestRect->right;

	if(SrcRect->right < 0) SrcRect->right = 0;

	// Partly off the left side of the screen
	if(*DestX < DestRect->left)
	{
		SrcRect->left += DestRect->left - *DestX;
		*DestX = DestRect->left;
	}

	// Partly off the top of the screen
	if(*DestY < DestRect->top)
	{
		SrcRect->top += DestRect->top - *DestY;
		*DestY = DestRect->top;
	}

	// If it's partly off the bottom side of the screen
	if(*DestY + (SrcRect->bottom - SrcRect->top) > DestRect->bottom)
	SrcRect->bottom -= ((SrcRect->bottom-SrcRect->top)+*DestY) - DestRect->bottom;

	if(SrcRect->bottom < 0) SrcRect->bottom = 0;

	return;
}

//////////////////////////////////////////////////////////////////////////////////
// Swap - swaps two integers
//////////////////////////////////////////////////////////////////////////////////
void CBUtils::Swap(int *a, int *b)
{
	int Temp = *a;
	*a = *b;
	*b = Temp;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBUtils::RGBtoNative(DDPIXELFORMAT pf, BYTE red, BYTE green, BYTE blue, DWORD* num_bytes)
{	
	DWORD i, j, rshift, gshift, bshift, rbits, gbits, bbits;
	
    j = (int) pf.dwRBitMask; rshift = 0;
	i = 1; while (!(i&j)) { rshift++; i<<=1; }
	rbits = 0; while (i&j) { rbits++; i<<=1; }

	j = (int) pf.dwGBitMask; gshift = 0;
	i = 1; while (!(i&j)) { gshift++; i<<=1; }
	gbits = 0; while (i&j) { gbits++; i<<=1; }

	j = (int) pf.dwBBitMask; bshift = 0;
	i = 1; while (!(i&j)) { bshift++; i<<=1; }
	bbits = 0; while (i&j) { bbits++; i<<=1; }
	
	if(num_bytes) *num_bytes = pf.dwRGBBitCount / 8;

	return (((red<<rshift)  >>(8-rbits)) & pf.dwRBitMask) |
      	   (((green<<gshift)>>(8-gbits)) & pf.dwGBitMask) |
		   (((blue<<bshift) >>(8-bbits)) & pf.dwBBitMask);
}


//////////////////////////////////////////////////////////////////////////
bool CBUtils::StrBeginsI(const char* String, const char* Fragment)
{
	return (_strnicmp(String, Fragment, strlen(Fragment))==0);
}


//////////////////////////////////////////////////////////////////////////
float CBUtils::NormalizeAngle(float Angle)
{
	while(Angle > 360) Angle -= 360;
	while(Angle < 0) Angle += 360;

	return Angle;
}

//////////////////////////////////////////////////////////////////////////
DWORD CBUtils::COLORREF2D3D(COLORREF Color)
{
	BYTE r = GetRValue(Color);
	BYTE g = GetGValue(Color);
	BYTE b = GetBValue(Color);
	BYTE a = 255;

	return ((DWORD)((((a)&0xff)<<24)|(((r)&0xff)<<16)|(((g)&0xff)<<8)|((b)&0xff)));
}


//////////////////////////////////////////////////////////////////////////
COLORREF CBUtils::D3D2COLORREF(DWORD Color)
{
	BYTE b = GetRValue(Color);
	BYTE g = GetGValue(Color);
	BYTE r = GetBValue(Color);

	return RGB(r, g, b);
}


//////////////////////////////////////////////////////////////////////////
void CBUtils::GetWindowsVersion(char* str)
{
	strcpy(str, "<unknown>");

	OSVERSIONINFO osvi;
	
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!GetVersionEx((OSVERSIONINFO*) &osvi)) return;
	
	char product[50];

	switch (osvi.dwPlatformId){
	case VER_PLATFORM_WIN32_NT:
		
		// Test for the product.		
		if(osvi.dwMajorVersion <= 4)
            strcpy(product, "Windows NT");
		
		else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0)
            strcpy (product, "Windows 2000");
		
		else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1)
            strcpy (product, "Windows XP");

	    else if(osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2)
            strcpy (product, "Windows Server 2003");

		else if(osvi.dwMajorVersion == 6 && osvi.dwMinorVersion == 0)
			strcpy (product, "Windows Vista");

		else strcpy (product, "Windows XP or higher");
		
		
		if (osvi.dwMajorVersion <= 4){
            sprintf (str, "%s version %d.%d %s (Build %d)",
				product,
				osvi.dwMajorVersion,
				osvi.dwMinorVersion,
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		else { 
            sprintf (str, "%s %s (Build %d)",
				product,
				osvi.szCSDVersion,
				osvi.dwBuildNumber & 0xFFFF);
		}
		break;
		
	case VER_PLATFORM_WIN32_WINDOWS:
		
		if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 0){
			strcpy (str, "Windows 95");
			if (osvi.szCSDVersion[1] == 'C' || osvi.szCSDVersion[1] == 'B')
                strcat(str, " OSR2");
		} 		
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 10){
			strcpy (str, "Windows 98");
			if(osvi.szCSDVersion[1] == 'A')
                strcat(str, " SE");
		} 		
		else if (osvi.dwMajorVersion == 4 && osvi.dwMinorVersion == 90){
			strcpy (str, "Windows Me");
		}
		else strcpy (str, "Windows 9x");

		break;
	}
}


////////////////////////////////////////////////////////////////////////////////
void CBUtils::CreatePath(char* Path, bool PathOnly)
{
	char fpath[MAX_PATH+1];

	if(PathOnly) strcpy(fpath, Path);
	else{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		_splitpath(Path, drive, dir, NULL, NULL);
		sprintf(fpath, "%s%s", drive, dir);
	}
	
	char *ChPtr;
	ChPtr=fpath;
	
	do {
		if (*ChPtr=='\\' || *ChPtr=='/' || *ChPtr==0)
		{
			*ChPtr=0;
			_mkdir(fpath);
			*ChPtr='\\';
		}
		ChPtr++;
	} while(*ChPtr!=0);
}


//////////////////////////////////////////////////////////////////////////
void CBUtils::DebugMessage(HWND hWnd, const char* Text)
{
	//MessageBox(hWnd, Text, "WME", MB_OK|MB_ICONINFORMATION);
}


//////////////////////////////////////////////////////////////////////////
char* CBUtils::SetString(char** String, char* Value)
{
	SAFE_DELETE_ARRAY(*String);
	*String = new char[strlen(Value)+1];
	if(*String) strcpy(*String, Value);
	return *String;
}

//////////////////////////////////////////////////////////////////////////
int CBUtils::StrNumEntries(const char* Str, const char Delim)
{
	int NumEntries = 1;
	for(int i=0; i<strlen(Str); i++)
	{
		if(Str[i]==Delim) NumEntries++;
	}
	return NumEntries;
}


//////////////////////////////////////////////////////////////////////////
char* CBUtils::StrEntry(int Entry, const char* Str, const char Delim)
{
	int NumEntries = 0;

	const char* Start = NULL;
	int Len = 0;

	for(int i=0; i<=strlen(Str); i++)
	{
		if(NumEntries==Entry)
		{
			if(!Start) Start = Str + i;
			else Len++;
		}
		if(Str[i]==Delim || Str[i]=='\0')
		{
			NumEntries++;
			if(Start)
			{
				char* Ret = new char[Len+1];
				memset(Ret, 0, Len+1);
				strncpy(Ret, Start, Len);
				return Ret;
			}
		}
	}
	return NULL;
}

//////////////////////////////////////////////////////////////////////////
int CBUtils::RandomInt(int From, int To)
{
	if(To < From)
	{
		int i = To;
		To = From;
		From = i;
	}
	return (rand() % (To - From + 1)) + From;
}

//////////////////////////////////////////////////////////////////////////
float CBUtils::RandomFloat(float From, float To)
{
	float RandNum = (float)rand () / (float)RAND_MAX;
	return From + (To - From) * RandNum;
}

//////////////////////////////////////////////////////////////////////////
float CBUtils::RandomAngle(float From, float To)
{
	while(To < From)
	{
		To += 360;
	}
	return NormalizeAngle(RandomFloat(From, To));
}

//////////////////////////////////////////////////////////////////////////
bool CBUtils::IsWinNT()
{
	OSVERSIONINFO osvi;
	ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	if(!GetVersionEx((OSVERSIONINFO*) &osvi)) return false;

	return osvi.dwPlatformId==VER_PLATFORM_WIN32_NT;
}


//////////////////////////////////////////////////////////////////////////
bool CBUtils::IsKeyDown(int VKey)
{
	return (GetAsyncKeyState(VKey) & 0x8000)!=0;
}


//////////////////////////////////////////////////////////////////////////
int CBUtils::GetArgCount(const char* CmdLine)
{
	int ret = 0;

	bool InQuote = false;
	for(int i=0; i<strlen(CmdLine); i++)
	{
		if(CmdLine[i]=='\"')
		{
			InQuote = !InQuote;
			if(InQuote) ret++;
		}
		else
		{
			if(CmdLine[i]!=' ' && (i==0 || (CmdLine[i-1]==' ' && !InQuote)))
			{
				ret++;
			}
		}
	}
	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool CBUtils::GetArg(int Num, const char* CmdLine, char* Buf)
{
	int CurrEntry=-1;
	int Pos = 0;
	bool InQuote = false;
	for(int i=0; i<strlen(CmdLine); i++)
	{
		if(CmdLine[i]=='\"')
		{
			InQuote = !InQuote;
			if(InQuote) CurrEntry++;
		}
		else
		{
			if(CmdLine[i]!=' ' && (i==0 || (CmdLine[i-1]==' ' && !InQuote)))
			{
				CurrEntry++;
			}

			if(CurrEntry==Num)
			{
				if(CmdLine[i]!=' ' || InQuote)
				{
					Buf[Pos] = CmdLine[i];
					Pos++;
				}
			}
		}
	}
	Buf[Pos] = '\0';
	return Pos > 0;
}


//////////////////////////////////////////////////////////////////////////
bool CBUtils::MatchesPattern(const char* Pattern, const char* String)
{
	char stringc, patternc;

	for(;; ++String)
	{
		stringc = toupper(*String);
		patternc = toupper(*Pattern++);

		switch(patternc)
		{
			case 0:
				return (stringc==0);

			case '?':
				if (stringc == 0) return false;
			break;

			case '*':
				if (!*Pattern) return true;

				if (*Pattern=='.')
				{
					char* dot;
					if (Pattern[1]=='*' && Pattern[2]==0) return true;
					dot = (char*)strchr(String, '.');
					if (Pattern[1]==0) return (dot==NULL || dot[1]==0);
					if (dot!=NULL)
					{
						String=dot;
						if (strpbrk(Pattern, "*?[")==NULL && strchr(String+1, '.')==NULL)
							return(stricmp(Pattern+1, String+1)==0);
					}
				}

				while (*String)
					if (CBUtils::MatchesPattern(Pattern, String++))
						return true;
				return false;

			default:
				if (patternc != stringc)
					if (patternc=='.' && stringc==0)
						return(CBUtils::MatchesPattern(Pattern, String));
					else
						return false;
			break;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
int CBUtils::GetUsedMemMB()
{
#ifdef _DEBUG
	PROCESS_MEMORY_COUNTERS Counters;
	Counters.cb = sizeof(PROCESS_MEMORY_COUNTERS);
	GetProcessMemoryInfo(GetCurrentProcess(), &Counters, sizeof(PROCESS_MEMORY_COUNTERS));
	return (int)(Counters.WorkingSetSize / 1024 / 1024);
#else
	return 0;
#endif
}

//////////////////////////////////////////////////////////////////////////
char* CBUtils::GetPath(char* Filename)
{
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(Filename, drive, dir, fname, ext);

	char* Ret = new char[MAX_PATH];

	sprintf(Ret, "%s%s", drive, dir);
	_fullpath(Ret, Ret, MAX_PATH);
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
char* CBUtils::GetFilename(char* Filename)
{
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, fname, ext);

	char* Ret = new char[MAX_PATH];

	sprintf(Ret, "%s%s", fname, ext);
	return Ret;
}

//////////////////////////////////////////////////////////////////////////
void CBUtils::RGBtoHSL(DWORD RGBColor, BYTE* OutH, BYTE* OutS, BYTE* OutL)
{
	float var_R = (D3DCOLGetR(RGBColor) / 255.0f);
	float var_G = (D3DCOLGetG(RGBColor) / 255.0f);
	float var_B = (D3DCOLGetB(RGBColor) / 255.0f);

	//Min. value of RGB
	float var_Min = min(var_R, var_G);
	var_Min = min(var_Min, var_B );

	//Max. value of RGB
	float var_Max = max(var_R, var_G);
	var_Max = max(var_Max, var_B);
	
	//Delta RGB value
	float del_Max = var_Max - var_Min;

	float H, S, L;

	L = (var_Max + var_Min) / 2.0f;

	//This is a gray, no chroma...
	if(del_Max == 0)
	{
		H = 0;
		S = 0;
	}
	//Chromatic data...
	else
	{
		if (L < 0.5f) S = del_Max / (var_Max + var_Min);
		else S = del_Max / (2.0f - var_Max - var_Min);

		float del_R = (((var_Max - var_R) / 6.0f) + (del_Max / 2.0f)) / del_Max;
		float del_G = (((var_Max - var_G) / 6.0f) + (del_Max / 2.0f)) / del_Max;
		float del_B = (((var_Max - var_B) / 6.0f) + (del_Max / 2.0f)) / del_Max;

		if(var_R == var_Max) H = del_B - del_G;
		else if(var_G == var_Max) H = (1.0f / 3.0f) + del_R - del_B;
		else if(var_B == var_Max) H = (2.0f / 3.0f) + del_G - del_R;

		if(H < 0) H += 1;
		if(H > 1) H -= 1;
	}

	*OutH = H * 255;
	*OutS = S * 255;
	*OutL = L * 255;
}


//////////////////////////////////////////////////////////////////////////
DWORD CBUtils::HSLtoRGB(BYTE InH, BYTE InS, BYTE InL)
{
	float H = InH / 255.0f;
	float S = InS / 255.0f;
	float L = InL / 255.0f;

	BYTE R, G, B;


	if ( S == 0 )
	{
		R = L * 255;
		G = L * 255;
		B = L * 255;
	}
	else
	{
		float var_1, var_2;

		if(L < 0.5) var_2 = L * (1.0 + S);
		else var_2 = (L + S) - (S * L);

		var_1 = 2.0f * L - var_2;

		R = 255 * Hue2RGB(var_1, var_2, H + (1.0f / 3.0f));
		G = 255 * Hue2RGB(var_1, var_2, H);
		B = 255 * Hue2RGB(var_1, var_2, H - (1.0f / 3.0f));
	}
	return D3DCOLOR_ARGB(255, R, G, B);
}


//////////////////////////////////////////////////////////////////////////
float CBUtils::Hue2RGB(float v1, float v2, float vH)
{
	if (vH < 0.0f) vH += 1.0f;
	if (vH > 1.0f) vH -= 1.0f;
	if ((6.0f * vH) < 1.0f) return (v1 + (v2 - v1) * 6.0f * vH);
	if ((2.0f * vH) < 1.0f) return (v2);
	if ((3.0f * vH) < 2.0f) return (v1 + (v2 - v1) * ((2.0f / 3.0f) - vH) * 6.0f);
	return (v1);
}
