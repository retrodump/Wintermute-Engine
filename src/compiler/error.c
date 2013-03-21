#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "main.h"
#include "error.h"

extern char *yytext;

int lineno = 0;
int errors = 0;

void yyerror(const char *format, ...)
{
/*
	va_list argList;
	
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);
	
	fprintf(stderr, "\n");
	fprintf(stderr, "*** syntax error before %s at line %i\n", yytext, lineno);
	fprintf(stderr, "*** compilation terminated\n");
*/
	reportError(lineno, "syntax error");
	//exit(1);
}

void reportError(int lineno, const char *format, ...)
{
	/*
	va_list argList;
	
	fprintf(stderr, "*** Error: ");
	
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);
	
	fprintf(stderr, " at line %i\n", lineno);
	
	errors++;
	*/

	char str[256];
	va_list argList;
		
	va_start(argList, format);
	vsprintf(str, format, argList);
	va_end(argList);
	
	if(callbacks.Dll_AddError) callbacks.Dll_AddError(ExtData, lineno, str);
	
	errors++;
}

void reportWarning(int lineno, const char *format, ...)
{
	/*
	va_list argList;
	
	fprintf(stderr, "*** Warning: ");
	
	va_start(argList, format);
	vfprintf(stderr, format, argList);
	va_end(argList);
	
	fprintf(stderr, " at line %i\n", lineno);
	*/
}


int errorCheck()
{
	if (errors!=0) {
		//fprintf(stderr, "*** %i error(s) encountered, compilation terminated\n",errors);
		return 1;
	}
	return 0;
}
