#ifndef __MAIN_H__
#define __MAIN_H__

#include "../engine_core/wme_base/dcscript.h"
#include "tree.h"

extern CALLBACKS callbacks;
extern void* ExtData;
void FinalCleanup();


typedef struct{
	char* name;
	SymbolKind kind;
	struct TExternalSymbol* next;
} TExternalSymbol;

extern TExternalSymbol* ExternalVars;
extern TExternalSymbol* ExternalFuncs;



__declspec(dllexport) BYTE* CompileBuffer(BYTE* Buffer, char* Source, DWORD BufferSize, DWORD* CompiledSize);
__declspec(dllexport) BYTE* CompileFile(char* Filename, DWORD* CompiledSize);
__declspec(dllexport) void ReleaseBuffer(unsigned char* Buffer);
__declspec(dllexport) void SetCallbacks(CALLBACKS* callbacks, void* in_data);
__declspec(dllexport) int DefineFunction(char* Name);
__declspec(dllexport) int DefineVariable(char* Name);

int IsUTF8(BYTE* Buffer, int BufferSize);

#endif
