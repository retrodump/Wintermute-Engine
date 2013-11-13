#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "pretty.h"
#define TAB_SIZE 2


/* Indented print: (local to this file) */
static void indentprintf(int indent, const char *format, ...)
{
	va_list argList;
	
	/* Indent: */
	printf("%*s", indent*TAB_SIZE, "");
	
	/* Print: */
	va_start(argList, format);
	vprintf(format, argList);
	va_end(argList);
}


/* Prettyprinter for each node in the AST: */


void prettySCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection)
{
	if (scriptcollection->toplevels != NULL)
		prettyTOPLEVEL(1, scriptcollection->toplevels);
}

void prettyTOPLEVEL(int indent, TOPLEVEL *toplevel)
{
	
	switch(toplevel->kind){
	case functionK:
		prettyFUNCTION(indent, toplevel->val.functionT);
		break;
	case eventK:
		prettyEVENT(indent, toplevel->val.eventT);
		break;
	case stmsK:
		prettySTM(indent, toplevel->val.stms);
		break;
	}
	
	if(toplevel->next != NULL) {
		indentprintf(indent, "\n");
		prettyTOPLEVEL(indent, toplevel->next);
	}
}



void prettyFUNCTION(int indent, FUNCTION *function)
{
	indentprintf(indent, "");
	//prettyTYPE(function->type);
	printf("function %s(",function->name);
	if (function->formals != NULL)
		prettyDECL(indent, function->formals);
	printf(")\n");
	indentprintf(indent, "{\n");
	if (function->stms != NULL)
		prettySTM(indent+1, function->stms);
	indentprintf(indent, "}\n");
}


void prettyEVENT(int indent, EVENT *event)
{
	indentprintf(indent, "");
	//prettyTYPE(function->type);
	printf("on \"%s\"",event->name);
	indentprintf(indent, "{\n");
	if (event->stms != NULL)
		prettySTM(indent+1, event->stms);
	indentprintf(indent, "}\n");
}


void prettyTYPE(TYPE *type)
{
	switch(type->kind){
	case localK:
		printf("var ");
		break;
	case globalK:
		printf("global ");
		break;
	case constK:
		printf("const ");
		break;
	}
}

void prettyDECL(int indent, DECL *decl)
{
	switch(decl->kind){
	case formalK:
		prettyTYPE(decl->type);
		printf("%s",decl->val.formalD.name);
		if(decl->next != NULL)
			printf(", ");
		break;
	case variableK:
		indentprintf(indent, "");
		prettyTYPE(decl->type);
		prettyIDENTIFIER(decl->val.variableD.identifiers);
		if (decl->val.variableD.initialization != NULL) {
			printf(" = ");
			prettyEXP(decl->val.variableD.initialization);
		}
		printf(";\n");
		break;
	case simplevarK:
		prettyTYPE(decl->type);
		printf("%s",decl->val.simplevarD.name);
		if (decl->val.simplevarD.initialization != NULL) {
			printf(" = ");
			prettyEXP(decl->val.simplevarD.initialization);
		}
		if(decl->next != NULL)
			printf(", ");
		break;
	}
	
	if(decl->next != NULL)
		prettyDECL(indent, decl->next);
}

void prettyFORINIT(int indent, FORINIT *forinit)
{
	switch (forinit->kind) {
	case declforinitK:
		prettyDECL(indent, forinit->val.declforinitF);
		break;
	case expforinitK:
		prettyEXP(forinit->val.expforinitF);
		break;
	}
	
	if (forinit->next != NULL) {
		printf(",");
		prettyFORINIT(indent, forinit->next);
	}
}

void prettySTM(int indent, STM *stm)
{
	switch (stm->kind) {
    case skipK:
		indentprintf(indent, ";\n");
		break;
    case expK:
		indentprintf(indent, "");
		prettyEXP(stm->val.expS);
		printf(";\n");
		break;
	case declstmK:
		prettyDECL(indent, stm->val.declstmS);
		break;
    case returnK:
		indentprintf(indent, "return ");
		if (stm->val.returnS.exp != NULL)
			prettyEXP(stm->val.returnS.exp);
		printf(";\n");
		break;
    case ifK:
		indentprintf(indent, "if (");
		prettyEXP(stm->val.ifS.condition);
		printf(")");
		prettySTM(indent, stm->val.ifS.body);
		break;
    case ifelseK:
		indentprintf(indent, "if (");
		prettyEXP(stm->val.ifelseS.condition);
		printf(")");
		prettySTM(indent, stm->val.ifelseS.thenpart);
		indentprintf(indent, " else ");
		prettySTM(indent, stm->val.ifelseS.elsepart);
		break;
    case whileK:
		indentprintf(indent, "while (");
		prettyEXP(stm->val.whileS.condition);
		printf(") ");
		prettySTM(indent, stm->val.whileS.body);
		break;
    case forK:
		indentprintf(indent, "for (");
		prettyFORINIT(indent, stm->val.forS.inits);
		printf("; ");
		prettyEXP(stm->val.forS.condition);
		printf("; ");
		prettyEXP(stm->val.forS.updates);
		printf(") ");
		prettySTM(indent, stm->val.forS.body);
		break;
	case sequenceK:
		prettySTM(indent, stm->val.sequenceS.first);
		prettySTM(indent, stm->val.sequenceS.second);
		break;
	case scopeK:
		printf("\n");
		indentprintf(indent, "{ /* new scope */\n");
		prettySTM(indent+1, stm->val.scopeS.stm);
		indentprintf(indent, "}\n");
		break;
	case sleepK:
		indentprintf(indent,"sleep(");
		prettyEXP(stm->val.sleepS.time);
		printf(")"); 
		break;
	case breakK:
		indentprintf(indent,"break;\n");
		break;
	case continueK:
		indentprintf(indent,"continue;\n");
		break;
	}
}

void prettyIDENTIFIER(IDENTIFIER *identifier)
{
	printf("%s", identifier->name);
	
	if (identifier->next != NULL) {
		printf(",");
		prettyIDENTIFIER(identifier->next);
	}
}

void prettyEXP(EXP *exp)
{ 
	if (exp == NULL)
		return;
	
	switch(exp->kind) {
	case intconstK:
		printf("%i",exp->val.intconstE);   
		break;
	case boolconstK:
		if (exp->val.boolconstE == 1)
			printf("true");
		else
			printf("false");	   
		break;
	case floatconstK:
		printf("%f",exp->val.floatconstE);   
		break;
	case stringconstK:
		printf("\"%s\"", exp->val.stringconstE);   
		break;
	case uminusK:
		printf("-");
		prettyEXP(exp->val.uminusE);	
		break;
	case decrementK:
		printf("%s--", exp->val.decrementE.name);
		break;
	case incrementK:
		printf("%s++", exp->val.incrementE.name);
		break;
	case notK:
		printf("!");
		prettyEXP(exp->val.notE.exp);	 
		break;
	case lvalueK:
		prettyLVALUE(exp->val.lvalueE);
		break;
	case assignmentK:
		prettyLVALUE(exp->val.assignmentE.left);
		printf("=");
		prettyEXP(exp->val.assignmentE.right);	 
		break;
	case equalsK:
		printf("(");
		prettyEXP(exp->val.equalsE.left);
		printf("==");
		prettyEXP(exp->val.equalsE.right);
		printf(")");	
		break;
	case nequalsK:
		printf("(");
		prettyEXP(exp->val.nequalsE.left);
		printf("!=");
		prettyEXP(exp->val.nequalsE.right);
		printf(")");		
		break;
	case strictequalsK:
		printf("(");
		prettyEXP(exp->val.strictequalsE.left);
		printf("===");
		prettyEXP(exp->val.strictequalsE.right);
		printf(")");	
		break;
	case strictnequalsK:
		printf("(");
		prettyEXP(exp->val.strictnequalsE.left);
		printf("!==");
		prettyEXP(exp->val.strictnequalsE.right);
		printf(")");		
		break;
		
	case lessK:
		printf("(");
		prettyEXP(exp->val.lessE.left);
		printf("<");
		prettyEXP(exp->val.lessE.right);
		printf(")");			
		break;
	case greaterK:
		printf("(");
		prettyEXP(exp->val.greaterE.left);
		printf(">");
		prettyEXP(exp->val.greaterE.right);
		printf(")");		 
		break;
	case lequalsK:
		printf("(");
		prettyEXP(exp->val.lequalsE.left);
		printf("<=");
		prettyEXP(exp->val.lequalsE.right);
		printf(")");			 
		break;
	case gequalsK:
		printf("(");
		prettyEXP(exp->val.gequalsE.left);
		printf(">=");
		prettyEXP(exp->val.gequalsE.right);
		printf(")");			
		break;
	case plusK:
		printf("(");
		prettyEXP(exp->val.plusE.left);
		printf("+");
		prettyEXP(exp->val.plusE.right);
		printf(")");		 
		break;
	case minusK:
		printf("(");
		prettyEXP(exp->val.minusE.left);
		printf("-");
		prettyEXP(exp->val.minusE.right);
		printf(")");	   
		break;
	case multK:
		printf("(");
		prettyEXP(exp->val.multE.left);
		printf("*");
		prettyEXP(exp->val.multE.right);
		printf(")");	 
		break;
	case divK:
		printf("(");
		prettyEXP(exp->val.divE.left);
		printf("/");
		prettyEXP(exp->val.divE.right);
		printf(")");		 
		break;
	case moduloK:
		printf("(");
		prettyEXP(exp->val.moduloE.left);
		printf("%%");
		prettyEXP(exp->val.moduloE.right);
		printf(")");		
		break;
	case andK:
		printf("(");
		prettyEXP(exp->val.andE.left);
		printf("&&");
		prettyEXP(exp->val.andE.right);
		printf(")");	   
		break;
	case orK:
		printf("(");
		prettyEXP(exp->val.orE.left);
		printf("||");
		prettyEXP(exp->val.orE.right);
		printf(")");		 
		break;
	case invokeK:
		printf("%s", exp->val.invokeE.name);
		printf("(");
		if (exp->val.invokeE.arguments != NULL)
			prettyEXP(exp->val.invokeE.arguments);
		printf(")");	
		break;
	case newK:
		printf("new %s", exp->val.newE.name);
		printf("(");
		if (exp->val.newE.arguments != NULL)
			prettyEXP(exp->val.newE.arguments);
		printf(")");	
		break;
	case invokemethodK:
		printf("%s", exp->val.invokemethodE.name);
		printf("[");
		prettyEXP(exp->val.invokemethodE.method);
		printf("](");
		if (exp->val.invokemethodE.arguments != NULL)
			prettyEXP(exp->val.invokemethodE.arguments);
		printf(")");	
		break;
	case castK:
		printf("(");
		prettyTYPE(exp->val.castE.type);
		printf(")");
		prettyEXP(exp->val.castE.exp);	  
		break;
	case nullK:
		printf("null");
		break;
		
  }
  
  if(exp->next != NULL) {
	  printf(",");	
	  prettyEXP(exp->next);  
  }  
}



void prettyLVALUE(LVALUE *lvalue)
{
	if(lvalue->id!=NULL) printf(lvalue->id);
	else printf("this");
	if(lvalue->member!=NULL){
		printf("[");
		prettyEXP(lvalue->member);  
		printf("]");
		
	}
}
