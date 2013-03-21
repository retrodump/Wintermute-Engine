#ifndef __tree_h
#define __tree_h 

/* -*** Includes ***-------------------------------------------------------- */
#include <stdio.h>
#include "typedef.h"

/* --** Types: **------------------------------------------------------ */
typedef enum {mdlEntity, particleEntity, playerEntity, lightEntity, cameraEntity} EntityKind;
typedef enum {functionSym, declSym} SymbolKind;


typedef struct SYMBOL {
    char *name;
    SymbolKind kind;
    union {
		struct FUNCTION *functionS;
		struct DECL *declS;
    } val;
	int external;
    struct SYMBOL *next;
} SYMBOL; 

typedef struct SCRIPTCOLLECTION {
	int lineno;
	struct TOPLEVEL *toplevels;
	struct SymbolTable *sym; /* symbol */
	struct OPCODE* opcodes;
} SCRIPTCOLLECTION;

typedef struct TOPLEVEL {
	int lineno;
	enum {functionK, stmsK, eventK, externalK} kind; 
	
	union {
		struct FUNCTION *functionT;
		struct STM* stms;
		struct EVENT *eventT;
		struct EXTERNAL *externalT;
	} val;
	struct TOPLEVEL *next;
} TOPLEVEL;

typedef struct FUNCTION {
	int lineno;
	char *name;
	struct TYPE *type;
	struct DECL *formals;
	struct STM *stms;
	struct SymbolTable *sym; 	/* symbol */
	int localslimit; 		/* resource */
	int labelcount; 		/* resource */
	char *signature; 		/* code */
	struct LABEL *labels; 	/* code */
	struct CODE *opcodes; 	/* code */
	int method;
} FUNCTION;


typedef struct EXTERNAL {
	int lineno;
	char *name;
	char *library;
	struct EXT_TYPE *returns;
	struct EXT_TYPE *formals;
	struct CALL_TYPE *call;
	struct SymbolTable *sym; 	/* symbol */
} EXTERNAL;


typedef struct EXT_TYPE {
	int lineno;
	enum { voidK, boolK, longK, byteK, stringK, floatK, doubleK, membufferK } kind;
	struct EXT_TYPE* next;
} EXT_TYPE;


typedef struct CALL_TYPE {
	int lineno;
	enum { stdcallK, cdeclK, thiscallK } kind;
} CALL_TYPE;


typedef struct EVENT {
	int lineno;
	char *name;
	struct STM *stms;
	struct SymbolTable *sym; 	/* symbol */
} EVENT;


typedef struct TYPE {
	int lineno;
	enum {localK, globalK, constK } kind;
} TYPE;



typedef struct DECL {
	int lineno;
	enum {formalK, variableK, simplevarK} kind;
	struct TYPE *type; /* Parser! */
    
	union {
		struct {
			int offset; /* resource */	
			char *name;
		} formalD;
		struct {
			struct IDENTIFIER *identifiers;
			struct EXP *initialization;
			SYMBOL *symbol; /* symbol */
			int offset; /* resource */
		} variableD;
		struct {
			char *name;
			struct EXP *initialization;
			int offset;  /* resource */
		} simplevarD;
	} val;
	
	struct DECL *next;
} DECL;

typedef struct FORINIT {
	int lineno;
	enum {declforinitK, expforinitK} kind;
	
	union{
		struct DECL *declforinitF;
		struct EXP *expforinitF;
	} val;
	
	struct FORINIT *next;
} FORINIT;


typedef struct CASE {
	int lineno;
	enum { normalcaseK, defaultcaseK } kind;

	union{
		struct {
			struct EXP* label;
			struct STM* stms;
		} normalcaseD;
		
		struct {
			struct STM* stms;
		} defaultcaseD;
	} val;

	struct CASE* next;
} CASE;


typedef struct STM {
	int lineno;
	enum {skipK, expK, declstmK, returnK, ifK, ifelseK, whileK, forK, sequenceK, scopeK, setintK, sleepK, breakK, continueK, switchK } kind;
	
	union{
		
		struct EXP *expS;
		
		struct DECL *declstmS;
		
		struct {
			struct EXP *exp;
		} returnS;
		
		struct {
			struct EXP *condition;
			struct STM *body;
			int stoplabel; /* resource */
		} ifS;
		
		struct {
			struct EXP *condition;
			struct STM *thenpart;
			struct STM *elsepart;
			int elselabel,stoplabel; /* resource */
		} ifelseS;
		
		struct {
			struct EXP *condition;
			struct STM *body;
			int startlabel,stoplabel; /* resource */
		} whileS;
		
		struct {
			struct FORINIT *inits;
			struct EXP *condition;
			struct EXP *updates;
			struct STM *body;
			int startlabel,stoplabel; /* resource */
		} forS;
		
		struct {
			struct STM *first;
			struct STM *second;
		} sequenceS;
		
		struct {
			struct SymbolTable *sym; /* symbol */
			struct STM *stm;
		} scopeS;
		
		struct {
			struct EXP *time;
		} sleepS;

		struct {
			struct EXP* condition;
			struct CASE* cases;
		} switchS;
		
	} val;
} STM;

typedef struct IDENTIFIER {
	int lineno;
	char *name;
	struct IDENTIFIER *next;
} IDENTIFIER;




typedef struct EXP {
	int lineno;
	TYPE *type; /* type */
	
	enum {nullK, intconstK, boolconstK, floatconstK, stringconstK, uminusK, notK, incrementK, decrementK, lvalueK, assignmentK, 
		equalsK, nequalsK, strictequalsK, strictnequalsK, lessK, greaterK, lequalsK, gequalsK, plusK, minusK, multK, divK, moduloK, 
		andK, orK,  invokeK, invokemethodK, castK, newK} kind;
	union {
		int intconstE;
		int boolconstE;
        double floatconstE;
		char *stringconstE;
		
		
		struct EXP *uminusE;

		struct{
			char *name;
			struct SYMBOL *symbol; /* symbol */
		} decrementE;

		struct{
			char *name;
			struct SYMBOL *symbol; /* symbol */
		} incrementE;

		
		struct {
			struct EXP *exp;
			int truelabel,stoplabel; /* resource */
		} notE;
		
		struct LVALUE *lvalueE;
		
		struct {
			struct LVALUE *left;
			struct EXP *right;
		} assignmentE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} equalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} nequalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} strictequalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} strictnequalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} lessE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} greaterE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} lequalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel,stoplabel; /* resource */
		} gequalsE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
		} plusE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
		} minusE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
		} multE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
		} divE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
		} moduloE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int falselabel; /* resource */
		} andE;
		
		struct {
			struct EXP *left;
			struct EXP *right;
			int truelabel; /* resource */
		} orE;
		
		struct {
			char *name;
			struct EXP *arguments;
			struct SYMBOL *symbol; /* symbol */
		} invokeE;
		
		struct {
			char *name;
			struct EXP *arguments;
			struct SYMBOL *symbol; /* symbol */
		} newE;
		
		struct {
			char *name;
			struct EXP *method;
			struct EXP *arguments;
			struct SYMBOL *symbol; /* symbol */
		} invokemethodE;
		
		struct {
			struct TYPE *type;
			struct EXP *exp;
		} castE;
		
  } val;
  struct EXP *next;
} EXP;



typedef struct LVALUE {
	int lineno;
	struct SYMBOL *symbol; /* symbol */
	struct TYPE *type;
	char *id;
	EXP* member;
} LVALUE;




/* --** Prototypes: **------------------------------------------------- */
struct SCRIPTCOLLECTION *makeSCRIPTCOLLECTION(TOPLEVEL *toplevels);

struct TOPLEVEL *makeTOPLEVELfunction(FUNCTION *function);
struct TOPLEVEL *makeTOPLEVELstms(STM* stms);
struct TOPLEVEL *makeTOPLEVELevent(EVENT* event);
struct TOPLEVEL *makeTOPLEVELexternal(EXTERNAL* external);

struct FUNCTION *makeFUNCTION(char *name,struct DECL *formals,struct STM *stms, int method);
struct EVENT *makeEVENT(char *name, struct STM *stms);
struct EXTERNAL *makeEXTERNAL(char *name, char *library, struct EXT_TYPE *returns, struct EXT_TYPE *formals, struct CALL_TYPE* call);

struct TYPE *makeTYPElocal();
struct TYPE *makeTYPEglobal();
struct TYPE *makeTYPEconst();
struct TYPE *makeTYPEvoid();

struct EXT_TYPE *makeEXT_TYPEvoid();
struct EXT_TYPE *makeEXT_TYPEbool();
struct EXT_TYPE *makeEXT_TYPElong();
struct EXT_TYPE *makeEXT_TYPEbyte();
struct EXT_TYPE *makeEXT_TYPEstring();
struct EXT_TYPE *makeEXT_TYPEfloat();
struct EXT_TYPE *makeEXT_TYPEdouble();
struct EXT_TYPE *makeEXT_TYPEmembuffer();

struct CALL_TYPE *makeCALL_TYPEstdcall();
struct CALL_TYPE *makeCALL_TYPEcdecl();
struct CALL_TYPE *makeCALL_TYPEthiscall();


struct DECL *makeDECLformal(char *name);
struct DECL *makeDECLvariable(struct TYPE *type, struct IDENTIFIER *identifiers, struct EXP *initialization);
struct DECL *makeDECLsimplevar(char *name, struct EXP *initialization);

struct FORINIT *makeFORINITdecl(DECL *decl);
struct FORINIT *makeFORINITexp(EXP *exp);

struct STM *makeSTMskip();
struct STM *makeSTMdecl(struct DECL *decl);
struct STM *makeSTMexp(struct EXP *exp);
struct STM *makeSTMreturn(struct EXP *exp);
struct STM *makeSTMif(struct EXP *condition, struct STM *body);
struct STM *makeSTMifelse(struct EXP *condition, struct STM *thenpart, struct STM *elsepart);
struct STM *makeSTMwhile(struct EXP *condition, struct STM *body);
struct STM *makeSTMfor(struct FORINIT *inits, struct EXP *condition, struct EXP *updates, struct STM *body);
struct STM *makeSTMsequence(struct STM *first, struct STM *second);
struct STM *makeSTMscope(struct STM *stm);
struct STM *makeSTMsetint(EntityKind kind, struct EXP *modelname, struct EXP *nr, struct EXP *val);
struct STM *makeSTMsleep(struct EXP *time);
struct STM *makeSTMbreak();
struct STM *makeSTMcontinue();
struct STM *makeSTMswitch(struct EXP *condition, struct CASE* cases);

struct CASE* makeCASEnormal(struct EXP* label, STM* body);
struct CASE* makeCASEdefault(STM* body);

struct IDENTIFIER *makeIDENTIFIER(char *name);

struct EXP *makeEXPnull();
struct EXP *makeEXPintconst(int value);
struct EXP *makeEXPboolconst(int value);
struct EXP *makeEXPfloatconst(double value);
struct EXP *makeEXPstringconst(char *value);
struct EXP *makeEXPuminus(struct EXP *exp);
struct EXP *makeEXPnot(struct EXP *exp);
struct EXP *makeEXPlvalue(struct LVALUE *lvalue);
struct EXP *makeEXPassignment(struct LVALUE *lvalue, struct EXP *right);
struct EXP *makeEXPequals(struct EXP *left, struct EXP *right);
struct EXP *makeEXPnequals(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPstrictequals(struct EXP *left, struct EXP *right);
struct EXP *makeEXPstrictnequals(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPless(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPgreater(struct EXP *left, struct EXP *right);
struct EXP *makeEXPlequals(struct EXP *left, struct EXP *right);
struct EXP *makeEXPgequals(struct EXP *left, struct EXP *right);
struct EXP *makeEXPplus(struct EXP *left, struct EXP *right);
struct EXP *makeEXPminus(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPmult(struct EXP *left, struct EXP *right);
struct EXP *makeEXPdiv(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPmodulo(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPand(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPor(struct EXP *left, struct  EXP *right);
struct EXP *makeEXPinvoke(char *name, struct  EXP *arguments);
struct EXP *makeEXPnew(char *name, struct  EXP *arguments);
struct EXP *makeEXPinvokemethod(char *name, struct EXP* method, struct  EXP *arguments);
struct EXP *makeEXPcast(struct TYPE *type,struct  EXP *exp);
struct EXP *makeEXPincrement(char* name);
struct EXP *makeEXPdecrement(char* name);

struct EXP* swapArguments(struct EXP *arguments);

struct LVALUE *makeLVALUEid(char *id, char* member);
struct LVALUE *makeLVALUEexp(char *id, EXP* exp);

void addDefaultCase(CASE* c, CASE* d);

#endif
