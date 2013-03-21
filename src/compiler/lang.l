%{
#include "main.h"
#include "parse.h"
#include <string.h>
#include <stdlib.h>
#include "memory.h"
#include "error.h"

#define fileno _fileno

extern int lineno;

int commentDepth=0;

typedef struct{
  YY_BUFFER_STATE state;
  BYTE* Buffer;
} TIncludeInfo;

#define MAX_INCLUDE_DEPTH 20
#define MAX_STR_CONST 4096

TIncludeInfo include_stack[MAX_INCLUDE_DEPTH];
int include_stack_ptr = 0;

DWORD BufferSize=0;

char string_buf[MAX_STR_CONST];
char *string_buf_ptr;


%}


  /* exclusive startconditions */
%x commentsc incl string
DIGIT    [0-9]

%%

  /*  comment start-condition for recognizing multiline comments */
<commentsc>{
  \r\n|\n                if(include_stack_ptr==0) lineno++;
  "*/"                   { commentDepth--; if (commentDepth == 0) BEGIN(INITIAL);}
  "/*"                     commentDepth++;
  "//"[^\n]*             /* Single line comments take precedence over multilines */;
  <<EOF>>                {  BEGIN(INITIAL); reportError(lineno, "Unclosed comment."); return 0; }
  [^*/\n]*               /* ignore anything thats not a '*' or '/' to increase speed */;
  .                      /* ignore (multiline comments)*/;
}

<incl>{
  [ \t]*       /* eat the whitespace */
  \"[^\"\n]*\" {
                 yytext[strlen(yytext)-1]='\0';

                 if (include_stack_ptr >= MAX_INCLUDE_DEPTH ){
                   BEGIN(INITIAL); reportError(lineno, "Includes nested too deeply (%d)", MAX_INCLUDE_DEPTH);
				   return 0;
                 }

                 include_stack[include_stack_ptr++].state = YY_CURRENT_BUFFER;
				 include_stack[include_stack_ptr].Buffer = NULL;
				 if(callbacks.Dll_LoadFile)	include_stack[include_stack_ptr].Buffer = callbacks.Dll_LoadFile(ExtData, yytext+1, &BufferSize);

                 if(include_stack[include_stack_ptr].Buffer==NULL){
                   BEGIN(INITIAL); reportError(lineno, "Error opening include file %s", yytext+1);
				   return 0;
				 }

				 if(IsUTF8(include_stack[include_stack_ptr].Buffer, BufferSize)) yy_switch_to_buffer(yy_scan_bytes(include_stack[include_stack_ptr].Buffer+3, BufferSize-3));
				 else yy_switch_to_buffer(yy_scan_bytes(include_stack[include_stack_ptr].Buffer, BufferSize));

                 BEGIN(INITIAL);
               }

  .            {  BEGIN(INITIAL); reportError(lineno, "Include filename expected"); return 0;}

}


<string>{

  \"                   { 
                         BEGIN(INITIAL);
                         *string_buf_ptr = '\0';
                         yylval.stringconst = _strdup(string_buf);
                         return tSTRINGCONST;
                       }

  \r\n|\n              { BEGIN(INITIAL); reportError(lineno, "Unterminated string constant"); return 0;}

  ~n                   {
                         if(string_buf_ptr-string_buf >= MAX_STR_CONST){
						   BEGIN(INITIAL);
						   reportError(lineno, "String constant too long");
						   return 0;
						 }
						 else *string_buf_ptr++ = '\n';
					   }

  ~~|~\"                {
                         if(string_buf_ptr-string_buf >= MAX_STR_CONST){
						   BEGIN(INITIAL);
						   reportError(lineno, "String constant too long");
						   return 0;
						 }
						 else *string_buf_ptr++ = yytext[1];
					   }



  .                    {
                         if(string_buf_ptr-string_buf >= MAX_STR_CONST){
						   BEGIN(INITIAL);
						   reportError(lineno, "String constant too long");
						   return 0;
						 }
						 else *string_buf_ptr++ = yytext[0];
					   }

}

 


#include             BEGIN(incl);

[ \t]+                /* ignore */;

"/*"                   { commentDepth++; BEGIN(commentsc);}

\r\n|\n                if(include_stack_ptr==0) lineno++;
"//"[^\n]*            /* ignore (singleline comment)*/;

#region[^\n]*         /* ignore */;
#endregion[^\n]*      /* ignore */;


external                 return tEXTERNAL;

bool                     return tBOOL;
long                     return tLONG;
short                    return tLONG;
byte                     return tBYTE;
string                   return tSTRING;
float                    return tFLOAT;
double                   return tDOUBLE;
int                      return tINT;
membuffer                return tMEMBUFFER;

stdcall                  return tSTDCALL;
cdecl                    return tCDECL;
thiscall                 return tTHISCALL;

switch                   return tSWITCH;
case                     return tCASE;
default                  return tDEFAULT;

else                     return tELSE;
for                      return tFOR;
if                       return tIF;
return                   return tRETURN;
while                    return tWHILE;
function                 return tFUNCTION;
method                   return tMETHOD;
var                      return tVAR;
global                   return tGLOBAL;
const                    return tCONST;
null                     return tNULL;
break;                   return tBREAK;
continue;                return tCONTINUE;
new                      return tNEW;
  /*
this                     return tTHIS;
  */
on                       return tON;

"&&"                     return tAND;
"=="                     return tEQUALS;
"==="                    return tSTRICTEQUALS;
">="                     return tGEQUALS;
"<="                     return tLEQUALS;
"!="                     return tNEQUALS;
"!=="                    return tSTRICTNEQUALS;
"||"                     return tOR;
"="                      return '=';
">"                      return '>';
"<"                      return '<';
"!"                      return '!';
"+"                      return '+';
"-"                      return '-';
"*"                      return '*';
"/"                      return '/';
"%"                      return '%';
"{"                      return '{';
"}"                      return '}';
";"                      return ';';
"("                      return '(';
")"                      return ')';
","                      return ',';
"."                      return '.';
"["                      return '[';
"]"                      return ']';
":"                      return ':';

  /*
"++"                     return tINCREMENT;
"--"                     return tDECREMENT;
  */

0|([1-9][0-9]*)        { yylval.intconst = atoi(yytext);
                         return tINTCONST; }
{DIGIT}+"."{DIGIT}*    { yylval.floatconst = atof(yytext);
                         return tFLOATCONST; }
true                   { yylval.boolconst = 1;
                         return tBOOLCONST; }
false                  { yylval.boolconst = 0;
                         return tBOOLCONST; }


\"                     { string_buf_ptr = string_buf; BEGIN(string); }


[A-Za-z_][A-Za-z0-9_@]* { yylval.identifier = _strdup(yytext);
                         return tIDENTIFIER; }

<<EOF>>                {
                         if(include_stack_ptr>=1 && callbacks.Dll_CloseFile) callbacks.Dll_CloseFile(ExtData, include_stack[include_stack_ptr].Buffer);
                         if (--include_stack_ptr < 0){
                           yyterminate();
                         }
                         else{
                           yy_delete_buffer( YY_CURRENT_BUFFER );
                           yy_switch_to_buffer(include_stack[include_stack_ptr].state);
                         }
                       }

.                      {  BEGIN(INITIAL); reportError(lineno, "Invalid character"); return 0;}

%%
