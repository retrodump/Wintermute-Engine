/* opcode.h
 *
 * History:
 */

#ifndef __opcode_h
#define __opcode_h 

/* -*** Includes ***-------------------------------------------------------- */
#include <stdio.h>
#include "typedef.h"
#include "main.h"

/*
typedef unsigned char BYTE;
typedef unsigned int DWORD;
*/
typedef struct{
	DWORD pos;
	char* name;
	int method;
	struct TFunctionPos* next;
} TFunctionPos;


typedef struct{
	int loop_nest;
	int loop_level;
	int is_end;
	DWORD start;
	DWORD end;
	struct TLoopPos* next;
} TLoopPos;


typedef struct{
	char* DllName;
	char* Name;
	TCallType CallType;
	TExternalType Returns;
	int NumParams;
	TExternalType* Params;

	struct TDllEntry* next;
} TDllEntry;


typedef struct{
	char* name;
	struct TSymbol* next;
} TSymbol;


BYTE* opcodeSCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection, char* Source, DWORD* CompiledSize);
void opcodeTOPLEVELstms(TOPLEVEL *toplevel);
void opcodeTOPLEVELfuncs(TOPLEVEL *toplevel);
void opcodeTOPLEVELevents(TOPLEVEL *toplevel);
void opcodeTOPLEVELexternals(TOPLEVEL *toplevel);
void opcodeSTM(struct STM* stms, int func, int event);
void opcodeFUNCTION(struct FUNCTION* function);
void opcodeEVENT(struct EVENT* event);
void opcodeEXTERNAL(struct EXTERNAL* external);
void opcodeDECL(DECL *decl);
void opcodeTYPE(TYPE *type);
void opcodeEXP(EXP *exp, BOOL compound);
void opcodeFORINIT(FORINIT *forinit);
void opcodeLVALUE(LVALUE *lvalue);


DWORD AddSymbol(char* Symbol);
void AddLoopPos(int loop_nest, int loop_level, DWORD start, DWORD end);
void AddLoopReq(int is_end, int loop_nest, int loop_level, DWORD pos);
void AddFuncReq(DWORD in_pos, char* in_name);
void AddFuncPos(DWORD in_pos, char* in_name, int method);
void AddEventPos(DWORD in_pos, char* in_name);
void ResolveFuncCalls();
void ResolveLoops();
void PutDWORD(DWORD val);
void PutInst(int lineno, DWORD val);
void PutDWORDAt(DWORD pos, DWORD val);
void PutString(char* val);
void OpcodeCleanup();

TExternalType GetExternalType(EXT_TYPE* Type);
TCallType GetCallType(CALL_TYPE* Type);


#endif
