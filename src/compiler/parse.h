typedef union {
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
} YYSTYPE;
#define	tAND	258
#define	tELSE	259
#define	tEQUALS	260
#define	tGEQUALS	261
#define	tIF	262
#define	tINT	263
#define	tLEQUALS	264
#define	tNEQUALS	265
#define	tOR	266
#define	tRETURN	267
#define	tSTRICTEQUALS	268
#define	tSTRICTNEQUALS	269
#define	tBOOL	270
#define	tLONG	271
#define	tBYTE	272
#define	tSTRING	273
#define	tVOID	274
#define	tFLOAT	275
#define	tDOUBLE	276
#define	tSHORT	277
#define	tMEMBUFFER	278
#define	tSTDCALL	279
#define	tCDECL	280
#define	tTHISCALL	281
#define	tINCREMENT	282
#define	tDECREMENT	283
#define	tWHILE	284
#define	tERROR	285
#define	tFOR	286
#define	tFUNCTION	287
#define	tMETHOD	288
#define	tVAR	289
#define	tGLOBAL	290
#define	tCONST	291
#define	tNULL	292
#define	tBREAK	293
#define	tCONTINUE	294
#define	tNEW	295
#define	tTHIS	296
#define	tON	297
#define	tEXTERNAL	298
#define	tSWITCH	299
#define	tCASE	300
#define	tDEFAULT	301
#define	tIDENTIFIER	302
#define	tINTCONST	303
#define	tBOOLCONST	304
#define	tFLOATCONST	305
#define	tSTRINGCONST	306


extern YYSTYPE yylval;
