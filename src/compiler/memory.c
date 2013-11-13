#include <stdio.h>
#include <stdlib.h>
#include "memory.h"
#include <stdarg.h>
#include <string.h>

void *Malloc(unsigned n)
{
	void *p;
	if (!(p = malloc(n))) {
		fprintf(stderr,"Malloc(%d) failed.\n",n);
		fflush(stderr);
		abort();
	}
	
	return p;
}
/*
char *strdup(const char *src)
{
	char *dest = (char*)Malloc(strlen(src) + 1);
	strcpy(dest, src);
	
	return dest;
}
*/