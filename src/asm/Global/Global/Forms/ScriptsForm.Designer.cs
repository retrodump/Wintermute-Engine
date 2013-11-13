namespace DeadCode.WME.Global
{
	partial class ScriptsForm
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
			this.toolStrip1 = new System.Windows.Forms.ToolStrip();
			this.BtnDown = new System.Windows.Forms.ToolStripButton();
			this.BtnUp = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.BtnAttach = new System.Windows.Forms.ToolStripButton();
			this.BtnEdit = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator2 = new System.Windows.Forms.ToolStripSeparator();
			this.BtnDelete = new System.Windows.Forms.ToolStripButton();
			this.ListScripts = new System.Windows.Forms.ListBox();
			this.BtnOK = new System.Windows.Forms.Button();
			this.BtnCancel = new System.Windows.Forms.Button();
			this.toolStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// toolStrip1
			// 
			this.toolStrip1.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.toolStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.BtnDown,
            this.BtnUp,
            this.toolStripSeparator1,
            this.BtnAttach,
            this.BtnEdit,
            this.toolStripSeparator2,
            this.BtnDelete});
			this.toolStrip1.Location = new System.Drawing.Point(0, 0);
			this.toolStrip1.Name = "toolStrip1";
			this.toolStrip1.Size = new System.Drawing.Size(394, 25);
			this.toolStrip1.TabIndex = 0;
			this.toolStrip1.Text = "toolStrip1";
			// 
			// BtnDown
			// 
			this.BtnDown.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnDown.Image = global::DeadCode.WME.Global.Properties.Resources.arrow_down;
			this.BtnDown.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnDown.Name = "BtnDown";
			this.BtnDown.Size = new System.Drawing.Size(23, 22);
			this.BtnDown.Text = "Move down";
			this.BtnDown.Click += new System.EventHandler(this.OnDown);
			// 
			// BtnUp
			// 
			this.BtnUp.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnUp.Image = global::DeadCode.WME.Global.Properties.Resources.arrow_up;
			this.BtnUp.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnUp.Name = "BtnUp";
			this.BtnUp.Size = new System.Drawing.Size(23, 22);
			this.BtnUp.Text = "Move up";
			this.BtnUp.Click += new System.EventHandler(this.OnUp);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// BtnAttach
			// 
			this.BtnAttach.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnAttach.Image = global::DeadCode.WME.Global.Properties.Resources.attachment;
			this.BtnAttach.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnAttach.Name = "BtnAttach";
			this.BtnAttach.Size = new System.Drawing.Size(23, 22);
			this.BtnAttach.Text = "Attach existing script";
			this.BtnAttach.Click += new System.EventHandler(this.OnAttach);
			// 
			// BtnEdit
			// 
			this.BtnEdit.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnEdit.Image = global::DeadCode.WME.Global.Properties.Resources.edit;
			this.BtnEdit.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnEdit.Name = "BtnEdit";
			this.BtnEdit.Size = new System.Drawing.Size(23, 22);
			this.BtnEdit.Text = "Edit selected script";
			this.BtnEdit.Click += new System.EventHandler(this.OnEdit);
			// 
			// toolStripSeparator2
			// 
			this.toolStripSeparator2.Name = "toolStripSeparator2";
			this.toolStripSeparator2.Size = new System.Drawing.Size(6, 25);
			// 
			// BtnDelete
			// 
			this.BtnDelete.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnDelete.Image = global::DeadCode.WME.Global.Properties.Resources.delete;
			this.BtnDelete.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnDelete.Name = "BtnDelete";
			this.BtnDelete.Size = new System.Drawing.Size(23, 22);
			this.BtnDelete.Text = "Detach selected script";
			this.BtnDelete.Click += new System.EventHandler(this.OnDetach);
			// 
			// ListScripts
			// 
			this.ListScripts.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ListScripts.FormattingEnabled = true;
			this.ListScripts.IntegralHeight = false;
			this.ListScripts.Location = new System.Drawing.Point(0, 28);
			this.ListScripts.Name = "ListScripts";
			this.ListScripts.Size = new System.Drawing.Size(394, 127);
			this.ListScripts.TabIndex = 1;
			this.ListScripts.DoubleClick += new System.EventHandler(this.OnListDblClick);
			this.ListScripts.SelectedValueChanged += new System.EventHandler(this.OnSelectionChanged);
			// 
			// BtnOK
			// 
			this.BtnOK.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnOK.DialogResult = System.Windows.Forms.DialogResult.OK;
			this.BtnOK.Location = new System.Drawing.Point(226, 161);
			this.BtnOK.Name = "BtnOK";
			this.BtnOK.Size = new System.Drawing.Size(75, 23);
			this.BtnOK.TabIndex = 2;
			this.BtnOK.Text = "OK";
			this.BtnOK.UseVisualStyleBackColor = true;
			this.BtnOK.Click += new System.EventHandler(this.OnOK);
			// 
			// BtnCancel
			// 
			this.BtnCancel.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Bottom | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnCancel.DialogResult = System.Windows.Forms.DialogResult.Cancel;
			this.BtnCancel.Location = new System.Drawing.Point(307, 161);
			this.BtnCancel.Name = "BtnCancel";
			this.BtnCancel.Size = new System.Drawing.Size(75, 23);
			this.BtnCancel.TabIndex = 3;
			this.BtnCancel.Text = "Cancel";
			this.BtnCancel.UseVisualStyleBackColor = true;
			// 
			// ScriptsForm
			// 
			this.AcceptButton = this.BtnOK;
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.CancelButton = this.BtnCancel;
			this.ClientSize = new System.Drawing.Size(394, 196);
			this.Controls.Add(this.BtnCancel);
			this.Controls.Add(this.BtnOK);
			this.Controls.Add(this.ListScripts);
			this.Controls.Add(this.toolStrip1);
			this.MaximizeBox = false;
			this.MinimizeBox = false;
			this.Name = "ScriptsForm";
			this.ShowIcon = false;
			this.ShowInTaskbar = false;
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterParent;
			this.Text = "Scripts";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.toolStrip1.ResumeLayout(false);
			this.toolStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.ToolStrip toolStrip1;
		private System.Windows.Forms.ListBox ListScripts;
		private System.Windows.Forms.Button BtnOK;
		private System.Windows.Forms.Button BtnCancel;
		private System.Windows.Forms.ToolStripButton BtnDown;
		private System.Windows.Forms.ToolStripButton BtnUp;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.ToolStripButton BtnAttach;
		private System.Windows.Forms.ToolStripButton BtnEdit;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator2;
		private System.Windows.Forms.ToolStripButton BtnDelete;
	}
}