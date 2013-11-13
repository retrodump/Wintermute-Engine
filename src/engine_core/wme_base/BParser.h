// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_BPARSER_H__F22A3086_3874_11D4_B319_88CB74523214__INCLUDED_)
#define AFX_BPARSER_H__F22A3086_3874_11D4_B319_88CB74523214__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


#define TOKEN_DEF_START 		\
  enum					\
  { 				\
	TOKEN_NONE = 0,
#define TOKEN_DEF(name) 		\
	TOKEN_ ## name,
#define TOKEN_DEF_END			\
	TOKEN_TOTAL_COUNT			\
  };
#define TOKEN_TABLE_START(name) 	\
  static CBParser::TokenDesc name [] =		\
  {
#define TOKEN_TABLE(name)		\
	{ TOKEN_ ## name, #name },
#define TOKEN_TABLE_END 		\
	{ 0, 0 }				\
  };

#define PARSERR_GENERIC 		-3
#define PARSERR_EOF				-2
#define PARSERR_TOKENNOTFOUND	-1



class CBParser:CBBase
{
public:
	struct TokenDesc
	{
		long	id;
		char	*token;
	};

public:
	int ScanStr (const char* in, const char* format, ...);
	char* GetLastOffender();
	void SkipToken(char** buf, char* tok, char* msg = NULL);
	int GetTokenInt(char** buf);
	float GetTokenFloat(char** buf);
	char* GetToken(char** buf);
	char* GetAssignmentText (char **buf);
	char* GetSubText (char **buf, char open, char close);
	void SkipCharacters (char **buf, const char *toSkip);
	long GetCommand (char **buf, TokenDesc * tokens, char **params);
	long GetObject (char **buf, TokenDesc * tokens, char **name, char **data);
	int m_ParserLine;
	char m_LastOffender[255];
	CBParser(CBGame* inGame=NULL);
	virtual ~CBParser();
	char* m_WhiteSpace;
};

#endif // !defined(AFX_BPARSER_H__F22A3086_3874_11D4_B319_88CB74523214__INCLUDED_)
