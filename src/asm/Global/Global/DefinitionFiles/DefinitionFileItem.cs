// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.DefinitionFileParser
{
	public class DefinitionFileItem
	{
		//////////////////////////////////////////////////////////////////////////
		protected class Token
		{
			public string Value;
			public int Line;
			public Token(string Value, int Line)
			{
				this.Value = Value;
				this.Line = Line;
			}
		};

				
		public string Name;
		public string Value;
		public int LineNum;
		public DefinitionFileItem Parent;
		public List<DefinitionFileItem> Children = new List<DefinitionFileItem>();


		//////////////////////////////////////////////////////////////////////////
		protected bool ParseStructure(ref List<Token> Tokens)
		{
			string Name;
			List<Token> Value;
			int LineNum;

			while(GetValue(ref Tokens, out Name, out Value, out LineNum))
			{
				DefinitionFileItem item = new DefinitionFileItem();
				item.Name = Name;
				item.Parent = this;
				item.LineNum = LineNum;

				Children.Add(item);
				

				string value;
				if(IsValue(Value, out value))
				{
					item.Value = value;
				}
				else
				{
					while (Value.Count > 0)
					{
						item.ParseStructure(ref Value);
					}
				}

			}
			if (Tokens.Count == 0) return true;
			else
			{
				Tokens.Clear();
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool IsValue(List<Token> Value, out string value)
		{
			value = "";
			if(Value.Count==1)
			{
				value = Value[0].Value;
				return true;
			}

			foreach(Token token in Value)
			{
				if (token.Value == "{" || token.Value == "}" || token.Value == "=") return false;
				else value += token.Value;
			}
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool GetValue(ref List<Token> Tokens, out string Name, out List<Token> Value, out int LineNum)
		{
			Name = null;
			Value = null;
			LineNum = 0;
			if (Tokens.Count < 3) return false;

			Value = new List<Token>();

			int Index = 0;
			Name = Tokens[Index++].Value;
			LineNum = Tokens[Index].Line;

			// optional =
			Token Temp = Tokens[Index++];
			if (Temp.Value == "=") Temp = Tokens[Index++];

			if(Temp.Value == "{")
			{
				int Nest = 1;
				while(Index < Tokens.Count)
				{
					Temp = Tokens[Index++];
					if (Temp.Value == "{")
					{
						Value.Add(Temp);
						Nest++;
					}
					else if(Temp.Value == "}")
					{
						Nest--;
						if (Nest == 0) break;
						Value.Add(Temp);
					}
					else Value.Add(Temp);
				}
				if (Nest != 0) throw new Exception("Syntax error");
			}
			else
			{
				Value.Add(Temp);
			}

			List<Token> NewTokens = new List<Token>();
			for(int i=Index; i<Tokens.Count; i++)
			{
				NewTokens.Add(Tokens[i]);
			}
			Tokens = NewTokens;

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetString()
		{
			if (Value != null) return Value;
			else return "";
		}

		//////////////////////////////////////////////////////////////////////////
		public int GetInt()
		{
			try
			{
				return Convert.ToInt32(Value);
			}
			catch
			{
				return 0;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public double GetDouble()
		{
			try
			{
				return Convert.ToDouble(Value);
			}
			catch
			{
				return 0.0f;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GetBool()
		{
			try
			{
				if (Value == null) return false;
				else return Value.ToLower() == "true" || Value.ToLower() == "yes" || Value.ToLower() == "true" || GetInt() > 0;
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public int[] GetIntArray()
		{
			List<int> IntList = new List<int>();
			string[] StrList = Value.Split(new char[] { ',' });
			
			foreach(string Str in StrList)
			{
				int i;
				try
				{
					i = Convert.ToInt32(Str.Trim());
				}
				catch
				{
					i = 0;
				}
				IntList.Add(i);
			}
			return IntList.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		public override string ToString()
		{
			string Ret;
			Ret = Name;
			if (Children.Count > 0) Ret += "{ " + Children.Count.ToString() + " }";
			else Ret += "=" + Value;

			return Ret;
		}
	}
}
