/* --** Includes: **--------------------------------------------------- */
#include "tree.h"
#include "memory.h"
#include "error.h"


/* --** Functions: **-------------------------------------------------- */
struct SCRIPTCOLLECTION *makeSCRIPTCOLLECTION(TOPLEVEL *toplevels)
{
	SCRIPTCOLLECTION *p = NEW(SCRIPTCOLLECTION);
	p->lineno = lineno;
	p->toplevels = toplevels;
	p->sym = NULL;
	return p;
}


struct TOPLEVEL *makeTOPLEVELfunction(FUNCTION *function)
{
	TOPLEVEL *p = NEW(TOPLEVEL);
	p->lineno = lineno;
	p->kind = functionK;
	p->val.functionT = function;
	p->next = NULL;
	return p;
}


struct TOPLEVEL *makeTOPLEVELevent(EVENT* event)
{
	TOPLEVEL *p = NEW(TOPLEVEL);
	p->lineno = lineno;
	p->kind = eventK;
	p->val.eventT = event;
	p->next = NULL;
	return p;
}


struct TOPLEVEL *makeTOPLEVELexternal(EXTERNAL* external)
{
	TOPLEVEL *p = NEW(TOPLEVEL);
	p->lineno = lineno;
	p->kind = externalK;
	p->val.externalT = external;
	p->next = NULL;
	return p;
}


struct TOPLEVEL *makeTOPLEVELstms(STM* stms)
{
	TOPLEVEL *p = NEW(TOPLEVEL);
	p->lineno = lineno;
	p->kind = stmsK;
	p->val.stms = stms;
	p->next = NULL;
	return p;
}


struct FUNCTION *makeFUNCTION(char *name,struct DECL *formals,struct STM *stms, int method)
{
	FUNCTION *p = NEW(FUNCTION);
	p->lineno = lineno;
	p->name = name;
	p->formals = formals;
	p->stms = stms;
	p->sym = NULL;
	p->opcodes = NULL;
	p->labels = NULL;
	p->method = method;
	return p;
}


struct EXTERNAL *makeEXTERNAL(char *name, char *library, struct EXT_TYPE *returns, struct EXT_TYPE *formals, struct CALL_TYPE *call)
{
	EXTERNAL *p = NEW(EXTERNAL);
	p->lineno = lineno;
	p->name = name;
	p->library = library;
	p->returns = returns;
	p->formals = formals;
	p->call = call;
	p->sym = NULL;
	return p;
}


struct EVENT *makeEVENT(char *name, struct STM *stms)
{
	EVENT *p = NEW(EVENT);
	p->lineno = lineno;
	p->name = name;
	p->stms = stms;
	p->sym = NULL;
	return p;
}


struct TYPE *makeTYPElocal()
{
	TYPE *p = NEW(TYPE);
	p->lineno = lineno;
	p->kind = localK;
	return p;
}

struct TYPE *makeTYPEglobal()
{
	TYPE *p = NEW(TYPE);
	p->lineno = lineno;
	p->kind = globalK;
	return p;
}

struct TYPE *makeTYPEconst()
{
	TYPE *p = NEW(TYPE);
	p->lineno = lineno;
	p->kind = constK;
	return p;
}


struct TYPE *makeTYPEvoid()
{
	TYPE *p = NEW(TYPE);
	p->lineno = lineno;
	p->kind = voidK;
	return p;
}


struct EXT_TYPE *makeEXT_TYPEvoid()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = voidK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEbool()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = boolK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPElong()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = longK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEbyte()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = byteK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEstring()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = stringK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEfloat()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = floatK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEdouble()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = doubleK;
	p->next = NULL;
	return p;
}

struct EXT_TYPE *makeEXT_TYPEmembuffer()
{
	EXT_TYPE *p = NEW(EXT_TYPE);
	p->lineno = lineno;
	p->kind = membufferK;
	p->next = NULL;
	return p;
}


struct CALL_TYPE *makeCALL_TYPEstdcall()
{
	CALL_TYPE *p = NEW(CALL_TYPE);
	p->lineno = lineno;
	p->kind = stdcallK;
	return p;
}

struct CALL_TYPE *makeCALL_TYPEcdecl()
{
	CALL_TYPE *p = NEW(CALL_TYPE);
	p->lineno = lineno;
	p->kind = cdeclK;
	return p;
}

struct CALL_TYPE *makeCALL_TYPEthiscall()
{
	CALL_TYPE *p = NEW(CALL_TYPE);
	p->lineno = lineno;
	p->kind = thiscallK;
	return p;
}

struct DECL *makeDECLformal(char *name)
{
	DECL *p = NEW(DECL);
	p->next = NULL;
	p->type = NULL;
	p->lineno = lineno;
	p->kind = formalK;
	p->type = makeTYPElocal();
	p->val.formalD.name = name;
	return p;
}

struct DECL *makeDECLvariable(struct TYPE *type, struct IDENTIFIER *identifiers, struct EXP *initialization)
{
	DECL *p = NEW(DECL);
	p->next = NULL;
	p->type = NULL;
	p->lineno = lineno;
	p->kind = variableK;
	p->type = type;
	p->val.variableD.identifiers = identifiers;
	p->val.variableD.initialization = initialization;
	return p;
}

struct DECL *makeDECLsimplevar(char *name, struct EXP *initialization)
{
	DECL *p = NEW(DECL);
	p->next = NULL;
	p->type = NULL;
	p->lineno = lineno;
	p->kind = simplevarK;
	p->type = makeTYPElocal();
	p->val.simplevarD.name = name;
	p->val.simplevarD.initialization = initialization;
	return p;
}

struct FORINIT *makeFORINITdecl(DECL *decl)
{
	FORINIT *p = NEW(FORINIT);
	p->lineno = lineno;
	p->kind = declforinitK;
	p->val.declforinitF = decl;
	p->next = NULL;
	
	return p;
}

struct FORINIT *makeFORINITexp(EXP *exp)
{
	FORINIT *p = NEW(FORINIT);
	p->lineno = lineno;
	p->kind = expforinitK;
	p->val.expforinitF = exp;
	p->next = NULL;
	
	return p;
}

struct STM *makeSTMskip()
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = skipK;
	return p;
}

struct STM *makeSTMdecl(struct DECL *decl)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = declstmK;
	p->val.declstmS = decl;
	return p; 
}

struct STM *makeSTMexp(struct EXP *exp)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = expK;
	p->val.expS = exp;
	return p;
}

struct STM *makeSTMreturn(struct EXP *exp)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = returnK;
	p->val.returnS.exp = exp;
	return p;
}
struct STM *makeSTMif(struct EXP *condition,struct STM *body)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = ifK;
	p->val.ifS.condition = condition;
	p->val.ifS.body = body;
	return p;
}
struct STM *makeSTMifelse(struct EXP *condition,struct STM *thenpart,struct STM *elsepart)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = ifelseK;
	p->val.ifelseS.condition = condition;
	p->val.ifelseS.thenpart = thenpart;
	p->val.ifelseS.elsepart = elsepart; 
	return p;
}
struct STM *makeSTMwhile(struct EXP *condition,struct STM *body)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = whileK;
	p->val.whileS.condition = condition;
	p->val.whileS.body = body;
	return p;
}

struct STM *makeSTMfor(struct FORINIT *inits, struct EXP *condition, struct EXP *updates, struct STM *body)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = forK;
	p->val.forS.inits = inits;
	p->val.forS.condition = condition;
	p->val.forS.updates = updates;
	p->val.forS.body = body;
	
	return p;
}

struct STM *makeSTMsequence(struct STM *first,struct STM *second)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = sequenceK;
	p->val.sequenceS.first = first;
	p->val.sequenceS.second = second;
	return p;
}

struct STM *makeSTMscope(struct STM *stm)
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = scopeK;
	p->val.scopeS.sym = NULL;
	p->val.scopeS.stm = stm;
	return p;
}

struct STM *makeSTMsleep(struct EXP *time) 
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = sleepK;
	p->val.sleepS.time = time;
	return p;	
}

struct STM *makeSTMbreak() 
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = breakK;
	return p;	
}

struct STM *makeSTMcontinue() 
{
	STM *p = NEW(STM);
	p->lineno = lineno;
	p->kind = continueK;
	return p;	
}


struct IDENTIFIER *makeIDENTIFIER(char *name)
{
	IDENTIFIER *p = NEW(IDENTIFIER);
	p->lineno = lineno;
	p->name = name;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPnull()
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = nullK;
	p->next = NULL;
	return p;
};


struct EXP *makeEXPintconst(int value)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = intconstK;
	p->val.intconstE = value;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPboolconst(int value)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = boolconstK;
	p->val.boolconstE = value;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPfloatconst(double value)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = floatconstK;
	p->val.floatconstE = value;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPstringconst(char *value)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = stringconstK;
	p->val.stringconstE = value;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPuminus(struct EXP *exp)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = uminusK;
	p->val.uminusE = exp;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPdecrement(char* name)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = decrementK;
	p->val.decrementE.name = name;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPincrement(char* name)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = incrementK;
	p->val.incrementE.name = name;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPnot(struct EXP *exp)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = notK;
	p->val.notE.exp = exp;
	p->next = NULL;
	return p;
}



struct EXP *makeEXPlvalue(struct LVALUE *lvalue)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = lvalueK;
	p->val.lvalueE = lvalue;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPassignment(struct LVALUE *lvalue, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = assignmentK;
	p->val.assignmentE.left = lvalue;
	p->val.assignmentE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPequals(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = equalsK;
	p->val.equalsE.left = left;
	p->val.equalsE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPnequals(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = nequalsK;
	p->val.nequalsE.left = left;
	p->val.nequalsE.right = right;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPstrictequals(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = strictequalsK;
	p->val.strictequalsE.left = left;
	p->val.strictequalsE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPstrictnequals(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = strictnequalsK;
	p->val.strictnequalsE.left = left;
	p->val.strictnequalsE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPless(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = lessK;
	p->val.lessE.left = left;
	p->val.lessE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPgreater(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = greaterK;
	p->val.greaterE.left = left;
	p->val.greaterE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPlequals(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = lequalsK;
	p->val.lequalsE.left = left;
	p->val.lequalsE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPgequals(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = gequalsK;
	p->val.gequalsE.left = left;
	p->val.gequalsE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPplus(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = plusK;
	p->val.plusE.left = left;
	p->val.plusE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPminus(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = minusK;
	p->val.minusE.left = left;
	p->val.minusE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPmult(struct EXP *left, struct EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = multK;
	p->val.multE.left = left;
	p->val.multE.right = right;
	p->next = NULL;\
		return p;
}

struct EXP *makeEXPdiv(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = divK;
	p->val.divE.left = left;
	p->val.divE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPmodulo(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = moduloK;
	p->val.moduloE.left = left;
	p->val.moduloE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPand(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = andK;
	p->val.andE.left = left;
	p->val.andE.right = right;
	p->next = NULL;
	return p;
}

struct EXP *makeEXPor(struct EXP *left,struct  EXP *right)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = orK;
	p->val.orE.left = left;
	p->val.orE.right = right;
	p->next = NULL;
	return p;
}


struct EXP *makeEXPinvoke(char *name,struct  EXP *arguments)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = invokeK;
	p->val.invokeE.name = name;
	p->val.invokeE.arguments = swapArguments(arguments);
	p->next = NULL;
	return p;
}


struct EXP *makeEXPnew(char *name,struct  EXP *arguments)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = newK;
	p->val.newE.name = name;
	p->val.newE.arguments = swapArguments(arguments);
	p->next = NULL;
	return p;
}


struct EXP *makeEXPinvokemethod(char *name, struct EXP* method, struct EXP *arguments)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = invokemethodK;
	p->val.invokemethodE.name = name;
	p->val.invokemethodE.method = method;
	p->val.invokemethodE.arguments = swapArguments(arguments);
	p->next = NULL;
	return p;
}


struct EXP *makeEXPcast(struct TYPE *type,struct  EXP *exp)
{
	EXP *p = NEW(EXP);
	p->type = NULL;
	p->lineno = lineno;
	p->kind = castK;
	p->val.castE.type = type;
	p->val.castE.exp = exp;
	p->next = NULL;
	return p;
}


struct LVALUE *makeLVALUEid(char *id, char* member)
{
	LVALUE *p = NEW(LVALUE);
	p->lineno = lineno;
	p->id = id;
	p->member = member==NULL?NULL:makeEXPstringconst(member);
	return p;
}


struct LVALUE *makeLVALUEexp(char *id, EXP* exp)
{
	LVALUE *p = NEW(LVALUE);
	p->lineno = lineno;
	p->id = id;
	p->member = exp;
	return p;
}


struct EXP* swapArguments(struct EXP *arguments)
{
	struct EXP* ret=NULL;
	struct EXP* dummy;
	while(arguments!=NULL){
		dummy = arguments->next;
		arguments->next = ret;

		ret = arguments;

		arguments = dummy;
	}
	return ret;
}


struct STM *makeSTMswitch(struct EXP *condition, struct CASE* cases)
{
	STM* p = NEW(STM);
	p->lineno = lineno;
	p->kind = switchK;
	p->val.switchS.condition = condition;
	p->val.switchS.cases = cases;

	return p;
}


struct CASE* makeCASEnormal(struct EXP* label, STM* body)
{
	CASE* p = NEW(CASE);
	p->lineno = lineno;
	p->kind = normalcaseK;
	p->val.normalcaseD.label = label;
	p->val.normalcaseD.stms = body;
	p->next = NULL;

	return p;
}


struct CASE* makeCASEdefault(STM* body)
{
	CASE* p = NEW(CASE);
	p->lineno = lineno;
	p->kind = defaultcaseK;
	p->val.defaultcaseD.stms = body;
	p->next = NULL;

	return p;
}


void addDefaultCase(CASE* c, CASE* d)
{
	while (c->next) c = c->next;
	c->next = d;
}