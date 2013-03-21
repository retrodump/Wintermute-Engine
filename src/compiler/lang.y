%{
  /* -*** Includes ***------------------------------------------------------------- */
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "error.h"
#include "tree.h" 
#include "main.h"

/* Prototype for the lexer (no .h file is generated): */
int yylex();

/* Outmost container */
extern SCRIPTCOLLECTION *thescriptcollection; 

%}

/* Token definitions */
%start scriptcollection

%union {
  struct SCRIPTCOLLECTION *scriptcollection;    /* A collection of functions and programs.. */
  struct TOPLEVEL *toplevel;
  struct FUNCTION *function;  /* A standard C-style function */
  struct EVENT *event;  /* A standard C-style function */
  struct EXTERNAL *external;
  struct TYPE *type;
  struct EXT_TYPE *ext_type;
  struct CALL_TYPE *call_type;
  struct IDENTIFIER *identifiers;
  struct EXP *exp;
  struct DECL *decl;
  struct FORINIT *forinit;
  struct CASE *case_branch;
  struct STM *stm;
  struct LVALUE *lvalue;

  char *identifier;
  int intconst;
  int boolconst;
  double floatconst;
  char *stringconst;
};

%token 
tAND tELSE tEQUALS tGEQUALS tIF tINT tLEQUALS tNEQUALS tOR tRETURN tSTRICTEQUALS tSTRICTNEQUALS
tBOOL tLONG tBYTE tSTRING tVOID tFLOAT tDOUBLE tINT tSHORT tMEMBUFFER
tSTDCALL tCDECL tTHISCALL
tINCREMENT tDECREMENT
tWHILE tERROR tFOR
tFUNCTION tMETHOD tVAR tGLOBAL tCONST tNULL tBREAK tCONTINUE tNEW tTHIS tON tEXTERNAL
tSWITCH tCASE tDEFAULT

%token <identifier> tIDENTIFIER
%token <intconst> tINTCONST
%token <boolconst> tBOOLCONST
%token <floatconst> tFLOATCONST
%token <stringconst> tSTRINGCONST

%type <scriptcollection> scriptcollection
%type <toplevel> toplevels netoplevels toplevel
%type <function> function
%type <event> event
%type <external> external
%type <type> type
%type <case_branch> case_branch case_branches dcases default_branch
%type <ext_type> ext_type ext_formal ext_formals neext_formals ext_returns
%type <call_type> call_type
%type <identifiers> identifiers
%type <exp> initialization exp unaryexp unarypostfixexp exps neexps
%type <decl> decl simpledecl formals neformals formal
%type <forinit> forinits neforinits forinit
%type <stm> nestms stm compoundstm
%type <lvalue> lvalue


/* Resolve dangling else conflict: */
%left ')'
%left tELSE


/* Resolve binary operator conflicts: */
%left '='
%left tOR tAND
%left tEQUALS tNEQUALS tSTRICTEQUALS tSTRICTNEQUALS tLEQUALS tGEQUALS '<' '>'
%left '+' '-' tINCREMENT tDECREMENT
%left '*' '/'
%left '%'

%% /* productions */

scriptcollection : toplevels 
          { thescriptcollection = makeSCRIPTCOLLECTION($1); } 
;

toplevels : /* empty */ 
            {$$ = NULL;}
          | netoplevels
            {$$ = $1;}
;

netoplevels : toplevel
              {$$ = $1;}  
            | toplevel netoplevels
              {$$ = $1; $$->next = $2;} 
;

toplevel : function 
           {$$ = makeTOPLEVELfunction($1);}
		 | nestms
		   {$$ = makeTOPLEVELstms($1); }
		 | event
		   {$$ = makeTOPLEVELevent($1); }
		 | external
		   {$$ = makeTOPLEVELexternal($1); }
;


decl : type identifiers initialization ';'
       {$$ = makeDECLvariable($1,$2,$3);}
;

simpledecl : tVAR tIDENTIFIER initialization
             {$$ = makeDECLsimplevar($2,$3);}
;


identifiers : tIDENTIFIER 
              {$$ = makeIDENTIFIER($1);}
            | tIDENTIFIER ',' identifiers 
              {$$ = makeIDENTIFIER($1); $$->next = $3;}
;

initialization : /* empty */ 
                 {$$ = NULL;}
               | '=' exp
                 {$$ = $2;}
;

type : tVAR
       {$$ = makeTYPElocal();}
     | tGLOBAL
       {$$ = makeTYPEglobal();}
     | tCONST
       {$$ = makeTYPEconst();}
;

ext_type : tBOOL
           {$$ = makeEXT_TYPEbool();}
         | tLONG
           {$$ = makeEXT_TYPElong();}
         | tSHORT
           {$$ = makeEXT_TYPElong();}
         | tINT
           {$$ = makeEXT_TYPElong();}
		 | tBYTE
           {$$ = makeEXT_TYPEbyte();}
         | tSTRING
           {$$ = makeEXT_TYPEstring();}
         | tFLOAT
           {$$ = makeEXT_TYPEfloat();}
         | tDOUBLE
           {$$ = makeEXT_TYPEdouble();}
         | tMEMBUFFER
           {$$ = makeEXT_TYPEmembuffer();}
;


ext_formals : /* empty */ 
              {$$ = NULL;}
            | neext_formals
              {$$ = $1;}
;

neext_formals : ext_formal 
              {$$ = $1};
              | ext_formal ',' neext_formals 
              {$$ = $1; $$->next = $3;}
;

ext_formal : ext_type
             {$$ = $1;}
		   | ext_type tIDENTIFIER
		     {$$ = $1;}
;



call_type : /* empty */ 
            {$$ = makeCALL_TYPEstdcall();}
          | tSTDCALL
            {$$ = makeCALL_TYPEstdcall();}
          | tCDECL
            {$$ = makeCALL_TYPEcdecl();}
          | tTHISCALL
            {$$ = makeCALL_TYPEthiscall();}
;

ext_returns : /* empty */ 
              {$$ = makeEXT_TYPEvoid();}
            | ext_type
              {$$ = $1;}
;


optvar : /* empty */
         | tVAR
;


external : tEXTERNAL tSTRINGCONST call_type ext_returns tIDENTIFIER '(' ext_formals ')' ';'
           {$$ = makeEXTERNAL($5, $2, $4, $7, $3);} 
;


function : tFUNCTION tIDENTIFIER '(' formals ')' compoundstm
           {$$ = makeFUNCTION($2, $4, $6, 0);} 
		 | tMETHOD tIDENTIFIER '(' formals ')' compoundstm
           {$$ = makeFUNCTION($2, $4, $6, 1);}
;

event : tON tSTRINGCONST  stm
           {$$ = makeEVENT($2, $3);} 

;


formals : /* empty */ 
            {$$ = NULL;}
        | neformals
            {$$ = $1;}
;

neformals : formal 
            {$$ = $1};
          | formal ',' neformals 
            {$$ = $1; $$->next = $3;}
;

formal : optvar tIDENTIFIER
         {$$ = makeDECLformal($2);}
;


compoundstm : '{' '}'
              {$$ = NULL;}
            | '{' nestms '}'
              {$$ = $2;}      
;

nestms : stm
         {$$ = $1;}
       | nestms stm
         {$$ = makeSTMsequence($1,$2);}
;


stm : ';'
      {$$ = makeSTMskip();}
    | tRETURN ';'
      {$$ = makeSTMreturn(NULL);}
    | tRETURN exp ';'
      {$$ = makeSTMreturn($2);}
    | tIF '(' exp ')' stm
      {$$ = makeSTMif($3,makeSTMscope($5));}
    | tIF '(' exp ')' stm tELSE stm
      {$$ = makeSTMifelse($3,makeSTMscope($5),makeSTMscope($7));}
    | tWHILE '(' exp ')' stm
      {$$ = makeSTMwhile($3,$5);}
    | tFOR '(' forinits ';' exp ';' exps ')' stm
      {$$ = makeSTMfor($3,$5,$7,makeSTMscope($9));}
    | compoundstm
      {$$ = makeSTMscope($1);}
    | decl
      {$$ = makeSTMdecl($1);}
    | exp ';'
      {$$ = makeSTMexp($1);}
	| tBREAK 
      {$$ = makeSTMbreak();}  
	| tCONTINUE 
      {$$ = makeSTMcontinue();}  
	| tSWITCH '(' exp ')' '{' dcases '}'
	  {$$ = makeSTMswitch($3, $6); }
;

forinits : /* empty */
           {$$ = NULL;}
         | neforinits
           {$$ = $1;}
;

neforinits : forinit
             {$$ = $1;}
           | forinit ',' neforinits
             {$$ = $1; $$->next = $3;}
;

forinit : simpledecl
          {$$ = makeFORINITdecl($1);}
        | exp
          {$$ = makeFORINITexp($1);}
;



exp : lvalue '=' exp
      {$$ = makeEXPassignment($1,$3);}
    | exp tEQUALS exp
      {$$ = makeEXPequals($1,$3);}
    | exp tNEQUALS exp
      {$$ = makeEXPnequals($1,$3);}
    | exp tSTRICTEQUALS exp
      {$$ = makeEXPstrictequals($1,$3);}
    | exp tSTRICTNEQUALS exp
      {$$ = makeEXPstrictnequals($1,$3);}
    | exp '<' exp
      {$$ = makeEXPless($1,$3);}
    | exp '>' exp
      {$$ = makeEXPgreater($1,$3);}
    | exp tLEQUALS exp
      {$$ = makeEXPlequals($1,$3);}
    | exp tGEQUALS exp
      {$$ = makeEXPgequals($1,$3);}
    | exp '+' exp
      {$$ = makeEXPplus($1,$3);}
    | exp '-' exp
      {$$ = makeEXPminus($1,$3);}
    | exp '*' exp
      {$$ = makeEXPmult($1,$3);}
    | exp '/' exp
      {$$ = makeEXPdiv($1,$3);}
    | exp '%' exp
      {$$ = makeEXPmodulo($1,$3);}
    | exp tAND exp
      {$$ = makeEXPand($1,$3);}
    | exp tOR exp
      {$$ = makeEXPor($1,$3);}
	| tNULL
      {$$ = makeEXPnull();}
    | unaryexp
      {$$ = $1;}
;

unaryexp : '-' unaryexp
           {$$ = makeEXPuminus($2);}
         | '!' unaryexp
           {$$ = makeEXPnot($2);}
         | tIDENTIFIER tINCREMENT
           {$$ = makeEXPincrement($1);}
         | tIDENTIFIER tDECREMENT
           {$$ = makeEXPdecrement($1);}
		 | '(' type ')' unaryexp
           {$$ = makeEXPcast($2,$4);}
         | unarypostfixexp
           {$$ = $1;}
;

unarypostfixexp : tINTCONST
                  {$$ = makeEXPintconst($1);}
                | tBOOLCONST
                  {$$ = makeEXPboolconst($1);}
                | tFLOATCONST
                  {$$ = makeEXPfloatconst($1);}
                | tSTRINGCONST
                  {
                    if(callbacks.Dll_ParseElement) callbacks.Dll_ParseElement(ExtData, lineno, ELEMENT_STRING, $1);
                    $$ = makeEXPstringconst($1);
                  }
                | lvalue
                  {$$ = makeEXPlvalue($1);}
                | '(' exp ')'
                  {$$ = $2;}
        		| tIDENTIFIER '(' exps ')'           /* Invokation */
                  {$$ = makeEXPinvoke($1,$3);}
				| tIDENTIFIER '.' tIDENTIFIER '(' exps ')'
				  {$$ = makeEXPinvokemethod($1, makeEXPstringconst($3), $5);}
				| tIDENTIFIER '[' exp ']' '(' exps ')'
                  {$$ = makeEXPinvokemethod($1, $3, $6);}
				| tNEW tIDENTIFIER
                  {$$ = makeEXPnew($2, NULL); }
				| tNEW tIDENTIFIER '(' exps ')'
                  {$$ = makeEXPnew($2, $4); }
;

exps : /* empty */ 
       {$$ = NULL;}
     | neexps
       {$$ = $1;}
;


dcases : default_branch
         {$$ = $1;}
       | case_branches
         {$$ = $1;}
       | case_branches default_branch
         {$$ = $1; /*$$->next = $2;*/ addDefaultCase($$, $2); }
;


case_branches : /* empty */
                { $$ = NULL; }
              | case_branch
                {$$ = $1;}
              | case_branch case_branches
              {$$ = $1; $$->next = $2;}
;


case_branch : tCASE unarypostfixexp ':' nestms
              { $$ = makeCASEnormal($2, $4); }
			| tCASE unarypostfixexp ':'
			{ $$ = makeCASEnormal($2, NULL); }
;

default_branch : /* empty */
               {$$ = NULL;}
               | tDEFAULT ':' nestms
	           { $$ = makeCASEdefault($3); }
;


neexps : exp 
         {$$ = $1;}
       | exp ',' neexps 
         {$$ = $1; $$->next = $3;}
;

lvalue : tIDENTIFIER
         {$$ = makeLVALUEid($1, NULL);}
       | tIDENTIFIER '.' tIDENTIFIER
         {$$ = makeLVALUEid($1, $3);}
	   | tIDENTIFIER '[' exp ']'
	     {$$ = makeLVALUEexp($1, $3);}
       /*| tTHIS '.' tIDENTIFIER
         {$$ = makeLVALUEid(NULL, $3);}*/
	   | tTHIS '[' exp ']'
	     {$$ = makeLVALUEexp(NULL, $3);}
;
