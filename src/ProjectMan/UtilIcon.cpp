#include "stdafx.h"

///////////////////////////////////////////////////////////
#pragma pack( push )
#pragma pack( 2 )

typedef struct
{
   BYTE bWidth; // Width, in pixels, of the image
   BYTE bHeight;  // Height, in pixels, of the image
   BYTE bColorCount;  // Number of colors in image (0 if >=8bpp)
   BYTE bReserved;  // Reserved ( must be 0)
   WORD wPlanes;  // Color Planes
   WORD wBitCount;  // Bits per pixel
   DWORD dwBytesInRes;  // How many bytes in this resource?
} ICONDIRENTRYCOMMON, *LPICONDIRENTRYCOMMON;

typedef struct
{
   ICONDIRENTRYCOMMON common;
   DWORD dwImageOffset; // Where in the file is this image?
} ICONDIRENTRY, *LPICONDIRENTRY;

typedef struct
{
   WORD idReserved; // Reserved (must be 0)
   WORD idType; // Resource Type (1 for icons)
   WORD idCount;  // How many images?
   ICONDIRENTRY idEntries[1]; // An entry for each image (idCount of 'em)
} ICONDIR, *LPICONDIR;

typedef struct
{
   ICONDIRENTRYCOMMON common;
   WORD nID;  // the ID
} GRPICONDIRENTRY, *LPGRPICONDIRENTRY;

typedef struct
{
   WORD idReserved; // Reserved (must be 0)
   WORD idType; // Resource type (1 for icons)
   WORD idCount;  // How many images?
   GRPICONDIRENTRY idEntries[1];  // The entries for each image
} GRPICONDIR, *LPGRPICONDIR;

#pragma pack( pop )


//////////////////////////////////////////////////////////////////////////
BOOL AddIconToRes(HANDLE hRes, LPCTSTR pszFile, UINT nIconID, UINT nGroupId)
{
   void *pvFile;
   DWORD nSize;
   int f, z;
   ICONDIR id, *pid;
   GRPICONDIR *pgid;

   ASSERT(pszFile);

   if (!strlen(pszFile))
      return FALSE;
   f = _lopen(pszFile, OF_READ);
   if (f == -1)
      return FALSE;

   _lread(f, &id, sizeof(id));
   _llseek(f, 0, SEEK_SET);
   pid = (ICONDIR *) malloc(sizeof(ICONDIR) + sizeof(ICONDIRENTRY) * (id.idCount - 1));
   pgid = (GRPICONDIR *) malloc(sizeof(GRPICONDIR) + sizeof(GRPICONDIRENTRY) * (id.idCount - 1));
   _lread(f, pid, sizeof(ICONDIR) + sizeof(ICONDIRENTRY) * (id.idCount - 1));
   memcpy(pgid, pid, sizeof(GRPICONDIR));

   for (z = 0; z < id.idCount; z++)
   {
      pgid->idEntries[z].common = pid->idEntries[z].common;
      pgid->idEntries[z].nID = z + 1;
      nSize = pid->idEntries[z].common.dwBytesInRes;

      pvFile = malloc(nSize);
      if (!pvFile)
         return _lclose(f), FALSE;

      _llseek(f, pid->idEntries[z].dwImageOffset, SEEK_SET);
      _lread(f, pvFile, nSize);
      UpdateResource(hRes, RT_ICON, MAKEINTRESOURCE(z + nIconID), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pvFile, nSize);
      free(pvFile);
   }

   _lclose(f);
   

   //I commented it because it modifies the icon group 
   //and the icon i add is then not shown. :)
   UpdateResource(hRes, RT_GROUP_ICON, MAKEINTRESOURCE(nGroupId), MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL), pgid, sizeof(GRPICONDIR) + sizeof(GRPICONDIRENTRY) * (id.idCount - 1));
   return TRUE;
}
///////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////

/*
HANDLE h = BeginUpdateResource(m_strExePath, FALSE);
if (h){
	AddIconToRes(h, m_strIconPath, 2);
	EndUpdateResource(h, FALSE);
}
*/