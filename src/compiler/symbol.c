#include <string.h>
#include "symbol.h"
#include "memory.h"
#include "error.h"
#include "main.h"


/* The hash-code function */
int Hash(char *str)
{
	unsigned int hash = 0;
	while (*str) hash = (hash << 1) + *str++; 
	return hash % HashSize;
}

/* Create and initialize an empty symbol table. */
SymbolTable *initSymbolTable()
{
	SymbolTable *t;
	int i;
	t = NEW(SymbolTable);
	for (i=0; i < HashSize; i++) t->table[i] = NULL;
	t->parent = NULL;
	return t;
}

/* Add a empty child symbol table to the linked list of symbol tables. This
is done when entering a new scope level */
SymbolTable *scopeSymbolTable(SymbolTable *parent)
{
	SymbolTable *t;
	t = initSymbolTable();
	t->parent = parent;
	return t;
}


SYMBOL *putSymbol(SymbolTable *t, char *name, SymbolKind kind)
{ 
	int i;
	SYMBOL *s;
	i = Hash(name);
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name,name)==0) return s;
	}
	s = NEW(SYMBOL);
	s->name = name;
	s->kind = kind;
	s->next = t->table[i];
	s->external = 0;
	t->table[i] = s;
	return s;
}


SYMBOL *putSymbol2(SymbolTable *t, char *name, SymbolKind kind)
{ 
	int i;
	SYMBOL *s;
	i = Hash(name);
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name,name)==0) return s;
	}
	s = NEW(SYMBOL);
	s->name = name;
	s->kind = kind;
	s->next = t->table[i];
	s->external = 1;
	t->table[i] = s;
	return s;
}


SYMBOL *getSymbol(SymbolTable *t, char *name)
{
	int i;
	SYMBOL *s;
	i = Hash(name);
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name,name)==0) return s;
	}
	if (t->parent==NULL) return NULL;
	return getSymbol(t->parent,name);
}



int defSymbol(SymbolTable *t, char *name)
{
	int i;
	SYMBOL *s;
	i = Hash(name);
	for (s = t->table[i]; s; s = s->next) {
		if (strcmp(s->name,name)==0) return 1;
	}
	return 0;
}



/* the functions that build the symboltable */



/* The main function for the symbol checking phase. */
void symSCRIPTCOLLECTION(SCRIPTCOLLECTION *s)
{
	sym1PassSCRIPTCOLLECTION(s);
	/* call errorCheck to make sure that parsetree is valid */
	if(errorCheck()) return;

	sym2PassSCRIPTCOLLECTION(s);
}

/*** First pass: ***/

void sym1PassSCRIPTCOLLECTION(SCRIPTCOLLECTION *s)
{
	TExternalSymbol* symbol;

	s->sym = initSymbolTable();

	// add external symbols
	symbol = ExternalVars;
	while(symbol!=NULL){
		putSymbol2(s->sym, symbol->name, symbol->kind);
		symbol = (TExternalSymbol*)symbol->next;
	}

	symbol = ExternalFuncs;
	while(symbol!=NULL){
		putSymbol2(s->sym, symbol->name, symbol->kind);
		symbol = (TExternalSymbol*)symbol->next;
	}


	if (s->toplevels != NULL)
		sym1PassTOPLEVEL(s->toplevels, s->sym);
}

void sym1PassTOPLEVEL(TOPLEVEL *t, SymbolTable *sym)
{
	switch (t->kind) {
	case functionK:
		sym1PassFUNCTION(t->val.functionT, sym);
		break;
	case eventK:
		sym1PassEVENT(t->val.eventT, sym);
		break;
	case externalK:
		sym1PassEXTERNAL(t->val.externalT, sym);
		break;
	case stmsK:
		sym1PassSTM(t->val.stms, sym);
		break;
	}
	
	if (t->next != NULL)
		sym1PassTOPLEVEL(t->next, sym);
}

void sym1PassFUNCTION(FUNCTION *f, SymbolTable *sym)
{
	struct SYMBOL *symbol;
	
	if (defSymbol(sym, f->name)) {
		reportError(f->lineno, "Duplicate declaration of function '%s'", f->name);
	}
	else {
		symbol = putSymbol(sym, f->name, functionSym);
		symbol->val.functionS = f;
		
		f->sym = scopeSymbolTable(sym);
		if (f->formals != NULL)
			sym1PassDECL(f->formals, f->sym);
		if (f->stms != NULL)   
			sym1PassSTM(f->stms, f->sym);
	}
}

void sym1PassEVENT(EVENT *e, SymbolTable *sym)
{			
	e->sym = scopeSymbolTable(sym);
	if (e->stms != NULL)   
		sym1PassSTM(e->stms, e->sym);
}


void sym1PassEXTERNAL(EXTERNAL *e, SymbolTable *sym)
{
	struct SYMBOL *symbol;
	
	if (defSymbol(sym, e->name)) {
		reportError(e->lineno, "Duplicate declaration of function '%s'", e->name);
	}
	else {
		symbol = putSymbol2(sym, e->name, functionSym);
		symbol->val.functionS = NULL;
	}
}



void sym1PassDECL(DECL *d, SymbolTable *sym)
{
	struct SYMBOL *symbol;
	
	switch (d->kind) {
	case formalK:
		if (defSymbol(sym, d->val.formalD.name))
			reportError(d->lineno, "Duplicate declaration of formal '%s'", d->val.formalD.name);
		else {
			symbol = putSymbol(sym, d->val.formalD.name, declSym);
			symbol->val.declS = d;
		}
		break;
	case variableK:
    /* Recurse on initialization first, so the variable declared is not used there. 
		(Otherwise int i = i; would be valid) */
		if (d->val.variableD.initialization != NULL)    
			sym1PassEXP(d->val.variableD.initialization, sym);
		
		sym1PassIDENTIFIER(d->val.variableD.identifiers, d, sym);
		
		d->val.variableD.symbol = getSymbol(sym, d->val.variableD.identifiers->name);
		break;
	case simplevarK:
		/* Recurse on initialization first, so the variable declared is not used there. */
		if (d->val.simplevarD.initialization != NULL)    
			sym1PassEXP(d->val.simplevarD.initialization, sym);
		
		if (defSymbol(sym, d->val.simplevarD.name))
			reportError(d->lineno, "Duplicate declaration of variable '%s'", d->val.simplevarD.name);
		else {
			symbol = putSymbol(sym, d->val.simplevarD.name, declSym);
			symbol->val.declS = d;
		}
		break;
	}
	
	if (d->next != NULL)
		sym1PassDECL(d->next, sym);
}

void sym1PassFORINIT(FORINIT *f, SymbolTable *sym)
{
	switch (f->kind) {
	case declforinitK:
		sym1PassDECL(f->val.declforinitF, sym);
		break;
	case expforinitK:
		sym1PassEXP(f->val.expforinitF, sym);
		break;
	}
	
	if (f->next != NULL)
		sym1PassFORINIT(f->next, sym);
}

void sym1PassSTM(STM *s, SymbolTable *sym)
{
	if(!s)return;	
	switch (s->kind) {
	case skipK:
		break;
	case expK:
		sym1PassEXP(s->val.expS, sym);
		break;
	case declstmK:
		sym1PassDECL(s->val.declstmS, sym);
		break;
	case returnK:
		if(s->val.returnS.exp != NULL)
			sym1PassEXP(s->val.returnS.exp, sym);
		break;
	case ifK:
		sym1PassEXP(s->val.ifS.condition, sym);
		sym1PassSTM(s->val.ifS.body, sym);
		break;
	case ifelseK:
		sym1PassEXP(s->val.ifelseS.condition, sym);
		sym1PassSTM(s->val.ifelseS.thenpart, sym);
		sym1PassSTM(s->val.ifelseS.elsepart, sym);
		break;
	case whileK:
		sym1PassEXP(s->val.whileS.condition, sym);
		sym1PassSTM(s->val.whileS.body, sym);
		break;
	case forK:
		sym1PassFORINIT(s->val.forS.inits, sym);
		sym1PassEXP(s->val.forS.condition, sym);
		sym1PassEXP(s->val.forS.updates, sym);
		sym1PassSTM(s->val.forS.body, sym);
		break;
	case sequenceK:
		sym1PassSTM(s->val.sequenceS.first, sym);
		sym1PassSTM(s->val.sequenceS.second, sym);
		break;
	case scopeK:
    /* Create an empty symbol table, add to the list and call on the statements inside
		the scope with the new, empty symbol table. */
		//s->val.scopeS.sym = scopeSymbolTable(sym);
		//sym1PassSTM(s->val.scopeS.stm, s->val.scopeS.sym);
		sym1PassSTM(s->val.scopeS.stm, sym);
		break;
	case sleepK:
		sym1PassEXP(s->val.sleepS.time, sym);  
		break;

	case switchK:
		sym1PassEXP(s->val.switchS.condition, sym);
		if(s->val.switchS.cases) sym1PassCASE(s->val.switchS.cases, sym);
		break;	
	}
}

void sym1PassIDENTIFIER(IDENTIFIER *i, DECL *decl, SymbolTable *sym)
{
	struct SYMBOL *symbol;
	
	if (defSymbol(sym, i->name)) {
		reportError(i->lineno, "Duplicate declaration of variable '%s'", i->name);
	} else {
		symbol = putSymbol(sym, i->name, declSym);
		symbol->val.declS = decl;
	}
	
	if (i->next != NULL)
		sym1PassIDENTIFIER(i->next, decl, sym);
}

void sym1PassEXP(EXP * e, SymbolTable *sym) 
{
	if (e == NULL)
		return;	
	
	switch (e->kind) {
	case intconstK:
		break;
	case boolconstK:
		break;
	case floatconstK:
		break;
	case stringconstK:
		break;
	case uminusK:
		sym1PassEXP(e->val.uminusE, sym);
		break;
	case notK:
		sym1PassEXP(e->val.notE.exp, sym);
		break;
	case lvalueK:
		sym1PassLVALUE(e->val.lvalueE, sym);    
		break;
	case assignmentK:
		sym1PassLVALUE(e->val.assignmentE.left, sym);
		sym1PassEXP(e->val.assignmentE.right, sym);
		break;
	case equalsK:
		sym1PassEXP(e->val.equalsE.left, sym);
		sym1PassEXP(e->val.equalsE.right, sym);    
		break;
	case nequalsK:
		sym1PassEXP(e->val.nequalsE.left, sym);
		sym1PassEXP(e->val.nequalsE.right, sym);    
		break;
	case strictequalsK:
		sym1PassEXP(e->val.strictequalsE.left, sym);
		sym1PassEXP(e->val.strictequalsE.right, sym);    
		break;
	case strictnequalsK:
		sym1PassEXP(e->val.strictnequalsE.left, sym);
		sym1PassEXP(e->val.strictnequalsE.right, sym);    
		break;
	case lessK:
		sym1PassEXP(e->val.lessE.left, sym);
		sym1PassEXP(e->val.lessE.right, sym);    
		break;
	case greaterK:
		sym1PassEXP(e->val.greaterE.left, sym);
		sym1PassEXP(e->val.greaterE.right, sym);    
		break;
	case lequalsK:
		sym1PassEXP(e->val.lequalsE.left, sym);
		sym1PassEXP(e->val.lequalsE.right, sym);    
		break;
	case gequalsK:
		sym1PassEXP(e->val.gequalsE.left, sym);
		sym1PassEXP(e->val.gequalsE.right, sym);    
		break;
	case plusK:
		sym1PassEXP(e->val.plusE.left, sym);
		sym1PassEXP(e->val.plusE.right, sym);    
		break;
	case minusK:
		sym1PassEXP(e->val.minusE.left, sym);
		sym1PassEXP(e->val.minusE.right, sym);    
		break;
	case multK:
		sym1PassEXP(e->val.multE.left, sym);
		sym1PassEXP(e->val.multE.right, sym);    
		break;
	case divK:
		sym1PassEXP(e->val.divE.left, sym);
		sym1PassEXP(e->val.divE.right, sym);    
		break;
	case moduloK:
		sym1PassEXP(e->val.moduloE.left, sym);
		sym1PassEXP(e->val.moduloE.right, sym);    
		break;
	case andK:
		sym1PassEXP(e->val.andE.left, sym);
		sym1PassEXP(e->val.andE.right, sym);    
		break;
	case orK:
		sym1PassEXP(e->val.orE.left, sym);
		sym1PassEXP(e->val.orE.right, sym);    
		break;
	case invokeK:
		/* function name check is in 2nd pass */
		if (e->val.invokeE.arguments != NULL) 
			sym1PassEXP(e->val.invokeE.arguments, sym);    
		break;
	case newK:
		/* function name check is in 2nd pass */
		if (e->val.newE.arguments != NULL) 
			sym1PassEXP(e->val.newE.arguments, sym);    
		break;
	case invokemethodK:
		/* function name check is in 2nd pass */
		if (e->val.invokemethodE.arguments != NULL) 
			sym1PassEXP(e->val.invokemethodE.arguments, sym);
		break;
		
	case castK:
		sym1PassEXP(e->val.castE.exp, sym);   
		break;
	} /* end switch */
	
	if (e->next != NULL)
		sym1PassEXP(e->next, sym);
	
}


void sym1PassLVALUE(LVALUE *l, SymbolTable *sym) 
{
	SYMBOL *symbol;
	
	if(l->id==NULL){
		l->symbol = NULL;
		return;
	}
	
	symbol = getSymbol(sym, l->id);
	if (symbol == NULL)
		reportError(l->lineno, "Variable '%s' is referenced but not defined", l->id);
	else {
		if (symbol->kind != declSym)
			reportError(l->lineno, "Identifier '%s' is not a variable", 
			l->id);
		else { 
			l->symbol = symbol;
		}					
	}
	// !!!
	sym1PassEXP(l->member, sym);
}

/*** Second pass: ***/

void sym2PassSCRIPTCOLLECTION(SCRIPTCOLLECTION *s)
{
	if (s->toplevels != NULL)
		sym2PassTOPLEVEL(s->toplevels, s->sym);
}

void sym2PassTOPLEVEL(TOPLEVEL *t, SymbolTable *sym)
{
	switch (t->kind) {
	case functionK:
		sym2PassFUNCTION(t->val.functionT, sym);
		break;
	case eventK:
		sym2PassEVENT(t->val.eventT, sym);
		break;
	case stmsK:
		sym2PassSTM(t->val.stms, sym);
	}
	
	if (t->next != NULL)
		sym2PassTOPLEVEL(t->next, sym);
}

void sym2PassFUNCTION(FUNCTION *f, SymbolTable *sym)
{
	if (f->stms != NULL)   
		sym2PassSTM(f->stms, f->sym);
}

void sym2PassEVENT(EVENT *e, SymbolTable *sym)
{
	if (e->stms != NULL)   
		sym2PassSTM(e->stms, e->sym);
}

void sym2PassDECL(DECL *d, SymbolTable *sym)
{
	switch (d->kind) {
	case formalK:
		break;
	case variableK:
		if (d->val.variableD.initialization != NULL)    
			sym2PassEXP(d->val.variableD.initialization, sym);
		break;
	case simplevarK:
		if (d->val.simplevarD.initialization != NULL)    
			sym2PassEXP(d->val.simplevarD.initialization, sym);
		break;
	}
	
	if (d->next != NULL)
		sym2PassDECL(d->next, sym);
}

void sym2PassFORINIT(FORINIT *f, SymbolTable *sym)
{
	switch (f->kind) {
	case declforinitK:
		sym2PassDECL(f->val.declforinitF, sym);
		break;
	case expforinitK:
		sym2PassEXP(f->val.expforinitF, sym);
		break;
	}
	
	if (f->next != NULL)
		sym2PassFORINIT(f->next, sym);
}

void sym2PassSTM(STM *s, SymbolTable *sym)
{
	if(!s) return;
	switch (s->kind) {
	case skipK:
		break;
	case expK:
		sym2PassEXP(s->val.expS, sym);
		break;
	case declstmK:
		sym2PassDECL(s->val.declstmS, sym);
		break;
	case returnK:
		if(s->val.returnS.exp != NULL)
			sym2PassEXP(s->val.returnS.exp, sym);
		break;
	case ifK:
		sym2PassEXP(s->val.ifS.condition, sym);
		sym2PassSTM(s->val.ifS.body, sym);
		break;
	case ifelseK:
		sym2PassEXP(s->val.ifelseS.condition, sym);
		sym2PassSTM(s->val.ifelseS.thenpart, sym);
		sym2PassSTM(s->val.ifelseS.elsepart, sym);
		break;
	case whileK:
		sym2PassEXP(s->val.whileS.condition, sym);
		sym2PassSTM(s->val.whileS.body, sym);
		break;
	case forK:
		sym2PassFORINIT(s->val.forS.inits, sym);
		sym2PassEXP(s->val.forS.condition, sym);
		sym2PassEXP(s->val.forS.updates, sym);
		sym2PassSTM(s->val.forS.body, sym);
		break;
	case sequenceK:
		sym2PassSTM(s->val.sequenceS.first, sym);
		sym2PassSTM(s->val.sequenceS.second, sym);
		break;
	case scopeK:
		//sym2PassSTM(s->val.scopeS.stm, s->val.scopeS.sym);
		sym2PassSTM(s->val.scopeS.stm, sym);
		break;
	case sleepK:
		sym2PassEXP(s->val.sleepS.time, sym);
		break;
	case switchK:
		sym2PassEXP(s->val.switchS.condition, sym);
		if(s->val.switchS.cases) sym2PassCASE(s->val.switchS.cases, sym);
		break;
	}
}

void sym2PassEXP(EXP * e, SymbolTable *sym) 
{
	SYMBOL *symbol;
	if(e == NULL)
		return;
	
	switch (e->kind) {
	case intconstK:
		break;
	case boolconstK:
		break;
	case floatconstK:
		break;
	case stringconstK:
		break;
	case uminusK:
		sym2PassEXP(e->val.uminusE, sym);
		break;
	case decrementK:
		symbol = getSymbol(sym, e->val.decrementE.name);
		
		if (symbol == NULL)
			reportError(e->lineno, "Undefined variable '%s'",  e->val.decrementE.name);
		else if (symbol->kind != declSym)
			reportError(e->lineno, "Cannot decrement function (%s)", e->val.decrementE.name);
		else
			e->val.decrementE.symbol = symbol;
		break;

	case incrementK:
		symbol = getSymbol(sym, e->val.incrementE.name);
		
		if (symbol == NULL)
			reportError(e->lineno, "Undefined variable '%s'",  e->val.incrementE.name);
		else if (symbol->kind != declSym)
			reportError(e->lineno, "Cannot increment function (%s)", e->val.incrementE.name);
		else
			e->val.incrementE.symbol = symbol;
		break;

	case notK:
		sym2PassEXP(e->val.notE.exp, sym);
		break;
	case lvalueK:
		sym2PassLVALUE(e->val.lvalueE, sym);    
		break;	case assignmentK:
		sym2PassEXP(e->val.assignmentE.right, sym);
		break;
	case equalsK:
		sym2PassEXP(e->val.equalsE.left, sym);
		sym2PassEXP(e->val.equalsE.right, sym);    
		break;
	case nequalsK:
		sym2PassEXP(e->val.nequalsE.left, sym);
		sym2PassEXP(e->val.nequalsE.right, sym);    
		break;
	case strictequalsK:
		sym2PassEXP(e->val.strictequalsE.left, sym);
		sym2PassEXP(e->val.strictequalsE.right, sym);    
		break;
	case strictnequalsK:
		sym2PassEXP(e->val.strictnequalsE.left, sym);
		sym2PassEXP(e->val.strictnequalsE.right, sym);    
		break;
	case lessK:
		sym2PassEXP(e->val.lessE.left, sym);
		sym2PassEXP(e->val.lessE.right, sym);    
		break;
	case greaterK:
		sym2PassEXP(e->val.greaterE.left, sym);
		sym2PassEXP(e->val.greaterE.right, sym);    
		break;
	case lequalsK:
		sym2PassEXP(e->val.lequalsE.left, sym);
		sym2PassEXP(e->val.lequalsE.right, sym);    
		break;
	case gequalsK:
		sym2PassEXP(e->val.gequalsE.left, sym);
		sym2PassEXP(e->val.gequalsE.right, sym);    
		break;
	case plusK:
		sym2PassEXP(e->val.plusE.left, sym);
		sym2PassEXP(e->val.plusE.right, sym);    
		break;
	case minusK:
		sym2PassEXP(e->val.minusE.left, sym);
		sym2PassEXP(e->val.minusE.right, sym);    
		break;
	case multK:
		sym2PassEXP(e->val.multE.left, sym);
		sym2PassEXP(e->val.multE.right, sym);    
		break;
	case divK:
		sym2PassEXP(e->val.divE.left, sym);
		sym2PassEXP(e->val.divE.right, sym);    
		break;
	case moduloK:
		sym2PassEXP(e->val.moduloE.left, sym);
		sym2PassEXP(e->val.moduloE.right, sym);    
		break;
	case andK:
		sym2PassEXP(e->val.andE.left, sym);
		sym2PassEXP(e->val.andE.right, sym);    
		break;
	case orK:
		sym2PassEXP(e->val.orE.left, sym);
		sym2PassEXP(e->val.orE.right, sym);    
		break;
	case invokeK:
		/* do function name check now */
		symbol = getSymbol(sym, e->val.invokeE.name);
		
		if (symbol == NULL)
			reportError(e->lineno, "Function '%s' is referenced but not defined",  e->val.invokeE.name);
		else if (symbol->kind != functionSym)
			reportError(e->lineno, "Trying to use variable %s as a function", e->val.invokeE.name);
		else
			e->val.invokeE.symbol = symbol;
		
		if (e->val.invokeE.arguments != NULL) 
			sym2PassEXP(e->val.invokeE.arguments, sym);    
		break;
	case newK:
		/* do function name check now */
		symbol = getSymbol(sym, e->val.newE.name);
		
		if (symbol == NULL)
			reportError(e->lineno, "Object '%s' is referenced but no constuctor is defined",  e->val.newE.name);
		else if (symbol->kind != functionSym)
			reportError(e->lineno, "Trying to use variable %s as an object", e->val.newE.name);
		else
			e->val.newE.symbol = symbol;
		
		if (e->val.newE.arguments != NULL) 
			sym2PassEXP(e->val.newE.arguments, sym);    
		break;
	case invokemethodK:
		/* do function name check now */
		symbol = getSymbol(sym, e->val.invokemethodE.name);
		
		if (symbol == NULL)
			reportError(e->lineno, "Method is called for '%s' which is not defined",  e->val.invokemethodE.name);
		else if (symbol->kind != declSym)
			reportError(e->lineno, "Trying to call method of a non-variable %s", e->val.invokemethodE.name);
		else
			e->val.invokemethodE.symbol = symbol;
		
		if (e->val.invokemethodE.arguments != NULL) 
			sym2PassEXP(e->val.invokemethodE.arguments, sym);
		break;
	case castK:
		sym2PassEXP(e->val.castE.exp, sym);   
		break;
  } /* end switch */
  
  if (e->next != NULL)
	  sym2PassEXP(e->next, sym);
}


void sym2PassLVALUE(LVALUE *l, SymbolTable *sym) 
{
	sym2PassEXP(l->member, sym);
}


void sym1PassCASE(CASE *c, SymbolTable *sym)
{
	switch(c->kind){
	case normalcaseK:
		sym1PassEXP(c->val.normalcaseD.label, sym);
		if(c->val.normalcaseD.stms) sym1PassSTM(c->val.normalcaseD.stms, sym);
		break;
	case defaultcaseK:
		if(c->val.defaultcaseD.stms) sym1PassSTM(c->val.defaultcaseD.stms, sym);
		break;
	}
	
	if(c->next != NULL)
		sym1PassCASE(c->next, sym);
}


void sym2PassCASE(CASE *c, SymbolTable *sym)
{
	switch(c->kind){
	case normalcaseK:
		sym2PassEXP(c->val.normalcaseD.label, sym);
		if(c->val.normalcaseD.stms) sym2PassSTM(c->val.normalcaseD.stms, sym);
		break;
	case defaultcaseK:
		if(c->val.defaultcaseD.stms) sym2PassSTM(c->val.defaultcaseD.stms, sym);
		break;
	}
	
	if(c->next != NULL)
		sym2PassCASE(c->next, sym);
}
