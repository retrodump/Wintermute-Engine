using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

// a simplistic HTML parser which breaks HTML files down into
// texts, comments, tags and tag parameters


namespace DeadCode.WME.HtmlParser
{
	public class HtmlParser
	{
		//////////////////////////////////////////////////////////////////////////
		public HtmlElement[] ParseHtml(string HtmlFile)
		{
			List<HtmlElement> Elements = new List<HtmlElement>();

			using (StreamReader sr = new StreamReader(HtmlFile))
			{
				HtmlElement El = GetElement(sr);

				while (El != null)
				{
					Elements.Add(El);
					El = GetElement(sr);
				}
			}
			return Elements.ToArray();
		}

		enum ParserState
		{
			None, InText, InTag, InComment
		};

		//////////////////////////////////////////////////////////////////////////
		HtmlElement GetElement(StreamReader sr)
		{
			int i;
			char Ch;
			string WhiteSpace = "";
			string Block = "";

			// get block
			bool Finished = false;
			ParserState State = ParserState.None;
			while ((i = sr.Peek()) >= 0 && !Finished)
			{
				Ch = (char)i;

				bool RejectChar = false;

				switch (State)
				{
					case ParserState.None:
						if (char.IsWhiteSpace(Ch)) WhiteSpace += Ch;
						else
						{
							if (Ch == '<')
							{
								State = ParserState.InTag;
								Block += Ch;
							}
							else
							{
								State = ParserState.InText;
								Block = WhiteSpace + Ch; // preserve white space we skipped
							}
						}
						break;

					case ParserState.InText:
						if (Ch == '<' || Ch == '>')
						{
							Finished = true;
							RejectChar = true;
						}
						else Block += Ch;
						break;

					case ParserState.InTag:
						// convert to comment if necessary
						if (Block + Ch == "<!--")
						{
							Block += Ch;
							State = ParserState.InComment;
						}
						else
						{
							// mismatched <
							if (Ch == '<')
							{
								RejectChar = true;
								Finished = true;
							}
							else if (Ch == '>')
							{
								Block += Ch;
								Finished = true;
							}
							else Block += Ch;
						}
						break;

					case ParserState.InComment:
						Block += Ch;
						if (Block.EndsWith("-->")) Finished = true;
						break;

				}
				if (!RejectChar) sr.Read();
			}

			// white space only
			if (State == ParserState.None && WhiteSpace != "")
			{
				State = ParserState.InText;
				Block = WhiteSpace;
			}

			switch (State)
			{
				case ParserState.InText:
					return new HtmlText(Block);

				case ParserState.InComment:
					// remove the comment chars
					Block = Block.Substring(4);
					Block = Block.Substring(0, Block.Length - 3);
					return new HtmlComment(Block);

				case ParserState.InTag:
					return ParseTag(Block);
			}
			return null;
		}

		enum TagState
		{
			InTag, InParam, AfterParam, InValue, InQuotedValue
		};

		//////////////////////////////////////////////////////////////////////////
		HtmlTag ParseTag(string Block)
		{
			HtmlTag Tag = new HtmlTag();


			// cut < >
			if (Block.StartsWith("<")) Block = Block.Substring(1);
			if (Block.EndsWith(">")) Block = Block.Substring(0, Block.Length - 1);

			string TagName = "";
			string ParamName = "";
			string ParamVal = "";
			TagState State = TagState.InTag;
			for (int i = 0; i < Block.Length; i++)
			{
				char Ch = Block[i];

				switch (State)
				{
					case TagState.InTag:
						// skip whitespace
						if (TagName == "" && char.IsWhiteSpace(Ch)) break;
						if (char.IsWhiteSpace(Ch))
						{
							State = TagState.InParam;
							ParamName = "";
							ParamVal = "";
						}
						else TagName += Ch;
						break;

					case TagState.InParam:
						// skip whitespace
						if (ParamName == "" && char.IsWhiteSpace(Ch)) break;
						if (char.IsWhiteSpace(Ch)) State = TagState.AfterParam;
						else if (Ch == '=') State = TagState.InValue;
						else ParamName += Ch;
						break;

					case TagState.AfterParam:
						if (char.IsWhiteSpace(Ch)) break;
						else if (Ch == '=') State = TagState.InValue;
						else
						{
							Tag.AddParam(ParamName, ParamVal);
							State = TagState.InParam;
							ParamName = "";
							ParamVal = "";
							ParamName += Ch;
						}
						break;

					case TagState.InValue:
						if (ParamVal == "" && (Ch == '\"' || Ch == '\''))
						{
							State = TagState.InQuotedValue;
							break;
						}
						// skip whitespace
						if (ParamVal == "" && char.IsWhiteSpace(Ch)) break;

						if (char.IsWhiteSpace(Ch))
						{
							Tag.AddParam(ParamName, ParamVal);
							State = TagState.InParam;
							ParamName = "";
							ParamVal = "";
						}
						else ParamVal += Ch;
						break;

					case TagState.InQuotedValue:
						if (Ch == '\"' || Ch == '\'')
						{
							Tag.AddParam(ParamName, ParamVal);
							State = TagState.InParam;
							ParamName = "";
							ParamVal = "";
						}
						else ParamVal += Ch;
						break;
				}
			}
			if (ParamName != "") Tag.AddParam(ParamName, ParamVal);

			if (TagName != "")
			{
				Tag.Name = TagName;
				return Tag;
			}
			else return null;
		}
	};



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public enum HtmlElementType
	{
		None, Tag, Text, Comment
	}


	//////////////////////////////////////////////////////////////////////////
	public class TagParam
	{
		public string Name = "";
		public string Value = "";

		public TagParam(string Name, string Value)
		{
			this.Name = Name;
			this.Value = Value;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	public class HtmlElement
	{
		public string Name = "";
		public HtmlElementType Type = HtmlElementType.None;
	};

	//////////////////////////////////////////////////////////////////////////
	public class HtmlTag : HtmlElement
	{
		public List<TagParam> Params = new List<TagParam>();
		public HtmlTag()
		{
			Type = HtmlElementType.Tag;
		}

		public HtmlTag(string Name)
		{
			Type = HtmlElementType.Tag;
			this.Name = Name;
		}

		public void AddParam(string Name, string Value)
		{
			if (Name == "") return;
			TagParam Param = new TagParam(Name, Value);
			Params.Add(Param);
		}

		public string GetParamValue(string ParamName)
		{
			foreach(TagParam Param in Params)
			{
				if (Param.Name.ToUpper() == ParamName.ToUpper()) return Param.Value;
			}
			return "";
		}

	};

	//////////////////////////////////////////////////////////////////////////
	public class HtmlComment : HtmlElement
	{
		public HtmlComment(string Name)
		{
			Type = HtmlElementType.Comment;
			this.Name = Name;
		}
	};

	//////////////////////////////////////////////////////////////////////////
	public class HtmlText : HtmlElement
	{
		public HtmlText(string Name)
		{
			Type = HtmlElementType.Text;
			this.Name = Name;
		}
	};

}
