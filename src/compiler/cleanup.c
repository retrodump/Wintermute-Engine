#include <malloc.h>
#include "symbol.h"
#include "cleanup.h"


void cleanupSCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection)
{
	if(!scriptcollection) return;
	if (scriptcollection->toplevels != NULL)
		cleanupTOPLEVEL(scriptcollection->toplevels);

	cleanupSymbolTable(scriptcollection->sym);

	free(scriptcollection);
	scriptcollection = NULL;
}

void cleanupTOPLEVEL(TOPLEVEL *toplevel)
{
	
	switch(toplevel->kind){
	case functionK:
		cleanupFUNCTION(toplevel->val.functionT);
		break;
	case eventK:
		cleanupEVENT(toplevel->val.eventT);
		break;
	case externalK:
		cleanupEXTERNAL(toplevel->val.externalT);
		break;

	case stmsK:
		cleanupSTM(toplevel->val.stms);
		break;
	}
	
	if(toplevel->next != NULL) {
		cleanupTOPLEVEL(toplevel->next);
	}
	free(toplevel);
	toplevel = NULL;
}



void cleanupFUNCTION(FUNCTION *function)
{
	free(function->name);
	if (function->formals != NULL)
		cleanupDECL(function->formals);
	if (function->stms != NULL)
		cleanupSTM(function->stms);
	cleanupSymbolTable(function->sym);
	free(function);
	function = NULL;
}


void cleanupEVENT(EVENT* event)
{
	free(event->name);
	if (event->stms != NULL)
		cleanupSTM(event->stms);
	cleanupSymbolTable(event->sym);
	free(event);
	event = NULL;
}


void cleanupEXTERNAL(EXTERNAL* external)
{
	free(external->name);
	free(external->library);

	if(external->call) free(external->call);
	if(external->formals) cleanupEXT_TYPE(external->formals);
	if(external->returns) cleanupEXT_TYPE(external->returns);

	cleanupSymbolTable(external->sym);
	free(external);
	external = NULL;
}


void cleanupEXT_TYPE(EXT_TYPE *ext_type)
{ 
	if (ext_type == NULL)
		return;

	if(ext_type->next) cleanupEXT_TYPE(ext_type->next);

	free(ext_type);
	ext_type = NULL;
}


void cleanupTYPE(TYPE *type)
{
	free(type);
	type = NULL;
}

void cleanupDECL(DECL *decl)
{
	switch(decl->kind){
	case formalK:
		free(decl->val.formalD.name);
		cleanupTYPE(decl->type);
		break;
	case variableK:
		//cleanupTYPE(decl->type);
		cleanupIDENTIFIER(decl->val.variableD.identifiers);
		if (decl->val.variableD.initialization != NULL) {
			cleanupEXP(decl->val.variableD.initialization);
		}
		break;
	case simplevarK:
		cleanupTYPE(decl->type);
		free(decl->val.simplevarD.name);
		if (decl->val.simplevarD.initialization != NULL) {
			cleanupEXP(decl->val.simplevarD.initialization);
		}
		break;
	}
	
	if(decl->next != NULL)
		cleanupDECL(decl->next);
	free(decl);
	decl = NULL;
}

void cleanupFORINIT(FORINIT *forinit)
{
	switch (forinit->kind) {
	case declforinitK:
		cleanupDECL(forinit->val.declforinitF);
		break;
	case expforinitK:
		cleanupEXP(forinit->val.expforinitF);
		break;
	}
	
	if (forinit->next != NULL) {
		cleanupFORINIT(forinit->next);
	}
	free(forinit);
	forinit = NULL;
}


void cleanupCASE(CASE *c)
{
	switch(c->kind){
	case normalcaseK:
		cleanupEXP(c->val.normalcaseD.label);
		if(c->val.normalcaseD.stms) cleanupSTM(c->val.normalcaseD.stms);
		break;
	case defaultcaseK:
		if(c->val.defaultcaseD.stms) cleanupSTM(c->val.defaultcaseD.stms);
		break;
	}
	
	if(c->next != NULL)
		cleanupCASE(c->next);

	free(c);
	c = NULL;
}



void cleanupSTM(STM *stm)
{
	if(!stm) return;
	
	switch (stm->kind) {
    case skipK:
		break;
    case expK:
		cleanupEXP(stm->val.expS);
		break;
	case declstmK:
		cleanupDECL(stm->val.declstmS);
		break;
    case returnK:
		if (stm->val.returnS.exp != NULL)
			cleanupEXP(stm->val.returnS.exp);
		break;
    case ifK:
		cleanupEXP(stm->val.ifS.condition);
		cleanupSTM(stm->val.ifS.body);
		break;
    case ifelseK:
		cleanupEXP(stm->val.ifelseS.condition);
		cleanupSTM(stm->val.ifelseS.thenpart);
		cleanupSTM(stm->val.ifelseS.elsepart);
		break;
    case whileK:
		cleanupEXP(stm->val.whileS.condition);
		cleanupSTM(stm->val.whileS.body);
		break;
    case forK:
		cleanupFORINIT(stm->val.forS.inits);
		cleanupEXP(stm->val.forS.condition);
		cleanupEXP(stm->val.forS.updates);
		cleanupSTM(stm->val.forS.body);
		break;
	case sequenceK:
		cleanupSTM(stm->val.sequenceS.first);
		cleanupSTM(stm->val.sequenceS.second);
		break;
	case scopeK:
		cleanupSTM(stm->val.scopeS.stm);
		break;
	case sleepK:
		cleanupEXP(stm->val.sleepS.time);
		break;
	case breakK:
		break;
	case continueK:
		break;
	case switchK:
		cleanupEXP(stm->val.switchS.condition);
		if(stm->val.switchS.cases) cleanupCASE(stm->val.switchS.cases);
		break;
	
	}
	free(stm);
	stm = NULL;
}

void cleanupIDENTIFIER(IDENTIFIER *identifier)
{
	free(identifier->name);
	if (identifier->next != NULL) {
		cleanupIDENTIFIER(identifier->next);
	}
	free(identifier);
	identifier = NULL;
}

void cleanupEXP(EXP *exp)
{ 
	if (exp == NULL)
		return;
	
	switch(exp->kind) {
	case intconstK:
		break;
	case boolconstK:
		break;
	case floatconstK:
		break;
	case stringconstK:
		free(exp->val.stringconstE);
		break;
	case uminusK:
		cleanupEXP(exp->val.uminusE);	
		break;
	case decrementK:
		free(exp->val.decrementE.name);
		break;
	case incrementK:
		free(exp->val.incrementE.name);
		break;
	case notK:
		cleanupEXP(exp->val.notE.exp);	 
		break;
	case lvalueK:
		cleanupLVALUE(exp->val.lvalueE);
		break;
	case assignmentK:
		cleanupLVALUE(exp->val.assignmentE.left);
		cleanupEXP(exp->val.assignmentE.right);	 
		break;
	case equalsK:
		cleanupEXP(exp->val.equalsE.left);
		cleanupEXP(exp->val.equalsE.right);
		break;
	case nequalsK:
		cleanupEXP(exp->val.nequalsE.left);
		cleanupEXP(exp->val.nequalsE.right);
		break;
	case strictequalsK:
		cleanupEXP(exp->val.strictequalsE.left);
		cleanupEXP(exp->val.strictequalsE.right);
		break;
	case strictnequalsK:
		cleanupEXP(exp->val.strictnequalsE.left);
		cleanupEXP(exp->val.strictnequalsE.right);
		break;
		
	case lessK:
		cleanupEXP(exp->val.lessE.left);
		cleanupEXP(exp->val.lessE.right);
		break;
	case greaterK:
		cleanupEXP(exp->val.greaterE.left);
		cleanupEXP(exp->val.greaterE.right);
		break;
	case lequalsK:
		cleanupEXP(exp->val.lequalsE.left);
		cleanupEXP(exp->val.lequalsE.right);
		break;
	case gequalsK:
		cleanupEXP(exp->val.gequalsE.left);
		cleanupEXP(exp->val.gequalsE.right);
		break;
	case plusK:
		cleanupEXP(exp->val.plusE.left);
		cleanupEXP(exp->val.plusE.right);
		break;
	case minusK:
		cleanupEXP(exp->val.minusE.left);
		cleanupEXP(exp->val.minusE.right);
		break;
	case multK:
		cleanupEXP(exp->val.multE.left);
		cleanupEXP(exp->val.multE.right);
		break;
	case divK:
		cleanupEXP(exp->val.divE.left);
		cleanupEXP(exp->val.divE.right);
		break;
	case moduloK:
		cleanupEXP(exp->val.moduloE.left);
		cleanupEXP(exp->val.moduloE.right);
		break;
	case andK:
		cleanupEXP(exp->val.andE.left);
		cleanupEXP(exp->val.andE.right);
		break;
	case orK:
		cleanupEXP(exp->val.orE.left);
		cleanupEXP(exp->val.orE.right);
		break;
	case invokeK:
		free(exp->val.invokeE.name);
		if (exp->val.invokeE.arguments != NULL)
			cleanupEXP(exp->val.invokeE.arguments);
		break;
	case newK:
		free(exp->val.newE.name);
		if (exp->val.newE.arguments != NULL)
			cleanupEXP(exp->val.newE.arguments);
		break;
	case invokemethodK:
		free(exp->val.invokemethodE.name);
		cleanupEXP(exp->val.invokemethodE.method);
		if (exp->val.invokemethodE.arguments != NULL)
			cleanupEXP(exp->val.invokemethodE.arguments);
		break;
	case castK:
		cleanupTYPE(exp->val.castE.type);
		cleanupEXP(exp->val.castE.exp);	  
		break;
	case nullK:
		break;		
  }
  
  if(exp->next != NULL) {
	  cleanupEXP(exp->next);  
  }
  free(exp);
  exp = NULL;
}



void cleanupLVALUE(LVALUE *lvalue)
{
	if(lvalue->member!=NULL){
		cleanupEXP(lvalue->member);
	}
}


void cleanupSymbolTable(SymbolTable* st){

	SYMBOL* s;
	SYMBOL* del;
	int i;

	if(!st) return;
	
	for(i=0; i<HashSize; i++){
		s = st->table[i];
		while(s){
			del = s;
			s = s->next;
			free(del);
		}
	}
	free(st);
	st = NULL;
}
