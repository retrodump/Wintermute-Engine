void yyerror(const char *format, ...);
void reportError(int lineno, const char *format, ...);
void reportWarning(int lineno, const char *format, ...);
int errorCheck();

extern int lineno;
