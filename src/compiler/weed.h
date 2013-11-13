#ifndef __WEED_H__
#define __WEED_H__

#include "tree.h"
#include "typedef.h"

void weedSCRIPTCOLLECTION(SCRIPTCOLLECTION *s);
void weedTOPLEVEL(TOPLEVEL *t);
void weedFUNCTION(FUNCTION *f);
void weedEVENT(EVENT *e);
/*bool weedSTMCheckReturns(STM *s);*/
bool weedSTMCheckExits(STM *s);
void weedDECL(DECL *d);
void weedFORINIT(FORINIT *f);
void weedSTM(STM *stm, int loop, int func, int event, int swtch);
void weedIDENTIFIER(IDENTIFIER *i);
void weedEXP(EXP *e);
void weedCASE(CASE *c, int loop, int func, int event, int swtch);

#endif
