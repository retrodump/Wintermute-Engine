namespace DeadCode.WME.WindowEdit
{
	partial class DocumentTypeForm
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
			this.RadWindow = new System.Windows.Forms.RadioButton();
			this.label1 = new System.Windows.Forms.Label();
			this.RadInvBox = new System.Windows.Forms.RadioButton();
			this.RadRespBox = new System.Windows.Forms.RadioButton();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.BtnOK = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// RadWindow
			// 
			this.RadWindow.AutoSize = true;
			this.RadWindow.Location = new System.Drawing.Point(15, 45);
			this.RadWindow.Name = "RadWindow";
			this.RadWindow.Size = new System.Drawing.Size(101, 17);
			this.RadWindow.TabIndex = 1;
			this.RadWindow.TabStop = true;
			this.RadWindow.Text = "Regular window";
			this.RadWindow.UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 9);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(219, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "What type of window do you want to create?";
			// 
			// RadInvBox
			// 
			this.RadInvBox.AutoSize = true;
			this.RadInvBox.Location = new System.Drawing.Point(15, 68);
			this.RadInvBox.Name = "RadInvBox";
			this.RadInvBox.Size = new System.Drawing.Size(89, 17);
			this.RadInvBox.TabIndex = 2;
			this.RadInvBox.TabStop = true;
			this.RadInvBox.Text = "Inventory box";
			this.RadInvBox.UseVisualStyleBackColor = true;
			// 
			// RadRespBox
			// 
			this.RadRespBox.AutoSize = true;
			this.RadRespBox.Location = new System.Drawing.Point(15, 91);
			this.RadRespBox.Name = "RadRespBox";
			this.RadRespBox.Size = new System.Drawing.Size(93, 17);
			this.RadRespBox.TabIndex = 3;
			this.RadRespBox.TabStop = true;
			this.RadRespBox.Text = "Response box";
			this.RadRespBox.UseVisualStyleBackColor = true;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(197, 140);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 5;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(116, 140);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 4;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			this.BtnOK.Click += new System.EventHandler(this.OnOK);
			// 
			// DocumentTypeForm
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnCancel;
			this.ClientSize = new System.Drawing.Size(284, 175);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.RadRespBox);
			this.Controls.Add(this.RadInvBox);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.RadWindow);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "DocumentTypeForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Document type";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.RadioButton RadWindow;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.RadioButton RadInvBox;
		private System.Windows.Forms.RadioButton RadRespBox;
		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.Button BtnOK;
	}
}