/*----------------------------------------------------------------------
       John Robbins - Microsoft Systems Journal Bugslayer Column
----------------------------------------------------------------------*/

/*//////////////////////////////////////////////////////////////////////
                                Includes
//////////////////////////////////////////////////////////////////////*/
#include "stdafx.h"
#include "BugslayerUtil.h"
// The project internal header file.
#include "Internal.h"

/*//////////////////////////////////////////////////////////////////////
                         File Specific Defines
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
                        File Specific Prototypes
//////////////////////////////////////////////////////////////////////*/

/*//////////////////////////////////////////////////////////////////////
                             Implementation
//////////////////////////////////////////////////////////////////////*/

BOOL BUGSUTIL_DLLINTERFACE __stdcall
                        HookOrdinalExport ( HMODULE hModule     ,
                                            LPCTSTR szImportMod ,
                                            DWORD   dwOrdinal   ,
                                            PROC    pHookFunc   ,
                                            PROC *  ppOrigAddr   )
{
    ASSERT ( NULL != hModule ) ;
    ASSERT ( FALSE == IsBadStringPtr ( szImportMod , MAX_PATH ) ) ;
    ASSERT ( 0 != dwOrdinal ) ;
    ASSERT ( FALSE == IsBadCodePtr ( pHookFunc ) ) ;
    if ( ( NULL  == hModule                                   ) ||
         ( TRUE  == IsBadStringPtr ( szImportMod , MAX_PATH ) ) ||
         ( 0     == dwOrdinal                                 ) ||
         ( TRUE  == IsBadCodePtr ( pHookFunc )                )   )
    {
        SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
        return ( FALSE ) ;
    }

    if ( NULL != ppOrigAddr )
    {
        ASSERT ( FALSE ==
                     IsBadWritePtr ( ppOrigAddr , sizeof ( PROC ) ) ) ;
        if ( TRUE == IsBadWritePtr ( ppOrigAddr , sizeof ( PROC ) ) )
        {
            SetLastErrorEx ( ERROR_INVALID_PARAMETER , SLE_ERROR ) ;
            return ( FALSE ) ;
        }
    }

    // Find the import descriptor first.
    PIMAGE_IMPORT_DESCRIPTOR pImportDesc =
                    GetNamedImportDescriptor ( hModule , szImportMod ) ;

    if ( NULL == pImportDesc )
    {
        // The requested module was not imported.  This is not an error.
        return ( TRUE ) ;
    }

    // Get the original thunk information for this DLL.  I cannot use
    //  the thunk information stored in the pImportDesc->FirstThunk
    //  because the that is the array that the loader has already
    //  bashed to fix up all the imports.  This pointer gives us acess
    //  to the function names.
    PIMAGE_THUNK_DATA pOrigThunk =
                        MakePtr ( PIMAGE_THUNK_DATA       ,
                                  hModule                 ,
                                  pImportDesc->OriginalFirstThunk  ) ;
    // Get the array pointed to by the pImportDesc->FirstThunk.  This is
    //  where I will do the actual bash.
    PIMAGE_THUNK_DATA pRealThunk = MakePtr ( PIMAGE_THUNK_DATA       ,
                                             hModule                 ,
                                             pImportDesc->FirstThunk  );

    // The flag is going to be set from the thunk so just make it
    // easier to look up.
    DWORD dwCompareOrdinal = IMAGE_ORDINAL_FLAG | dwOrdinal ;

    // Loop through and look for the one that matches the ordinal.
    while ( NULL != pOrigThunk->u1.Function )
    {
        // Only look at those that are imported by ordinal.
        if (  IMAGE_ORDINAL_FLAG ==
                        ( pOrigThunk->u1.Ordinal & IMAGE_ORDINAL_FLAG ))
        {
            // Is this the one?
            if ( dwCompareOrdinal == pOrigThunk->u1.Ordinal )
            {
                // I found it.  Now I need to change the protection to
                //  writable before I do the blast.  Note that I am now
                //  blasting into the real thunk area!
                MEMORY_BASIC_INFORMATION mbi_thunk ;

                VirtualQuery ( pRealThunk                          ,
                               &mbi_thunk                          ,
                               sizeof ( MEMORY_BASIC_INFORMATION )  ) ;

                VERIFY ( VirtualProtect ( mbi_thunk.BaseAddress ,
                                          mbi_thunk.RegionSize  ,
                                          PAGE_READWRITE        ,
                                          &mbi_thunk.Protect     ) ) ;

                // Save the original address if requested.
                if ( NULL != ppOrigAddr )
                {
                    *ppOrigAddr = (PROC)pRealThunk->u1.Function ;
                }

                // Do the actual hook.
                pRealThunk->u1.Function = (DWORD)pHookFunc ;

                DWORD dwOldProtect ;

                // Change the protection back to what it was before I
                //  blasted.
                VERIFY ( VirtualProtect ( mbi_thunk.BaseAddress ,
                                          mbi_thunk.RegionSize  ,
                                          mbi_thunk.Protect     ,
                                          &dwOldProtect          ) ) ;
                // Life is good.
                SetLastError ( ERROR_SUCCESS ) ;
                return ( TRUE ) ;
            }
        }
        // Increment both tables.
        pOrigThunk++ ;
        pRealThunk++ ;
    }

    // Nothing was hooked.  This is technically not an error.  It just
    // means that the module is imported but the one function is not.
    SetLastError ( ERROR_SUCCESS ) ;
    return ( FALSE ) ;
}


