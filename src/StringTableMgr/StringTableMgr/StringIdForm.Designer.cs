namespace DeadCode.WME.StringTableMgr
{
	partial class StringIdForm
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
			this.TxtStringID = new System.Windows.Forms.TextBox();
			this.label1 = new System.Windows.Forms.Label();
			this.ListRecentIDs = new System.Windows.Forms.ListBox();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// TxtStringID
			// 
			this.TxtStringID.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtStringID.AutoCompleteMode = System.Windows.Forms.AutoCompleteMode.Append;
			this.TxtStringID.AutoCompleteSource = System.Windows.Forms.AutoCompleteSource.CustomSource;
			this.TxtStringID.Location = new System.Drawing.Point(12, 25);
			this.TxtStringID.Name = "TxtStringID";
			this.TxtStringID.Size = new System.Drawing.Size(276, 20);
			this.TxtStringID.TabIndex = 1;
			this.TxtStringID.TextChanged += new System.EventHandler(this.OnTextChanged);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(77, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "String ID base:";
			// 
			// ListRecentIDs
			// 
			this.ListRecentIDs.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListRecentIDs.FormattingEnabled = true;
			this.ListRecentIDs.Location = new System.Drawing.Point(12, 51);
			this.ListRecentIDs.Name = "ListRecentIDs";
			this.ListRecentIDs.Size = new System.Drawing.Size(276, 238);
			this.ListRecentIDs.TabIndex = 2;
			this.ListRecentIDs.DoubleClick += new System.EventHandler(this.OnItemDblClick);
			this.ListRecentIDs.SelectedIndexChanged += new System.EventHandler(this.OnSelectedItem);
			// 
			// BtnOK
			// 
			this.BtnOK.Location = new System.Drawing.Point(132, 297);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 3;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			this.BtnOK.Click += new System.EventHandler(this.OnOK);
			// 
			// BtnCancel
			// 
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(213, 297);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 4;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// StringIdForm
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnCancel;
			this.ClientSize = new System.Drawing.Size(300, 332);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.ListRecentIDs);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.TxtStringID);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "StringIdForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Choose string ID";
			this.Load += new System.EventHandler(this.OnLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.TextBox TxtStringID;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.ListBox ListRecentIDs;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
	}
}