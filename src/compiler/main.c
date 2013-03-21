#include <stdio.h>
#include <stdlib.h>
#include "main.h"
#include "../engine_core/wme_base/dcscript.h"
#include "error.h"
#include "memory.h"
#include "pretty.h"
#include "tree.h"
#include "weed.h"
#include "symbol.h"
#include "opcode.h"
#include "cleanup.h"


CALLBACKS callbacks;
void* ExtData;

/* Prototypes for bison */
typedef struct yy_buffer_state *YY_BUFFER_STATE;
void yyparse();
YY_BUFFER_STATE yy_scan_bytes (const char *bytes, int len);
void yy_delete_buffer (YY_BUFFER_STATE b);


int yywrap(){
	return 1;
} 


TExternalSymbol* ExternalVars=NULL;
TExternalSymbol* ExternalFuncs=NULL;


/* The declaration of the extern variable from lang.y */
struct SCRIPTCOLLECTION *thescriptcollection;


//////////////////////////////////////////////////////////////////////////
BOOL APIENTRY DllMain( HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call){
		case DLL_PROCESS_ATTACH:
			callbacks.Dll_AddError = NULL;
			callbacks.Dll_CloseFile = NULL;
			callbacks.Dll_LoadFile = NULL;
			callbacks.Dll_ParseElement = NULL;
			break;

		case DLL_THREAD_ATTACH:
		case DLL_THREAD_DETACH:
		case DLL_PROCESS_DETACH:
			break;
    }
    return TRUE;
}

extern int errors;
extern int buffer_size;
extern DWORD BufferSize;
extern int include_stack_ptr;
extern int lineno;
extern int commentDepth;

//////////////////////////////////////////////////////////////////////////
void FinalCleanup(){

	TExternalSymbol* sym;

	while(ExternalVars!=NULL){
		sym = ExternalVars;
		ExternalVars = (TExternalSymbol*)ExternalVars->next;
		free(sym->name);
		free(sym);
	}
	while(ExternalFuncs!=NULL){
		sym = ExternalFuncs;
		ExternalFuncs = (TExternalSymbol*)ExternalFuncs->next;
		free(sym->name);
		free(sym);
	}
	
	cleanupSCRIPTCOLLECTION(thescriptcollection);
	thescriptcollection = NULL;

	ExternalVars = NULL;
	ExternalFuncs = NULL;

	errors=0;
	buffer_size=0;
	BufferSize=0;
	include_stack_ptr=0;
	lineno=0;
	commentDepth=0;
}


//////////////////////////////////////////////////////////////////////////
// EXPORTED FUNCTIONS
//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) BYTE* CompileBuffer(BYTE* Buffer, char* Source, DWORD BufferSize, DWORD* CompiledSize){
	
	YY_BUFFER_STATE state;
	BYTE* ret = NULL;

	if(Buffer==NULL){
		if(CompiledSize)*CompiledSize = 0;
		return NULL;
	}

	state = yy_scan_bytes(Buffer, BufferSize);
	lineno=1;
	yyparse();
	yy_delete_buffer(state);
	if(errorCheck()) goto finish_error;

	weedSCRIPTCOLLECTION(thescriptcollection);
	if(errorCheck()) goto finish_error;
	
	symSCRIPTCOLLECTION(thescriptcollection);
	if(errorCheck()) goto finish_error;
	
	ret = opcodeSCRIPTCOLLECTION(thescriptcollection, Source, CompiledSize);
	if(errorCheck()) goto finish_error;

	FinalCleanup();
	return ret;


finish_error:
	if(ret!=NULL) free(ret);
	*CompiledSize = 0;
	FinalCleanup();
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) BYTE* CompileFile(char* Filename, DWORD* CompiledSize){
	BYTE* Buffer=NULL;
	DWORD BufferSize=0;
	BYTE* ret;

	if(callbacks.Dll_LoadFile){
		Buffer = callbacks.Dll_LoadFile(ExtData, Filename, &BufferSize);
	}

	if(Buffer==NULL){
		if(callbacks.Dll_AddError) callbacks.Dll_AddError(ExtData, 0, "Error opening script.");
		if(CompiledSize) *CompiledSize = 0;
		return NULL;
	}
	else{
		if(IsUTF8(Buffer, BufferSize)) ret = CompileBuffer(Buffer+3, Filename, BufferSize-3, CompiledSize);
		else ret = CompileBuffer(Buffer, Filename, BufferSize, CompiledSize);

		if(callbacks.Dll_CloseFile) callbacks.Dll_CloseFile(ExtData, Buffer);
		return ret;
	}
}


//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void ReleaseBuffer(unsigned char* Buffer){

	free(Buffer);
}


//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) void SetCallbacks(CALLBACKS* in_callbacks, void* in_data){
	memcpy(&callbacks, in_callbacks, sizeof(CALLBACKS));
	ExtData = in_data;
}


//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) int DefineFunction(char* Name){
	TExternalSymbol* sym;
	sym = NEW(TExternalSymbol);
	sym->kind = functionSym;
	sym->name = malloc(strlen(Name)+1);
	strcpy(sym->name, Name);
	
	sym->next = (struct TExternalSymbol*)ExternalFuncs;
	ExternalFuncs = sym;

	return TRUE;
}


//////////////////////////////////////////////////////////////////////////
__declspec(dllexport) int DefineVariable(char* Name){

	TExternalSymbol* sym;
	sym = NEW(TExternalSymbol);
	sym->kind = declSym;
	sym->name = malloc(strlen(Name)+1);
	strcpy(sym->name, Name);
	
	sym->next = (struct TExternalSymbol*)ExternalVars;
	ExternalVars = sym;

	return TRUE;	
}


//////////////////////////////////////////////////////////////////////////
int IsUTF8(BYTE* Buffer, int BufferSize)
{
	if(BufferSize>3 && Buffer[0]==0xEF && Buffer[1]==0xBB && Buffer[2]==0xBF) return 1;
	else return 0;
}
