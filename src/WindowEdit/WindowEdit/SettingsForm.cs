// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Controls;

namespace DeadCode.WME.WindowEdit
{
	public partial class SettingsForm : Form
	{
		public SettingsForm()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnGridLeave(object sender, EventArgs e)
		{
			MaskedTextBox Txt = sender as MaskedTextBox;
			if (Txt == null) return;

			int Val;
			if (!int.TryParse(Txt.Text, out Val)) Val = 0;

			Txt.Text = Val.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnColorPick(object sender, EventArgs e)
		{
			ColorButton Btn = sender as ColorButton;
			if(Btn == null) return;

			ColorDialog dlg = new ColorDialog();
			dlg.Color = Btn.ButtonColor;
			if(dlg.ShowDialog() == DialogResult.OK)
			{
				Btn.ButtonColor = dlg.Color;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public Size GridSize
		{
			get
			{
				int Width;
				if (!int.TryParse(TxtGridWidth.Text, out Width)) Width = 0;

				int Height;
				if (!int.TryParse(TxtGridHeight.Text, out Height)) Height = 0;

				return new Size(Width, Height);
			}
			set
			{
				TxtGridWidth.Text = value.Width.ToString();
				TxtGridHeight.Text = value.Height.ToString();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public Color SelectionColor
		{
			get
			{
				return BtnSelectionCol.ButtonColor;
			}
			set
			{
				BtnSelectionCol.ButtonColor = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public Color BoxColor
		{
			get
			{
				return BtnBoxCol.ButtonColor;
			}
			set
			{
				BtnBoxCol.ButtonColor = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public Color BackgroundColor
		{
			get
			{
				return BtnBackCol.ButtonColor;
			}
			set
			{
				BtnBackCol.ButtonColor = value;
			}
		}

	}
}