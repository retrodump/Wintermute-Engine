namespace Integrator
{
	partial class ModUltraEdit
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

		#region Component Designer generated code

		/// <summary> 
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.label1 = new System.Windows.Forms.Label();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.BtnBrowseUe = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.TxtUeInstallDir = new System.Windows.Forms.TextBox();
			this.BtnAddLanguage = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.ListLanguages = new System.Windows.Forms.ListBox();
			this.splitContainer1 = new System.Windows.Forms.SplitContainer();
			this.BtnAddTool = new System.Windows.Forms.Button();
			this.ListTools = new System.Windows.Forms.ListBox();
			this.label4 = new System.Windows.Forms.Label();
			this.BtnAssociate = new System.Windows.Forms.Button();
			this.splitContainer1.Panel1.SuspendLayout();
			this.splitContainer1.Panel2.SuspendLayout();
			this.splitContainer1.SuspendLayout();
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(3, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(254, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "UltraEdit is shareware and can be downloaded here:";
			// 
			// linkLabel1
			// 
			this.linkLabel1.AutoSize = true;
			this.linkLabel1.Location = new System.Drawing.Point(263, 9);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.Size = new System.Drawing.Size(94, 13);
			this.linkLabel1.TabIndex = 1;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "www.ultraedit.com";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnDownloadLink);
			// 
			// BtnBrowseUe
			// 
			this.BtnBrowseUe.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowseUe.Location = new System.Drawing.Point(348, 51);
			this.BtnBrowseUe.Name = "BtnBrowseUe";
			this.BtnBrowseUe.Size = new System.Drawing.Size(25, 23);
			this.BtnBrowseUe.TabIndex = 4;
			this.BtnBrowseUe.Text = "...";
			this.BtnBrowseUe.UseVisualStyleBackColor = true;
			this.BtnBrowseUe.Click += new System.EventHandler(this.OnBrowseDir);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 37);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(145, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "UltraEdit installation directory:";
			// 
			// TxtUeInstallDir
			// 
			this.TxtUeInstallDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtUeInstallDir.Location = new System.Drawing.Point(6, 53);
			this.TxtUeInstallDir.Name = "TxtUeInstallDir";
			this.TxtUeInstallDir.Size = new System.Drawing.Size(336, 20);
			this.TxtUeInstallDir.TabIndex = 3;
			this.TxtUeInstallDir.Leave += new System.EventHandler(this.OnDirLeave);
			// 
			// BtnAddLanguage
			// 
			this.BtnAddLanguage.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.BtnAddLanguage.Location = new System.Drawing.Point(0, 177);
			this.BtnAddLanguage.Name = "BtnAddLanguage";
			this.BtnAddLanguage.Size = new System.Drawing.Size(181, 23);
			this.BtnAddLanguage.TabIndex = 2;
			this.BtnAddLanguage.Text = "Add WME scripts";
			this.BtnAddLanguage.UseVisualStyleBackColor = true;
			this.BtnAddLanguage.Click += new System.EventHandler(this.OnAddLanguage);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(3, 0);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(124, 13);
			this.label3.TabIndex = 0;
			this.label3.Text = "Available language slots:";
			// 
			// ListLanguages
			// 
			this.ListLanguages.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListLanguages.FormattingEnabled = true;
			this.ListLanguages.IntegralHeight = false;
			this.ListLanguages.Location = new System.Drawing.Point(0, 16);
			this.ListLanguages.Name = "ListLanguages";
			this.ListLanguages.Size = new System.Drawing.Size(181, 155);
			this.ListLanguages.TabIndex = 1;
			this.ListLanguages.SelectedValueChanged += new System.EventHandler(this.OnSelectionChanged);
			// 
			// splitContainer1
			// 
			this.splitContainer1.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.splitContainer1.IsSplitterFixed = true;
			this.splitContainer1.Location = new System.Drawing.Point(6, 108);
			this.splitContainer1.Name = "splitContainer1";
			// 
			// splitContainer1.Panel1
			// 
			this.splitContainer1.Panel1.Controls.Add(this.ListLanguages);
			this.splitContainer1.Panel1.Controls.Add(this.BtnAddLanguage);
			this.splitContainer1.Panel1.Controls.Add(this.label3);
			// 
			// splitContainer1.Panel2
			// 
			this.splitContainer1.Panel2.Controls.Add(this.BtnAddTool);
			this.splitContainer1.Panel2.Controls.Add(this.ListTools);
			this.splitContainer1.Panel2.Controls.Add(this.label4);
			this.splitContainer1.Size = new System.Drawing.Size(367, 203);
			this.splitContainer1.SplitterDistance = 184;
			this.splitContainer1.TabIndex = 19;
			// 
			// BtnAddTool
			// 
			this.BtnAddTool.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.BtnAddTool.Location = new System.Drawing.Point(2, 177);
			this.BtnAddTool.Name = "BtnAddTool";
			this.BtnAddTool.Size = new System.Drawing.Size(177, 23);
			this.BtnAddTool.TabIndex = 2;
			this.BtnAddTool.Text = "Add check syntax command";
			this.BtnAddTool.UseVisualStyleBackColor = true;
			this.BtnAddTool.Click += new System.EventHandler(this.OnAddTool);
			// 
			// ListTools
			// 
			this.ListTools.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListTools.FormattingEnabled = true;
			this.ListTools.IntegralHeight = false;
			this.ListTools.Location = new System.Drawing.Point(2, 16);
			this.ListTools.Name = "ListTools";
			this.ListTools.Size = new System.Drawing.Size(177, 155);
			this.ListTools.TabIndex = 1;
			this.ListTools.SelectedValueChanged += new System.EventHandler(this.OnSelectionChanged);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(3, 0);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(97, 13);
			this.label4.TabIndex = 0;
			this.label4.Text = "Available tool slots:";
			// 
			// BtnAssociate
			// 
			this.BtnAssociate.Location = new System.Drawing.Point(6, 79);
			this.BtnAssociate.Name = "BtnAssociate";
			this.BtnAssociate.Size = new System.Drawing.Size(207, 23);
			this.BtnAssociate.TabIndex = 20;
			this.BtnAssociate.Text = "Associate file extensions with UltraEdit";
			this.BtnAssociate.UseVisualStyleBackColor = true;
			this.BtnAssociate.Click += new System.EventHandler(this.OnAssociate);
			// 
			// ModUltraEdit
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.BtnAssociate);
			this.Controls.Add(this.splitContainer1);
			this.Controls.Add(this.BtnBrowseUe);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.TxtUeInstallDir);
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this.label1);
			this.Name = "ModUltraEdit";
			this.Size = new System.Drawing.Size(376, 314);
			this.Load += new System.EventHandler(this.OnLoad);
			this.splitContainer1.Panel1.ResumeLayout(false);
			this.splitContainer1.Panel1.PerformLayout();
			this.splitContainer1.Panel2.ResumeLayout(false);
			this.splitContainer1.Panel2.PerformLayout();
			this.splitContainer1.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.LinkLabel linkLabel1;
		private System.Windows.Forms.Button BtnBrowseUe;
		private System.Windows.Forms.TextBox TxtUeInstallDir;
		private System.Windows.Forms.Button BtnAddLanguage;
		private System.Windows.Forms.Label label3;
		private System.Windows.Forms.ListBox ListLanguages;
		private System.Windows.Forms.SplitContainer splitContainer1;
		private System.Windows.Forms.Button BtnAddTool;
		private System.Windows.Forms.ListBox ListTools;
		private System.Windows.Forms.Label label4;
		protected System.Windows.Forms.Button BtnAssociate;
		protected System.Windows.Forms.Label label1;
		protected System.Windows.Forms.Label label2;

	}
}
