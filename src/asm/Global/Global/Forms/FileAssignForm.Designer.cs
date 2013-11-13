namespace DeadCode.WME.Global
{
	partial class FileAssignForm
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
			this.label1 = new System.Windows.Forms.Label();
			this.TxtFilename = new System.Windows.Forms.TextBox();
			this.BtnBrowse = new System.Windows.Forms.Button();
			this.BtnRemove = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.LabelFullPath = new System.Windows.Forms.Label();
			this.BtnEdit = new System.Windows.Forms.Button();
			this.Tooltips = new System.Windows.Forms.ToolTip(this.components);
			this.SuspendLayout();
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(55, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "File name:";
			// 
			// TxtFilename
			// 
			this.TxtFilename.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtFilename.Location = new System.Drawing.Point(15, 25);
			this.TxtFilename.Name = "TxtFilename";
			this.TxtFilename.Size = new System.Drawing.Size(316, 20);
			this.TxtFilename.TabIndex = 1;
			this.TxtFilename.TextChanged += new System.EventHandler(this.OnTextChanged);
			// 
			// BtnBrowse
			// 
			this.BtnBrowse.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowse.Location = new System.Drawing.Point(337, 22);
			this.BtnBrowse.Name = "BtnBrowse";
			this.BtnBrowse.Size = new System.Drawing.Size(26, 23);
			this.BtnBrowse.TabIndex = 2;
			this.BtnBrowse.Text = "...";
			this.Tooltips.SetToolTip(this.BtnBrowse, "Browse for file");
			this.BtnBrowse.UseVisualStyleBackColor = true;
			this.BtnBrowse.Click += new System.EventHandler(this.OnBrowseFile);
			// 
			// BtnRemove
			// 
			this.BtnRemove.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Left)));
			this.BtnRemove.DialogResult = System.Windows.Forms.DialogResult.Abort;
			this.BtnRemove.Location = new System.Drawing.Point(15, 75);
			this.BtnRemove.Name = "BtnRemove";
			this.BtnRemove.Size = new System.Drawing.Size(126, 23);
			this.BtnRemove.TabIndex = 7;
			this.BtnRemove.Text = "Remove assigned file";
			this.BtnRemove.UseVisualStyleBackColor = true;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(239, 75);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 5;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(320, 75);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 6;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// LabelFullPath
			// 
			this.LabelFullPath.AutoSize = true;
			this.LabelFullPath.Location = new System.Drawing.Point(12, 48);
			this.LabelFullPath.Name = "LabelFullPath";
			this.LabelFullPath.Size = new System.Drawing.Size(35, 13);
			this.LabelFullPath.TabIndex = 4;
			this.LabelFullPath.Text = "label2";
			// 
			// BtnEdit
			// 
			this.BtnEdit.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnEdit.BackgroundImage = global::DeadCode.WME.Global.Properties.Resources.edit;
			this.BtnEdit.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Center;
			this.BtnEdit.Location = new System.Drawing.Point(369, 23);
			this.BtnEdit.Name = "BtnEdit";
			this.BtnEdit.Size = new System.Drawing.Size(26, 23);
			this.BtnEdit.TabIndex = 3;
			this.Tooltips.SetToolTip(this.BtnEdit, "Edit selected file");
			this.BtnEdit.UseVisualStyleBackColor = true;
			this.BtnEdit.Click += new System.EventHandler(this.OnEdit);
			// 
			// FileAssignForm
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnCancel;
			this.ClientSize = new System.Drawing.Size(407, 110);
			this.Controls.Add(this.BtnEdit);
			this.Controls.Add(this.LabelFullPath);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnRemove);
			this.Controls.Add(this.BtnBrowse);
			this.Controls.Add(this.TxtFilename);
			this.Controls.Add(this.label1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "FileAssignForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "File selection";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Button BtnBrowse;
		private System.Windows.Forms.Button BtnRemove;
		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.Label LabelFullPath;
		private System.Windows.Forms.ToolTip Tooltips;
		protected System.Windows.Forms.Button BtnEdit;
		protected System.Windows.Forms.TextBox TxtFilename;
		protected System.Windows.Forms.Button BtnOK;
	}
}