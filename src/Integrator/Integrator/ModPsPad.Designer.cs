namespace Integrator
{
	partial class ModPsPad
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
			this.BtnAssociate = new System.Windows.Forms.Button();
			this.BtnBrowsePsPad = new System.Windows.Forms.Button();
			this.label2 = new System.Windows.Forms.Label();
			this.TxtPsPadInstallDir = new System.Windows.Forms.TextBox();
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.label1 = new System.Windows.Forms.Label();
			this.ListLanguages = new System.Windows.Forms.ListBox();
			this.BtnAddLanguage = new System.Windows.Forms.Button();
			this.label3 = new System.Windows.Forms.Label();
			this.SuspendLayout();
			// 
			// BtnAssociate
			// 
			this.BtnAssociate.Location = new System.Drawing.Point(6, 79);
			this.BtnAssociate.Name = "BtnAssociate";
			this.BtnAssociate.Size = new System.Drawing.Size(207, 23);
			this.BtnAssociate.TabIndex = 26;
			this.BtnAssociate.Text = "Associate file extensions with PSPad";
			this.BtnAssociate.UseVisualStyleBackColor = true;
			this.BtnAssociate.Click += new System.EventHandler(this.OnAssociate);
			// 
			// BtnBrowsePsPad
			// 
			this.BtnBrowsePsPad.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowsePsPad.Location = new System.Drawing.Point(334, 51);
			this.BtnBrowsePsPad.Name = "BtnBrowsePsPad";
			this.BtnBrowsePsPad.Size = new System.Drawing.Size(25, 23);
			this.BtnBrowsePsPad.TabIndex = 25;
			this.BtnBrowsePsPad.Text = "...";
			this.BtnBrowsePsPad.UseVisualStyleBackColor = true;
			this.BtnBrowsePsPad.Click += new System.EventHandler(this.OnBrowseDir);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(3, 37);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(138, 13);
			this.label2.TabIndex = 23;
			this.label2.Text = "PSPad installation directory:";
			// 
			// TxtPsPadInstallDir
			// 
			this.TxtPsPadInstallDir.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtPsPadInstallDir.Location = new System.Drawing.Point(6, 53);
			this.TxtPsPadInstallDir.Name = "TxtPsPadInstallDir";
			this.TxtPsPadInstallDir.Size = new System.Drawing.Size(322, 20);
			this.TxtPsPadInstallDir.TabIndex = 24;
			this.TxtPsPadInstallDir.Leave += new System.EventHandler(this.OnDirLeave);
			// 
			// linkLabel1
			// 
			this.linkLabel1.AutoSize = true;
			this.linkLabel1.Location = new System.Drawing.Point(248, 9);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.Size = new System.Drawing.Size(86, 13);
			this.linkLabel1.TabIndex = 22;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "www.pspad.com";
			this.linkLabel1.Click += new System.EventHandler(this.OnDownloadLink);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(3, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(239, 13);
			this.label1.TabIndex = 21;
			this.label1.Text = "PSPad is freeware and can be downloaded here:";
			// 
			// ListLanguages
			// 
			this.ListLanguages.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)));
			this.ListLanguages.FormattingEnabled = true;
			this.ListLanguages.IntegralHeight = false;
			this.ListLanguages.Location = new System.Drawing.Point(6, 121);
			this.ListLanguages.Name = "ListLanguages";
			this.ListLanguages.Size = new System.Drawing.Size(207, 183);
			this.ListLanguages.TabIndex = 28;
			this.ListLanguages.SelectedIndexChanged += new System.EventHandler(this.OnSelectionChanged);
			// 
			// BtnAddLanguage
			// 
			this.BtnAddLanguage.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.BtnAddLanguage.Location = new System.Drawing.Point(6, 310);
			this.BtnAddLanguage.Name = "BtnAddLanguage";
			this.BtnAddLanguage.Size = new System.Drawing.Size(207, 23);
			this.BtnAddLanguage.TabIndex = 29;
			this.BtnAddLanguage.Text = "Add WME scripts";
			this.BtnAddLanguage.UseVisualStyleBackColor = true;
			this.BtnAddLanguage.Click += new System.EventHandler(this.OnAddLanguage);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(3, 105);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(124, 13);
			this.label3.TabIndex = 27;
			this.label3.Text = "Available language slots:";
			// 
			// ModPsPad
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.ListLanguages);
			this.Controls.Add(this.BtnAddLanguage);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.BtnAssociate);
			this.Controls.Add(this.BtnBrowsePsPad);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.TxtPsPadInstallDir);
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this.label1);
			this.Name = "ModPsPad";
			this.Size = new System.Drawing.Size(362, 336);
			this.Load += new System.EventHandler(this.OnLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button BtnAssociate;
		private System.Windows.Forms.Button BtnBrowsePsPad;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TextBox TxtPsPadInstallDir;
		private System.Windows.Forms.LinkLabel linkLabel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox ListLanguages;
		private System.Windows.Forms.Button BtnAddLanguage;
		private System.Windows.Forms.Label label3;
	}
}
