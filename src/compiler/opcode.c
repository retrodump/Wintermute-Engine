/*
 *  opcode.c
 */


/*
DWORD magic
DWORD version
DWORD code_start
DWORD function_table_start
DWORD string_table_start
DWORD event_table_start
0x0101: DWORD dll_table_start
0x0102: DWORD method_table_start
char[?] source_name

  code
  function table (num,  pos, name...)
  string table (num,  id, name...)
  event table (num,  id, name...)
  0x0101: dll table (num,  dll, name, call, returns, num_params, param1, param2...)
  0x0102: method table (num,  pos, name...)
  	
*/


/* --** Includes: **--------------------------------------------------- */
#include <malloc.h>
#include <string.h>
#include "tree.h"
#include "memory.h"
#include "opcode.h"
#include "symbol.h"
#include "../engine_core/wme_base/dcscript.h"


#define BUFFER_INIT_SIZE 32768
#define BUFFER_GROW_BY 16384


TFunctionPos* FuncReq=NULL;
TFunctionPos* FuncPos=NULL;
TFunctionPos* EventPos=NULL;
TDllEntry* DllTable=NULL;

TLoopPos* LoopPos=NULL;
TLoopPos* LoopReq=NULL;
TSymbol* Symbols=NULL;

BYTE* buffer;
int buffer_size;
int curr_ip;
int curr_line;
int loop_nest;
int loop_level;

//////////////////////////////////////////////////////////////////////////
DWORD AddSymbol(char* Symbol){
	TSymbol* s = Symbols;
	TSymbol* last = NULL;
	DWORD id=0;
	while(s!=NULL){
		if(strcmp(s->name, Symbol)==0) return id;
		last = s;
		s = (TSymbol*)s->next;
		id++;
	}
	s = NEW(TSymbol);
	s->name = malloc(strlen(Symbol)+1);
	strcpy(s->name, Symbol);
	s->next = NULL;

	if(last==NULL) Symbols = s;
	else (TSymbol*)last->next = s;

	return id;
}

//////////////////////////////////////////////////////////////////////////
void AddLoopPos(int loop_nest, int loop_level, DWORD start, DWORD end){
	TLoopPos* loop;
	loop = NEW(TLoopPos);
	loop->loop_nest = loop_nest;
	loop->loop_level = loop_level;
	loop->start = start;
	loop->end = end;
	
	loop->next = (struct TLoopPos*)LoopPos;
	LoopPos = loop;
}


void AddLoopReq(int is_end, int loop_nest, int loop_level, DWORD pos){
	TLoopPos* loop;
	loop = NEW(TLoopPos);
	loop->loop_nest = loop_nest;
	loop->loop_level = loop_level;
	loop->is_end = is_end;
	loop->start = pos;
	
	loop->next = (struct TLoopPos*)LoopReq;
	LoopReq = loop;
}


//////////////////////////////////////////////////////////////////////////
void AddFuncReq(DWORD in_pos, char* in_name){
	TFunctionPos* req;
	req = NEW(TFunctionPos);
	req->pos = in_pos;
	req->name = malloc(strlen(in_name)+1);
	strcpy(req->name, in_name);
	
	req->next = (struct TFunctionPos*)FuncReq;
	FuncReq = req;
}


//////////////////////////////////////////////////////////////////////////
void AddFuncPos(DWORD in_pos, char* in_name, int method){
	TFunctionPos* pos;
	pos = NEW(TFunctionPos);
	pos->pos = in_pos;
	pos->name = malloc(strlen(in_name)+1);
	pos->method = method;
	strcpy(pos->name, in_name);
	
	pos->next = (struct TFunctionPos*)FuncPos;
	FuncPos = pos;
}


//////////////////////////////////////////////////////////////////////////
void AddEventPos(DWORD in_pos, char* in_name){
	TFunctionPos* pos;
	pos = NEW(TFunctionPos);
	pos->pos = in_pos;
	pos->name = malloc(strlen(in_name)+1);
	strcpy(pos->name, in_name);
	
	pos->next = (struct TFunctionPos*)EventPos;
	EventPos = pos;
}


//////////////////////////////////////////////////////////////////////////
void AddDllEntry(char* DllName, char* Name, TCallType CallType, TExternalType Returns, int NumParams, TExternalType* Params){
	TDllEntry* entry;
	entry = NEW(TDllEntry);
	
	entry->DllName = malloc(strlen(DllName)+1);
	strcpy(entry->DllName, DllName);
	
	entry->Name = malloc(strlen(Name)+1);
	strcpy(entry->Name, Name);

	entry->CallType = CallType;
	entry->Returns = Returns;
	entry->NumParams = NumParams;
	entry->Params = Params;

	
	entry->next = (struct TDllEntry*)DllTable;
	DllTable = entry;
}


//////////////////////////////////////////////////////////////////////////
void ResolveFuncCalls(){
	TFunctionPos* req = FuncReq;
	TFunctionPos* pos;
	while(req!=NULL){
		pos = FuncPos;
		while(pos!=NULL){
			if(strcmp(req->name, pos->name)==0){
				PutDWORDAt(req->pos, pos->pos);
				break;
			}
			pos = (TFunctionPos*)pos->next;
		}
		req = (TFunctionPos*)req->next;
	}
}


//////////////////////////////////////////////////////////////////////////
void ResolveLoops(){
	TLoopPos* req = LoopReq;
	TLoopPos* pos;
	while(req!=NULL){
		pos = LoopPos;
		while(pos!=NULL){
			if(req->loop_nest==pos->loop_nest && req->loop_level==pos->loop_level){
				PutDWORDAt(req->start, req->is_end?pos->end:pos->start);
				break;
			}
			pos = (TLoopPos*)pos->next;
		}
		req = (TLoopPos*)req->next;
	}
}


//////////////////////////////////////////////////////////////////////////
void PutDWORD(DWORD val){
	if((int)(curr_ip + sizeof(DWORD)) > buffer_size){
		buffer_size+=BUFFER_GROW_BY;
		buffer = realloc(buffer, buffer_size);
	}
	
	*(DWORD*)(buffer+curr_ip) = val;
	curr_ip += sizeof(DWORD);
}


//////////////////////////////////////////////////////////////////////////
void PutFLOAT(double val){
	if((int)(curr_ip + sizeof(double)) > buffer_size){
		buffer_size+=BUFFER_GROW_BY;
		buffer = realloc(buffer, buffer_size);
	}
	
	*(double*)(buffer+curr_ip) = val;
	curr_ip += sizeof(double);
}


//////////////////////////////////////////////////////////////////////////
void PutInst(int lineno, DWORD val){
	if(curr_line!=lineno && lineno > 0){
		curr_line = lineno;
		PutDWORD(II_DBG_LINE);
		PutDWORD(curr_line);
	}
	PutDWORD(val);
}

//////////////////////////////////////////////////////////////////////////
void PutDWORDAt(DWORD pos, DWORD val){
	*(DWORD*)(buffer+pos) = val;
}


//////////////////////////////////////////////////////////////////////////
void PutString(char* val){
	while((int)(curr_ip + strlen(val)+1) > buffer_size){
		buffer_size+=BUFFER_GROW_BY;
		buffer = realloc(buffer, buffer_size);
	}
	
	memcpy((BYTE*)(buffer+curr_ip), val, strlen(val)+1);
	curr_ip += (strlen(val)+1);
}


//////////////////////////////////////////////////////////////////////////
void OpcodeCleanup(){

	TFunctionPos* fp;
	TLoopPos* lp;
	TSymbol* s;
	TDllEntry* de;


	while(FuncReq!=NULL){
		fp = FuncReq;
		FuncReq = (TFunctionPos*)FuncReq->next;
		free(fp->name);
		free(fp);
	}
	while(FuncPos!=NULL){
		fp = FuncPos;
		FuncPos = (TFunctionPos*)FuncPos->next;
		free(fp->name);
		free(fp);
	}
	while(EventPos!=NULL){
		fp = EventPos;
		EventPos = (TFunctionPos*)EventPos->next;
		free(fp->name);
		free(fp);
	}
	while(DllTable!=NULL){
		de = DllTable;
		DllTable = (TDllEntry*)DllTable->next;
		free(de->Name);
		free(de->DllName);
		free(de->Params);
		free(de);
	}

	while(LoopReq!=NULL){
		lp = LoopReq;
		LoopReq = (TLoopPos*)LoopReq->next;
		free(lp);
	}
	while(LoopPos!=NULL){
		lp = LoopPos;
		LoopPos = (TLoopPos*)LoopPos->next;
		free(lp);
	}
	while(Symbols!=NULL){
		s = Symbols;
		Symbols = (TSymbol*)Symbols->next;
		free(s->name);
		free(s);
	}


	FuncReq=NULL;
	FuncPos=NULL;
	EventPos=NULL;
	DllTable=NULL;

	LoopPos=NULL;
	LoopReq=NULL;
	Symbols=NULL;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
BYTE* opcodeSCRIPTCOLLECTION(SCRIPTCOLLECTION *scriptcollection, char* Source, DWORD* CompiledSize)
{
	TFunctionPos* pos;
	TSymbol* s;
	DWORD num_func, num_symbols, num_events, id, num_dll_entries;
	TDllEntry* de;
	int i;
	
	buffer_size = BUFFER_INIT_SIZE;
	buffer = malloc(buffer_size);
	curr_ip = 0;
	
	
	PutDWORD(SCRIPT_MAGIC);
	PutDWORD(SCRIPT_VERSION);
	PutDWORD(0); // code start
	PutDWORD(0); // functions start
	PutDWORD(0); // strings start
	PutDWORD(0); // events start
	PutDWORD(0); // dll table start
	PutDWORD(0); // method table start
	PutString(Source);
	
	
	loop_nest = 0;
	loop_level = 0;
	curr_line = -1;
	
	// code start
	PutDWORDAt(2*sizeof(DWORD), curr_ip);
	
	//printf("ADD SCOPE\n");
	//PutInst(-1, II_SCOPE);
	
	if (scriptcollection->toplevels != NULL){
		opcodeTOPLEVELstms(scriptcollection->toplevels);
		opcodeTOPLEVELfuncs(scriptcollection->toplevels);
		opcodeTOPLEVELevents(scriptcollection->toplevels);
		opcodeTOPLEVELexternals(scriptcollection->toplevels);
	}
    //printf("RET\n\n");
	PutInst(scriptcollection->lineno, II_RET);
	
	ResolveFuncCalls();
	ResolveLoops();
	
	// write address of function table
	PutDWORDAt(3*sizeof(DWORD), curr_ip);
	
	// write function table
	num_func = 0;
	pos = FuncPos;
	while(pos!=NULL){
		if(!pos->method) num_func++;
		pos = (TFunctionPos*)pos->next;
	}
	PutDWORD(num_func);
	
	pos = FuncPos;
	while(pos!=NULL){
		if(!pos->method){
			PutDWORD(pos->pos);
			PutString(pos->name);
		}		
		pos = (TFunctionPos*)pos->next;
	}
	
	
	// write address of string table
	PutDWORDAt(4*sizeof(DWORD), curr_ip);
	
	// write string table
	num_symbols = 0;
	s = Symbols;
	while(s!=NULL){
		num_symbols++;
		s = (TSymbol*)s->next;
	}
	PutDWORD(num_symbols);
	
	id = 0;
	s = Symbols;
	while(s!=NULL){
		PutDWORD(id);
		PutString(s->name);
		
		s = (TSymbol*)s->next;
		id++;
	}
	
	// write address of events table
	PutDWORDAt(5*sizeof(DWORD), curr_ip);
	
	// write event table
	num_events = 0;
	pos = EventPos;
	while(pos!=NULL){
		num_events++;
		pos = (TFunctionPos*)pos->next;
	}
	PutDWORD(num_events);
	
	pos = EventPos;
	while(pos!=NULL){
		PutDWORD(pos->pos);
		PutString(pos->name);
		
		pos = (TFunctionPos*)pos->next;
	}


	// write address of dll table
	PutDWORDAt(6*sizeof(DWORD), curr_ip);
	
	// write dll table
	num_dll_entries = 0;
	de = DllTable;
	while(de!=NULL){
		num_dll_entries++;
		de = (TDllEntry*)de->next;
	}
	PutDWORD(num_dll_entries);
	
	de = DllTable;
	while(de!=NULL){
		PutString(de->DllName);
		PutString(de->Name);
		PutDWORD(de->CallType);
		PutDWORD(de->Returns);
		PutDWORD(de->NumParams);
		for(i=0; i<de->NumParams; i++) PutDWORD(de->Params[i]);

		de = (TDllEntry*)de->next;
	}


	// write address of method table
	PutDWORDAt(7*sizeof(DWORD), curr_ip);
	
	// write method table
	num_func = 0;
	pos = FuncPos;
	while(pos!=NULL){
		if(pos->method)	num_func++;
		pos = (TFunctionPos*)pos->next;
	}
	PutDWORD(num_func);
	
	pos = FuncPos;
	while(pos!=NULL){
		if(pos->method){
			PutDWORD(pos->pos);
			PutString(pos->name);
		}
		
		pos = (TFunctionPos*)pos->next;
	}


	OpcodeCleanup();

	if(CompiledSize) *CompiledSize = curr_ip;
	return buffer;	


	//printf("Compiler version: %d.%d\n", SCRIPT_VERSION/256, SCRIPT_VERSION%256);
}


void opcodeTOPLEVELstms(TOPLEVEL *toplevel)
{
	if(toplevel->kind == stmsK)	opcodeSTM(toplevel->val.stms, 0, 0);
	if(toplevel->next != NULL)	opcodeTOPLEVELstms(toplevel->next);
}

void opcodeTOPLEVELfuncs(TOPLEVEL *toplevel)
{
	if(toplevel->kind == functionK)	opcodeFUNCTION(toplevel->val.functionT);
	if(toplevel->next != NULL)	opcodeTOPLEVELfuncs(toplevel->next);
}


void opcodeTOPLEVELevents(TOPLEVEL *toplevel)
{
	if(toplevel->kind == eventK)	opcodeEVENT(toplevel->val.eventT);
	if(toplevel->next != NULL)	opcodeTOPLEVELevents(toplevel->next);
}


void opcodeTOPLEVELexternals(TOPLEVEL *toplevel)
{
	if(toplevel->kind == externalK)	opcodeEXTERNAL(toplevel->val.externalT);
	if(toplevel->next != NULL)	opcodeTOPLEVELexternals(toplevel->next);
}


void opcodeSTM(struct STM* stm, int func, int event)
{
	DWORD label1, label2, label3;
	CASE* c;
	int curr_loop;

	if(!stm)return;

	switch (stm->kind) {
    case skipK:
		break;
    case expK:
		opcodeEXP(stm->val.expS, FALSE);
		break;
		
    case declstmK:
		opcodeDECL(stm->val.declstmS);
		break;
    case returnK:
		if(event){
			PutInst(stm->lineno, II_RET_EVENT);
		}
		else{
			if (stm->val.returnS.exp != NULL)
				opcodeEXP(stm->val.returnS.exp, TRUE);
			else PutInst(stm->lineno, II_PUSH_NULL);
			//printf("RET\n\n");
			//PutDWORD(II_RET);
			PutInst(stm->lineno, II_RET);
		}
		break;
		
    case ifK:
		opcodeEXP(stm->val.ifS.condition, TRUE);
		//printf("JMP FALSE  LABEL-END IF\n");
		PutInst(stm->lineno, II_JMP_FALSE);
		label1 = curr_ip;
		PutDWORD(0);
		
		opcodeSTM(stm->val.ifS.body, func, event);
		
		//printf("LABEL-END IF\n");
		PutDWORDAt(label1, curr_ip);
		break;
		
    case ifelseK:
		opcodeEXP(stm->val.ifelseS.condition, TRUE);
		//printf("JMP FALSE  LABEL ELSE\n");
		PutInst(stm->lineno, II_JMP_FALSE);
		label1 = curr_ip;
		PutDWORD(0);
		
		opcodeSTM(stm->val.ifelseS.thenpart, func, event);
		//printf("JMP LABEL-END IF\n");
		PutInst(stm->lineno, II_JMP);
		label2 = curr_ip;
		PutDWORD(0);
		
		//printf("LABEL ELSE\n");
		PutDWORDAt(label1, curr_ip);
		
		opcodeSTM(stm->val.ifelseS.elsepart, func, event);
		
		//printf("LABEL-END IF\n");
		PutDWORDAt(label2, curr_ip);
		break;
		
    case whileK:
		loop_nest++;
		loop_level++;
		curr_loop = loop_nest;
		
		//printf("LABEL WHILE\n");
		label1 = curr_ip;
		
		opcodeEXP(stm->val.whileS.condition, TRUE);
		
		//printf("JMP FALSE  LABEL-END WHILE\n");
		PutInst(-1, II_JMP_FALSE);
		label2 = curr_ip;
		PutDWORD(0);
		
		opcodeSTM(stm->val.whileS.body, func, event);
		
		//printf("JMP LABEL WHILE\n");
		PutInst(stm->lineno, II_JMP);
		PutDWORD(label1);
		
		//printf("LABEL-END WHILE\n");
		PutDWORDAt(label2, curr_ip);
		
		AddLoopPos(curr_loop, loop_level, label1, curr_ip);
		loop_level--;
		break;

	case switchK:		
		loop_nest++;
		loop_level++;
		curr_loop = loop_nest;
		label3 = curr_ip;
		
		opcodeEXP(stm->val.switchS.condition, TRUE);
		PutInst(stm->lineno, II_POP_REG1);
		c = stm->val.switchS.cases;
		label2 = 0;
		label1 = 0;
		while(c){
			if(c->kind == normalcaseK){
				opcodeEXP(c->val.normalcaseD.label, TRUE);
				PutInst(c->lineno, II_PUSH_REG1);
				PutInst(c->lineno, II_CMP_EQ);
				PutInst(c->lineno, II_JMP_FALSE);
				
				label1 = curr_ip;
				PutDWORD(0); // jump to next condition
				
				if(label2>0){
					PutDWORDAt(label2, curr_ip);
					label2 = 0;
				}

				opcodeSTM(c->val.normalcaseD.stms, func, event);
				
				PutInst(c->lineno, II_JMP);
				label2 = curr_ip;
				PutDWORD(0); // jump to next branch code

				PutDWORDAt(label1, curr_ip);
			}
			else if(c->kind == defaultcaseK){
				if(label2>0){
					PutDWORDAt(label2, curr_ip);
					label2 = 0;
				}
				opcodeSTM(c->val.defaultcaseD.stms, func, event);
			}
			c = c->next;
		}
		AddLoopPos(curr_loop, loop_level, label3, curr_ip);

		if(label2>0) PutDWORDAt(label2, curr_ip);

		loop_level--;

		break;
		
    case forK:
		loop_nest++;
		loop_level++;
		curr_loop = loop_nest;
		
		opcodeFORINIT(stm->val.forS.inits);
		//printf("LABEL FOR\n");
		label1 = curr_ip;
		
		opcodeEXP(stm->val.forS.condition, TRUE);
		
		//printf("JMP FALSE LABEL-END FOR\n");
		PutInst(-1, II_JMP_FALSE);
		label2 = curr_ip;
		PutDWORD(0);
		
		opcodeSTM(stm->val.forS.body, func, event);

		label3 = curr_ip;
		opcodeEXP(stm->val.forS.updates, TRUE);

		//printf("JMP FOR\n");
		PutInst(stm->lineno, II_JMP);
		PutDWORD(label1);
		
		//printf("LABEL-END FOR\n");
		PutDWORDAt(label2, curr_ip);
		
		AddLoopPos(curr_loop, loop_level, label3, curr_ip);
		loop_level--;
		
		break;
		
	case sequenceK:
		opcodeSTM(stm->val.sequenceS.first, func, event);
		opcodeSTM(stm->val.sequenceS.second, func, event);
		break;
		
	case scopeK:
		opcodeSTM(stm->val.scopeS.stm, func, event);
		break;
		/*
		case sleepK:
		indentprintf(indent,"sleep(");
		prettyEXP(stm->val.sleepS.time);
		printf(")"); 
		break;
		*/
	case breakK:
		//printf("JMP LOOP END\n");
		PutInst(stm->lineno, II_JMP);
		AddLoopReq(1, loop_nest, loop_level, curr_ip);
		
		PutDWORD(0);
		
		break;
		
	case continueK:
		//printf("JMP LOOP START\n");
		PutInst(stm->lineno, II_JMP);
		AddLoopReq(0, loop_nest, loop_level, curr_ip);
		PutDWORD(0);
		
		break;
  }
}


void opcodeEXP(EXP *exp, BOOL compound)
{ 
	int num_params;
	EXP* param;
	
	if (exp == NULL)
		return;
	
	switch(exp->kind) {
	case intconstK:
		//printf("PUSH-INT %i\n",exp->val.intconstE);
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(exp->val.intconstE);
		break;
		
	case boolconstK:
		//printf("PUSH-BOOL %s\n", exp->val.boolconstE == 1?"true":"false");
		PutInst(exp->lineno, II_PUSH_BOOL);
		PutDWORD(exp->val.boolconstE==1?1:0);
		break;

	case floatconstK:
		//printf("PUSH-FLOAT %i\n",exp->val.intconstE);
		PutInst(exp->lineno, II_PUSH_FLOAT);
		PutFLOAT(exp->val.floatconstE);
		break;
	
	case stringconstK:
		//printf("PUSH-STRING \"%s\"\n", exp->val.stringconstE);
		PutInst(exp->lineno, II_PUSH_STRING);
		PutString(exp->val.stringconstE);
		break;
		
	case uminusK:
		//printf("PUSH-INT 0\n");
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(0);
		
		opcodeEXP(exp->val.uminusE, TRUE);
		
		//printf("SUB\n");
		PutInst(exp->lineno, II_SUB);
		break;

	case decrementK:
		PutInst(exp->lineno, II_PUSH_VAR);
		PutDWORD(AddSymbol(exp->val.decrementE.name));
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(1);
		PutInst(exp->lineno, II_SUB);
		PutInst(exp->lineno, II_POP_VAR);
		PutDWORD(AddSymbol(exp->val.decrementE.name));
		break;
		
	case incrementK:
		PutInst(exp->lineno, II_PUSH_VAR);
		PutDWORD(AddSymbol(exp->val.incrementE.name));
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(1);
		PutInst(exp->lineno, II_ADD);
		PutInst(exp->lineno, II_POP_VAR);
		PutDWORD(AddSymbol(exp->val.incrementE.name));
		break;

	case notK:
		opcodeEXP(exp->val.notE.exp, TRUE);
		//printf("NOT\n");
		PutInst(exp->lineno, II_NOT);
		break;
		
	case lvalueK:
		//opcodeLVALUE(exp->val.lvalueE);
		if(exp->val.lvalueE->member==NULL){
			//printf("PUSH VAR %s\n", exp->val.lvalueE->id);
			PutInst(exp->lineno, II_PUSH_VAR);
			PutDWORD(AddSymbol(exp->val.lvalueE->id));
		}
		else{
			if(exp->val.lvalueE->id!=NULL){
				//printf("PUSH VAR %s\n", exp->val.lvalueE->id);
				PutInst(exp->lineno, II_PUSH_VAR_REF);
				PutDWORD(AddSymbol(exp->val.lvalueE->id));
			}
			else{
				//printf("PUSH VAR THIS\n");
				PutInst(exp->lineno, II_PUSH_VAR_THIS);
			}
			
			opcodeEXP(exp->val.lvalueE->member, TRUE);
			
			//printf("PUSH BY EXP\n");
			PutInst(exp->lineno, II_PUSH_BY_EXP);
		}
		break;
		
	case assignmentK:
		opcodeEXP(exp->val.assignmentE.right, TRUE);		
		opcodeLVALUE(exp->val.assignmentE.left);
		break;
		
	case equalsK:
		opcodeEXP(exp->val.equalsE.left, TRUE);
		opcodeEXP(exp->val.equalsE.right, TRUE);
		//printf("CMP EQ\n");
		PutInst(exp->lineno, II_CMP_EQ);
		
		break;
		
	case nequalsK:
		opcodeEXP(exp->val.nequalsE.left, TRUE);
		opcodeEXP(exp->val.nequalsE.right, TRUE);
		//printf("CMP NE\n");
		PutInst(exp->lineno, II_CMP_NE);
		
		break;
		
	case strictequalsK:
		opcodeEXP(exp->val.strictequalsE.left, TRUE);
		opcodeEXP(exp->val.strictequalsE.right, TRUE);
		//printf("CMP STRICT EQ\n");
		PutInst(exp->lineno, II_CMP_STRICT_EQ);
		
		break;
		
	case strictnequalsK:
		opcodeEXP(exp->val.strictnequalsE.left, TRUE);
		opcodeEXP(exp->val.strictnequalsE.right, TRUE);
		//printf("CMP STRICT NE\n");
		PutInst(exp->lineno, II_CMP_STRICT_NE);
		
		break;
		
	case lessK:
		opcodeEXP(exp->val.lessE.left, TRUE);
		opcodeEXP(exp->val.lessE.right, TRUE);
		//printf("CMP L\n");
		PutInst(exp->lineno, II_CMP_L);
		
		break;
		
	case greaterK:
		opcodeEXP(exp->val.greaterE.left, TRUE);
		opcodeEXP(exp->val.greaterE.right, TRUE);
		//printf("CMP G\n");
		PutInst(exp->lineno, II_CMP_G);
		break;
		
	case lequalsK:
		opcodeEXP(exp->val.lequalsE.left, TRUE);
		opcodeEXP(exp->val.lequalsE.right, TRUE);
		//printf("CMP LE\n");
		PutInst(exp->lineno, II_CMP_LE);
		break;
		
	case gequalsK:
		opcodeEXP(exp->val.gequalsE.left, TRUE);
		opcodeEXP(exp->val.gequalsE.right, TRUE);
		//printf("CMP GE\n");
		PutInst(exp->lineno, II_CMP_GE);
		break;
		
	case plusK:
		opcodeEXP(exp->val.plusE.left, TRUE);
		opcodeEXP(exp->val.plusE.right, TRUE);
		//printf("ADD\n");
		PutInst(exp->lineno, II_ADD);
		break;
		
	case minusK:
		opcodeEXP(exp->val.minusE.left, TRUE);
		opcodeEXP(exp->val.minusE.right, TRUE);
		//printf("SUB\n");
		PutInst(exp->lineno, II_SUB);
		break;
		
	case multK:
		opcodeEXP(exp->val.multE.left, TRUE);
		opcodeEXP(exp->val.multE.right, TRUE);
		//printf("MUL\n");
		PutInst(exp->lineno, II_MUL);
		break;
		
	case divK:
		opcodeEXP(exp->val.divE.left, TRUE);
		opcodeEXP(exp->val.divE.right, TRUE);
		//printf("DIV\n");
		PutInst(exp->lineno, II_DIV);
		break;
		
	case moduloK:
		opcodeEXP(exp->val.moduloE.left, TRUE);
		opcodeEXP(exp->val.moduloE.right, TRUE);
		//printf("MODULO\n");
		PutInst(exp->lineno, II_MODULO);
		break;
		
	case andK:
		opcodeEXP(exp->val.andE.left, TRUE);
		opcodeEXP(exp->val.andE.right, TRUE);
		//printf("AND\n");
		PutInst(exp->lineno, II_AND);
		break;
		
	case orK:
		opcodeEXP(exp->val.orE.left, TRUE);
		opcodeEXP(exp->val.orE.right, TRUE);
		//printf("OR\n");
		PutInst(exp->lineno, II_OR);
		break;
		
	case invokeK:
		num_params = 0;
		param = exp->val.invokeE.arguments;
		while(param!=NULL){
			num_params++;
			param = param->next;
		}
		
		if (exp->val.invokeE.arguments != NULL)
			opcodeEXP(exp->val.invokeE.arguments, TRUE);
		
		//printf("PUSH-INT %d\n", num_params);
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(num_params);

		if(exp->val.invokeE.symbol->external){
			PutInst(exp->lineno, II_EXTERNAL_CALL);
			PutDWORD(AddSymbol(exp->val.invokeE.name));
		}
		else{		
			//printf("CALL LABEL %s\n", exp->val.invokeE.name);
			PutInst(exp->lineno, II_CALL);
			AddFuncReq(curr_ip, exp->val.invokeE.name);
			PutDWORD(0);
		}

		if(!compound) PutInst(exp->lineno, II_POP_EMPTY); // destroy return value
		
		break;
		
	case newK:
		//printf("CREATE OBJECT\n");
		PutInst(exp->lineno, II_CREATE_OBJECT);
		
		//printf("PUSH THIS\n");
		PutInst(exp->lineno, II_PUSH_THIS_FROM_STACK);
		
		num_params = 0;
		param = exp->val.newE.arguments;
		while(param!=NULL){
			num_params++;
			param = param->next;
		}
		
		if (exp->val.newE.arguments != NULL)
			opcodeEXP(exp->val.newE.arguments, TRUE);
		
		//printf("PUSH-INT %d\n", num_params);
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(num_params);

		if(exp->val.invokeE.symbol->external){
			PutInst(exp->lineno, II_EXTERNAL_CALL);
			PutDWORD(AddSymbol(exp->val.invokeE.name));
		}
		else{
			//printf("CALL LABEL %s\n", exp->val.newE.name);
			PutInst(exp->lineno, II_CALL);
			AddFuncReq(curr_ip, exp->val.newE.name);
			PutDWORD(0);
		}
		
		//printf("POP THIS\n");
		PutInst(exp->lineno, II_POP_THIS);
		//printf("POP EMPTY\n");
		PutInst(exp->lineno, II_POP_EMPTY); // return value of function is never used

		// object is still on stack
		if(!compound) PutInst(exp->lineno, II_POP_EMPTY); // destroy return value
		
		break;
		
	case invokemethodK:
		
		num_params = 0;
		param = exp->val.invokemethodE.arguments;
		while(param!=NULL){
			num_params++;
			param = param->next;
		}
		
		//printf("PUSH THIS ");
		PutInst(exp->lineno, II_PUSH_THIS);
		//printf("%s", exp->val.invokemethodE.name);
		PutDWORD(AddSymbol(exp->val.invokemethodE.name));
		
		if (exp->val.invokemethodE.arguments != NULL)
			opcodeEXP(exp->val.invokemethodE.arguments, TRUE);
		
		//printf("PUSH-INT %d\n", num_params);
		PutInst(exp->lineno, II_PUSH_INT);
		PutDWORD(num_params);
		
		//printf("PUSH VAR %s\n", exp->val.invokemethodE.name);
		PutInst(exp->lineno, II_PUSH_VAR);
		PutDWORD(AddSymbol(exp->val.invokemethodE.name));
		
		opcodeEXP(exp->val.invokemethodE.method, TRUE);
		//printf("CALL BY EXP\n");
		PutInst(exp->lineno, II_CALL_BY_EXP);

		if(!compound) PutInst(exp->lineno, II_POP_EMPTY); // destroy return value
		
		//printf("POP THIS\n");
		PutInst(exp->lineno, II_POP_THIS);
		
		break;
		/*
		case castK:
		printf("(");
		prettyTYPE(exp->val.castE.type);
		printf(")");
		prettyEXP(exp->val.castE.exp);    
		break;
		*/
	case nullK:
		//printf("PUSH null\n");
		PutInst(exp->lineno, II_PUSH_NULL);
		break;
		
  }
  //if(!compound) PutInst(exp->lineno, II_POP_EMPTY); // destroy return value
  
  if(exp->next != NULL) {
	  opcodeEXP(exp->next, TRUE);
  }
}


void opcodeFUNCTION(struct FUNCTION* function)
{
	int num_params=0;
	DECL* formals;
	DWORD label1;
	
	//printf("JMP LABEL-END %s\n",function->name);
	PutInst(function->lineno, II_JMP);
	label1 = curr_ip;
	PutDWORD(0);
	
	//printf("LABEL %s\n",function->name);
	AddFuncPos(curr_ip, function->name, function->method);
	//printf("ADD SCOPE\n");
	PutInst(function->lineno, II_SCOPE);
	
	formals = function->formals;
	while(formals!=NULL){
		num_params++;
		formals = formals->next;
	}
	//printf("POP NUM-PARAMS\n");
	//printf("CORRECT STACK TO %d\n", num_params);
	PutInst(function->lineno, II_CORRECT_STACK);
	PutDWORD(num_params);
	
	
	if (function->formals != NULL)
		opcodeDECL(function->formals);
	
	if (function->stms != NULL)
		opcodeSTM(function->stms, 1, 0);
	
	//printf("PUSH VAL null\n");
	PutInst(function->lineno, II_PUSH_NULL);
	
	//printf("RET\n\n");
	PutInst(function->lineno, II_RET);
	
	//printf("LABEL-END %s\n",function->name);
	PutDWORDAt(label1, curr_ip);
}


void opcodeEVENT(struct EVENT* event)
{
	DWORD label1;
	
	//printf("JMP LABEL-END %s\n",function->name);
	PutInst(event->lineno, II_JMP);
	label1 = curr_ip;
	PutDWORD(0);
	
	//printf("LABEL %s\n",function->name);
	AddEventPos(curr_ip, event->name);
	//printf("ADD SCOPE\n");
	PutInst(event->lineno, II_SCOPE);
		
	if (event->stms != NULL)
		opcodeSTM(event->stms, 0, 1);
		
	//printf("RET_EVENT\n\n");
	PutInst(event->lineno, II_RET_EVENT);
	
	//printf("LABEL-END %s\n",function->name);
	PutDWORDAt(label1, curr_ip);
}


void opcodeEXTERNAL(struct EXTERNAL* external)
{
	int NumParams;
	EXT_TYPE* param;
	TExternalType Returns = GetExternalType(external->returns);
	TCallType Call = GetCallType(external->call);
	TExternalType* Params = NULL;
	int i;

	NumParams = 0;
	param = external->formals;
	while(param){
		NumParams++;
		param = param->next;
	}
	
	if(NumParams>0){
		Params = (TExternalType*)malloc(sizeof(TExternalType)*NumParams);
		param = external->formals;
		i = 0;
		while(param){
			Params[i++] = GetExternalType(param);
			param = param->next;
		}
	}

	AddDllEntry(external->library, external->name, Call, Returns, NumParams, Params);
}



void opcodeTYPE(TYPE *type)
{
	switch(type->kind){
	case localK:
		//printf("DEF VAR ");
		PutInst(type->lineno, II_DEF_VAR);
		break;

	case globalK:
		//printf("DEF GLOB ");
		PutInst(type->lineno, II_DEF_GLOB_VAR);
		break;
	
	case constK:
		//printf("DEF CONST ");
		PutInst(type->lineno, II_DEF_CONST_VAR);
		break;
	}
}


void opcodeDECL(DECL *decl)
{
	IDENTIFIER* identifier;

	switch(decl->kind){
	case formalK:
		opcodeTYPE(decl->type);
		//printf("%s\n",decl->val.formalD.name);
		PutDWORD(AddSymbol(decl->val.formalD.name));
		
		//printf("POP VAR %s\n",decl->val.formalD.name);
		PutInst(decl->lineno, II_POP_VAR);
		PutDWORD(AddSymbol(decl->val.formalD.name));
		
		break;

	case variableK:
		identifier = decl->val.variableD.identifiers;
		while(identifier!=NULL){
			opcodeTYPE(decl->type);
			PutDWORD(AddSymbol(identifier->name));
			if (decl->val.variableD.initialization != NULL){
				opcodeEXP(decl->val.variableD.initialization, TRUE);
				PutInst(decl->lineno, II_POP_VAR);
				PutDWORD(AddSymbol(identifier->name));
			}
			identifier = identifier->next;
		}
		break;
		
	case simplevarK:
		
		opcodeTYPE(decl->type);
		//printf("%s",decl->val.simplevarD.name);
		PutDWORD(AddSymbol(decl->val.simplevarD.name));
		
		if (decl->val.simplevarD.initialization != NULL) {      
			opcodeEXP(decl->val.simplevarD.initialization, TRUE);
			//printf("POP VAR %s\n", decl->val.simplevarD.name);
			PutInst(decl->lineno, II_POP_VAR);
			PutDWORD(AddSymbol(decl->val.simplevarD.name));
		}
		break;
	}
	if(decl->next != NULL)
		opcodeDECL(decl->next);
	
}


void opcodeFORINIT(FORINIT *forinit)
{
	switch (forinit->kind) {
	case declforinitK:
		opcodeDECL(forinit->val.declforinitF);
		break;
	case expforinitK:
		opcodeEXP(forinit->val.expforinitF, TRUE);
		break;
	}
	
	if (forinit->next != NULL) {
		opcodeFORINIT(forinit->next);
	}
}


void opcodeLVALUE(LVALUE *lvalue)
{
    if(lvalue->member==NULL){
		//printf("POP VAR %s\n", lvalue->id);
		PutInst(lvalue->lineno, II_POP_VAR);
		PutDWORD(AddSymbol(lvalue->id));
	}
	else{
		if(lvalue->id!=NULL){
			//printf("PUSH VAR %s\n", lvalue->id);
			PutInst(lvalue->lineno, II_PUSH_VAR_REF);
			PutDWORD(AddSymbol(lvalue->id));
		}
		else{
			//printf("PUSH VAR THIS\n");
			PutInst(lvalue->lineno, II_PUSH_VAR_THIS);
		}
		
		opcodeEXP(lvalue->member, TRUE);
		//printf("POP BY EXP\n");
		PutInst(lvalue->lineno, II_POP_BY_EXP);
	}
}


//////////////////////////////////////////////////////////////////////////
TExternalType GetExternalType(EXT_TYPE* Type)
{
	switch(Type->kind){
		case voidK:   return TYPE_VOID;
		case boolK:   return TYPE_BOOL;
		case longK:   return TYPE_LONG;
		case byteK:   return TYPE_BYTE;
		case stringK: return TYPE_STRING;
		case floatK:  return TYPE_FLOAT;
		case doubleK: return TYPE_DOUBLE;
		case membufferK: return TYPE_MEMBUFFER;
	}
	return TYPE_VOID;
}


//////////////////////////////////////////////////////////////////////////
TCallType GetCallType(CALL_TYPE* Type)
{
	switch(Type->kind){
		case stdcallK:  return CALL_STDCALL;
		case cdeclK:    return CALL_CDECL;
		case thiscallK: return CALL_THISCALL;
	}
	return CALL_STDCALL;
}
