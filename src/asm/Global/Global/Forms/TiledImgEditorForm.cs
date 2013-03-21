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
using DeadCode.WME.Core;
using System.IO;

namespace DeadCode.WME.Global
{
	public partial class TiledImgEditorForm : FormBase
	{
		//////////////////////////////////////////////////////////////////////////
		public TiledImgEditorForm()
		{
			InitializeComponent();
		}

		private TextBox[] TxtCol;
		private TextBox[] TxtRow;
		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;
			if (AppMgr != null) LoadLayout(AppMgr.Settings);

			TxtCol = new TextBox[3];
			TxtCol[0] = TxtCol1;
			TxtCol[1] = TxtCol2;
			TxtCol[2] = TxtCol3;

			TxtRow = new TextBox[3];
			TxtRow[0] = TxtRow1;
			TxtRow[1] = TxtRow2;
			TxtRow[2] = TxtRow3;

			if (OrigFilename != "" && OrigFilename != null)
			{
				LoadFile(OrigFilename);
			}
			else Filename = "";

			DisplayData();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null) SaveLayout(AppMgr.Settings);

			if (TiledImg != null) TiledImg.Dispose();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosing(object sender, FormClosingEventArgs e)
		{
			e.Cancel = !CanClose();
		}

		private string _OrigFilename;
		//////////////////////////////////////////////////////////////////////////
		public string OrigFilename
		{
			get
			{
				return _OrigFilename;
			}
			set
			{
				_OrigFilename = value;
			}
		}

		private WGame _Game;
		//////////////////////////////////////////////////////////////////////////
		public WGame Game
		{
			get
			{
				return _Game;
			}
			set
			{
				_Game = value;
			}
		}

		private string _Filename = "";
		//////////////////////////////////////////////////////////////////////////
		public string Filename
		{
			get
			{
				return _Filename;
			}
			set
			{
				_Filename = value;
				DisplayFilename();
			}
		}

		private bool _IsDirty = false;
		//////////////////////////////////////////////////////////////////////////
		public bool IsDirty
		{
			get
			{
				return _IsDirty;
			}
			set
			{
				_IsDirty = value;
				DisplayFilename();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplayFilename()
		{
			string Name = "File name: ";
			if (Filename != "") Name += Filename;
			else Name += "untitled";

			if (IsDirty) Name += "*";

			LabelFilename.Text = Name;
		}

		WUITiledImage TiledImg = null;

		//////////////////////////////////////////////////////////////////////////
		private void OnOpenFile(object sender, EventArgs e)
		{
			if (!CanClose()) return;

			string OrigFile = Filename;
			if (OrigFile != "") OrigFile = Game.MakeAbsolutePath(OrigFile);

			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = "Tiled images (*.image)|*.image|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;

			if (File.Exists(OrigFile)) dlg.FileName = OrigFile;

			if(dlg.ShowDialog()!=DialogResult.OK) return;

			string NewFile = Game.MakeRelativePath(dlg.FileName);
			LoadFile(NewFile);
			DisplayData();
		}

		//////////////////////////////////////////////////////////////////////////
		private void LoadFile(string Filename)
		{
			WUITiledImage NewImage = new WUITiledImage(Game);
			if (NewImage.LoadFromFile(Filename))
			{
				if (TiledImg != null)
				{
					TiledImg.ClaimOwnership();
					TiledImg.Dispose();
				}

				this.TiledImg = NewImage;
				this.Filename = Filename;
				this.IsDirty = false;				
			}
			else MessageBox.Show("Error loading file.", Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplayData()
		{
			DisplayImage();

			LabelDimension.Text = "Size: ";
			if (PictBox.Image != null)
				LabelDimension.Text += PictBox.Image.Width.ToString() + "x" + PictBox.Image.Height.ToString();

			int[] Cols = GetCols();
			int[] Rows = GetRows();

			for (int i = 0; i < 3; i++)
			{
				if(Cols != null) TxtCol[i].Text = Cols[i].ToString();
				else TxtCol[i].Text = "";

				if (Rows != null) TxtRow[i].Text = Rows[i].ToString();
				else TxtRow[i].Text = "";
			}

			bool RowsValid = true;
			bool ColsValid = true;
			if(Cols != null && PictBox.Image != null)
			{
				if (Cols[0] + Cols[1] + Cols[2] != PictBox.Image.Width)
					ColsValid = false;
				if (Rows[0] + Rows[1] + Rows[2] != PictBox.Image.Height)
					RowsValid = false;
			}
			for (int i = 0; i < 3; i++)
			{
				TxtCol[i].BackColor = ColsValid ? SystemColors.Window : Color.Red;
				TxtCol[i].ForeColor = ColsValid ? SystemColors.WindowText : Color.White;

				TxtRow[i].BackColor = RowsValid ? SystemColors.Window : Color.Red;
				TxtRow[i].ForeColor = RowsValid ? SystemColors.WindowText : Color.White;
			}

			// enable / disable stuff
			for (int i = 0; i < 3; i++)
			{
				TxtCol[i].Enabled = (TiledImg != null);
				TxtRow[i].Enabled = (TiledImg != null);
			}
			BtnSave.Enabled = (TiledImg != null);
			BtnSaveAs.Enabled = (TiledImg != null);
			BtnImage.Enabled = (TiledImg != null);
			BtnOK.Enabled = (Filename != "");

		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplayImage()
		{
			if (TiledImg != null)
			{
				if (TiledImg.Image != null)
					PictBox.Image = TiledImg.Image.GetBitmap();
				else
					PictBox.Image = null;
				
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnImagePaint(object sender, PaintEventArgs e)
		{
			if(PictBox.Image != null)
			{
				int[] Rows = GetRows();
				int[] Cols = GetCols();

				float ModX = (float)PictBox.ClientSize.Width / (float)PictBox.Image.Width;
				float ModY = (float)PictBox.ClientSize.Height / (float)PictBox.Image.Height;

				if(Rows != null)
				{
					int CurrY = 0;
					foreach(int i in Rows)
					{
						CurrY += i;
						e.Graphics.DrawLine(Pens.Red, 0, CurrY * ModY, PictBox.Image.Width * ModX, CurrY * ModY);
					}
				}

				if (Cols != null)
				{
					int CurrX = 0;
					foreach (int i in Cols)
					{
						CurrX += i;
						e.Graphics.DrawLine(Pens.Red, CurrX * ModX, 0, CurrX * ModX, PictBox.Image.Height * ModY);
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		int[] GetCols()
		{
			if (TiledImg != null)
			{
				int[] Ret = new int[3];
				Ret[0] = TiledImg.UpLeft.Right;
				Ret[1] = TiledImg.UpMiddle.Right - TiledImg.UpMiddle.Left;
				Ret[2] = TiledImg.UpRight.Right - TiledImg.UpRight.Left;

				return Ret;
			}
			else return null;
		}

		//////////////////////////////////////////////////////////////////////////
		int[] GetRows()
		{
			if (TiledImg != null)
			{
				int[] Ret = new int[3];
				Ret[0] = TiledImg.UpLeft.Bottom;
				Ret[1] = TiledImg.MiddleLeft.Bottom - TiledImg.MiddleLeft.Top;
				Ret[2] = TiledImg.DownLeft.Bottom - TiledImg.DownLeft.Top;

				return Ret;
			}
			else return null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTxtLeave(object sender, EventArgs e)
		{
			if (TiledImg == null) return;

			TextBox Txt = sender as TextBox;
			if (Txt == null) return;

			int Value;
			if (!int.TryParse(Txt.Text, out Value)) Value = 0;

			Txt.Text = Value.ToString();

			if (UpdateValues()) IsDirty = true;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool UpdateValues()
		{
			if (TiledImg == null) return false;

			int[] OrigCols = GetCols();
			int[] OrigRows = GetRows();

			int[] Cols = new int[3];
			int[] Rows = new int[3];

			bool Changed = false;
			for(int i=0; i<3; i++)
			{
				int.TryParse(TxtCol[i].Text, out Cols[i]);
				if (Cols[i] != OrigCols[i]) Changed = true;

				int.TryParse(TxtRow[i].Text, out Rows[i]);
				if (Rows[i] != OrigRows[i]) Changed = true;
			}
			if (!Changed) return false;

			TiledImg.UpLeft       = new Rectangle(0,       0,       Cols[0], Rows[0]);
			TiledImg.UpMiddle     = new Rectangle(Cols[0], 0,       Cols[1], Rows[0]);
			TiledImg.UpRight      = new Rectangle(Cols[1], 0,       Cols[2], Rows[0]);

			TiledImg.MiddleLeft   = new Rectangle(0,       Rows[0], Cols[0], Rows[1]);
			TiledImg.MiddleMiddle = new Rectangle(Cols[0], Rows[0], Cols[1], Rows[1]);
			TiledImg.MiddleRight  = new Rectangle(Cols[1], Rows[0], Cols[2], Rows[1]);

			TiledImg.DownLeft     = new Rectangle(0,       Rows[1], Cols[0], Rows[2]);
			TiledImg.DownMiddle   = new Rectangle(Cols[0], Rows[1], Cols[1], Rows[2]);
			TiledImg.DownRight    = new Rectangle(Cols[1], Rows[1], Cols[2], Rows[2]);

			DisplayData();

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTxtKeyDown(object sender, KeyEventArgs e)
		{
			if(e.KeyCode == Keys.Enter)
			{
				OnTxtLeave(sender, new EventArgs());
				e.Handled = true;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnChooseImage(object sender, EventArgs e)
		{
			string OrigFile = null;
			if (TiledImg != null && TiledImg.Image != null && TiledImg.Image.ImageFilename != null)
				OrigFile = Game.MakeAbsolutePath(TiledImg.Image.ImageFilename);

			OpenFileDialog dlg = new OpenFileDialog();
			dlg.Filter = "Images (*.png; *.bmp; *.jpg; *.tga)|*.png;*.bmp;*.jpg;*.tga|All files (*.*)|*.*";
			dlg.RestoreDirectory = true;
			dlg.CheckFileExists = true;

			if (OrigFile != null && File.Exists(OrigFile)) dlg.FileName = OrigFile;

			if (dlg.ShowDialog() != DialogResult.OK) return;

			string NewFile = Game.MakeRelativePath(dlg.FileName);
			WSubFrame NewImg = new WSubFrame(Game);
			if(NewImg.SetImage(NewFile))
			{
				TiledImg.Image = NewImg;
				IsDirty = true;
				DisplayData();
			}
			else
			{
				NewImg.Dispose();
				MessageBox.Show("Error loading file.", Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnNewFile(object sender, EventArgs e)
		{
			if (!CanClose()) return;

			if(TiledImg != null)
			{
				TiledImg.ClaimOwnership();
				TiledImg.Dispose();
			}
			TiledImg = new WUITiledImage(Game);
			Filename = "";
			IsDirty = true;
			DisplayData();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSaveFile(object sender, EventArgs e)
		{
			SaveFile(false);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSaveAs(object sender, EventArgs e)
		{
			SaveFile(true);
		}

		//////////////////////////////////////////////////////////////////////////
		private bool SaveFile(bool SaveAs)
		{
			if (Filename == "" || Filename == null) SaveAs = true;

			string NewFileName;

			if (SaveAs)
			{
				SaveFileDialog dlg = new SaveFileDialog();				
				dlg.Filter = "Tiles images (*.image)|*.image|All files (*.*)|*.*";
				dlg.AddExtension = true;
				dlg.CheckPathExists = true;
				dlg.OverwritePrompt = true;
				dlg.RestoreDirectory = true;

				if (Filename != null && Filename != "")
					dlg.FileName = Game.MakeAbsolutePath(Filename);

				if (dlg.ShowDialog() != DialogResult.OK) return false;

				NewFileName = dlg.FileName;
			}
			else NewFileName = Game.MakeAbsolutePath(Filename);


			WDynBuffer Buf = new WDynBuffer(Game);
			TiledImg.SaveAsText(Buf);

			string FileContent = "";
			FileContent += "; generated by WindowEdit\n\n";
			FileContent += Buf.Text;

			Buf.Dispose();

			try
			{
				using (StreamWriter sw = new StreamWriter(NewFileName, false, Encoding.Default))
				{
					sw.WriteLine(FileContent);
				}

				Filename = Game.MakeRelativePath(NewFileName);
				IsDirty = false;
				DisplayData();

				return true;
			}
			catch (Exception e)
			{
				MessageBox.Show("Error saving file.\n\n" + e.Message, Form.ActiveForm.Text, MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}			
		}

		//////////////////////////////////////////////////////////////////////////
		private bool CanClose()
		{
			if (IsDirty)
			{
				DialogResult Res = MessageBox.Show("Do you want to save your changes?", Form.ActiveForm.Text, MessageBoxButtons.YesNoCancel, MessageBoxIcon.Question);

				switch (Res)
				{
					case DialogResult.Yes:
						return SaveFile(false);
					case DialogResult.No:
						return true;
					default:
						return false;
				}
			}
			else return true;
		}

	}
}