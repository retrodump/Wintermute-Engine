namespace DeadCode.WME.Global
{
	partial class ProjectSelectionForm
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
			this.RadRecent = new System.Windows.Forms.RadioButton();
			this.RadOther = new System.Windows.Forms.RadioButton();
			this.label1 = new System.Windows.Forms.Label();
			this.ListRecent = new System.Windows.Forms.ListBox();
			this.TxtProject = new System.Windows.Forms.TextBox();
			this.BtnBrowseProject = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOk = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// RadRecent
			// 
			this.RadRecent.AutoSize = true;
			this.RadRecent.Location = new System.Drawing.Point(15, 43);
			this.RadRecent.Name = "RadRecent";
			this.RadRecent.Size = new System.Drawing.Size(128, 17);
			this.RadRecent.TabIndex = 1;
			this.RadRecent.TabStop = true;
			this.RadRecent.Text = "&Recently used project";
			this.RadRecent.UseVisualStyleBackColor = true;
			this.RadRecent.CheckedChanged += new System.EventHandler(this.OnRadRecent);
			// 
			// RadOther
			// 
			this.RadOther.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.RadOther.AutoSize = true;
			this.RadOther.Location = new System.Drawing.Point(15, 188);
			this.RadOther.Name = "RadOther";
			this.RadOther.Size = new System.Drawing.Size(86, 17);
			this.RadOther.TabIndex = 3;
			this.RadOther.TabStop = true;
			this.RadOther.Text = "&Other project";
			this.RadOther.UseVisualStyleBackColor = true;
			this.RadOther.CheckedChanged += new System.EventHandler(this.OnRadOther);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(279, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Please choose a project the new document will belong to:";
			// 
			// ListRecent
			// 
			this.ListRecent.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListRecent.FormattingEnabled = true;
			this.ListRecent.IntegralHeight = false;
			this.ListRecent.Location = new System.Drawing.Point(15, 66);
			this.ListRecent.Name = "ListRecent";
			this.ListRecent.Size = new System.Drawing.Size(371, 108);
			this.ListRecent.TabIndex = 2;
			this.ListRecent.DoubleClick += new System.EventHandler(this.OnRecentDblClick);
			this.ListRecent.SelectedIndexChanged += new System.EventHandler(this.OnRecentChanged);
			// 
			// TxtProject
			// 
			this.TxtProject.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtProject.Location = new System.Drawing.Point(15, 211);
			this.TxtProject.Name = "TxtProject";
			this.TxtProject.Size = new System.Drawing.Size(341, 20);
			this.TxtProject.TabIndex = 4;
			this.TxtProject.TextChanged += new System.EventHandler(this.OnOtherChanged);
			// 
			// BtnBrowseProject
			// 
			this.BtnBrowseProject.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowseProject.Location = new System.Drawing.Point(362, 211);
			this.BtnBrowseProject.Name = "BtnBrowseProject";
			this.BtnBrowseProject.Size = new System.Drawing.Size(24, 23);
			this.BtnBrowseProject.TabIndex = 5;
			this.BtnBrowseProject.Text = "...";
			this.BtnBrowseProject.UseVisualStyleBackColor = true;
			this.BtnBrowseProject.Click += new System.EventHandler(this.OnBrowseProject);
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(311, 263);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 7;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// BtnOk
			// 
			this.BtnOk.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOk.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOk.Location = new System.Drawing.Point(230, 263);
			this.BtnOk.Name = "BtnOk";
			this.BtnOk.Size = new System.Drawing.Size(75, 23);
			this.BtnOk.TabIndex = 6;
			this.BtnOk.Text = "OK";
			this.BtnOk.UseVisualStyleBackColor = true;
			this.BtnOk.Click += new System.EventHandler(this.OnOK);
			// 
			// ProjectSelectionForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(398, 298);
			this.Controls.Add(this.BtnOk);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnBrowseProject);
			this.Controls.Add(this.TxtProject);
			this.Controls.Add(this.ListRecent);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.RadOther);
			this.Controls.Add(this.RadRecent);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ProjectSelectionForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Project selection";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.RadioButton RadRecent;
		private System.Windows.Forms.RadioButton RadOther;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox ListRecent;
		private System.Windows.Forms.TextBox TxtProject;
		private System.Windows.Forms.Button BtnBrowseProject;
		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.Button BtnOk;
	}
}