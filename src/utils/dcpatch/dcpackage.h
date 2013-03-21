#ifndef _DCPACKAGE_H_
#define _DCPACKAGE_H_


#define PACKAGE_MAGIC_1   0xDEC0ADDE
#define PACKAGE_MAGIC_2   0x4B4E554A	// "JUNK"
#define PACKAGE_VERSION   0x00000100
#define PACKAGE_EXTENSION "dcp"

#include <time.h>

typedef struct{
	DWORD Magic1;
	DWORD Magic2;
	DWORD PackageVersion;
	DWORD GameVersion;
	BYTE Priority;
	BYTE CD;
	bool MasterIndex;
	time_t CreationTime;
	char Desc[100];
	DWORD NumDirs;
}TPackageHeader;

/*
Dir: BYTE NameLength
	 char Name [NameLength]
	 BYTE CD;
	 DWORD NumEntries


Entry: BYTE NameLength
       char Name [NameLength]
       DWORD Offset
       DWORD Length
	   DWORD CompLength
       DWORD Flags
*/


#endif // _DCPACKAGE_H_
