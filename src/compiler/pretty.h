#ifndef __PRETTY_H__
#define __PRETTY_H__


#include <stdio.h>
#include "tree.h"

void prettySCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection);
void prettyTOPLEVEL(int indent, TOPLEVEL *toplevel);
void prettyFUNCTION(int indent, FUNCTION *function);
void prettyEVENT(int indent, EVENT *function);
void prettyTYPE(TYPE *type);
void prettyDECL(int indent, DECL *decl);
void prettyFORINIT(int indent, FORINIT *forinit);
void prettySTM(int indent, STM *stm);
void prettyIDENTIFIER(IDENTIFIER *identifier);
void prettyEXP(EXP *exp);
void prettyLVALUE(LVALUE *lvalue);

#endif
