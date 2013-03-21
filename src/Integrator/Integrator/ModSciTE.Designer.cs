namespace Integrator
{
	partial class ModSciTE
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
			this.linkLabel1 = new System.Windows.Forms.LinkLabel();
			this.label1 = new System.Windows.Forms.Label();
			this.BtnAssociate = new System.Windows.Forms.Button();
			this.BtnIntegrate = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// linkLabel1
			// 
			this.linkLabel1.AutoSize = true;
			this.linkLabel1.Location = new System.Drawing.Point(3, 39);
			this.linkLabel1.Name = "linkLabel1";
			this.linkLabel1.Size = new System.Drawing.Size(86, 13);
			this.linkLabel1.TabIndex = 24;
			this.linkLabel1.TabStop = true;
			this.linkLabel1.Text = "www.scintilla.org";
			this.linkLabel1.LinkClicked += new System.Windows.Forms.LinkLabelLinkClickedEventHandler(this.OnDownloadLink);
			// 
			// label1
			// 
			this.label1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.label1.Location = new System.Drawing.Point(3, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(431, 30);
			this.label1.TabIndex = 23;
			this.label1.Text = "SciTE is a freeware open-source text editor bundled with WME.  The original versi" +
				"on and sources can be downloaded at:";
			// 
			// BtnAssociate
			// 
			this.BtnAssociate.Location = new System.Drawing.Point(6, 65);
			this.BtnAssociate.Name = "BtnAssociate";
			this.BtnAssociate.Size = new System.Drawing.Size(207, 23);
			this.BtnAssociate.TabIndex = 27;
			this.BtnAssociate.Text = "Associate file extensions with SciTE";
			this.BtnAssociate.UseVisualStyleBackColor = true;
			this.BtnAssociate.Click += new System.EventHandler(this.OnAssociate);
			// 
			// BtnIntegrate
			// 
			this.BtnIntegrate.Location = new System.Drawing.Point(6, 94);
			this.BtnIntegrate.Name = "BtnIntegrate";
			this.BtnIntegrate.Size = new System.Drawing.Size(207, 23);
			this.BtnIntegrate.TabIndex = 28;
			this.BtnIntegrate.Text = "Add syntax highlighting/tools support";
			this.BtnIntegrate.UseVisualStyleBackColor = true;
			this.BtnIntegrate.Click += new System.EventHandler(this.OnIntegrate);
			// 
			// ModSciTE
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.Controls.Add(this.BtnIntegrate);
			this.Controls.Add(this.BtnAssociate);
			this.Controls.Add(this.linkLabel1);
			this.Controls.Add(this.label1);
			this.Name = "ModSciTE";
			this.Size = new System.Drawing.Size(437, 235);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.LinkLabel linkLabel1;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button BtnAssociate;
		private System.Windows.Forms.Button BtnIntegrate;
	}
}
