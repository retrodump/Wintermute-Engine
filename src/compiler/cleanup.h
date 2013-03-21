#ifndef __CLEANUP_H__
#define __CLEANUP_H__


#include <stdio.h>
#include "tree.h"

void cleanupSCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection);
void cleanupTOPLEVEL(TOPLEVEL *toplevel);
void cleanupFUNCTION(FUNCTION *function);
void cleanupEVENT(EVENT *event);
void cleanupEXTERNAL(EXTERNAL *external);
void cleanupEXT_TYPE(EXT_TYPE *ext_type);
void cleanupTYPE(TYPE *type);
void cleanupDECL(DECL *decl);
void cleanupFORINIT(FORINIT *forinit);
void cleanupCASE(CASE *c);
void cleanupSTM(STM *stm);
void cleanupIDENTIFIER(IDENTIFIER *identifier);
void cleanupEXP(EXP *exp);
void cleanupLVALUE(LVALUE *lvalue);
void cleanupSymbolTable(SymbolTable* st);

#endif
