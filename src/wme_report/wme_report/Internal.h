/*----------------------------------------------------------------------
       John Robbins - Microsoft Systems Journal Bugslayer Column

The internal header for the BugslayerUtil.DLL code.
----------------------------------------------------------------------*/

#ifndef _INTERNAL_H
#define _INTERNAL_H

// The NT4 specific version of GetLoadedModules.
BOOL NT4GetLoadedModules ( DWORD     dwPID        ,
                           UINT      uiCount      ,
                           HMODULE * paModArray   ,
                           LPDWORD   pdwRealCount   ) ;

// The TOOLHELP32 specific version of GetLoadedModules.
BOOL TLHELPGetLoadedModules ( DWORD     dwPID        ,
                              UINT      uiCount      ,
                              HMODULE * paModArray   ,
                              LPDWORD   pdwRealCount   ) ;

// The NT version of GetModuleBaseName.
DWORD __stdcall NTGetModuleBaseName ( HANDLE  hProcess   ,
                                      HMODULE hModule    ,
                                      LPTSTR  lpBaseName ,
                                      DWORD   nSize       ) ;

// A helper function to get the import descriptor for a the specified
// module.
PIMAGE_IMPORT_DESCRIPTOR
                     GetNamedImportDescriptor ( HMODULE hModule     ,
                                                LPCSTR  szImportMod  ) ;

// A useful macro.
#define MakePtr( cast , ptr , AddValue ) \
                                 (cast)( (DWORD)(ptr)+(DWORD)(AddValue))



#endif  // _INTERNAL_H


