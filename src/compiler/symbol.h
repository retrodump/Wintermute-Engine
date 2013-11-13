#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "tree.h"

/* It has been shown by experiments that using a hashtable size of 317 will lead to 
   the best distribution of symbol names under the hash-code function used here */ 
#define HashSize 317

/* functions defining the datastructure used */
typedef struct SymbolTable {
    SYMBOL *table[HashSize];
    struct SymbolTable *parent;
} SymbolTable;

/* Initialize the hash table which makes up the symbol table. */
SymbolTable *initSymbolTable();

SymbolTable *scopeSymbolTable(SymbolTable *t);

/* Insert into the symboltable. */
SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind);
SYMBOL *putSymbol2(SymbolTable *t, char *name, SymbolKind kind);

/* Retrieve from the symbol table. */
SYMBOL *getSymbol(SymbolTable *t, char *name);

/* Returns 1 (true) if a symbol of the given name is found in the symbol table, 0 otherwise */
int defSymbol(SymbolTable *t, char *name);




/* the functions that build the symboltable */

void symSCRIPTCOLLECTION(SCRIPTCOLLECTION *s);


/* First pass functions */

void sym1PassSCRIPTCOLLECTION(SCRIPTCOLLECTION *s);
void sym1PassTOPLEVEL(TOPLEVEL *t, SymbolTable *sym);
void sym1PassFUNCTION(FUNCTION *f, SymbolTable *sym);
void sym1PassEXTERNAL(EXTERNAL *e, SymbolTable *sym);
void sym1PassEVENT(EVENT *e, SymbolTable *sym);
void sym1PassDECL(DECL *d, SymbolTable *sym);
void sym1PassFORINIT(FORINIT *f, SymbolTable *sym);
void sym1PassSTM(STM *s, SymbolTable *sym);
void sym1PassIDENTIFIER(IDENTIFIER *i, DECL *decl, SymbolTable *sym);
void sym1PassEXP(EXP * e, SymbolTable *sym);
void sym1PassLVALUE(LVALUE *l, SymbolTable *sym);
void sym1PassCASE(CASE *c, SymbolTable *sym);

/* Second pass functions */

void sym2PassSCRIPTCOLLECTION(SCRIPTCOLLECTION *s);
void sym2PassTOPLEVEL(TOPLEVEL *t, SymbolTable *sym);
void sym2PassFUNCTION(FUNCTION *f, SymbolTable *sym);
void sym2PassEVENT(EVENT *e, SymbolTable *sym);
void sym2PassDECL(DECL *d, SymbolTable *sym);
void sym2PassFORINIT(FORINIT *f, SymbolTable *sym);
void sym2PassSTM(STM *s, SymbolTable *sym);
void sym2PassEXP(EXP * e, SymbolTable *sym);
void sym2PassLVALUE(LVALUE *l, SymbolTable *sym);
void sym2PassCASE(CASE *c, SymbolTable *sym);


#endif

