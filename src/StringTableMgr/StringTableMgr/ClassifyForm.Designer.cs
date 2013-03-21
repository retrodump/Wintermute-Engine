namespace DeadCode.WME.StringTableMgr
{
	partial class ClassifyForm
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
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ClassifyForm));
			this.BtnClose = new System.Windows.Forms.Button();
			this.SplitTop = new System.Windows.Forms.SplitContainer();
			this.SplitMain = new System.Windows.Forms.SplitContainer();
			this.LblStatusStrings = new System.Windows.Forms.Label();
			this.label1 = new System.Windows.Forms.Label();
			this.ListStrings = new System.Windows.Forms.ListView();
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.StringMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.LblStatusIgnored = new System.Windows.Forms.Label();
			this.label2 = new System.Windows.Forms.Label();
			this.ListIgnored = new System.Windows.Forms.ListView();
			this.columnHeader3 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader4 = new System.Windows.Forms.ColumnHeader();
			this.IgnoreMenu = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.LblContext = new System.Windows.Forms.Label();
			this.TxtContext = new System.Windows.Forms.TextBox();
			this.SplitTop.Panel1.SuspendLayout();
			this.SplitTop.Panel2.SuspendLayout();
			this.SplitTop.SuspendLayout();
			this.SplitMain.Panel1.SuspendLayout();
			this.SplitMain.Panel2.SuspendLayout();
			this.SplitMain.SuspendLayout();
			this.SuspendLayout();
			// 
			// BtnClose
			// 
			this.BtnClose.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnClose.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnClose.Location = new System.Drawing.Point(566, 470);
			this.BtnClose.Name = "BtnClose";
			this.BtnClose.Size = new System.Drawing.Size(75, 23);
			this.BtnClose.TabIndex = 1;
			this.BtnClose.Text = "Close";
			this.BtnClose.UseVisualStyleBackColor = true;
			this.BtnClose.Click += new System.EventHandler(this.OnClose);
			// 
			// SplitTop
			// 
			this.SplitTop.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.SplitTop.Location = new System.Drawing.Point(0, 0);
			this.SplitTop.Name = "SplitTop";
			this.SplitTop.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitTop.Panel1
			// 
			this.SplitTop.Panel1.Controls.Add(this.SplitMain);
			// 
			// SplitTop.Panel2
			// 
			this.SplitTop.Panel2.Controls.Add(this.LblContext);
			this.SplitTop.Panel2.Controls.Add(this.TxtContext);
			this.SplitTop.Size = new System.Drawing.Size(652, 464);
			this.SplitTop.SplitterDistance = 374;
			this.SplitTop.TabIndex = 0;
			// 
			// SplitMain
			// 
			this.SplitMain.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.SplitMain.Location = new System.Drawing.Point(-1, -111);
			this.SplitMain.Name = "SplitMain";
			// 
			// SplitMain.Panel1
			// 
			this.SplitMain.Panel1.Controls.Add(this.LblStatusStrings);
			this.SplitMain.Panel1.Controls.Add(this.label1);
			this.SplitMain.Panel1.Controls.Add(this.ListStrings);
			// 
			// SplitMain.Panel2
			// 
			this.SplitMain.Panel2.Controls.Add(this.LblStatusIgnored);
			this.SplitMain.Panel2.Controls.Add(this.label2);
			this.SplitMain.Panel2.Controls.Add(this.ListIgnored);
			this.SplitMain.Size = new System.Drawing.Size(654, 482);
			this.SplitMain.SplitterDistance = 312;
			this.SplitMain.TabIndex = 0;
			// 
			// LblStatusStrings
			// 
			this.LblStatusStrings.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LblStatusStrings.AutoSize = true;
			this.LblStatusStrings.Location = new System.Drawing.Point(4, 469);
			this.LblStatusStrings.Name = "LblStatusStrings";
			this.LblStatusStrings.Size = new System.Drawing.Size(70, 13);
			this.LblStatusStrings.TabIndex = 4;
			this.LblStatusStrings.Text = "Status strings";
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(4, 120);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(141, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Strings to add to string table:";
			// 
			// ListStrings
			// 
			this.ListStrings.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListStrings.CheckBoxes = true;
			this.ListStrings.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.ListStrings.ContextMenuStrip = this.StringMenu;
			this.ListStrings.FullRowSelect = true;
			this.ListStrings.GridLines = true;
			this.ListStrings.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ListStrings.HideSelection = false;
			this.ListStrings.Location = new System.Drawing.Point(0, 136);
			this.ListStrings.MultiSelect = false;
			this.ListStrings.Name = "ListStrings";
			this.ListStrings.ShowItemToolTips = true;
			this.ListStrings.Size = new System.Drawing.Size(311, 330);
			this.ListStrings.TabIndex = 1;
			this.ListStrings.UseCompatibleStateImageBehavior = false;
			this.ListStrings.View = System.Windows.Forms.View.Details;
			this.ListStrings.ColumnWidthChanged += new System.Windows.Forms.ColumnWidthChangedEventHandler(this.OnColumnWidthChanged);
			this.ListStrings.ItemChecked += new System.Windows.Forms.ItemCheckedEventHandler(this.OnStringChecked);
			this.ListStrings.Resize += new System.EventHandler(this.OnListResize);
			this.ListStrings.SelectedIndexChanged += new System.EventHandler(this.OnStringSelected);
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "ID";
			this.columnHeader1.Width = 74;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "String";
			this.columnHeader2.Width = 208;
			// 
			// StringMenu
			// 
			this.StringMenu.Name = "StringMenu";
			this.StringMenu.Size = new System.Drawing.Size(61, 4);
			this.StringMenu.Opening += new System.ComponentModel.CancelEventHandler(this.OnStringMenu);
			// 
			// LblStatusIgnored
			// 
			this.LblStatusIgnored.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LblStatusIgnored.AutoSize = true;
			this.LblStatusIgnored.Location = new System.Drawing.Point(3, 469);
			this.LblStatusIgnored.Name = "LblStatusIgnored";
			this.LblStatusIgnored.Size = new System.Drawing.Size(75, 13);
			this.LblStatusIgnored.TabIndex = 5;
			this.LblStatusIgnored.Text = "Status ignored";
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 120);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(79, 13);
			this.label2.TabIndex = 0;
			this.label2.Text = "Ignored strings:";
			// 
			// ListIgnored
			// 
			this.ListIgnored.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListIgnored.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader3,
            this.columnHeader4});
			this.ListIgnored.ContextMenuStrip = this.IgnoreMenu;
			this.ListIgnored.FullRowSelect = true;
			this.ListIgnored.GridLines = true;
			this.ListIgnored.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Nonclickable;
			this.ListIgnored.HideSelection = false;
			this.ListIgnored.Location = new System.Drawing.Point(1, 136);
			this.ListIgnored.MultiSelect = false;
			this.ListIgnored.Name = "ListIgnored";
			this.ListIgnored.ShowItemToolTips = true;
			this.ListIgnored.Size = new System.Drawing.Size(337, 330);
			this.ListIgnored.TabIndex = 1;
			this.ListIgnored.UseCompatibleStateImageBehavior = false;
			this.ListIgnored.View = System.Windows.Forms.View.Details;
			this.ListIgnored.ColumnWidthChanged += new System.Windows.Forms.ColumnWidthChangedEventHandler(this.OnColumnWidthChanged);
			this.ListIgnored.Resize += new System.EventHandler(this.OnListResize);
			this.ListIgnored.SelectedIndexChanged += new System.EventHandler(this.OnStringSelected);
			// 
			// columnHeader3
			// 
			this.columnHeader3.Text = "String";
			this.columnHeader3.Width = 201;
			// 
			// columnHeader4
			// 
			this.columnHeader4.Text = "Ignore reason";
			this.columnHeader4.Width = 113;
			// 
			// IgnoreMenu
			// 
			this.IgnoreMenu.Name = "IgnoreMenu";
			this.IgnoreMenu.Size = new System.Drawing.Size(61, 4);
			this.IgnoreMenu.Opening += new System.ComponentModel.CancelEventHandler(this.OnIgnoreMenu);
			// 
			// LblContext
			// 
			this.LblContext.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.LblContext.AutoSize = true;
			this.LblContext.Location = new System.Drawing.Point(3, 73);
			this.LblContext.Name = "LblContext";
			this.LblContext.Size = new System.Drawing.Size(35, 13);
			this.LblContext.TabIndex = 1;
			this.LblContext.Text = "label3";
			// 
			// TxtContext
			// 
			this.TxtContext.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtContext.Font = new System.Drawing.Font("Courier New", 9.75F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
			this.TxtContext.HideSelection = false;
			this.TxtContext.Location = new System.Drawing.Point(0, 3);
			this.TxtContext.MaxLength = 0;
			this.TxtContext.Multiline = true;
			this.TxtContext.Name = "TxtContext";
			this.TxtContext.ReadOnly = true;
			this.TxtContext.ScrollBars = System.Windows.Forms.ScrollBars.Both;
			this.TxtContext.Size = new System.Drawing.Size(652, 67);
			this.TxtContext.TabIndex = 0;
			this.TxtContext.WordWrap = false;
			// 
			// ClassifyForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnClose;
			this.ClientSize = new System.Drawing.Size(653, 505);
			this.Controls.Add(this.SplitTop);
			this.Controls.Add(this.BtnClose);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "ClassifyForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Classify strings";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.SplitTop.Panel1.ResumeLayout(false);
			this.SplitTop.Panel2.ResumeLayout(false);
			this.SplitTop.Panel2.PerformLayout();
			this.SplitTop.ResumeLayout(false);
			this.SplitMain.Panel1.ResumeLayout(false);
			this.SplitMain.Panel1.PerformLayout();
			this.SplitMain.Panel2.ResumeLayout(false);
			this.SplitMain.Panel2.PerformLayout();
			this.SplitMain.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.Button BtnClose;
		private System.Windows.Forms.SplitContainer SplitTop;
		private System.Windows.Forms.SplitContainer SplitMain;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListView ListStrings;
		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ListView ListIgnored;
		private System.Windows.Forms.ColumnHeader columnHeader3;
		private System.Windows.Forms.ColumnHeader columnHeader4;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.Label LblStatusStrings;
		private System.Windows.Forms.Label LblStatusIgnored;
		private System.Windows.Forms.Label LblContext;
		private System.Windows.Forms.TextBox TxtContext;
		private System.Windows.Forms.ContextMenuStrip StringMenu;
		private System.Windows.Forms.ContextMenuStrip IgnoreMenu;
	}
}