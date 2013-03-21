namespace DeadCode.WME.Debugger
{
	partial class DebugWindow
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(DebugWindow));
			this.SplitMain = new System.Windows.Forms.SplitContainer();
			this.SplitScript = new System.Windows.Forms.SplitContainer();
			this.SplitSource = new System.Windows.Forms.SplitContainer();
			this.TreeScripts = new System.Windows.Forms.TreeView();
			this.ImgScript = new System.Windows.Forms.ImageList(this.components);
			this.LblScriptInfo = new System.Windows.Forms.Label();
			this.ToolScript = new System.Windows.Forms.ToolStrip();
			this.BtnAlwaysOnTop = new System.Windows.Forms.ToolStripButton();
			this.BtnContinue = new System.Windows.Forms.ToolStripButton();
			this.BtnStep = new System.Windows.Forms.ToolStripButton();
			this.toolStripSeparator1 = new System.Windows.Forms.ToolStripSeparator();
			this.BtnToggleBreak = new System.Windows.Forms.ToolStripButton();
			this.TabVars = new System.Windows.Forms.TabControl();
			this.PageScript = new System.Windows.Forms.TabPage();
			this.GridVars = new System.Windows.Forms.PropertyGrid();
			this.PageGlobal = new System.Windows.Forms.TabPage();
			this.GridGlobals = new System.Windows.Forms.PropertyGrid();
			this.LogText = new System.Windows.Forms.RichTextBox();
			this.PageWatch = new System.Windows.Forms.TabPage();
			this.GridWatch = new System.Windows.Forms.PropertyGrid();
			this.MenuWatch = new System.Windows.Forms.ContextMenuStrip(this.components);
			this.ScrViewer = new DeadCode.WME.Debugger.ScriptViewer();
			this.SplitMain.Panel1.SuspendLayout();
			this.SplitMain.Panel2.SuspendLayout();
			this.SplitMain.SuspendLayout();
			this.SplitScript.Panel1.SuspendLayout();
			this.SplitScript.Panel2.SuspendLayout();
			this.SplitScript.SuspendLayout();
			this.SplitSource.Panel1.SuspendLayout();
			this.SplitSource.Panel2.SuspendLayout();
			this.SplitSource.SuspendLayout();
			this.ToolScript.SuspendLayout();
			this.TabVars.SuspendLayout();
			this.PageScript.SuspendLayout();
			this.PageGlobal.SuspendLayout();
			this.PageWatch.SuspendLayout();
			this.SuspendLayout();
			// 
			// SplitMain
			// 
			this.SplitMain.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitMain.Location = new System.Drawing.Point(0, 0);
			this.SplitMain.Name = "SplitMain";
			this.SplitMain.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitMain.Panel1
			// 
			this.SplitMain.Panel1.Controls.Add(this.SplitScript);
			// 
			// SplitMain.Panel2
			// 
			this.SplitMain.Panel2.Controls.Add(this.LogText);
			this.SplitMain.Size = new System.Drawing.Size(655, 531);
			this.SplitMain.SplitterDistance = 371;
			this.SplitMain.TabIndex = 0;
			// 
			// SplitScript
			// 
			this.SplitScript.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitScript.Location = new System.Drawing.Point(0, 0);
			this.SplitScript.Name = "SplitScript";
			this.SplitScript.Orientation = System.Windows.Forms.Orientation.Horizontal;
			// 
			// SplitScript.Panel1
			// 
			this.SplitScript.Panel1.Controls.Add(this.SplitSource);
			// 
			// SplitScript.Panel2
			// 
			this.SplitScript.Panel2.Controls.Add(this.TabVars);
			this.SplitScript.Size = new System.Drawing.Size(655, 371);
			this.SplitScript.SplitterDistance = 205;
			this.SplitScript.TabIndex = 0;
			// 
			// SplitSource
			// 
			this.SplitSource.Dock = System.Windows.Forms.DockStyle.Fill;
			this.SplitSource.Location = new System.Drawing.Point(0, 0);
			this.SplitSource.Name = "SplitSource";
			// 
			// SplitSource.Panel1
			// 
			this.SplitSource.Panel1.Controls.Add(this.TreeScripts);
			// 
			// SplitSource.Panel2
			// 
			this.SplitSource.Panel2.Controls.Add(this.ScrViewer);
			this.SplitSource.Panel2.Controls.Add(this.LblScriptInfo);
			this.SplitSource.Panel2.Controls.Add(this.ToolScript);
			this.SplitSource.Size = new System.Drawing.Size(655, 205);
			this.SplitSource.SplitterDistance = 218;
			this.SplitSource.TabIndex = 0;
			// 
			// TreeScripts
			// 
			this.TreeScripts.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TreeScripts.HideSelection = false;
			this.TreeScripts.ImageIndex = 0;
			this.TreeScripts.ImageList = this.ImgScript;
			this.TreeScripts.Location = new System.Drawing.Point(0, 0);
			this.TreeScripts.Name = "TreeScripts";
			this.TreeScripts.SelectedImageIndex = 0;
			this.TreeScripts.Size = new System.Drawing.Size(218, 205);
			this.TreeScripts.TabIndex = 0;
			this.TreeScripts.AfterSelect += new System.Windows.Forms.TreeViewEventHandler(this.OnScriptSelection);
			this.TreeScripts.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// ImgScript
			// 
			this.ImgScript.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImgScript.ImageStream")));
			this.ImgScript.TransparentColor = System.Drawing.Color.Fuchsia;
			this.ImgScript.Images.SetKeyName(0, "Folder.bmp");
			this.ImgScript.Images.SetKeyName(1, "FolderClosed.bmp");
			this.ImgScript.Images.SetKeyName(2, "script_running.bmp");
			this.ImgScript.Images.SetKeyName(3, "script_persistent.bmp");
			this.ImgScript.Images.SetKeyName(4, "script_finished.bmp");
			this.ImgScript.Images.SetKeyName(5, "script_waiting.bmp");
			this.ImgScript.Images.SetKeyName(6, "script_error.bmp");
			// 
			// LblScriptInfo
			// 
			this.LblScriptInfo.AutoSize = true;
			this.LblScriptInfo.Location = new System.Drawing.Point(3, 25);
			this.LblScriptInfo.Name = "LblScriptInfo";
			this.LblScriptInfo.Size = new System.Drawing.Size(35, 13);
			this.LblScriptInfo.TabIndex = 1;
			this.LblScriptInfo.Text = "label1";
			// 
			// ToolScript
			// 
			this.ToolScript.GripStyle = System.Windows.Forms.ToolStripGripStyle.Hidden;
			this.ToolScript.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.BtnAlwaysOnTop,
            this.BtnContinue,
            this.BtnStep,
            this.toolStripSeparator1,
            this.BtnToggleBreak});
			this.ToolScript.Location = new System.Drawing.Point(0, 0);
			this.ToolScript.Name = "ToolScript";
			this.ToolScript.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.ToolScript.Size = new System.Drawing.Size(433, 25);
			this.ToolScript.TabIndex = 0;
			// 
			// BtnAlwaysOnTop
			// 
			this.BtnAlwaysOnTop.Alignment = System.Windows.Forms.ToolStripItemAlignment.Right;
			this.BtnAlwaysOnTop.CheckOnClick = true;
			this.BtnAlwaysOnTop.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnAlwaysOnTop.Image = global::DeadCode.WME.Debugger.Properties.Resources.pin;
			this.BtnAlwaysOnTop.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnAlwaysOnTop.Name = "BtnAlwaysOnTop";
			this.BtnAlwaysOnTop.Size = new System.Drawing.Size(23, 22);
			this.BtnAlwaysOnTop.Text = "Always on top";
			this.BtnAlwaysOnTop.Click += new System.EventHandler(this.OnAlwaysOnTop);
			// 
			// BtnContinue
			// 
			this.BtnContinue.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnContinue.Image = global::DeadCode.WME.Debugger.Properties.Resources._continue;
			this.BtnContinue.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnContinue.Name = "BtnContinue";
			this.BtnContinue.Size = new System.Drawing.Size(23, 22);
			this.BtnContinue.Text = "Continue";
			this.BtnContinue.Click += new System.EventHandler(this.OnContinue);
			// 
			// BtnStep
			// 
			this.BtnStep.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Image;
			this.BtnStep.Image = global::DeadCode.WME.Debugger.Properties.Resources.step;
			this.BtnStep.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnStep.Name = "BtnStep";
			this.BtnStep.Size = new System.Drawing.Size(23, 22);
			this.BtnStep.Text = "Step";
			this.BtnStep.Click += new System.EventHandler(this.OnStep);
			// 
			// toolStripSeparator1
			// 
			this.toolStripSeparator1.Name = "toolStripSeparator1";
			this.toolStripSeparator1.Size = new System.Drawing.Size(6, 25);
			// 
			// BtnToggleBreak
			// 
			this.BtnToggleBreak.AutoToolTip = false;
			this.BtnToggleBreak.DisplayStyle = System.Windows.Forms.ToolStripItemDisplayStyle.Text;
			this.BtnToggleBreak.ImageTransparentColor = System.Drawing.Color.Magenta;
			this.BtnToggleBreak.Name = "BtnToggleBreak";
			this.BtnToggleBreak.Size = new System.Drawing.Size(111, 22);
			this.BtnToggleBreak.Text = "Toggle breakpoint";
			this.BtnToggleBreak.Click += new System.EventHandler(this.OnToggleBreak);
			// 
			// TabVars
			// 
			this.TabVars.Controls.Add(this.PageScript);
			this.TabVars.Controls.Add(this.PageGlobal);
			this.TabVars.Controls.Add(this.PageWatch);
			this.TabVars.Dock = System.Windows.Forms.DockStyle.Fill;
			this.TabVars.Location = new System.Drawing.Point(0, 0);
			this.TabVars.Multiline = true;
			this.TabVars.Name = "TabVars";
			this.TabVars.SelectedIndex = 0;
			this.TabVars.Size = new System.Drawing.Size(655, 162);
			this.TabVars.TabIndex = 0;
			// 
			// PageScript
			// 
			this.PageScript.Controls.Add(this.GridVars);
			this.PageScript.Location = new System.Drawing.Point(4, 22);
			this.PageScript.Name = "PageScript";
			this.PageScript.Padding = new System.Windows.Forms.Padding(3);
			this.PageScript.Size = new System.Drawing.Size(647, 136);
			this.PageScript.TabIndex = 0;
			this.PageScript.Text = "Script variables";
			this.PageScript.UseVisualStyleBackColor = true;
			// 
			// GridVars
			// 
			this.GridVars.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GridVars.HelpVisible = false;
			this.GridVars.Location = new System.Drawing.Point(3, 3);
			this.GridVars.Name = "GridVars";
			this.GridVars.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.GridVars.Size = new System.Drawing.Size(641, 130);
			this.GridVars.TabIndex = 0;
			this.GridVars.ToolbarVisible = false;
			// 
			// PageGlobal
			// 
			this.PageGlobal.Controls.Add(this.GridGlobals);
			this.PageGlobal.Location = new System.Drawing.Point(4, 22);
			this.PageGlobal.Name = "PageGlobal";
			this.PageGlobal.Padding = new System.Windows.Forms.Padding(3);
			this.PageGlobal.Size = new System.Drawing.Size(647, 136);
			this.PageGlobal.TabIndex = 1;
			this.PageGlobal.Text = "Global variables";
			this.PageGlobal.UseVisualStyleBackColor = true;
			// 
			// GridGlobals
			// 
			this.GridGlobals.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GridGlobals.HelpVisible = false;
			this.GridGlobals.Location = new System.Drawing.Point(3, 3);
			this.GridGlobals.Name = "GridGlobals";
			this.GridGlobals.PropertySort = System.Windows.Forms.PropertySort.Categorized;
			this.GridGlobals.Size = new System.Drawing.Size(641, 130);
			this.GridGlobals.TabIndex = 1;
			this.GridGlobals.ToolbarVisible = false;
			// 
			// LogText
			// 
			this.LogText.BackColor = System.Drawing.SystemColors.Window;
			this.LogText.Dock = System.Windows.Forms.DockStyle.Fill;
			this.LogText.Location = new System.Drawing.Point(0, 0);
			this.LogText.Name = "LogText";
			this.LogText.ReadOnly = true;
			this.LogText.Size = new System.Drawing.Size(655, 156);
			this.LogText.TabIndex = 0;
			this.LogText.Text = "";
			// 
			// PageWatch
			// 
			this.PageWatch.Controls.Add(this.GridWatch);
			this.PageWatch.Location = new System.Drawing.Point(4, 22);
			this.PageWatch.Name = "PageWatch";
			this.PageWatch.Padding = new System.Windows.Forms.Padding(3);
			this.PageWatch.Size = new System.Drawing.Size(647, 136);
			this.PageWatch.TabIndex = 2;
			this.PageWatch.Text = "Watch";
			this.PageWatch.UseVisualStyleBackColor = true;
			// 
			// GridWatch
			// 
			this.GridWatch.ContextMenuStrip = this.MenuWatch;
			this.GridWatch.Dock = System.Windows.Forms.DockStyle.Fill;
			this.GridWatch.HelpVisible = false;
			this.GridWatch.Location = new System.Drawing.Point(3, 3);
			this.GridWatch.Name = "GridWatch";
			this.GridWatch.PropertySort = System.Windows.Forms.PropertySort.NoSort;
			this.GridWatch.Size = new System.Drawing.Size(641, 130);
			this.GridWatch.TabIndex = 1;
			this.GridWatch.ToolbarVisible = false;
			// 
			// MenuWatch
			// 
			this.MenuWatch.Name = "MenuWatch";
			this.MenuWatch.RenderMode = System.Windows.Forms.ToolStripRenderMode.System;
			this.MenuWatch.Size = new System.Drawing.Size(61, 4);
			this.MenuWatch.Opening += new System.ComponentModel.CancelEventHandler(this.OnMenuWatch);
			// 
			// ScrViewer
			// 
			this.ScrViewer.Anchor = ((System.Windows.Forms.AnchorStyles)((((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Bottom)
						| System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.ScrViewer.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
			this.ScrViewer.FullRowSelect = true;
			this.ScrViewer.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.ScrViewer.Location = new System.Drawing.Point(0, 41);
			this.ScrViewer.MultiSelect = false;
			this.ScrViewer.Name = "ScrViewer";
			this.ScrViewer.Size = new System.Drawing.Size(433, 164);
			this.ScrViewer.TabIndex = 2;
			this.ScrViewer.UseCompatibleStateImageBehavior = false;
			this.ScrViewer.View = System.Windows.Forms.View.Details;
			this.ScrViewer.KeyDown += new System.Windows.Forms.KeyEventHandler(this.OnKeyDown);
			// 
			// DebugWindow
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(655, 531);
			this.Controls.Add(this.SplitMain);
			this.Icon = ((System.Drawing.Icon)(resources.GetObject("$this.Icon")));
			this.Name = "DebugWindow";
			this.Text = "Debugging console";
			this.FormClosing += new System.Windows.Forms.FormClosingEventHandler(this.OnFormClosing);
			this.Load += new System.EventHandler(this.OnLoad);
			this.SplitMain.Panel1.ResumeLayout(false);
			this.SplitMain.Panel2.ResumeLayout(false);
			this.SplitMain.ResumeLayout(false);
			this.SplitScript.Panel1.ResumeLayout(false);
			this.SplitScript.Panel2.ResumeLayout(false);
			this.SplitScript.ResumeLayout(false);
			this.SplitSource.Panel1.ResumeLayout(false);
			this.SplitSource.Panel2.ResumeLayout(false);
			this.SplitSource.Panel2.PerformLayout();
			this.SplitSource.ResumeLayout(false);
			this.ToolScript.ResumeLayout(false);
			this.ToolScript.PerformLayout();
			this.TabVars.ResumeLayout(false);
			this.PageScript.ResumeLayout(false);
			this.PageGlobal.ResumeLayout(false);
			this.PageWatch.ResumeLayout(false);
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.SplitContainer SplitMain;
		private System.Windows.Forms.RichTextBox LogText;
		private System.Windows.Forms.SplitContainer SplitScript;
		private System.Windows.Forms.SplitContainer SplitSource;
		private System.Windows.Forms.TreeView TreeScripts;
		private System.Windows.Forms.PropertyGrid GridVars;
		private System.Windows.Forms.ImageList ImgScript;
		private System.Windows.Forms.TabControl TabVars;
		private System.Windows.Forms.TabPage PageScript;
		private System.Windows.Forms.TabPage PageGlobal;
		private System.Windows.Forms.PropertyGrid GridGlobals;
		private System.Windows.Forms.Label LblScriptInfo;
		private System.Windows.Forms.ToolStrip ToolScript;
		private System.Windows.Forms.ToolStripButton BtnAlwaysOnTop;
		private ScriptViewer ScrViewer;
		private System.Windows.Forms.ToolStripButton BtnToggleBreak;
		private System.Windows.Forms.ToolStripButton BtnContinue;
		private System.Windows.Forms.ToolStripButton BtnStep;
		private System.Windows.Forms.ToolStripSeparator toolStripSeparator1;
		private System.Windows.Forms.TabPage PageWatch;
		private System.Windows.Forms.PropertyGrid GridWatch;
		private System.Windows.Forms.ContextMenuStrip MenuWatch;

	}
}