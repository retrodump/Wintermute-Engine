namespace DeadCode.WME.Global
{
	partial class TiledImgEditorForm
	{
		/// <summary>
		/// Required designer variable.
		/// </summary>
		private System.ComponentModel.IContainer components = null;

		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing && (components != null))
			{
				components.Dispose();
			}
			base.Dispose(disposing);
		}

		#region Windows Form Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(TiledImgEditorForm));
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.BtnNew = new System.Windows.Forms.ToolStripButton();
			this.BtnOpen = new System.Windows.Forms.ToolStripButton();
			this.BtnSave = new System.Windows.Forms.ToolStripButton();
			this.BtnSaveAs = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.BtnImage = new System.Windows.Forms.ToolStripButton();
			this.PictBox = new System.Windows.Forms.PictureBox();
			this.LabelFilename = new System.Windows.Forms.Label();
			this.LabelDimension = new System.Windows.Forms.Label();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnClose = new System.Windows.Forms.Button();
			this.tableLayoutPanel1 = new System.Windows.Forms.TableLayoutPanel();
			this.TxtCol1 = new System.Windows.Forms.TextBox();
			this.TxtCol2 = new System.Windows.Forms.TextBox();
			this.TxtCol3 = new System.Windows.Forms.TextBox();
			this.tableLayoutPanel2 = new System.Windows.Forms.TableLayoutPanel();
			this.TxtRow2 = new System.Windows.Forms.TextBox();
			this.TxtRow3 = new System.Windows.Forms.TextBox();
			this.TxtRow1 = new System.Windows.Forms.TextBox();
			this.toolStrip1.SuspendLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictBox)).BeginInit();
			this.tableLayoutPanel1.SuspendLayout();
			this.tableLayoutPanel2.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.BtnNew,
            this.BtnOpen,
            this.BtnSave,
            this.BtnSaveAs,
            this.toolStripSeparator1,
            this.BtnImage});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(367, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// BtnNew
			// 
			this.BtnNew.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnNew.Image = ((System.Drawing.Image)(resources.GetObject("BtnNew.Image")));
			this.BtnNew.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnNew.Name = "BtnNew";
			this.BtnNew.Size = new System.Drawing.Size(23, 22);
			this.BtnNew.Text = "&New";
			this.BtnNew.Click += new System.EventHandler(this.OnNewFile);
			// 
			// BtnOpen
			// 
			this.BtnOpen.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnOpen.Image = ((System.Drawing.Image)(resources.GetObject("BtnOpen.Image")));
			this.BtnOpen.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnOpen.Name = "BtnOpen";
			this.BtnOpen.Size = new System.Drawing.Size(23, 22);
			this.BtnOpen.Text = "&Open";
			this.BtnOpen.Click += new System.EventHandler(this.OnOpenFile);
			// 
			// BtnSave
			// 
			this.BtnSave.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnSave.Image = ((System.Drawing.Image)(resources.GetObject("BtnSave.Image")));
			this.BtnSave.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnSave.Name = "BtnSave";
			this.BtnSave.Size = new System.Drawing.Size(23, 22);
			this.BtnSave.Text = "&Save";
			this.BtnSave.Click += new System.EventHandler(this.OnSaveFile);
			// 
			// BtnSaveAs
			// 
			this.BtnSaveAs.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.BtnSaveAs.Image = ((System.Drawing.Image)(resources.GetObject("BtnSaveAs.Image")));
			this.BtnSaveAs.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnSaveAs.Name = "BtnSaveAs";
			this.BtnSaveAs.Size = new System.Drawing.Size(58, 22);
			this.BtnSaveAs.Text = "Save as...";
			this.BtnSaveAs.Click += new System.EventHandler(this.OnSaveAs);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// BtnImage
			// 
			this.BtnImage.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnImage.Image = global::DeadCode.WME.Global.Properties.Resources.image;
			this.BtnImage.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnImage.Name = "BtnImage";
			this.BtnImage.Size = new System.Drawing.Size(23, 22);
			this.BtnImage.Text = "Choose image";
			this.BtnImage.Click += new System.EventHandler(this.OnChooseImage);
			// 
			// PictBox
			// 
			this.PictBox.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.PictBox.BorderStyle = System.Windows.Forms.BorderStyle.Fixed3D;
			this.PictBox.Location = new System.Drawing.Point(12, 94);
			this.PictBox.Name = "PictBox";
			this.PictBox.Size = new System.Drawing.Size(285, 263);
			this.PictBox.SizeMode = System.Windows.Forms.PictureBoxSizeMode.StretchImage;
			this.PictBox.TabIndex = 1;
			this.PictBox.TabStop = false;
			this.PictBox.Paint += new System.Windows.Forms.PaintEventHandler(this.OnImagePaint);
			// 
			// LabelFilename
			// 
			this.LabelFilename.AutoSize = true;
			this.LabelFilename.Location = new System.Drawing.Point(9, 25);
			this.LabelFilename.Name = "LabelFilename";
			this.LabelFilename.Size = new System.Drawing.Size(49, 13);
			this.LabelFilename.TabIndex = 1;
			this.LabelFilename.Text = "Filename";
			// 
			// LabelDimension
			// 
			this.LabelDimension.AutoSize = true;
			this.LabelDimension.Location = new System.Drawing.Point(9, 40);
			this.LabelDimension.Name = "LabelDimension";
			this.LabelDimension.Size = new System.Drawing.Size(74, 13);
			this.LabelDimension.TabIndex = 2;
			this.LabelDimension.Text = "Size: 000x000";
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(203, 363);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 5;
			this.BtnOK.Text = "Use this file";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// BtnClose
			// 
			this.BtnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnClose.Location = new System.Drawing.Point(284, 363);
			this.BtnClose.Name = "BtnClose";
			this.BtnClose.Size = new System.Drawing.Size(75, 23);
			this.BtnClose.TabIndex = 6;
			this.BtnClose.Text = "Close";
			this.BtnClose.UseVisualStyleBackColor = true;
			// 
			// tableLayoutPanel1
			// 
			this.tableLayoutPanel1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel1.ColumnCount = 3;
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
			this.tableLayoutPanel1.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 33.33334F));
			this.tableLayoutPanel1.Controls.Add(this.TxtCol1, 0, 0);
			this.tableLayoutPanel1.Controls.Add(this.TxtCol2, 1, 0);
			this.tableLayoutPanel1.Controls.Add(this.TxtCol3, 2, 0);
			this.tableLayoutPanel1.Location = new System.Drawing.Point(12, 62);
			this.tableLayoutPanel1.Name = "tableLayoutPanel1";
			this.tableLayoutPanel1.RowCount = 1;
			this.tableLayoutPanel1.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel1.Size = new System.Drawing.Size(285, 26);
			this.tableLayoutPanel1.TabIndex = 3;
			// 
			// TxtCol1
			// 
			this.TxtCol1.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtCol1.Location = new System.Drawing.Point(22, 3);
			this.TxtCol1.Name = "TxtCol1";
			this.TxtCol1.Size = new System.Drawing.Size(50, 20);
			this.TxtCol1.TabIndex = 0;
			this.TxtCol1.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtCol1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// TxtCol2
			// 
			this.TxtCol2.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtCol2.Location = new System.Drawing.Point(117, 3);
			this.TxtCol2.Name = "TxtCol2";
			this.TxtCol2.Size = new System.Drawing.Size(50, 20);
			this.TxtCol2.TabIndex = 1;
			this.TxtCol2.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtCol2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// TxtCol3
			// 
			this.TxtCol3.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtCol3.Location = new System.Drawing.Point(212, 3);
			this.TxtCol3.Name = "TxtCol3";
			this.TxtCol3.Size = new System.Drawing.Size(50, 20);
			this.TxtCol3.TabIndex = 2;
			this.TxtCol3.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtCol3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// tableLayoutPanel2
			// 
			this.tableLayoutPanel2.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.tableLayoutPanel2.ColumnCount = 1;
			this.tableLayoutPanel2.ColumnStyles.Add(new System.Windows.Forms.ColumnStyle(System.Windows.Forms.SizeType.Percent, 100F));
			this.tableLayoutPanel2.Controls.Add(this.TxtRow2, 0, 1);
			this.tableLayoutPanel2.Controls.Add(this.TxtRow3, 0, 2);
			this.tableLayoutPanel2.Controls.Add(this.TxtRow1, 0, 0);
			this.tableLayoutPanel2.Location = new System.Drawing.Point(303, 94);
			this.tableLayoutPanel2.Name = "tableLayoutPanel2";
			this.tableLayoutPanel2.RowCount = 3;
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel2.RowStyles.Add(new System.Windows.Forms.RowStyle(System.Windows.Forms.SizeType.Percent, 33.33333F));
			this.tableLayoutPanel2.Size = new System.Drawing.Size(56, 263);
			this.tableLayoutPanel2.TabIndex = 4;
			// 
			// TxtRow2
			// 
			this.TxtRow2.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtRow2.Location = new System.Drawing.Point(3, 120);
			this.TxtRow2.Name = "TxtRow2";
			this.TxtRow2.Size = new System.Drawing.Size(50, 20);
			this.TxtRow2.TabIndex = 1;
			this.TxtRow2.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtRow2.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// TxtRow3
			// 
			this.TxtRow3.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtRow3.Location = new System.Drawing.Point(3, 208);
			this.TxtRow3.Name = "TxtRow3";
			this.TxtRow3.Size = new System.Drawing.Size(50, 20);
			this.TxtRow3.TabIndex = 2;
			this.TxtRow3.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtRow3.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// TxtRow1
			// 
			this.TxtRow1.Anchor = System.Windows.Forms.AnchorStyles.None;
			this.TxtRow1.Location = new System.Drawing.Point(3, 33);
			this.TxtRow1.Name = "TxtRow1";
			this.TxtRow1.Size = new System.Drawing.Size(50, 20);
			this.TxtRow1.TabIndex = 0;
			this.TxtRow1.Leave += new System.EventHandler(this.OnTxtLeave);
			this.TxtRow1.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnTxtKeyDown);
			// 
			// TiledImgEditorForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(367, 398);
			this.Controls.Add(this.tableLayoutPanel2);
			this.Controls.Add(this.tableLayoutPanel1);
			this.Controls.Add(this.BtnClose);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.LabelDimension);
			this.Controls.Add(this.LabelFilename);
			this.Controls.Add(this.PictBox);
			this.Controls.Add(this.toolStrip1);
			this.Name = "TiledImgEditorForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Tiled image editor";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.Load += new System.EventHandler(this.OnLoad);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			((System.ComponentModel.ISupportInitialize)(this.PictBox)).EndInit();
			this.tableLayoutPanel1.ResumeLayout(false);
			this.tableLayoutPanel1.PerformLayout();
			this.tableLayoutPanel2.ResumeLayout(false);
			this.tableLayoutPanel2.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ToolStripButton BtnNew;
		private System.Windows.Forms.ToolStripButton BtnOpen;
		private System.Windows.Forms.ToolStripButton BtnSave;
		private System.Windows.Forms.ToolStripButton BtnSaveAs;
		private System.Windows.Forms.PictureBox PictBox;
		private System.Windows.Forms.Label LabelFilename;
		private System.Windows.Forms.Label LabelDimension;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnClose;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel1;
		private System.Windows.Forms.TextBox TxtCol1;
		private System.Windows.Forms.TextBox TxtCol2;
		private System.Windows.Forms.TextBox TxtCol3;
		private System.Windows.Forms.TableLayoutPanel tableLayoutPanel2;
		private System.Windows.Forms.TextBox TxtRow1;
		private System.Windows.Forms.TextBox TxtRow2;
		private System.Windows.Forms.TextBox TxtRow3;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton BtnImage;
	}
}