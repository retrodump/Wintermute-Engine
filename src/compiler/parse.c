
/*  A Bison parser, made from lang.y with Bison version GNU Bison version 1.24
  */

#define YYBISON 1  /* Identify Bison output.  */

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

#line 6 "lang.y"

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


#line 26 "lang.y"
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

#ifndef YYLTYPE
typedef
  struct yyltype
    {
      int timestamp;
      int first_line;
      int first_column;
      int last_line;
      int last_column;
      char *text;
   }
  yyltype;

#define YYLTYPE yyltype
#endif

#include <stdio.h>

#ifndef __cplusplus
#ifndef __STDC__
#define const
#endif
#endif



#define	YYFINAL		222
#define	YYFLAG		-32768
#define	YYNTBASE	71

#define YYTRANSLATE(x) ((unsigned)(x) <= 306 ? yytranslate[x] : 109)

static const char yytranslate[] = {     0,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    66,     2,     2,     2,    60,     2,     2,    63,
    52,    58,    56,    62,    57,    67,    59,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,    70,    61,    54,
    53,    55,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
    68,     2,    69,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,    64,     2,    65,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     2,     2,     2,     2,     2,     1,     2,     3,     4,     5,
     6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
    16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
    26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
    36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
    46,    47,    48,    49,    50,    51
};

#if YYDEBUG != 0
static const short yyprhs[] = {     0,
     0,     2,     3,     5,     7,    10,    12,    14,    16,    18,
    23,    27,    29,    33,    34,    37,    39,    41,    43,    45,
    47,    49,    51,    53,    55,    57,    59,    61,    62,    64,
    66,    70,    72,    75,    76,    78,    80,    82,    83,    85,
    86,    88,    98,   105,   112,   116,   117,   119,   121,   125,
   128,   131,   135,   137,   140,   142,   145,   149,   155,   163,
   169,   179,   181,   183,   186,   188,   190,   198,   199,   201,
   203,   207,   209,   211,   215,   219,   223,   227,   231,   235,
   239,   243,   247,   251,   255,   259,   263,   267,   271,   275,
   277,   279,   282,   285,   288,   291,   296,   298,   300,   302,
   304,   306,   308,   312,   317,   324,   332,   335,   341,   342,
   344,   346,   348,   351,   352,   354,   357,   362,   366,   367,
   371,   373,   377,   379,   383,   388
};

static const short yyrhs[] = {    72,
     0,     0,    73,     0,    74,     0,    74,    73,     0,    88,
     0,    94,     0,    89,     0,    87,     0,    79,    77,    78,
    61,     0,    34,    47,    78,     0,    47,     0,    47,    62,
    77,     0,     0,    53,    99,     0,    34,     0,    35,     0,
    36,     0,    15,     0,    16,     0,    22,     0,     8,     0,
    17,     0,    18,     0,    20,     0,    21,     0,    23,     0,
     0,    82,     0,    83,     0,    83,    62,    82,     0,    80,
     0,    80,    47,     0,     0,    24,     0,    25,     0,    26,
     0,     0,    80,     0,     0,    34,     0,    43,    51,    84,
    85,    47,    63,    81,    52,    61,     0,    32,    47,    63,
    90,    52,    93,     0,    33,    47,    63,    90,    52,    93,
     0,    42,    51,    95,     0,     0,    91,     0,    92,     0,
    92,    62,    91,     0,    86,    47,     0,    64,    65,     0,
    64,    94,    65,     0,    95,     0,    94,    95,     0,    61,
     0,    12,    61,     0,    12,    99,    61,     0,     7,    63,
    99,    52,    95,     0,     7,    63,    99,    52,    95,     4,
    95,     0,    29,    63,    99,    52,    95,     0,    31,    63,
    96,    61,    99,    61,   102,    52,    95,     0,    93,     0,
    75,     0,    99,    61,     0,    38,     0,    39,     0,    44,
    63,    99,    52,    64,   103,    65,     0,     0,    97,     0,
    98,     0,    98,    62,    97,     0,    76,     0,    99,     0,
   108,    53,    99,     0,    99,     5,    99,     0,    99,    10,
    99,     0,    99,    13,    99,     0,    99,    14,    99,     0,
    99,    54,    99,     0,    99,    55,    99,     0,    99,     9,
    99,     0,    99,     6,    99,     0,    99,    56,    99,     0,
    99,    57,    99,     0,    99,    58,    99,     0,    99,    59,
    99,     0,    99,    60,    99,     0,    99,     3,    99,     0,
    99,    11,    99,     0,    37,     0,   100,     0,    57,   100,
     0,    66,   100,     0,    47,    27,     0,    47,    28,     0,
    63,    79,    52,   100,     0,   101,     0,    48,     0,    49,
     0,    50,     0,    51,     0,   108,     0,    63,    99,    52,
     0,    47,    63,   102,    52,     0,    47,    67,    47,    63,
   102,    52,     0,    47,    68,    99,    69,    63,   102,    52,
     0,    40,    47,     0,    40,    47,    63,   102,    52,     0,
     0,   107,     0,   106,     0,   104,     0,   104,   106,     0,
     0,   105,     0,   105,   104,     0,    45,   101,    70,    94,
     0,    45,   101,    70,     0,     0,    46,    70,    94,     0,
    99,     0,    99,    62,   107,     0,    47,     0,    47,    67,
    47,     0,    47,    68,    99,    69,     0,    41,    68,    99,
    69,     0
};

#endif

#if YYDEBUG != 0
static const short yyrline[] = { 0,
    97,   101,   103,   107,   109,   113,   115,   117,   119,   124,
   128,   133,   135,   139,   141,   145,   147,   149,   153,   155,
   157,   159,   161,   163,   165,   167,   169,   174,   176,   180,
   182,   186,   188,   194,   196,   198,   200,   204,   206,   211,
   212,   216,   221,   223,   227,   233,   235,   239,   241,   245,
   250,   252,   256,   258,   263,   265,   267,   269,   271,   273,
   275,   277,   279,   281,   283,   285,   287,   291,   293,   297,
   299,   303,   305,   311,   313,   315,   317,   319,   321,   323,
   325,   327,   329,   331,   333,   335,   337,   339,   341,   343,
   345,   349,   351,   353,   355,   357,   359,   363,   365,   367,
   369,   374,   376,   378,   380,   382,   384,   386,   390,   392,
   397,   399,   401,   406,   408,   410,   415,   417,   421,   423,
   428,   430,   434,   436,   438,   442
};

static const char * const yytname[] = {   "$","error","$undefined.","tAND","tELSE",
"tEQUALS","tGEQUALS","tIF","tINT","tLEQUALS","tNEQUALS","tOR","tRETURN","tSTRICTEQUALS",
"tSTRICTNEQUALS","tBOOL","tLONG","tBYTE","tSTRING","tVOID","tFLOAT","tDOUBLE",
"tSHORT","tMEMBUFFER","tSTDCALL","tCDECL","tTHISCALL","tINCREMENT","tDECREMENT",
"tWHILE","tERROR","tFOR","tFUNCTION","tMETHOD","tVAR","tGLOBAL","tCONST","tNULL",
"tBREAK","tCONTINUE","tNEW","tTHIS","tON","tEXTERNAL","tSWITCH","tCASE","tDEFAULT",
"tIDENTIFIER","tINTCONST","tBOOLCONST","tFLOATCONST","tSTRINGCONST","')'","'='",
"'<'","'>'","'+'","'-'","'*'","'/'","'%'","';'","','","'('","'{'","'}'","'!'",
"'.'","'['","']'","':'","scriptcollection","toplevels","netoplevels","toplevel",
"decl","simpledecl","identifiers","initialization","type","ext_type","ext_formals",
"neext_formals","ext_formal","call_type","ext_returns","optvar","external","function",
"event","formals","neformals","formal","compoundstm","nestms","stm","forinits",
"neforinits","forinit","exp","unaryexp","unarypostfixexp","exps","dcases","case_branches",
"case_branch","default_branch","neexps","lvalue",""
};
#endif

static const short yyr1[] = {     0,
    71,    72,    72,    73,    73,    74,    74,    74,    74,    75,
    76,    77,    77,    78,    78,    79,    79,    79,    80,    80,
    80,    80,    80,    80,    80,    80,    80,    81,    81,    82,
    82,    83,    83,    84,    84,    84,    84,    85,    85,    86,
    86,    87,    88,    88,    89,    90,    90,    91,    91,    92,
    93,    93,    94,    94,    95,    95,    95,    95,    95,    95,
    95,    95,    95,    95,    95,    95,    95,    96,    96,    97,
    97,    98,    98,    99,    99,    99,    99,    99,    99,    99,
    99,    99,    99,    99,    99,    99,    99,    99,    99,    99,
    99,   100,   100,   100,   100,   100,   100,   101,   101,   101,
   101,   101,   101,   101,   101,   101,   101,   101,   102,   102,
   103,   103,   103,   104,   104,   104,   105,   105,   106,   106,
   107,   107,   108,   108,   108,   108
};

static const short yyr2[] = {     0,
     1,     0,     1,     1,     2,     1,     1,     1,     1,     4,
     3,     1,     3,     0,     2,     1,     1,     1,     1,     1,
     1,     1,     1,     1,     1,     1,     1,     0,     1,     1,
     3,     1,     2,     0,     1,     1,     1,     0,     1,     0,
     1,     9,     6,     6,     3,     0,     1,     1,     3,     2,
     2,     3,     1,     2,     1,     2,     3,     5,     7,     5,
     9,     1,     1,     2,     1,     1,     7,     0,     1,     1,
     3,     1,     1,     3,     3,     3,     3,     3,     3,     3,
     3,     3,     3,     3,     3,     3,     3,     3,     3,     1,
     1,     2,     2,     2,     2,     4,     1,     1,     1,     1,
     1,     1,     3,     4,     6,     7,     2,     5,     0,     1,
     1,     1,     2,     0,     1,     2,     4,     3,     0,     3,
     1,     3,     1,     3,     4,     4
};

static const short yydefact[] = {     2,
     0,     0,     0,     0,     0,     0,    16,    17,    18,    90,
    65,    66,     0,     0,     0,     0,     0,   123,    98,    99,
   100,   101,     0,    55,     0,     0,     0,     1,     3,     4,
    63,     0,     9,     6,     8,    62,     7,    53,     0,    91,
    97,   102,     0,    56,     0,     0,    68,     0,     0,   107,
     0,     0,    34,     0,    94,    95,   109,     0,     0,    92,
   102,     0,     0,    51,     0,    93,     5,    12,    14,    54,
     0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     0,     0,     0,     0,     0,    64,     0,     0,    57,     0,
     0,    72,     0,    69,    70,    73,    40,    40,   109,     0,
    45,    35,    36,    37,    38,     0,   121,     0,   110,   124,
     0,     0,   103,    52,     0,     0,     0,    88,    75,    82,
    81,    76,    89,    77,    78,    79,    80,    83,    84,    85,
    86,    87,    74,     0,     0,    14,     0,     0,    41,     0,
     0,    47,    48,     0,     0,   126,    22,    19,    20,    23,
    24,    25,    26,    21,    27,    39,     0,     0,     0,   104,
   109,   125,    96,    13,    15,    10,    58,    60,    11,     0,
    71,    50,     0,    40,     0,   108,     0,   114,   122,     0,
   109,     0,   109,    43,    49,    44,    28,     0,     0,     0,
   112,   114,   111,   105,     0,    59,     0,    32,     0,    29,
    30,   123,     0,     0,     0,    67,   113,   116,   106,     0,
    33,     0,     0,   118,   120,    61,    42,    31,   117,     0,
     0,     0
};

static const short yydefgoto[] = {   220,
    28,    29,    30,    31,    92,    69,   117,    32,   198,   199,
   200,   201,   105,   157,   140,    33,    34,    35,   141,   142,
   143,    36,    37,    38,    93,    94,    95,    39,    40,    41,
   108,   190,   191,   192,   193,   109,    42
};

static const short yypact[] = {   176,
   -51,   589,   -39,   -38,   -14,    -8,-32768,-32768,-32768,-32768,
-32768,-32768,     6,   -17,    -3,     3,    -7,    66,-32768,-32768,
-32768,-32768,   618,-32768,   546,   222,   618,-32768,-32768,   176,
-32768,    18,-32768,-32768,-32768,-32768,   314,-32768,   388,-32768,
-32768,   -10,   594,-32768,   400,   594,   551,     5,    21,    23,
   594,   314,    -4,   594,-32768,-32768,   594,    19,   594,-32768,
-32768,    36,   471,-32768,   268,-32768,-32768,    28,    42,-32768,
   594,   594,   594,   594,   594,   594,   594,   594,   594,   594,
   594,   594,   594,   594,   594,-32768,   594,   483,-32768,   495,
    49,-32768,    37,-32768,    38,   565,   -26,   -26,   594,   106,
-32768,-32768,-32768,-32768,   656,   507,   376,    56,-32768,    50,
   137,   618,-32768,-32768,    18,   594,    57,     4,    47,    47,
    47,    47,     4,    47,    47,    47,    47,   -29,   -29,    39,
    39,-32768,   565,   314,   314,    42,   594,   551,-32768,    63,
    69,-32768,    60,    71,    72,-32768,-32768,-32768,-32768,-32768,
-32768,-32768,-32768,-32768,-32768,-32768,    78,    62,   594,-32768,
   594,    64,-32768,-32768,   565,-32768,   124,-32768,-32768,   412,
-32768,-32768,    67,    96,    67,-32768,    74,     1,-32768,    80,
   594,   314,   594,-32768,-32768,-32768,   656,   -13,    68,    76,
    93,    99,-32768,-32768,   100,-32768,   101,    98,   102,-32768,
    94,   -52,   594,    85,   314,-32768,-32768,-32768,-32768,   314,
-32768,    97,   656,   314,   314,-32768,-32768,-32768,   314,   168,
   169,-32768
};

static const short yypgoto[] = {-32768,
-32768,   140,-32768,-32768,-32768,    58,    35,   147,    73,-32768,
   -37,-32768,-32768,-32768,-32768,-32768,-32768,-32768,    86,     0,
-32768,  -133,   -25,   -33,-32768,    52,-32768,    -2,   -20,    -1,
   -94,-32768,     7,-32768,    -6,    41,   -21
};


#define	YYLAST		684


static const short yytable[] = {    45,
    65,    61,    60,    70,   145,    61,    66,   139,    72,    73,
    57,    43,    74,    75,    58,    59,    77,    78,   101,   102,
   103,   104,    63,    46,    47,   -46,    13,    14,    83,    84,
    85,    70,    48,   202,    19,    20,    21,    22,    49,   184,
    88,   186,    87,    90,    96,   188,   189,    52,   100,   203,
    51,   106,    50,    53,   107,    54,   111,    79,    80,    81,
    82,    83,    84,    85,    68,   110,   180,    97,   118,   119,
   120,   121,   122,   123,   124,   125,   126,   127,   128,   129,
   130,   131,   132,    98,   133,    99,   195,   112,   197,   115,
    61,   163,    55,    56,   116,   136,   107,   137,    85,   138,
   167,   168,    81,    82,    83,    84,    85,   160,    71,   172,
    72,    73,   161,   165,    74,    75,    76,   166,    77,    78,
   173,   174,   175,   176,   177,   178,   181,   182,    57,   139,
    26,   194,    58,    59,   170,    96,   187,   205,   189,    71,
   206,    72,    73,   188,   211,    74,    75,    76,   196,    77,
    78,   209,   210,   212,   214,   213,   107,   217,   107,    79,
    80,    81,    82,    83,    84,    85,    61,   221,   222,    67,
   169,    62,   164,   185,   146,   218,   216,   156,   107,   215,
   107,    70,     1,   144,   207,    70,   204,     2,   219,   171,
    79,    80,    81,    82,    83,    84,    85,     0,   208,   179,
    63,     0,     0,     0,     3,   162,     4,     5,     6,     7,
     8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
     0,     0,    18,    19,    20,    21,    22,     0,     1,     0,
     0,     0,    23,     2,     0,     0,    24,     0,    25,    26,
     0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
     3,     0,     4,     0,     0,     7,     8,     9,    10,    11,
    12,    13,    14,     0,     0,    17,     0,     0,    18,    19,
    20,    21,    22,     0,     1,     0,     0,     0,    23,     2,
     0,     0,    24,     0,    25,    26,    64,    27,     0,     0,
     0,     0,     0,     0,     0,     0,     3,     0,     4,     0,
     0,     7,     8,     9,    10,    11,    12,    13,    14,     0,
     0,    17,     0,     0,    18,    19,    20,    21,    22,     0,
     1,     0,     0,     0,    23,     2,     0,     0,    24,     0,
    25,    26,   114,    27,     0,     0,     0,     0,     0,     0,
     0,     0,     3,     0,     4,     0,     0,     7,     8,     9,
    10,    11,    12,    13,    14,     0,     0,    17,     0,     0,
    18,    19,    20,    21,    22,     0,     0,     0,     0,     0,
    23,     0,     0,     0,    24,     0,    25,    26,    71,    27,
    72,    73,     0,     0,    74,    75,    76,     0,    77,    78,
    71,     0,    72,    73,     0,     0,    74,    75,    76,     0,
    77,    78,    71,     0,    72,    73,     0,     0,    74,    75,
    76,     0,    77,    78,    71,     0,    72,    73,     0,     0,
    74,    75,    76,     0,    77,    78,     0,     0,     0,    79,
    80,    81,    82,    83,    84,    85,     0,   159,     0,     0,
     0,    79,    80,    81,    82,    83,    84,    85,    86,     0,
     0,     0,     0,    79,    80,    81,    82,    83,    84,    85,
    89,     0,     0,     0,     0,    79,    80,    81,    82,    83,
    84,    85,   183,    71,     0,    72,    73,     0,     0,    74,
    75,    76,     0,    77,    78,    71,     0,    72,    73,     0,
     0,    74,    75,    76,     0,    77,    78,    71,     0,    72,
    73,     0,     0,    74,    75,    76,     0,    77,    78,    71,
     0,    72,    73,     0,     0,    74,    75,    76,     0,    77,
    78,     0,   113,     0,    79,    80,    81,    82,    83,    84,
    85,     0,     0,     0,   134,     0,    79,    80,    81,    82,
    83,    84,    85,     0,     0,     0,   135,     0,    79,    80,
    81,    82,    83,    84,    85,     0,     0,     0,   158,     0,
    79,    80,    81,    82,    83,    84,    85,    71,     0,    72,
    73,     0,     0,    74,    75,    76,     0,    77,    78,     7,
     8,     9,    10,     0,    91,    13,    14,    10,     0,     0,
    13,    14,    18,    19,    20,    21,    22,    18,    19,    20,
    21,    22,    23,     0,     0,     0,     0,    23,    25,     0,
     0,    27,     0,    25,     0,     0,    27,     0,    79,    80,
    81,    82,    83,    84,    85,    10,     0,     0,    13,    14,
    10,     0,     0,    13,    14,    18,    19,    20,    21,    22,
    18,    19,    20,    21,    22,    23,     0,     0,     0,    44,
    23,    25,     0,     0,    27,     0,    25,    13,    14,    27,
     0,     0,     0,   147,    18,    19,    20,    21,    22,     0,
   148,   149,   150,   151,    23,   152,   153,   154,   155,     0,
    25,     0,     0,    27
};

static const short yycheck[] = {     2,
    26,    23,    23,    37,    99,    27,    27,    34,     5,     6,
    63,    63,     9,    10,    67,    68,    13,    14,    52,    24,
    25,    26,    25,    63,    63,    52,    40,    41,    58,    59,
    60,    65,    47,    47,    48,    49,    50,    51,    47,   173,
    43,   175,    53,    46,    47,    45,    46,    51,    51,    63,
    68,    54,    47,    51,    57,    63,    59,    54,    55,    56,
    57,    58,    59,    60,    47,    47,   161,    63,    71,    72,
    73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
    83,    84,    85,    63,    87,    63,   181,    52,   183,    62,
   112,   112,    27,    28,    53,    47,    99,    61,    60,    62,
   134,   135,    56,    57,    58,    59,    60,    52,     3,    47,
     5,     6,    63,   116,     9,    10,    11,    61,    13,    14,
    52,    62,    52,    52,    47,    64,    63,     4,    63,    34,
    64,    52,    67,    68,   137,   138,    63,    70,    46,     3,
    65,     5,     6,    45,    47,     9,    10,    11,   182,    13,
    14,    52,    52,    52,    70,    62,   159,    61,   161,    54,
    55,    56,    57,    58,    59,    60,   188,     0,     0,    30,
   136,    25,   115,   174,    69,   213,   210,   105,   181,   205,
   183,   215,     7,    98,   191,   219,   188,    12,   214,   138,
    54,    55,    56,    57,    58,    59,    60,    -1,   192,   159,
   203,    -1,    -1,    -1,    29,    69,    31,    32,    33,    34,
    35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
    -1,    -1,    47,    48,    49,    50,    51,    -1,     7,    -1,
    -1,    -1,    57,    12,    -1,    -1,    61,    -1,    63,    64,
    -1,    66,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
    29,    -1,    31,    -1,    -1,    34,    35,    36,    37,    38,
    39,    40,    41,    -1,    -1,    44,    -1,    -1,    47,    48,
    49,    50,    51,    -1,     7,    -1,    -1,    -1,    57,    12,
    -1,    -1,    61,    -1,    63,    64,    65,    66,    -1,    -1,
    -1,    -1,    -1,    -1,    -1,    -1,    29,    -1,    31,    -1,
    -1,    34,    35,    36,    37,    38,    39,    40,    41,    -1,
    -1,    44,    -1,    -1,    47,    48,    49,    50,    51,    -1,
     7,    -1,    -1,    -1,    57,    12,    -1,    -1,    61,    -1,
    63,    64,    65,    66,    -1,    -1,    -1,    -1,    -1,    -1,
    -1,    -1,    29,    -1,    31,    -1,    -1,    34,    35,    36,
    37,    38,    39,    40,    41,    -1,    -1,    44,    -1,    -1,
    47,    48,    49,    50,    51,    -1,    -1,    -1,    -1,    -1,
    57,    -1,    -1,    -1,    61,    -1,    63,    64,     3,    66,
     5,     6,    -1,    -1,     9,    10,    11,    -1,    13,    14,
     3,    -1,     5,     6,    -1,    -1,     9,    10,    11,    -1,
    13,    14,     3,    -1,     5,     6,    -1,    -1,     9,    10,
    11,    -1,    13,    14,     3,    -1,     5,     6,    -1,    -1,
     9,    10,    11,    -1,    13,    14,    -1,    -1,    -1,    54,
    55,    56,    57,    58,    59,    60,    -1,    62,    -1,    -1,
    -1,    54,    55,    56,    57,    58,    59,    60,    61,    -1,
    -1,    -1,    -1,    54,    55,    56,    57,    58,    59,    60,
    61,    -1,    -1,    -1,    -1,    54,    55,    56,    57,    58,
    59,    60,    61,     3,    -1,     5,     6,    -1,    -1,     9,
    10,    11,    -1,    13,    14,     3,    -1,     5,     6,    -1,
    -1,     9,    10,    11,    -1,    13,    14,     3,    -1,     5,
     6,    -1,    -1,     9,    10,    11,    -1,    13,    14,     3,
    -1,     5,     6,    -1,    -1,     9,    10,    11,    -1,    13,
    14,    -1,    52,    -1,    54,    55,    56,    57,    58,    59,
    60,    -1,    -1,    -1,    52,    -1,    54,    55,    56,    57,
    58,    59,    60,    -1,    -1,    -1,    52,    -1,    54,    55,
    56,    57,    58,    59,    60,    -1,    -1,    -1,    52,    -1,
    54,    55,    56,    57,    58,    59,    60,     3,    -1,     5,
     6,    -1,    -1,     9,    10,    11,    -1,    13,    14,    34,
    35,    36,    37,    -1,    34,    40,    41,    37,    -1,    -1,
    40,    41,    47,    48,    49,    50,    51,    47,    48,    49,
    50,    51,    57,    -1,    -1,    -1,    -1,    57,    63,    -1,
    -1,    66,    -1,    63,    -1,    -1,    66,    -1,    54,    55,
    56,    57,    58,    59,    60,    37,    -1,    -1,    40,    41,
    37,    -1,    -1,    40,    41,    47,    48,    49,    50,    51,
    47,    48,    49,    50,    51,    57,    -1,    -1,    -1,    61,
    57,    63,    -1,    -1,    66,    -1,    63,    40,    41,    66,
    -1,    -1,    -1,     8,    47,    48,    49,    50,    51,    -1,
    15,    16,    17,    18,    57,    20,    21,    22,    23,    -1,
    63,    -1,    -1,    66
};
/* -*-C-*-  Note some compilers choke on comments on `#line' lines.  */
#line 3 "bison.simple"

/* Skeleton output parser for bison,
   Copyright (C) 1984, 1989, 1990 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.  */

/* As a special exception, when this file is copied by Bison into a
   Bison output file, you may use that output file without restriction.
   This special exception was added by the Free Software Foundation
   in version 1.24 of Bison.  */

#ifndef alloca
#ifdef __GNUC__
#define alloca __builtin_alloca
#else /* not GNU C.  */
#if (!defined (__STDC__) && defined (sparc)) || defined (__sparc__) || defined (__sparc) || defined (__sgi)
#include <alloca.h>
#else /* not sparc */
#if defined (MSDOS) && !defined (__TURBOC__)
#include <malloc.h>
#else /* not MSDOS, or __TURBOC__ */
#if defined(_AIX)
#include <malloc.h>
 #pragma alloca
#else /* not MSDOS, __TURBOC__, or _AIX */
#ifdef __hpux
#ifdef __cplusplus
extern "C" {
void *alloca (unsigned int);
};
#else /* not __cplusplus */
void *alloca ();
#endif /* not __cplusplus */
#endif /* __hpux */
#endif /* not _AIX */
#endif /* not MSDOS, or __TURBOC__ */
#endif /* not sparc.  */
#endif /* not GNU C.  */
#endif /* alloca not defined.  */

/* This is the parser code that is written into each bison parser
  when the %semantic_parser declaration is not specified in the grammar.
  It was written by Richard Stallman by simplifying the hairy parser
  used when %semantic_parser is specified.  */

/* Note: there must be only one dollar sign in this file.
   It is replaced by the list of actions, each action
   as one case of the switch.  */

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		-2
#define YYEOF		0
#define YYACCEPT	return(0)
#define YYABORT 	return(1)
#define YYERROR		goto yyerrlab1
/* Like YYERROR except do call yyerror.
   This remains here temporarily to ease the
   transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */
#define YYFAIL		goto yyerrlab
#define YYRECOVERING()  (!!yyerrstatus)
#define YYBACKUP(token, value) \
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    { yychar = (token), yylval = (value);			\
      yychar1 = YYTRANSLATE (yychar);				\
      YYPOPSTACK;						\
      goto yybackup;						\
    }								\
  else								\
    { yyerror ("syntax error: cannot back up"); YYERROR; }	\
while (0)

#define YYTERROR	1
#define YYERRCODE	256

#ifndef YYPURE
#define YYLEX		yylex()
#endif

#ifdef YYPURE
#ifdef YYLSP_NEEDED
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, &yylloc, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval, &yylloc)
#endif
#else /* not YYLSP_NEEDED */
#ifdef YYLEX_PARAM
#define YYLEX		yylex(&yylval, YYLEX_PARAM)
#else
#define YYLEX		yylex(&yylval)
#endif
#endif /* not YYLSP_NEEDED */
#endif

/* If nonreentrant, generate the variables here */

#ifndef YYPURE

int	yychar;			/*  the lookahead symbol		*/
YYSTYPE	yylval;			/*  the semantic value of the		*/
				/*  lookahead symbol			*/

#ifdef YYLSP_NEEDED
YYLTYPE yylloc;			/*  location data for the lookahead	*/
				/*  symbol				*/
#endif

int yynerrs;			/*  number of parse errors so far       */
#endif  /* not YYPURE */

#if YYDEBUG != 0
int yydebug;			/*  nonzero means print parse trace	*/
/* Since this is uninitialized, it does not stop multiple parsers
   from coexisting.  */
#endif

/*  YYINITDEPTH indicates the initial size of the parser's stacks	*/

#ifndef	YYINITDEPTH
#define YYINITDEPTH 200
#endif

/*  YYMAXDEPTH is the maximum size the stacks can grow to
    (effective only if the built-in stack extension method is used).  */

#if YYMAXDEPTH == 0
#undef YYMAXDEPTH
#endif

#ifndef YYMAXDEPTH
#define YYMAXDEPTH 10000
#endif

/* Prevent warning if -Wstrict-prototypes.  */
#ifdef __GNUC__
int yyparse (void);
#endif

#if __GNUC__ > 1		/* GNU C and GNU C++ define this.  */
#define __yy_memcpy(FROM,TO,COUNT)	__builtin_memcpy(TO,FROM,COUNT)
#else				/* not GNU C or C++ */
#ifndef __cplusplus

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (from, to, count)
     char *from;
     char *to;
     int count;
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#else /* __cplusplus */

/* This is the most reliable way to avoid incompatibilities
   in available built-in functions on various systems.  */
static void
__yy_memcpy (char *from, char *to, int count)
{
  register char *f = from;
  register char *t = to;
  register int i = count;

  while (i-- > 0)
    *t++ = *f++;
}

#endif
#endif

#line 192 "bison.simple"

/* The user can define YYPARSE_PARAM as the name of an argument to be passed
   into yyparse.  The argument should have type void *.
   It should actually point to an object.
   Grammar actions can access the variable by casting it
   to the proper pointer type.  */

#ifdef YYPARSE_PARAM
#define YYPARSE_PARAM_DECL void *YYPARSE_PARAM;
#else
#define YYPARSE_PARAM
#define YYPARSE_PARAM_DECL
#endif

int
yyparse(YYPARSE_PARAM)
     YYPARSE_PARAM_DECL
{
  register int yystate;
  register int yyn;
  register short *yyssp;
  register YYSTYPE *yyvsp;
  int yyerrstatus;	/*  number of tokens to shift before error messages enabled */
  int yychar1 = 0;		/*  lookahead token as an internal (translated) token number */

  short	yyssa[YYINITDEPTH];	/*  the state stack			*/
  YYSTYPE yyvsa[YYINITDEPTH];	/*  the semantic value stack		*/

  short *yyss = yyssa;		/*  refer to the stacks thru separate pointers */
  YYSTYPE *yyvs = yyvsa;	/*  to allow yyoverflow to reallocate them elsewhere */

#ifdef YYLSP_NEEDED
  YYLTYPE yylsa[YYINITDEPTH];	/*  the location stack			*/
  YYLTYPE *yyls = yylsa;
  YYLTYPE *yylsp;

#define YYPOPSTACK   (yyvsp--, yyssp--, yylsp--)
#else
#define YYPOPSTACK   (yyvsp--, yyssp--)
#endif

  int yystacksize = YYINITDEPTH;

#ifdef YYPURE
  int yychar;
  YYSTYPE yylval;
  int yynerrs;
#ifdef YYLSP_NEEDED
  YYLTYPE yylloc;
#endif
#endif

  YYSTYPE yyval;		/*  the variable used to return		*/
				/*  semantic values from the action	*/
				/*  routines				*/

  int yylen;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Starting parse\n");
#endif

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss - 1;
  yyvsp = yyvs;
#ifdef YYLSP_NEEDED
  yylsp = yyls;
#endif

/* Push a new state, which is found in  yystate  .  */
/* In all cases, when you get here, the value and location stacks
   have just been pushed. so pushing a state here evens the stacks.  */
yynewstate:

  *++yyssp = yystate;

  if (yyssp >= yyss + yystacksize - 1)
    {
      /* Give user a chance to reallocate the stack */
      /* Use copies of these so that the &'s don't force the real ones into memory. */
      YYSTYPE *yyvs1 = yyvs;
      short *yyss1 = yyss;
#ifdef YYLSP_NEEDED
      YYLTYPE *yyls1 = yyls;
#endif

      /* Get the current used size of the three stacks, in elements.  */
      int size = yyssp - yyss + 1;

#ifdef yyoverflow
      /* Each stack pointer address is followed by the size of
	 the data in use in that stack, in bytes.  */
#ifdef YYLSP_NEEDED
      /* This used to be a conditional around just the two extra args,
	 but that might be undefined if yyoverflow is a macro.  */
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yyls1, size * sizeof (*yylsp),
		 &yystacksize);
#else
      yyoverflow("parser stack overflow",
		 &yyss1, size * sizeof (*yyssp),
		 &yyvs1, size * sizeof (*yyvsp),
		 &yystacksize);
#endif

      yyss = yyss1; yyvs = yyvs1;
#ifdef YYLSP_NEEDED
      yyls = yyls1;
#endif
#else /* no yyoverflow */
      /* Extend the stack our own way.  */
      if (yystacksize >= YYMAXDEPTH)
	{
	  yyerror("parser stack overflow");
	  return 2;
	}
      yystacksize *= 2;
      if (yystacksize > YYMAXDEPTH)
	yystacksize = YYMAXDEPTH;
      yyss = (short *) alloca (yystacksize * sizeof (*yyssp));
      __yy_memcpy ((char *)yyss1, (char *)yyss, size * sizeof (*yyssp));
      yyvs = (YYSTYPE *) alloca (yystacksize * sizeof (*yyvsp));
      __yy_memcpy ((char *)yyvs1, (char *)yyvs, size * sizeof (*yyvsp));
#ifdef YYLSP_NEEDED
      yyls = (YYLTYPE *) alloca (yystacksize * sizeof (*yylsp));
      __yy_memcpy ((char *)yyls1, (char *)yyls, size * sizeof (*yylsp));
#endif
#endif /* no yyoverflow */

      yyssp = yyss + size - 1;
      yyvsp = yyvs + size - 1;
#ifdef YYLSP_NEEDED
      yylsp = yyls + size - 1;
#endif

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Stack size increased to %d\n", yystacksize);
#endif

      if (yyssp >= yyss + yystacksize - 1)
	YYABORT;
    }

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Entering state %d\n", yystate);
#endif

  goto yybackup;
 yybackup:

/* Do appropriate processing given the current state.  */
/* Read a lookahead token if we need one and don't already have one.  */
/* yyresume: */

  /* First try to decide what to do without reference to lookahead token.  */

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* yychar is either YYEMPTY or YYEOF
     or a valid token in external form.  */

  if (yychar == YYEMPTY)
    {
#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Reading a token: ");
#endif
      yychar = YYLEX;
    }

  /* Convert token to internal form (in yychar1) for indexing tables with */

  if (yychar <= 0)		/* This means end of input. */
    {
      yychar1 = 0;
      yychar = YYEOF;		/* Don't call YYLEX any more */

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Now at end of input.\n");
#endif
    }
  else
    {
      yychar1 = YYTRANSLATE(yychar);

#if YYDEBUG != 0
      if (yydebug)
	{
	  fprintf (stderr, "Next token is %d (%s", yychar, yytname[yychar1]);
	  /* Give the individual parser a way to print the precise meaning
	     of a token, for further debugging info.  */
#ifdef YYPRINT
	  YYPRINT (stderr, yychar, yylval);
#endif
	  fprintf (stderr, ")\n");
	}
#endif
    }

  yyn += yychar1;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != yychar1)
    goto yydefault;

  yyn = yytable[yyn];

  /* yyn is what to do for this token type in this state.
     Negative => reduce, -yyn is rule number.
     Positive => shift, yyn is new state.
       New state is final state => don't bother to shift,
       just return success.
     0, or most negative number => error.  */

  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrlab;

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Shift the lookahead token.  */

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting token %d (%s), ", yychar, yytname[yychar1]);
#endif

  /* Discard the token being shifted unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  /* count tokens shifted since error; after three, turn off error status.  */
  if (yyerrstatus) yyerrstatus--;

  yystate = yyn;
  goto yynewstate;

/* Do the default action for the current state.  */
yydefault:

  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;

/* Do a reduction.  yyn is the number of a rule to reduce with.  */
yyreduce:
  yylen = yyr2[yyn];
  if (yylen > 0)
    yyval = yyvsp[1-yylen]; /* implement default value of the action */

#if YYDEBUG != 0
  if (yydebug)
    {
      int i;

      fprintf (stderr, "Reducing via rule %d (line %d), ",
	       yyn, yyrline[yyn]);

      /* Print the symbols being reduced, and their result.  */
      for (i = yyprhs[yyn]; yyrhs[i] > 0; i++)
	fprintf (stderr, "%s ", yytname[yyrhs[i]]);
      fprintf (stderr, " -> %s\n", yytname[yyr1[yyn]]);
    }
#endif


  switch (yyn) {

case 1:
#line 98 "lang.y"
{ thescriptcollection = makeSCRIPTCOLLECTION(yyvsp[0].toplevel); ;
    break;}
case 2:
#line 102 "lang.y"
{yyval.toplevel = NULL;;
    break;}
case 3:
#line 104 "lang.y"
{yyval.toplevel = yyvsp[0].toplevel;;
    break;}
case 4:
#line 108 "lang.y"
{yyval.toplevel = yyvsp[0].toplevel;;
    break;}
case 5:
#line 110 "lang.y"
{yyval.toplevel = yyvsp[-1].toplevel; yyval.toplevel->next = yyvsp[0].toplevel;;
    break;}
case 6:
#line 114 "lang.y"
{yyval.toplevel = makeTOPLEVELfunction(yyvsp[0].function);;
    break;}
case 7:
#line 116 "lang.y"
{yyval.toplevel = makeTOPLEVELstms(yyvsp[0].stm); ;
    break;}
case 8:
#line 118 "lang.y"
{yyval.toplevel = makeTOPLEVELevent(yyvsp[0].event); ;
    break;}
case 9:
#line 120 "lang.y"
{yyval.toplevel = makeTOPLEVELexternal(yyvsp[0].external); ;
    break;}
case 10:
#line 125 "lang.y"
{yyval.decl = makeDECLvariable(yyvsp[-3].type,yyvsp[-2].identifiers,yyvsp[-1].exp);;
    break;}
case 11:
#line 129 "lang.y"
{yyval.decl = makeDECLsimplevar(yyvsp[-1].identifier,yyvsp[0].exp);;
    break;}
case 12:
#line 134 "lang.y"
{yyval.identifiers = makeIDENTIFIER(yyvsp[0].identifier);;
    break;}
case 13:
#line 136 "lang.y"
{yyval.identifiers = makeIDENTIFIER(yyvsp[-2].identifier); yyval.identifiers->next = yyvsp[0].identifiers;;
    break;}
case 14:
#line 140 "lang.y"
{yyval.exp = NULL;;
    break;}
case 15:
#line 142 "lang.y"
{yyval.exp = yyvsp[0].exp;;
    break;}
case 16:
#line 146 "lang.y"
{yyval.type = makeTYPElocal();;
    break;}
case 17:
#line 148 "lang.y"
{yyval.type = makeTYPEglobal();;
    break;}
case 18:
#line 150 "lang.y"
{yyval.type = makeTYPEconst();;
    break;}
case 19:
#line 154 "lang.y"
{yyval.ext_type = makeEXT_TYPEbool();;
    break;}
case 20:
#line 156 "lang.y"
{yyval.ext_type = makeEXT_TYPElong();;
    break;}
case 21:
#line 158 "lang.y"
{yyval.ext_type = makeEXT_TYPElong();;
    break;}
case 22:
#line 160 "lang.y"
{yyval.ext_type = makeEXT_TYPElong();;
    break;}
case 23:
#line 162 "lang.y"
{yyval.ext_type = makeEXT_TYPEbyte();;
    break;}
case 24:
#line 164 "lang.y"
{yyval.ext_type = makeEXT_TYPEstring();;
    break;}
case 25:
#line 166 "lang.y"
{yyval.ext_type = makeEXT_TYPEfloat();;
    break;}
case 26:
#line 168 "lang.y"
{yyval.ext_type = makeEXT_TYPEdouble();;
    break;}
case 27:
#line 170 "lang.y"
{yyval.ext_type = makeEXT_TYPEmembuffer();;
    break;}
case 28:
#line 175 "lang.y"
{yyval.ext_type = NULL;;
    break;}
case 29:
#line 177 "lang.y"
{yyval.ext_type = yyvsp[0].ext_type;;
    break;}
case 30:
#line 181 "lang.y"
{yyval.ext_type = yyvsp[0].ext_type;
    break;}
case 31:
#line 183 "lang.y"
{yyval.ext_type = yyvsp[-2].ext_type; yyval.ext_type->next = yyvsp[0].ext_type;;
    break;}
case 32:
#line 187 "lang.y"
{yyval.ext_type = yyvsp[0].ext_type;;
    break;}
case 33:
#line 189 "lang.y"
{yyval.ext_type = yyvsp[-1].ext_type;;
    break;}
case 34:
#line 195 "lang.y"
{yyval.call_type = makeCALL_TYPEstdcall();;
    break;}
case 35:
#line 197 "lang.y"
{yyval.call_type = makeCALL_TYPEstdcall();;
    break;}
case 36:
#line 199 "lang.y"
{yyval.call_type = makeCALL_TYPEcdecl();;
    break;}
case 37:
#line 201 "lang.y"
{yyval.call_type = makeCALL_TYPEthiscall();;
    break;}
case 38:
#line 205 "lang.y"
{yyval.ext_type = makeEXT_TYPEvoid();;
    break;}
case 39:
#line 207 "lang.y"
{yyval.ext_type = yyvsp[0].ext_type;;
    break;}
case 42:
#line 217 "lang.y"
{yyval.external = makeEXTERNAL(yyvsp[-4].identifier, yyvsp[-7].stringconst, yyvsp[-5].ext_type, yyvsp[-2].ext_type, yyvsp[-6].call_type);;
    break;}
case 43:
#line 222 "lang.y"
{yyval.function = makeFUNCTION(yyvsp[-4].identifier, yyvsp[-2].decl, yyvsp[0].stm, 0);;
    break;}
case 44:
#line 224 "lang.y"
{yyval.function = makeFUNCTION(yyvsp[-4].identifier, yyvsp[-2].decl, yyvsp[0].stm, 1);;
    break;}
case 45:
#line 228 "lang.y"
{yyval.event = makeEVENT(yyvsp[-1].stringconst, yyvsp[0].stm);;
    break;}
case 46:
#line 234 "lang.y"
{yyval.decl = NULL;;
    break;}
case 47:
#line 236 "lang.y"
{yyval.decl = yyvsp[0].decl;;
    break;}
case 48:
#line 240 "lang.y"
{yyval.decl = yyvsp[0].decl;
    break;}
case 49:
#line 242 "lang.y"
{yyval.decl = yyvsp[-2].decl; yyval.decl->next = yyvsp[0].decl;;
    break;}
case 50:
#line 246 "lang.y"
{yyval.decl = makeDECLformal(yyvsp[0].identifier);;
    break;}
case 51:
#line 251 "lang.y"
{yyval.stm = NULL;;
    break;}
case 52:
#line 253 "lang.y"
{yyval.stm = yyvsp[-1].stm;;
    break;}
case 53:
#line 257 "lang.y"
{yyval.stm = yyvsp[0].stm;;
    break;}
case 54:
#line 259 "lang.y"
{yyval.stm = makeSTMsequence(yyvsp[-1].stm,yyvsp[0].stm);;
    break;}
case 55:
#line 264 "lang.y"
{yyval.stm = makeSTMskip();;
    break;}
case 56:
#line 266 "lang.y"
{yyval.stm = makeSTMreturn(NULL);;
    break;}
case 57:
#line 268 "lang.y"
{yyval.stm = makeSTMreturn(yyvsp[-1].exp);;
    break;}
case 58:
#line 270 "lang.y"
{yyval.stm = makeSTMif(yyvsp[-2].exp,makeSTMscope(yyvsp[0].stm));;
    break;}
case 59:
#line 272 "lang.y"
{yyval.stm = makeSTMifelse(yyvsp[-4].exp,makeSTMscope(yyvsp[-2].stm),makeSTMscope(yyvsp[0].stm));;
    break;}
case 60:
#line 274 "lang.y"
{yyval.stm = makeSTMwhile(yyvsp[-2].exp,yyvsp[0].stm);;
    break;}
case 61:
#line 276 "lang.y"
{yyval.stm = makeSTMfor(yyvsp[-6].forinit,yyvsp[-4].exp,yyvsp[-2].exp,makeSTMscope(yyvsp[0].stm));;
    break;}
case 62:
#line 278 "lang.y"
{yyval.stm = makeSTMscope(yyvsp[0].stm);;
    break;}
case 63:
#line 280 "lang.y"
{yyval.stm = makeSTMdecl(yyvsp[0].decl);;
    break;}
case 64:
#line 282 "lang.y"
{yyval.stm = makeSTMexp(yyvsp[-1].exp);;
    break;}
case 65:
#line 284 "lang.y"
{yyval.stm = makeSTMbreak();;
    break;}
case 66:
#line 286 "lang.y"
{yyval.stm = makeSTMcontinue();;
    break;}
case 67:
#line 288 "lang.y"
{yyval.stm = makeSTMswitch(yyvsp[-4].exp, yyvsp[-1].case_branch); ;
    break;}
case 68:
#line 292 "lang.y"
{yyval.forinit = NULL;;
    break;}
case 69:
#line 294 "lang.y"
{yyval.forinit = yyvsp[0].forinit;;
    break;}
case 70:
#line 298 "lang.y"
{yyval.forinit = yyvsp[0].forinit;;
    break;}
case 71:
#line 300 "lang.y"
{yyval.forinit = yyvsp[-2].forinit; yyval.forinit->next = yyvsp[0].forinit;;
    break;}
case 72:
#line 304 "lang.y"
{yyval.forinit = makeFORINITdecl(yyvsp[0].decl);;
    break;}
case 73:
#line 306 "lang.y"
{yyval.forinit = makeFORINITexp(yyvsp[0].exp);;
    break;}
case 74:
#line 312 "lang.y"
{yyval.exp = makeEXPassignment(yyvsp[-2].lvalue,yyvsp[0].exp);;
    break;}
case 75:
#line 314 "lang.y"
{yyval.exp = makeEXPequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 76:
#line 316 "lang.y"
{yyval.exp = makeEXPnequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 77:
#line 318 "lang.y"
{yyval.exp = makeEXPstrictequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 78:
#line 320 "lang.y"
{yyval.exp = makeEXPstrictnequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 79:
#line 322 "lang.y"
{yyval.exp = makeEXPless(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 80:
#line 324 "lang.y"
{yyval.exp = makeEXPgreater(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 81:
#line 326 "lang.y"
{yyval.exp = makeEXPlequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 82:
#line 328 "lang.y"
{yyval.exp = makeEXPgequals(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 83:
#line 330 "lang.y"
{yyval.exp = makeEXPplus(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 84:
#line 332 "lang.y"
{yyval.exp = makeEXPminus(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 85:
#line 334 "lang.y"
{yyval.exp = makeEXPmult(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 86:
#line 336 "lang.y"
{yyval.exp = makeEXPdiv(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 87:
#line 338 "lang.y"
{yyval.exp = makeEXPmodulo(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 88:
#line 340 "lang.y"
{yyval.exp = makeEXPand(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 89:
#line 342 "lang.y"
{yyval.exp = makeEXPor(yyvsp[-2].exp,yyvsp[0].exp);;
    break;}
case 90:
#line 344 "lang.y"
{yyval.exp = makeEXPnull();;
    break;}
case 91:
#line 346 "lang.y"
{yyval.exp = yyvsp[0].exp;;
    break;}
case 92:
#line 350 "lang.y"
{yyval.exp = makeEXPuminus(yyvsp[0].exp);;
    break;}
case 93:
#line 352 "lang.y"
{yyval.exp = makeEXPnot(yyvsp[0].exp);;
    break;}
case 94:
#line 354 "lang.y"
{yyval.exp = makeEXPincrement(yyvsp[-1].identifier);;
    break;}
case 95:
#line 356 "lang.y"
{yyval.exp = makeEXPdecrement(yyvsp[-1].identifier);;
    break;}
case 96:
#line 358 "lang.y"
{yyval.exp = makeEXPcast(yyvsp[-2].type,yyvsp[0].exp);;
    break;}
case 97:
#line 360 "lang.y"
{yyval.exp = yyvsp[0].exp;;
    break;}
case 98:
#line 364 "lang.y"
{yyval.exp = makeEXPintconst(yyvsp[0].intconst);;
    break;}
case 99:
#line 366 "lang.y"
{yyval.exp = makeEXPboolconst(yyvsp[0].boolconst);;
    break;}
case 100:
#line 368 "lang.y"
{yyval.exp = makeEXPfloatconst(yyvsp[0].floatconst);;
    break;}
case 101:
#line 370 "lang.y"
{
                    if(callbacks.Dll_ParseElement) callbacks.Dll_ParseElement(ExtData, lineno, ELEMENT_STRING, yyvsp[0].stringconst);
                    yyval.exp = makeEXPstringconst(yyvsp[0].stringconst);
                  ;
    break;}
case 102:
#line 375 "lang.y"
{yyval.exp = makeEXPlvalue(yyvsp[0].lvalue);;
    break;}
case 103:
#line 377 "lang.y"
{yyval.exp = yyvsp[-1].exp;;
    break;}
case 104:
#line 379 "lang.y"
{yyval.exp = makeEXPinvoke(yyvsp[-3].identifier,yyvsp[-1].exp);;
    break;}
case 105:
#line 381 "lang.y"
{yyval.exp = makeEXPinvokemethod(yyvsp[-5].identifier, makeEXPstringconst(yyvsp[-3].identifier), yyvsp[-1].exp);;
    break;}
case 106:
#line 383 "lang.y"
{yyval.exp = makeEXPinvokemethod(yyvsp[-6].identifier, yyvsp[-4].exp, yyvsp[-1].exp);;
    break;}
case 107:
#line 385 "lang.y"
{yyval.exp = makeEXPnew(yyvsp[0].identifier, NULL); ;
    break;}
case 108:
#line 387 "lang.y"
{yyval.exp = makeEXPnew(yyvsp[-3].identifier, yyvsp[-1].exp); ;
    break;}
case 109:
#line 391 "lang.y"
{yyval.exp = NULL;;
    break;}
case 110:
#line 393 "lang.y"
{yyval.exp = yyvsp[0].exp;;
    break;}
case 111:
#line 398 "lang.y"
{yyval.case_branch = yyvsp[0].case_branch;;
    break;}
case 112:
#line 400 "lang.y"
{yyval.case_branch = yyvsp[0].case_branch;;
    break;}
case 113:
#line 402 "lang.y"
{yyval.case_branch = yyvsp[-1].case_branch; /*$$->next = $2;*/ addDefaultCase(yyval.case_branch, yyvsp[0].case_branch); ;
    break;}
case 114:
#line 407 "lang.y"
{ yyval.case_branch = NULL; ;
    break;}
case 115:
#line 409 "lang.y"
{yyval.case_branch = yyvsp[0].case_branch;;
    break;}
case 116:
#line 411 "lang.y"
{yyval.case_branch = yyvsp[-1].case_branch; yyval.case_branch->next = yyvsp[0].case_branch;;
    break;}
case 117:
#line 416 "lang.y"
{ yyval.case_branch = makeCASEnormal(yyvsp[-2].exp, yyvsp[0].stm); ;
    break;}
case 118:
#line 418 "lang.y"
{ yyval.case_branch = makeCASEnormal(yyvsp[-1].exp, NULL); ;
    break;}
case 119:
#line 422 "lang.y"
{yyval.case_branch = NULL;;
    break;}
case 120:
#line 424 "lang.y"
{ yyval.case_branch = makeCASEdefault(yyvsp[0].stm); ;
    break;}
case 121:
#line 429 "lang.y"
{yyval.exp = yyvsp[0].exp;;
    break;}
case 122:
#line 431 "lang.y"
{yyval.exp = yyvsp[-2].exp; yyval.exp->next = yyvsp[0].exp;;
    break;}
case 123:
#line 435 "lang.y"
{yyval.lvalue = makeLVALUEid(yyvsp[0].identifier, NULL);;
    break;}
case 124:
#line 437 "lang.y"
{yyval.lvalue = makeLVALUEid(yyvsp[-2].identifier, yyvsp[0].identifier);;
    break;}
case 125:
#line 439 "lang.y"
{yyval.lvalue = makeLVALUEexp(yyvsp[-3].identifier, yyvsp[-1].exp);;
    break;}
case 126:
#line 443 "lang.y"
{yyval.lvalue = makeLVALUEexp(NULL, yyvsp[-1].exp);;
    break;}
}
   /* the action file gets copied in in place of this dollarsign */
#line 487 "bison.simple"

  yyvsp -= yylen;
  yyssp -= yylen;
#ifdef YYLSP_NEEDED
  yylsp -= yylen;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

  *++yyvsp = yyval;

#ifdef YYLSP_NEEDED
  yylsp++;
  if (yylen == 0)
    {
      yylsp->first_line = yylloc.first_line;
      yylsp->first_column = yylloc.first_column;
      yylsp->last_line = (yylsp-1)->last_line;
      yylsp->last_column = (yylsp-1)->last_column;
      yylsp->text = 0;
    }
  else
    {
      yylsp->last_line = (yylsp+yylen-1)->last_line;
      yylsp->last_column = (yylsp+yylen-1)->last_column;
    }
#endif

  /* Now "shift" the result of the reduction.
     Determine what state that goes to,
     based on the state we popped back to
     and the rule number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTBASE] + *yyssp;
  if (yystate >= 0 && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTBASE];

  goto yynewstate;

yyerrlab:   /* here on detecting error */

  if (! yyerrstatus)
    /* If not already recovering from an error, report this error.  */
    {
      ++yynerrs;

#ifdef YYERROR_VERBOSE
      yyn = yypact[yystate];

      if (yyn > YYFLAG && yyn < YYLAST)
	{
	  int size = 0;
	  char *msg;
	  int x, count;

	  count = 0;
	  /* Start X at -yyn if nec to avoid negative indexes in yycheck.  */
	  for (x = (yyn < 0 ? -yyn : 0);
	       x < (sizeof(yytname) / sizeof(char *)); x++)
	    if (yycheck[x + yyn] == x)
	      size += strlen(yytname[x]) + 15, count++;
	  msg = (char *) malloc(size + 15);
	  if (msg != 0)
	    {
	      strcpy(msg, "parse error");

	      if (count < 5)
		{
		  count = 0;
		  for (x = (yyn < 0 ? -yyn : 0);
		       x < (sizeof(yytname) / sizeof(char *)); x++)
		    if (yycheck[x + yyn] == x)
		      {
			strcat(msg, count == 0 ? ", expecting `" : " or `");
			strcat(msg, yytname[x]);
			strcat(msg, "'");
			count++;
		      }
		}
	      yyerror(msg);
	      free(msg);
	    }
	  else
	    yyerror ("parse error; also virtual memory exceeded");
	}
      else
#endif /* YYERROR_VERBOSE */
	yyerror("parse error");
    }

  goto yyerrlab1;
yyerrlab1:   /* here on error raised explicitly by an action */

  if (yyerrstatus == 3)
    {
      /* if just tried and failed to reuse lookahead token after an error, discard it.  */

      /* return failure if at end of input */
      if (yychar == YYEOF)
	YYABORT;

#if YYDEBUG != 0
      if (yydebug)
	fprintf(stderr, "Discarding token %d (%s).\n", yychar, yytname[yychar1]);
#endif

      yychar = YYEMPTY;
    }

  /* Else will try to reuse lookahead token
     after shifting the error token.  */

  yyerrstatus = 3;		/* Each real token shifted decrements this */

  goto yyerrhandle;

yyerrdefault:  /* current state does not do anything special for the error token. */

#if 0
  /* This is wrong; only states that explicitly want error tokens
     should shift them.  */
  yyn = yydefact[yystate];  /* If its default is to accept any token, ok.  Otherwise pop it.*/
  if (yyn) goto yydefault;
#endif

yyerrpop:   /* pop the current state because it cannot handle the error token */

  if (yyssp == yyss) YYABORT;
  yyvsp--;
  yystate = *--yyssp;
#ifdef YYLSP_NEEDED
  yylsp--;
#endif

#if YYDEBUG != 0
  if (yydebug)
    {
      short *ssp1 = yyss - 1;
      fprintf (stderr, "Error: state stack now");
      while (ssp1 != yyssp)
	fprintf (stderr, " %d", *++ssp1);
      fprintf (stderr, "\n");
    }
#endif

yyerrhandle:

  yyn = yypact[yystate];
  if (yyn == YYFLAG)
    goto yyerrdefault;

  yyn += YYTERROR;
  if (yyn < 0 || yyn > YYLAST || yycheck[yyn] != YYTERROR)
    goto yyerrdefault;

  yyn = yytable[yyn];
  if (yyn < 0)
    {
      if (yyn == YYFLAG)
	goto yyerrpop;
      yyn = -yyn;
      goto yyreduce;
    }
  else if (yyn == 0)
    goto yyerrpop;

  if (yyn == YYFINAL)
    YYACCEPT;

#if YYDEBUG != 0
  if (yydebug)
    fprintf(stderr, "Shifting error token, ");
#endif

  *++yyvsp = yylval;
#ifdef YYLSP_NEEDED
  *++yylsp = yylloc;
#endif

  yystate = yyn;
  goto yynewstate;
}
#line 445 "lang.y"
