namespace Integrator
{
	partial class MainForm
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(MainForm));
			this.BtnBrowseProject = new System.Windows.Forms.Button();
			this.label1 = new System.Windows.Forms.Label();
			this.TxtProjectFile = new System.Windows.Forms.TextBox();
			this.label2 = new System.Windows.Forms.Label();
			this.TxtExtensions = new System.Windows.Forms.TextBox();
			this.TabModules = new System.Windows.Forms.TabControl();
			this.TabIntro = new System.Windows.Forms.TabPage();
			this.label3 = new System.Windows.Forms.Label();
			this.label4 = new System.Windows.Forms.Label();
			this.TabModules.SuspendLayout();
			this.TabIntro.SuspendLayout();
			this.SuspendLayout();
			// 
			// BtnBrowseProject
			// 
			this.BtnBrowseProject.Anchor = ((System.Windows.Forms.AnchorStyles)((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Right)));
			this.BtnBrowseProject.Location = new System.Drawing.Point(462, 24);
			this.BtnBrowseProject.Name = "BtnBrowseProject";
			this.BtnBrowseProject.Size = new System.Drawing.Size(25, 23);
			this.BtnBrowseProject.TabIndex = 6;
			this.BtnBrowseProject.Text = "...";
			this.BtnBrowseProject.UseVisualStyleBackColor = true;
			this.BtnBrowseProject.Click += new System.EventHandler(this.OnBrowseProject);
			// 
			// label1
			// 
			this.label1.AutoSize = true;
			this.label1.Location = new System.Drawing.Point(9, 10);
			this.label1.Name = "label1";
			this.label1.Size = new System.Drawing.Size(188, 13);
			this.label1.TabIndex = 4;
			this.label1.Text = "Your WME game &project file (optional):";
			// 
			// TxtProjectFile
			// 
			this.TxtProjectFile.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtProjectFile.Location = new System.Drawing.Point(12, 26);
			this.TxtProjectFile.Name = "TxtProjectFile";
			this.TxtProjectFile.Size = new System.Drawing.Size(444, 20);
			this.TxtProjectFile.TabIndex = 5;
			// 
			// label2
			// 
			this.label2.AutoSize = true;
			this.label2.Location = new System.Drawing.Point(9, 49);
			this.label2.Name = "label2";
			this.label2.Size = new System.Drawing.Size(273, 13);
			this.label2.TabIndex = 7;
			this.label2.Text = "Script file &extensions delimited by spaces (e.g. script inc):";
			// 
			// TxtExtensions
			// 
			this.TxtExtensions.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TxtExtensions.Location = new System.Drawing.Point(12, 65);
			this.TxtExtensions.Name = "TxtExtensions";
			this.TxtExtensions.Size = new System.Drawing.Size(444, 20);
			this.TxtExtensions.TabIndex = 8;
			// 
			// TabModules
			// 
			this.TabModules.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.TabModules.Controls.Add(this.TabIntro);
			this.TabModules.Location = new System.Drawing.Point(12, 91);
			this.TabModules.Name = "TabModules";
			this.TabModules.SelectedIndex = 0;
			this.TabModules.Size = new System.Drawing.Size(475, 300);
			this.TabModules.TabIndex = 9;
			this.TabModules.Selected += new System.Windows.Forms.TabControlEventHandler(this.OnTabSelected);
			// 
			// TabIntro
			// 
			this.TabIntro.Controls.Add(this.label4);
			this.TabIntro.Controls.Add(this.label3);
			this.TabIntro.Location = new System.Drawing.Point(4, 22);
			this.TabIntro.Name = "TabIntro";
			this.TabIntro.Padding = new System.Windows.Forms.Padding(3);
			this.TabIntro.Size = new System.Drawing.Size(467, 274);
			this.TabIntro.TabIndex = 0;
			this.TabIntro.Text = "Intro";
			this.TabIntro.UseVisualStyleBackColor = true;
			// 
			// label3
			// 
			this.label3.Dock = System.Windows.Forms.DockStyle.Fill;
			this.label3.Location = new System.Drawing.Point(3, 3);
			this.label3.Name = "label3";
			this.label3.Size = new System.Drawing.Size(461, 268);
			this.label3.TabIndex = 0;
			this.label3.Text = resources.GetString("label3.Text");
			// 
			// label4
			// 
			this.label4.AutoSize = true;
			this.label4.Location = new System.Drawing.Point(6, 125);
			this.label4.Name = "label4";
			this.label4.Size = new System.Drawing.Size(395, 13);
			this.label4.TabIndex = 1;
			this.label4.Text = "Note: WME Integrator may require administrator privileges to perform some actions" +
				".";
			// 
			// MainForm
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(499, 403);
			this.Controls.Add(this.TabModules);
			this.Controls.Add(this.label2);
			this.Controls.Add(this.TxtExtensions);
			this.Controls.Add(this.BtnBrowseProject);
			this.Controls.Add(this.label1);
			this.Controls.Add(this.TxtProjectFile);
			this.Name = "MainForm";
			this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
			this.Text = "WME Integrator";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.Load += new System.EventHandler(this.OnLoad);
			this.TabModules.ResumeLayout(false);
			this.TabIntro.ResumeLayout(false);
			this.TabIntro.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button BtnBrowseProject;
		private System.Windows.Forms.Label label1;
		private System.Windows.Forms.Label label2;
		private System.Windows.Forms.TabControl TabModules;
		private System.Windows.Forms.TabPage TabIntro;
		private System.Windows.Forms.Label label3;
		internal System.Windows.Forms.TextBox TxtProjectFile;
		internal System.Windows.Forms.TextBox TxtExtensions;
		private System.Windows.Forms.Label label4;
	}
}

