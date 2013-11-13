// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace DeadCode.WME.DefinitionFileParser
{
	public class DefinitionFile : DefinitionFileItem
	{
		//////////////////////////////////////////////////////////////////////////
		public bool ParseFile(string Filename)
		{
			List<Token> tokens = new List<Token>();

			StreamReader reader = null;
			try
			{
				reader = new StreamReader(Filename, Encoding.Default);
				string line;
				bool inString = false;
				string token = "";
				int lineNum = 0;
				while ((line = reader.ReadLine()) != null)
				{
					lineNum++;

					if (line.TrimStart().StartsWith(";")) continue;
					ParseTokens(lineNum, line + "\n", tokens, ref inString, ref token);
				}
				if (token != "") tokens.Add(new Token(token, lineNum));

				reader.Close();
				reader = null;
			}
			catch
			{
				if (reader != null) reader.Close();
				return false;
			}

			try
			{
				return ParseStructure(ref tokens);
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private bool ParseTokens(int LineNum, string Line, List<Token> Tokens, ref bool InString, ref string Token)
		{
			for(int i=0; i<Line.Length; i++)
			{
				char c = Line[i];

				if(InString)
				{
					if(c=='\"')
					{
						InString = false;
						
						Tokens.Add(new Token(Token, LineNum));
						Token = "";
					}
					else Token += c;
				}
				else
				{
					if (char.IsLetterOrDigit(c) || c == '_' || c == '-') Token += c;
					else
					{
						if (Token != "") Tokens.Add(new Token(Token, LineNum));
						Token = "";

						if (char.IsWhiteSpace(c))
						{
							continue;
						}
						else if (c == '\"')
						{
							InString = true;
						}
						else Tokens.Add(new Token(new string(c, 1), LineNum));
					}

				}
			}
			return true;
		}
	}
}
