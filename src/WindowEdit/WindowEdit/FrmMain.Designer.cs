namespace DeadCode.WME.WindowEdit
{
	partial class FrmMain
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(FrmMain));
			this.MainMenu = new System.Windows.Forms.MenuStrip();
			this.MainToolbar = new System.Windows.Forms.ToolStrip();
			this.MainStatusbar = new System.Windows.Forms.StatusStrip();
			this.SplitMain = new System.Windows.Forms.SplitContainer();
			this.SplitTasks = new System.Windows.Forms.SplitContainer();
			this.TreeLayout = new DeadCode.WME.Controls.TreeViewMS();
			this.PropItem = new System.Windows.Forms.PropertyGrid();
			this.WindowCanvas = new DeadCode.WME.Controls.WmeCanvas();
			this.SplitMain.Panel1.SuspendLayout();
			this.SplitMain.Panel2.SuspendLayout();
			this.SplitMain.SuspendLayout();
			this.SplitTasks.Panel1.SuspendLayout();
			this.SplitTasks.Panel2.SuspendLayout();
			this.SplitTasks.SuspendLayout();
			this.SuspendLayout();
			// 
			// MainMenu
			// 
			this.MainMenu.Location = new System.Drawing.Point(0, 0);
			this.MainMenu.Name = "MainMenu";
			this.MainMenu.Size = new System.Drawing.Size(648, 24);
			this.MainMenu.TabIndex = 0;
			this.MainMenu.Text = "menuStrip1";
			// 
			// MainToolbar
			// 
			this.MainToolbar.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.MainToolbar.Location = new System.Drawing.Point(0, 24);
			this.MainToolbar.Name = "MainToolbar";
			this.MainToolbar.Size = new System.Drawing.Size(648, 25);
			this.MainToolbar.TabIndex = 1;
			this.MainToolbar.Text = "toolStrip1";
			// 
			// MainStatusbar
			// 
			this.MainStatusbar.Location = new System.Drawing.Point(0, 469);
			this.MainStatusbar.Name = "MainStatusbar";
			this.MainStatusbar.Size = new System.Drawing.Size(648, 22);
			this.MainStatusbar.TabIndex = 2;
			this.MainStatusbar.Text = "statusStrip1";
			// 
			// SplitMain
			// 
			this.SplitMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitMain.Location = new System.Drawing.Point(0, 49);
			this.SplitMain.Name = "SplitMain";
			// 
			// SplitMain.Panel1
			// 
			this.SplitMain.Panel1.Controls.Add(this.SplitTasks);
			// 
			// SplitMain.Panel2
			// 
			this.SplitMain.Panel2.Controls.Add(this.WindowCanvas);
			this.SplitMain.Size = new System.Drawing.Size(648, 420);
			this.SplitMain.SplitterDistance = 216;
			this.SplitMain.TabIndex = 3;
			// 
			// SplitTasks
			// 
			this.SplitTasks.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitTasks.Location = new System.Drawing.Point(0, 0);
			this.SplitTasks.Name = "SplitTasks";
			this.SplitTasks.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitTasks.Panel1
			// 
			this.SplitTasks.Panel1.Controls.Add(this.TreeLayout);
			// 
			// SplitTasks.Panel2
			// 
			this.SplitTasks.Panel2.Controls.Add(this.PropItem);
			this.SplitTasks.Size = new System.Drawing.Size(216, 420);
			this.SplitTasks.SplitterDistance = 194;
			this.SplitTasks.TabIndex = 0;
			// 
			// TreeLayout
			// 
			this.TreeLayout.AllowDrop = true;
			this.TreeLayout.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TreeLayout.Location = new System.Drawing.Point(0, 0);
			this.TreeLayout.Name = "TreeLayout";
			this.TreeLayout.SelectionBackColor = System.Drawing.SystemColors.Highlight;
			this.TreeLayout.SelectionMode = DeadCode.WME.Controls.TreeViewSelectionMode.MultiSelect;
			this.TreeLayout.Size = new System.Drawing.Size(216, 194);
			this.TreeLayout.TabIndex = 0;
			// 
			// PropItem
			// 
			this.PropItem.Dock = System.Windows.Forms.DockStyle.Fill;
			this.PropItem.Location = new System.Drawing.Point(0, 0);
			this.PropItem.Name = "PropItem";
			this.PropItem.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.PropItem.Size = new System.Drawing.Size(216, 222);
			this.PropItem.TabIndex = 0;
			this.PropItem.ToolbarVisible = false;
			// 
			// WindowCanvas
			// 
			this.WindowCanvas.AutoScroll = true;
			this.WindowCanvas.BackColor = System.Drawing.Color.Black;
			this.WindowCanvas.Dock = System.Windows.Forms.DockStyle.Fill;
			this.WindowCanvas.Location = new System.Drawing.Point(0, 0);
			this.WindowCanvas.Name = "WindowCanvas";
			this.WindowCanvas.RouteMouseClicks = false;
			this.WindowCanvas.Size = new System.Drawing.Size(428, 420);
			this.WindowCanvas.SuspendedRendering = false;
			this.WindowCanvas.SuspendedUpdate = false;
			this.WindowCanvas.TabIndex = 0;
			this.WindowCanvas.WantArrowKeys = true;
			// 
			// FrmMain
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(648, 491);
			this.Controls.Add(this.SplitMain);
			this.Controls.Add(this.MainStatusbar);
			this.Controls.Add(this.MainToolbar);
			this.Controls.Add(this.MainMenu);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.MainMenuStrip = this.MainMenu;
			this.Name = "FrmMain";
			this.Text = "WindowEdit";
			this.FormClosed += new System.Windows.Forms.FormClosedEventHandler(this.OnFormClosed);
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.Load += new System.EventHandler(this.OnLoad);
			this.SplitMain.Panel1.ResumeLayout(false);
			this.SplitMain.Panel2.ResumeLayout(false);
			this.SplitMain.ResumeLayout(false);
			this.SplitTasks.Panel1.ResumeLayout(false);
			this.SplitTasks.Panel2.ResumeLayout(false);
			this.SplitTasks.ResumeLayout(false);
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.MenuStrip MainMenu;
		private System.Windows.Forms.ToolStrip MainToolbar;
		private System.Windows.Forms.StatusStrip MainStatusbar;
		private System.Windows.Forms.SplitContainer SplitMain;
		private System.Windows.Forms.SplitContainer SplitTasks;
		private DeadCode.WME.Controls.WmeCanvas WindowCanvas;
		private DeadCode.WME.Controls.TreeViewMS TreeLayout;
		private System.Windows.Forms.PropertyGrid PropItem;
	}
}

