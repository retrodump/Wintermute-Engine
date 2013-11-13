using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Drawing.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Controls
{
	//////////////////////////////////////////////////////////////////////////
	public class ComplexText
	{
		//////////////////////////////////////////////////////////////////////////
		private class TextElement
		{
			public enum ElementType
			{
				Text,
				Image,
				Space
			};

			public SizeF Size;

			private ElementType Type = ElementType.Space;			
			private string Text;
			private Color Color;
			private Font Font;
			private Image Image;

			//////////////////////////////////////////////////////////////////////////
			public TextElement(Graphics G, Image Image)
			{
				this.Type = ElementType.Image;
				this.Image = Image;
				this.Size = new SizeF(Image.Width, Image.Height);
			}

			//////////////////////////////////////////////////////////////////////////
			public TextElement(Graphics G, string Text, Font Font, Color Color, bool Bold)
			{
				this.Type = ElementType.Text;
				this.Text = Text;
				this.Color = Color;
				this.Font = Bold ? new Font(Font, FontStyle.Bold) : Font;

				Size S = TextRenderer.MeasureText(G, this.Text, this.Font, new Size(int.MaxValue, int.MaxValue), TextFormatFlags.NoPadding);
				this.Size = new SizeF(S.Width, S.Height);
			}

			//////////////////////////////////////////////////////////////////////////
			public TextElement(Graphics G, float Width, float Height)
			{
				this.Type = ElementType.Space;
				this.Size = new SizeF(Width, Height);
			}

			//////////////////////////////////////////////////////////////////////////
			public void Paint(Graphics G, float X, float Y)
			{
				switch (Type)
				{
					case ElementType.Text:
						TextRenderer.DrawText(G, Text, Font, new Point((int)X, (int)Y), this.Color, TextFormatFlags.NoPadding);
						break;

					case ElementType.Image:
						G.DrawImage(Image, X, Y, Image.Width, Image.Height);
						break;

					case ElementType.Space:
						break;
				}
			}
		};

		//////////////////////////////////////////////////////////////////////////
		private class TextLine
		{
			private List<TextElement> Elements = new List<TextElement>();

			//////////////////////////////////////////////////////////////////////////
			public void AddElement(TextElement Element)
			{
				Elements.Add(Element);
			}

			//////////////////////////////////////////////////////////////////////////
			public float Width
			{
				get
				{
					float Ret = 0.0f;
					foreach(TextElement El in Elements)
					{
						Ret += El.Size.Width;
					}
					return Ret;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			public float Height
			{
				get
				{
					float Ret = 0.0f;
					foreach(TextElement El in Elements)
					{
						Ret = Math.Max(El.Size.Height, Ret);
					}
					return Ret;
				}
			}

			//////////////////////////////////////////////////////////////////////////
			public void Paint(Graphics G, float Y)
			{
				float X = 0;
				foreach(TextElement El in Elements)
				{
					El.Paint(G, X, Y);
					X += El.Size.Width;
				}
			}
		};

		private List<TextLine> Lines = new List<TextLine>();
		private TextLine CurrentLine = null;

		//////////////////////////////////////////////////////////////////////////
		public void NewLine()
		{
			CurrentLine = new TextLine();
			Lines.Add(CurrentLine);
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddElement(TextElement Element)
		{
			if (CurrentLine == null) NewLine();
			CurrentLine.AddElement(Element);
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddElement(Graphics G, Image Image)
		{
			AddElement(new TextElement(G, Image));
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddElement(Graphics G, string Text, Font Font, Color Color, bool Bold)
		{
			AddElement(new TextElement(G, Text, Font, Color, Bold));
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddElement(Graphics G, float Width, float Height)
		{
			AddElement(new TextElement(G, Width, Height));
		}

		//////////////////////////////////////////////////////////////////////////
		public void Paint(Graphics G)
		{
			float Y = 0.0f;
			foreach(TextLine Line in Lines)
			{
				Line.Paint(G, Y);
				Y += Line.Height;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public float Width
		{
			get
			{
				float Ret = 0.0f;
				foreach(TextLine Line in Lines)
				{
					Ret = Math.Max(Ret, Line.Width);
				}
				return Ret;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public float Height
		{
			get
			{
				float Ret = 0.0f;
				foreach (TextLine Line in Lines)
				{
					Ret += Line.Height;
				}
				return Ret;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		static public SizeF MeasureDisplayStringWidth(Graphics graphics, string text, Font font)
		{
			if (text == "") return new SizeF(0, 0);

			System.Drawing.StringFormat format = new System.Drawing.StringFormat();
			System.Drawing.RectangleF rect = new System.Drawing.RectangleF(0, 0, 1000, 1000);
			System.Drawing.CharacterRange[] ranges = 
									   { new System.Drawing.CharacterRange(0, 
															   text.Length) };
			System.Drawing.Region[] regions = new System.Drawing.Region[1];

			format.SetMeasurableCharacterRanges(ranges);

			regions = graphics.MeasureCharacterRanges(text, font, rect, format);
			rect = regions[0].GetBounds(graphics);

			return new SizeF(rect.Right + 1.0f, rect.Bottom);
		}
	};


}
