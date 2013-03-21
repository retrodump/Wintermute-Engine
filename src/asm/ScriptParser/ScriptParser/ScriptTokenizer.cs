using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using DeadCode.WME.Global;

namespace DeadCode.WME.ScriptParser
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptTokenizer
	{
		private TextReader Reader;
		public List<ScriptToken> Tokens = new List<ScriptToken>();
		public bool ScanIncludes = true;

		//////////////////////////////////////////////////////////////////////////
		public ScriptTokenizer(TextReader Reader):this(Reader, true)
		{
		}
		//////////////////////////////////////////////////////////////////////////
		public ScriptTokenizer(TextReader Reader, bool ScanIncludes)
		{
			if(Reader==null)
			{
				throw new System.ApplicationException("Error: Bad input stream initializer.");
			}
			this.Reader = Reader;
			this.ScanIncludes = ScanIncludes;
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptTokenizer(FileStream InStream):this(InStream, true)
		{			
		}
		//////////////////////////////////////////////////////////////////////////
		public ScriptTokenizer(FileStream InStream, bool ScanIncludes)
		{
			if(InStream==null)
			{
				throw new System.ApplicationException("Error: Bad input stream initializer.");
			}
			this.Reader = new StreamReader(InStream, Encoding.Default, true);
			this.ScanIncludes = ScanIncludes;
		}

		private enum State
		{
			Default,
			LineComment,
			BlockComment,
			String,
			Operator
		}


		public String Filename = "";

		private int CurrentLine = 0;
		private int CurrentCol = 0;
		//////////////////////////////////////////////////////////////////////////
		public bool GetTokens()
		{
			Tokens.Clear();

			char Ch1='\0', Ch2='\0';
			State CurrState = State.Default;
			int CommentNest = 0;
			String CurrToken = "";


			CurrentLine = 1;
			CurrentCol = 0;
			char PrevChar = '\0';

			int StartLine = 0;
			int StartCol = 0;

			bool PrevProcessed = true;
			while(true)
			{
				if (PrevProcessed)
				{
					int i = Reader.Read();
					if (i < 0) break;

					CurrentCol++;

					Ch1 = (char)i;
					if (Reader.Peek() >= 0) Ch2 = (char)Reader.Peek();
					else Ch2 = '\0';

					if (Ch1 == '\n')
					{
						CurrentLine++;
						CurrentCol = 0;
					}
				}
				
				PrevProcessed = true;
				switch(CurrState)
				{
					case State.Default:
						if (char.IsWhiteSpace(Ch1))
						{
							StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
						}

						else if (Ch1 == '/' && Ch2 == '*')
						{
							CurrState = State.BlockComment;
							CommentNest++;
							Reader.Read();
							CurrentCol++;

							StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
						}

						else if (Ch1 == '/' && Ch2 == '/')
						{
							CurrState = State.LineComment;
							Reader.Read();
							CurrentCol++;

							StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
						}

						else if (Ch1 == '\"')
						{
							CurrState = State.String;

							StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
							CurrToken += Ch1;
						}

						else if (IsOperatorChar(Ch1))
						{
							CurrState = State.Operator;

							StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";

							if (CurrToken == "")
							{
								StartLine = CurrentLine;
								StartCol = CurrentCol;
							}
							CurrToken += Ch1;
						}

						else
						{
							if (CurrToken == "")
							{
								StartLine = CurrentLine;
								StartCol = CurrentCol;
							}
							CurrToken += Ch1;
						}

						break;

					case State.String:
						if (Ch1 == '\"' || Ch1 == '\n')
						{
							if (PrevChar == '~') CurrToken += Ch1;
							else
							{
								CurrToken += Ch1;
								CurrState = State.Default;
								StoreToken(CurrToken, ScriptToken.TokenType.String, StartLine, StartCol, CurrentLine, CurrentCol);
								CurrToken = "";
							}
						}
						else
						{
							if (CurrToken == "")
							{
								StartLine = CurrentLine;
								StartCol = CurrentCol;
							}
							CurrToken += Ch1;
						}

						break;

					case State.LineComment:
						if (Ch1 == '\n')
						{
							CurrState = State.Default;
							StoreToken(CurrToken, ScriptToken.TokenType.Comment, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
						}
						else
						{
							if (CurrToken == "")
							{
								StartLine = CurrentLine;
								StartCol = CurrentCol;
							}
							CurrToken += Ch1;
						}

						break;

					case State.BlockComment:
						if (Ch1 == '/' && Ch2 == '*')
						{
							CommentNest++;
							Reader.Read();
							CurrentCol++;
						}
						else if (Ch1 == '*' && Ch2 == '/')
						{
							CommentNest--;
							Reader.Read();
							CurrentCol++;

							if (CommentNest < 0) CommentNest = 0;
							if (CommentNest == 0)
							{
								CurrState = State.Default;
								StoreToken(CurrToken, ScriptToken.TokenType.Comment, StartLine, StartCol, CurrentLine, CurrentCol-1);
								CurrToken = "";
							}
						}
						else
						{
							if (CurrToken == "")
							{
								StartLine = CurrentLine;
								StartCol = CurrentCol;
							}
							CurrToken += Ch1;
						}

						break;

					case State.Operator:
						if(IsOperatorChar(Ch1))
						{
							if(IsOperator(CurrToken + Ch1 + Ch2))
							{
								if (CurrToken == "")
								{
									StartLine = CurrentLine;
									StartCol = CurrentCol;
								}
								CurrToken = CurrToken + Ch1 + Ch2;
								CurrState = State.Default;
								StoreToken(CurrToken, ScriptToken.TokenType.Operator, StartLine, StartCol, CurrentLine, CurrentCol+2);
								CurrToken = "";
								Reader.Read();
								CurrentCol++;
							}
							else if (IsOperator(CurrToken + Ch1))
							{
								if (CurrToken == "")
								{
									StartLine = CurrentLine;
									StartCol = CurrentCol;
								}
								CurrToken += Ch1;
								CurrState = State.Default;
								StoreToken(CurrToken, ScriptToken.TokenType.Operator, StartLine, StartCol, CurrentLine, CurrentCol+1);
								CurrToken = "";
							}
							else
							{
								CurrState = State.Default;
								StoreToken(CurrToken, ScriptToken.TokenType.Operator, StartLine, StartCol, CurrentLine, CurrentCol);
								CurrToken = "";
								PrevProcessed = false;
							}

						}
						else
						{
							CurrState = State.Default;
							StoreToken(CurrToken, ScriptToken.TokenType.Operator, StartLine, StartCol, CurrentLine, CurrentCol);
							CurrToken = "";
							PrevProcessed = false;
						}

						break;

				}
				PrevChar = Ch1;
			}
			if(CurrState==State.BlockComment)
				StoreToken(CurrToken, ScriptToken.TokenType.Comment, StartLine, StartCol, CurrentLine, CurrentCol + 1);
			else
				StoreToken(CurrToken, StartLine, StartCol, CurrentLine, CurrentCol+1);
			
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void StoreToken(string TokenStr, int StartLine, int StartCol, int EndLine, int EndCol)
		{
			StoreToken(TokenStr, ScriptToken.TokenType.Unknown, StartLine, StartCol, EndLine, EndCol);
		}

		//////////////////////////////////////////////////////////////////////////
		private void StoreToken(string TokenStr, ScriptToken.TokenType Type, int StartLine, int StartCol, int EndLine, int EndCol)
		{
			if (TokenStr == "") return;

			if(Type==ScriptToken.TokenType.Unknown)
			{
				if (IsKeyword(TokenStr)) Type = ScriptToken.TokenType.Keyword;
				else if (IsOperator(TokenStr)) Type = ScriptToken.TokenType.Operator;
				else if (char.IsNumber(TokenStr[0])) Type = ScriptToken.TokenType.Number;
				else Type = ScriptToken.TokenType.Identifier;
			}
			ScriptToken Token = new ScriptToken(TokenStr, Type, Filename, StartLine, StartCol, EndLine, EndCol);
			Tokens.Add(Token);

			// handle #includes
			if(Type==ScriptToken.TokenType.String && Tokens.Count > 1)
			{
				ScriptToken PrevToken = Tokens[Tokens.Count - 2];
				if(PrevToken.Type==ScriptToken.TokenType.Identifier && PrevToken.Value=="#include")
				{
					String IncPath = FileManager.GetFullPath(Token.Value.Trim(new char[] { '\"' }));
					if (IncPath != null) ReadInclude(IncPath);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptToken GetTokenAt(int Line, int Col)
		{
			foreach(ScriptToken Token in Tokens)
			{
				if((Token.StartLine<Line || (Token.StartLine==Line && Token.StartCol<=Col)) &&
				   (Token.EndLine>Line || (Token.EndLine==Line && Token.EndCol>=Col)))
				{
					return Token;
				}
			}
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		public int GetTokenIndexAt(int Line, int Col)
		{
			for(int i=0; i<Tokens.Count; i++)
			{
				ScriptToken Token = Tokens[i];
				if ((Token.StartLine < Line || (Token.StartLine == Line && Token.StartCol <= Col)) &&
				   (Token.EndLine > Line || (Token.EndLine == Line && Token.EndCol >= Col)))
				{
					return i;
				}
			}
			return -1;
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptToken GetNearestTokenAt(int Line, int Col)
		{
			ScriptToken PrevToken = null;
			foreach (ScriptToken Token in Tokens)
			{
				if (PrevToken != null)
				{
					if ((Token.StartLine > Line || (Token.StartLine == Line && Token.StartCol > Col)))
					{
						return PrevToken;
					}
				}
				PrevToken = Token;
			}
			return PrevToken;
		}

		//////////////////////////////////////////////////////////////////////////
		public int GetNearestLeftTokenIndexAt(int Line, int Col)
		{
			int PrevTokenIndex = -1;
			for (int i = 0; i < Tokens.Count; i++)
			{
				ScriptToken Token = Tokens[i];
				if (PrevTokenIndex >= 0)
				{
					if ((Token.StartLine > Line || (Token.StartLine == Line && Token.StartCol >= Col)))
					{
						return PrevTokenIndex;
					}
				}
				PrevTokenIndex = i;
			}
			return PrevTokenIndex;
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptToken GetPrevToken(ScriptToken Token)
		{
			if (Token == null) return null;
			int Index = Tokens.IndexOf(Token) - 1;
			
			if (Index < 0) return null;
			else return Tokens[Index];
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptToken GetNextToken(ScriptToken Token)
		{
			if (Token == null) return null;
			int Index = Tokens.IndexOf(Token);
			if (Index < 0) return null;

			Index++;
			if (Index < Tokens.Count) return Tokens[Index];
			else return null;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool ReadInclude(String IncPath)
		{
			try
			{
				using (StreamReader sr = new StreamReader(IncPath))
				{
					ScriptTokenizer Tokenizer = new ScriptTokenizer(sr, ScanIncludes);
					Tokenizer.Filename = IncPath;

					Tokenizer.GetTokens();
					
					//Tokens.RemoveAt(Tokens.Count - 1);
					//Tokens.RemoveAt(Tokens.Count - 1);

					foreach (ScriptToken Token in Tokenizer.Tokens)
					{
						Token.StartLine = CurrentLine;
						Token.EndLine = CurrentLine;
						Token.StartCol = 0;
						Token.EndCol = 0;
						Tokens.Add(Token);
					}
				}
				return true;
			}
			catch
			{
				return false;
			}
			
		}
		
#region Keyword definitions
		public static String[] Keywords;
		public static String[] Operators;
		public static char[] OperatorChars;
		//////////////////////////////////////////////////////////////////////////
		static ScriptTokenizer()
		{
			Keywords = new String[]
			{
				"external",
				"bool",
				"long",
				"short",
				"byte",
				"string",
				"float",
				"double",
				"int",
				"membuffer",
				"stdcall",
				"cdecl",
				"thiscall",
				"switch",
				"case",
				"default",
				"else",
				"for",
				"if",
				"return",
				"while",
				"function",
				"method",
				"var",
				"global",
				"const",
				"null",
				"break",
				"continue",
				"new",
				"on",
				"true",
				"false",
				"this"
			};
			Array.Sort(Keywords);

			Operators = new String[]
			{
				"&&",
				"==",
				"===",
				">=",
				"<=",
				"!=",
				"!==",
				"||",
				"=",
				">",
				"<",
				"!",
				"+",
				"-",
				"*",
				"/",
				"%",
				"{",
				"}",
				";",
				"(",
				")",
				",",
				".",
				"[",
				"]",
				":"
			};
			Array.Sort(Operators);

			OperatorChars = new char[]
			{
				'%', '&', '*', '(', ')', '-', '+', '=', '|', '{', '}',
				'[', ']', ':', ';', '<', '>', ',', '/', '?', '!', '.'
			};
			Array.Sort(OperatorChars);
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool IsKeyword(String TestWord)
		{
			return Array.BinarySearch(Keywords, TestWord) >= 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool IsOperator(String TestWord)
		{
			return Array.BinarySearch(Operators, TestWord) >= 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool IsOperatorChar(char TestChar)
		{
			return Array.BinarySearch(OperatorChars, TestChar) >= 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool IsWordStart(char TestChar)
		{
			return (TestChar < 128) && (char.IsLetter(TestChar) || TestChar == '_');
		}

#endregion
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptToken
	{
		public enum TokenType
		{
			Unknown, String, Keyword, Operator, Identifier, Comment, Number
		}

		public TokenType Type = TokenType.Unknown;
		public String Value = "";
		public int OrigLine = 0;
		public String Filename = "";
		public int StartLine = 0;
		public int StartCol = 0;
		public int EndLine = 0;
		public int EndCol = 0;

		//////////////////////////////////////////////////////////////////////////
		internal ScriptToken(String Value, TokenType Type, String Filename, int StartLine, int StartCol, int EndLine, int EndCol)
		{
			this.OrigLine = StartLine;
			this.Value = Value;
			this.Type = Type;
			this.Filename = Filename;
			this.StartLine = StartLine;
			this.StartCol = StartCol;
			this.EndLine = EndLine;
			this.EndCol = EndCol;
		}

		//////////////////////////////////////////////////////////////////////////
		public override string ToString()
		{
			String Str = "";
			switch(Type)
			{
				case TokenType.String:
					Str += "[str]";
					break;
				case TokenType.Keyword:
					Str += "[kwd]";
					break;
				case TokenType.Operator:
					Str += "[ope]";
					break;
				case TokenType.Identifier:
					Str += "[idf]";
					break;
				case TokenType.Comment:
					Str += "[com]";
					break;
				case TokenType.Number:
					Str += "[num]";
					break;
				default:
					Str += "[unk]";
					break;
			}
			return	StartLine.ToString() + "," + StartCol.ToString() + "-" +
					EndLine.ToString() + "," + EndCol.ToString() + " " +
					Str + " " + Value;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool IsKeyword
		{
			get
			{
				return Type == TokenType.Keyword;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public bool IsComment
		{
			get
			{
				return Type == TokenType.Comment;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public bool IsString
		{
			get
			{
				return Type == TokenType.String;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public bool IsIdentifier
		{
			get
			{
				return Type == TokenType.Identifier;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public bool IsOperator
		{
			get
			{
				return Type == TokenType.Operator;
			}
		}
		//////////////////////////////////////////////////////////////////////////
		public bool IsNumber
		{
			get
			{
				return Type == TokenType.Number;
			}
		}



	};
}
