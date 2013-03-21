// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace DeadCode.WME.Controls
{
	[
		ToolboxBitmap(typeof(Button)),
		DefaultProperty("ButtonColor")
	]
	public partial class ColorButton : Button
	{
		private Color buttonColor = SystemColors.ControlDark;
		private SolidBrush buttonColorBrush;

		//////////////////////////////////////////////////////////////////////////
		public ColorButton()
		{
			buttonColorBrush = new SolidBrush(buttonColor);
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		protected override void OnPaint(PaintEventArgs e)
		{
			base.OnPaint(e);

			Graphics g = e.Graphics;
			Rectangle r = ClientRectangle;

			byte border = 4;
			byte right_border = 12;

			Rectangle rc = new Rectangle(r.Left + border, r.Top + border,
										 r.Width - border - right_border - 1, r.Height - border * 2 - 1);

			g.FillRectangle(buttonColorBrush, rc);
			g.DrawRectangle(Pens.Black, rc);

			//draw the arrow
			Point p1 = new Point(r.Width - 9, r.Height / 2 - 1);
			Point p2 = new Point(r.Width - 5, r.Height / 2 - 1);
			g.DrawLine(Pens.Black, p1, p2);

			p1 = new Point(r.Width - 8, r.Height / 2);
			p2 = new Point(r.Width - 6, r.Height / 2);
			g.DrawLine(Pens.Black, p1, p2);

			p1 = new Point(r.Width - 7, r.Height / 2);
			p2 = new Point(r.Width - 7, r.Height / 2 + 1);
			g.DrawLine(Pens.Black, p1, p2);
		}

		#region Properties
		[
		Browsable(false),
		EditorBrowsable(EditorBrowsableState.Never),
		DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)
		]
		public override string Text
		{
			get
			{
				return "";
			}
			set
			{
				throw new NotSupportedException();
			}
		}
		[
		Browsable(false),
		EditorBrowsable(EditorBrowsableState.Never),
		DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)
		]
		public override Color ForeColor
		{
			get
			{
				return Color.Empty;
			}
			set
			{
				throw new NotSupportedException();
			}
		}
		[
		Browsable(true),
		Category("ButtonColor"),
		AmbientValue(typeof(Color), "Blue"),
		DefaultValue(typeof(Color), "White"),
		Description("The color of the button")
		]
		public Color ButtonColor
		{
			get
			{
				if (this.buttonColor == Color.Empty)
					return SystemColors.ControlDark;

				return this.buttonColor;
			}
			set
			{
				this.buttonColor = value;
				buttonColorBrush = new SolidBrush(buttonColor);
				this.Invalidate();
			}
		}
		#endregion
	}

}
