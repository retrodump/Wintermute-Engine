namespace DeadCode.WME.WindowEdit
{
	partial class SettingsForm
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
			this.BtnSelectionCol = new DeadCode.WME.Controls.ColorButton();
			this.label1 = new System.Windows.Forms.Label();
			this.TxtGridWidth = new System.Windows.Forms.MaskedTextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.TxtGridHeight = new System.Windows.Forms.MaskedTextBox();
			this.label3 = new System.Windows.Forms.Label();
			this.BtnBoxCol = new DeadCode.WME.Controls.ColorButton();
			this.label4 = new System.Windows.Forms.Label();
			this.label5 = new System.Windows.Forms.Label();
			this.BtnBackCol = new DeadCode.WME.Controls.ColorButton();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.SuspendLayout();
			// 
			// BtnSelectionCol
			// 
			this.BtnSelectionCol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnSelectionCol.ButtonColor = System.Drawing.SystemColors.ControlDark;
			this.BtnSelectionCol.Location = new System.Drawing.Point(168, 64);
			this.BtnSelectionCol.Name = "BtnSelectionCol";
			this.BtnSelectionCol.Size = new System.Drawing.Size(75, 22);
			this.BtnSelectionCol.TabIndex = 5;
			this.BtnSelectionCol.UseVisualStyleBackColor = true;
			this.BtnSelectionCol.Click += new System.EventHandler(this.OnColorPick);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(12, 15);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(77, 13);
			this.label1.TabIndex = 0;
			this.label1.Text = "Horizontal grid:";
			// 
			// TxtGridWidth
			// 
			this.TxtGridWidth.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TxtGridWidth.Location = new System.Drawing.Point(168, 12);
			this.TxtGridWidth.Mask = "000";
			this.TxtGridWidth.Name = "TxtGridWidth";
			this.TxtGridWidth.PromptChar = ' ';
			this.TxtGridWidth.Size = new System.Drawing.Size(35, 20);
			this.TxtGridWidth.TabIndex = 1;
			this.TxtGridWidth.Leave += new System.EventHandler(this.OnGridLeave);
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(12, 41);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(65, 13);
			this.label2.TabIndex = 2;
			this.label2.Text = "Vertical grid:";
			// 
			// TxtGridHeight
			// 
			this.TxtGridHeight.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.TxtGridHeight.HidePromptOnLeave = true;
			this.TxtGridHeight.Location = new System.Drawing.Point(168, 38);
			this.TxtGridHeight.Mask = "000";
			this.TxtGridHeight.Name = "TxtGridHeight";
			this.TxtGridHeight.PromptChar = ' ';
			this.TxtGridHeight.Size = new System.Drawing.Size(35, 20);
			this.TxtGridHeight.TabIndex = 3;
			this.TxtGridHeight.Leave += new System.EventHandler(this.OnGridLeave);
			// 
			// label3
			// 
			this.label3.AutoSize = true;
			this.label3.Location = new System.Drawing.Point(12, 69);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(80, 13);
			this.label3.TabIndex = 4;
			this.label3.Text = "Selection color:";
			// 
			// BtnBoxCol
			// 
			this.BtnBoxCol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBoxCol.ButtonColor = System.Drawing.SystemColors.ControlDark;
			this.BtnBoxCol.Location = new System.Drawing.Point(168, 92);
			this.BtnBoxCol.Name = "BtnBoxCol";
			this.BtnBoxCol.Size = new System.Drawing.Size(75, 22);
			this.BtnBoxCol.TabIndex = 7;
			this.BtnBoxCol.UseVisualStyleBackColor = true;
			this.BtnBoxCol.Click += new System.EventHandler(this.OnColorPick);
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(12, 97);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(148, 13);
			this.label4.TabIndex = 6;
			this.label4.Text = "Inventory/response box color:";
			// 
			// label5
			// 
			this.label5.AutoSize = true;
			this.label5.Location = new System.Drawing.Point(12, 125);
			this.label5.Name = "label5";
			this.label5.Size = new System.Drawing.Size(123, 13);
			this.label5.TabIndex = 8;
			this.label5.Text = "Editor background color:";
			// 
			// BtnBackCol
			// 
			this.BtnBackCol.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBackCol.ButtonColor = System.Drawing.SystemColors.ControlDark;
			this.BtnBackCol.Location = new System.Drawing.Point(168, 120);
			this.BtnBackCol.Name = "BtnBackCol";
			this.BtnBackCol.Size = new System.Drawing.Size(75, 22);
			this.BtnBackCol.TabIndex = 9;
			this.BtnBackCol.UseVisualStyleBackColor = true;
			this.BtnBackCol.Click += new System.EventHandler(this.OnColorPick);
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(87, 164);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 10;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(168, 164);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 11;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// SettingsForm
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnCancel;
			this.ClientSize = new System.Drawing.Size(255, 199);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.BtnBackCol);
			this.Controls.Add(this.label5);
			this.Controls.Add(this.label4);
			this.Controls.Add(this.BtnBoxCol);
			this.Controls.Add(this.label3);
			this.Controls.Add(this.TxtGridHeight);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.TxtGridWidth);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.BtnSelectionCol);
			this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "SettingsForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "Options";
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private DeadCode.WME.Controls.ColorButton BtnSelectionCol;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.MaskedTextBox TxtGridWidth;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.MaskedTextBox TxtGridHeight;
		private System.Windows.Forms.Label label3;
		private DeadCode.WME.Controls.ColorButton BtnBoxCol;
		private System.Windows.Forms.Label label4;
		private System.Windows.Forms.Label label5;
		private DeadCode.WME.Controls.ColorButton BtnBackCol;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
	}
}