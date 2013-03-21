using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace DeadCode.WME.ScriptParser
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptParser
	{
		public bool ScanIncludes = true;

		private TextReader Reader;

		public List<ScriptFunction> Functions;
		public List<ScriptEventHandler> EventHandlers;

		private ScriptScope GlobalScope;
		private List<ScriptScope> Scopes;
		
		private ScriptTokenizer _Tokenizer;
		public ScriptTokenizer Tokenizer
		{
			get
			{
				return _Tokenizer;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Parse(FileStream InStream, bool ScanIncludes)
		{
			if (InStream == null)
			{
				throw new System.ApplicationException("Error: Bad input stream initializer.");
			}
			this.Reader = new StreamReader(InStream);
			this.ScanIncludes = ScanIncludes;

			return Parse();
		}
		//////////////////////////////////////////////////////////////////////////
		public bool Parse(FileStream InStream)
		{
			return Parse(InStream, true);
		}


		//////////////////////////////////////////////////////////////////////////
		public bool Parse(TextReader Reader, bool ScanIncludes)
		{
			if (Reader == null)
			{
				throw new System.ApplicationException("Error: Bad input stream initializer.");
			}
			this.Reader = Reader;
			this.ScanIncludes = ScanIncludes;

			return Parse();
		}
		//////////////////////////////////////////////////////////////////////////
		public bool Parse(TextReader Reader)
		{
			return Parse(Reader, true);
		}

		//////////////////////////////////////////////////////////////////////////
		private bool Parse()
		{
			// cleanup
			GlobalScope = new ScriptScope();
			Scopes = new List<ScriptScope>();
			Functions = new List<ScriptFunction>();
			EventHandlers = new List<ScriptEventHandler>();

			_Tokenizer = new ScriptTokenizer(Reader);
			if (!_Tokenizer.GetTokens()) return false;

			ScriptScope CurrentScope = GlobalScope;
						
			bool ReadingBlock = false;
			int BlockNest = 0;
			
			List<ScriptToken>.Enumerator TokenEnum = _Tokenizer.Tokens.GetEnumerator();			
			while(TokenEnum.MoveNext())
			{
				ScriptToken Token = TokenEnum.Current;
				if (Token.IsComment) continue;

				// functions
				if (Token.IsKeyword && (Token.Value == "function" || Token.Value == "method" || Token.Value == "external"))
				{
					ScriptScope Scope = ParseFunction(ref TokenEnum);
					if(Scope != null)
					{
						if(ReadingBlock)
						{
							CurrentScope.EndLine = Token.StartLine;
							CurrentScope.EndCol = Token.StartCol;
						}

						CurrentScope = Scope;
						Scopes.Add(Scope);
						ReadingBlock = true;
						BlockNest = 0;
					}
				}

				// event handles
				else if(Token.IsKeyword && Token.Value=="on")
				{
					ScriptScope Scope = ParseEventHandler(ref TokenEnum);
					if (Scope != null)
					{
						if (ReadingBlock)
						{
							CurrentScope.EndLine = Token.StartLine;
							CurrentScope.EndCol = Token.StartCol;
						}

						CurrentScope = Scope;
						Scopes.Add(Scope);
						ReadingBlock = true;
						BlockNest = 0;
					}
				}

				// variables
				else if(Token.IsKeyword && (Token.Value=="var" || Token.Value=="global" || Token.Value=="const"))
				{
					ScriptVariable Var = ParseVariable(ref TokenEnum, false);
					if(Var!=null && Var.Name!="")
					{
						if(ReadingBlock && BlockNest>0)	CurrentScope.Variables.Add(Var);
						else GlobalScope.Variables.Add(Var);
					}
				}

				// curly braces
				else if(Token.IsOperator)
				{
					if (Token.Value == "{" && ReadingBlock) BlockNest++;
					else if(Token.Value=="}" && ReadingBlock)
					{
						BlockNest--;
						if (BlockNest <= 0)
						{
							ReadingBlock = false;
							CurrentScope.EndLine = Token.EndLine;
							CurrentScope.EndCol = Token.EndCol;
						}
					}
					else if(Token.Value==";" && ReadingBlock && BlockNest==0)
					{
						ReadingBlock = false;
						CurrentScope.EndLine = Token.EndLine;
						CurrentScope.EndCol = Token.EndCol;
					}

				}
			}

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private ScriptScope ParseFunction(ref List<ScriptToken>.Enumerator TokenEnum)
		{			
			ScriptToken Token = TokenEnum.Current;
			if(Token==null) return null;

			// setup function
			ScriptFunction Func = new ScriptFunction();
			Func.Signature = Token.Value + " ";
			Func.Line = Token.OrigLine;
			Func.Filename = Token.Filename;
			switch(Token.Value)
			{
				case "function":
					Func.Type = ScriptFunctionType.Function;
					break;
				case "method":
					Func.Type = ScriptFunctionType.Method;
					break;
				case "external":
					Func.Type = ScriptFunctionType.External;
					break;
				default:
					Func.Type = ScriptFunctionType.Unknown;
					break;
			}

			// setup scope
			ScriptScope Scope = new ScriptScope();
			Scope.Owner = Func;
			Scope.StartLine = Token.StartLine;
			Scope.StartCol = Token.StartCol;

			bool ReadingName = true;
			bool ReadingParams = false;

			while (TokenEnum.MoveNext())
			{
				Token = TokenEnum.Current;
				if (Token.IsComment) continue;

				if (ReadingName)
				{
					if (!Token.IsIdentifier)
					{
						if(Func.Type==ScriptFunctionType.External)
						{
							Func.Signature += Token.Value + " ";
						}
						else return null;
					}
					else
					{
						Func.Name = Token.Value;
						Func.Signature += Token.Value;

						ReadingName = false;
					}
				}
				else if(ReadingParams)
				{
					if (Token.IsOperator && Token.Value == ")")
					{
						Func.Signature += Token.Value;
						break;
					}
					else if(Token.IsOperator && Token.Value==",")
					{
						Func.Signature += Token.Value + " ";
					}
					else
					{
						ScriptVariable Var = ParseVariable(ref TokenEnum, true);
						if (Var == null) return null;
						else
						{
							Func.Signature += Var.Signature;
							Func.Params.Add(Var.Signature);

							if(Var.Name!="") Scope.Variables.Add(Var);

							// hack because of unnamed external's parameters
							Token = TokenEnum.Current;
							if(Token.IsOperator && Token.Value==")")
							{
								Func.Signature += Token.Value;
								break;
							}
						}
					}
				}
				else
				{
					if (Token.IsOperator && Token.Value == "(")
					{
						ReadingParams = true;
						Func.Signature += Token.Value;
					}
					else return null;
				}
			}

			Functions.Add(Func);
			return Scope;
		}

		//////////////////////////////////////////////////////////////////////////
		private ScriptScope ParseEventHandler(ref List<ScriptToken>.Enumerator TokenEnum)
		{
			ScriptToken Token = TokenEnum.Current;
			if (Token == null) return null;

			// setup handler
			ScriptEventHandler Handler = new ScriptEventHandler();
			Handler.Signature = Token.Value + " ";
			Handler.Line = Token.OrigLine;
			Handler.Filename = Token.Filename;

			// setup scope
			ScriptScope Scope = new ScriptScope();
			Scope.Owner = Handler;
			Scope.StartLine = Token.StartLine;
			Scope.StartCol = Token.StartCol;

			while (TokenEnum.MoveNext())
			{
				Token = TokenEnum.Current;
				if (Token.IsComment) continue;

				if (Token.IsString)
				{
					Handler.Signature += Token.Value;
					Handler.Name = Token.Value.Trim(new char[] { '\"' });
					break;
				}
				else return null;
			}
			EventHandlers.Add(Handler);
			return Scope;
		}


		//////////////////////////////////////////////////////////////////////////
		private ScriptVariable ParseVariable(ref List<ScriptToken>.Enumerator TokenEnum, bool Param)
		{
			ScriptToken Token = TokenEnum.Current;
			if (Token == null) return null;

			ScriptVariable Var = new ScriptVariable();
			Var.Line = Token.OrigLine;
			Var.Filename = Token.Filename;

			if (Param) Var.Type = ScriptVariableType.Parameter;
			else
			{
				switch (Token.Value)
				{
					case "var":
						Var.Type = ScriptVariableType.Var;
						break;
					case "global":
						Var.Type = ScriptVariableType.Global;
						break;
					case "const":
						Var.Type = ScriptVariableType.Const;
						break;
					default:
						Var.Type = ScriptVariableType.Unknown;
						break;
				}
			}

			while(true)
			{
				if(!Token.IsComment)
				{
					if (Token.IsIdentifier)
					{
						if (Var.Name == "") Var.Name = Token.Value;
						Var.Signature += Token.Value + " ";
						if (Param) break;
					}
					else if (Token.IsOperator && (Token.Value == ";" || Token.Value == ","))
					{
						break;
					}
					else if (Token.IsOperator && Param && Token.Value == ")")
					{
						break;
					}

					else
					{
						Var.Signature += Token.Value + " ";
					}
				}
				

				if (!TokenEnum.MoveNext()) return null;
				Token = TokenEnum.Current;
			}

			Var.Signature = Var.Signature.Trim();
			if (Var.Signature != "")
			{
				Var.ValidAfterLine = Token.EndLine;
				Var.ValidAfterCol = Token.EndCol;

				return Var;
			}
			else return null;
		}


		//////////////////////////////////////////////////////////////////////////
		public ScriptScope GetScope(int Line, int Col)
		{
			foreach(ScriptScope Scope in Scopes)
			{
				if (Scope.PointInScope(Line, Col)) return Scope;
			}
			return GlobalScope;
		}

		//////////////////////////////////////////////////////////////////////////
		public ScriptVariable[] GetVariables(int Line, int Col)
		{
			List<ScriptVariable> Vars = new List<ScriptVariable>();
			if(GlobalScope!=null)
			{
				foreach(ScriptVariable Var in GlobalScope.Variables)
				{
					if(Var.ValidAfterLine<Line || (Var.ValidAfterLine==Line && Var.ValidAfterCol<=Col))
					{
						AddVar(Vars, Var);
					}
				}
			}

			foreach (ScriptScope Scope in Scopes)
			{
				if (Scope.PointInScope(Line, Col))
				{
					foreach (ScriptVariable Var in Scope.Variables)
					{
						if (Var.ValidAfterLine < Line || (Var.ValidAfterLine == Line && Var.ValidAfterCol <= Col))
						{
							AddVar(Vars, Var);
						}
					}
					break;
				}
			}

			return Vars.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddVar(List<ScriptVariable> Vars, ScriptVariable Var)
		{
			// delete all previous instances first
			for(int i=0; i<Vars.Count; i++)
			{
				if(Vars[i].Name==Var.Name)
				{
					Vars.RemoveAt(i);
					i--;
				}
			}
			Vars.Add(Var);
		}
	};
	
	

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptScope
	{
		public int StartLine = 0;
		public int StartCol = 0;
		public int EndLine = 0;
		public int EndCol = 0;

		public List<ScriptVariable> Variables = new List<ScriptVariable>();
		public ScriptElement Owner = null;

		//////////////////////////////////////////////////////////////////////////
		public bool PointInScope(int Line, int Col)
		{
			if (Line < StartLine || Line > EndLine) return false;
			if (Line == StartLine && Col < StartCol) return false;
			if (Line == EndLine && Col > EndCol) return false;
			return true;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public abstract class ScriptElement
	{
		public String Name = "";
		public String Signature = "";
		public String Filename = "";
		public int Line = 0;

		//////////////////////////////////////////////////////////////////////////
		public override string ToString()
		{
			return Signature;
		}

	};

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptVariable : ScriptElement
	{
		public int ValidAfterLine = 0;
		public int ValidAfterCol = 0;
		public ScriptVariableType Type = ScriptVariableType.Unknown;
	};


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptFunction : ScriptElement
	{
		public ScriptFunctionType Type = ScriptFunctionType.Unknown;
		public List<String> Params = new List<String>();
	};

	//////////////////////////////////////////////////////////////////////////
	public enum ScriptFunctionType
	{
		Unknown, Function, Method, External
	};

	//////////////////////////////////////////////////////////////////////////
	public enum ScriptVariableType
	{
		Unknown, Var, Global, Const, Parameter
	};


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptEventHandler : ScriptElement
	{
	};

}