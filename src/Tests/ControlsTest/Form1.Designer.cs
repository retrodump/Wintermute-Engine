namespace ControlsTest
{
	partial class Form1
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
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(Form1));
			this.button1 = new System.Windows.Forms.Button();
			this.imageList1 = new System.Windows.Forms.ImageList(this.components);
			this.button3 = new System.Windows.Forms.Button();
			this.menuStrip1 = new System.Windows.Forms.MenuStrip();
			this.toolStripMenuItem1 = new System.Windows.Forms.ToolStripMenuItem();
			this.treeColumn1 = new Aga.Controls.Tree.TreeColumn();
			this.button2 = new System.Windows.Forms.Button();
			this.button4 = new System.Windows.Forms.Button();
			this.button5 = new System.Windows.Forms.Button();
			this.treeListView1 = new DeadCode.WME.Controls.TreeListView();
			this.nodeTextBox1 = new Aga.Controls.Tree.NodeControls.NodeTextBox();
			this.imageListBox1 = new DeadCode.WME.Controls.ImageListBox();
			this.menuStrip1.SuspendLayout();
			this.SuspendLayout();
			// 
			// button1
			// 
			this.button1.Location = new System.Drawing.Point(12, 35);
			this.button1.Name = "button1";
			this.button1.Size = new System.Drawing.Size(75, 23);
			this.button1.TabIndex = 0;
			this.button1.Text = "tooltip";
			this.button1.Click += new System.EventHandler(this.button1_Click);
			// 
			// imageList1
			// 
			this.imageList1.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("imageList1.ImageStream")));
			this.imageList1.TransparentColor = System.Drawing.Color.Transparent;
			this.imageList1.Images.SetKeyName(0, "event.bmp");
			this.imageList1.Images.SetKeyName(1, "method.bmp");
			this.imageList1.Images.SetKeyName(2, "property.bmp");
			this.imageList1.Images.SetKeyName(3, "variable.bmp");
			// 
			// button3
			// 
			this.button3.Location = new System.Drawing.Point(91, 64);
			this.button3.Name = "button3";
			this.button3.Size = new System.Drawing.Size(75, 23);
			this.button3.TabIndex = 3;
			this.button3.Text = "fill listbox";
			this.button3.Click += new System.EventHandler(this.button3_Click);
			// 
			// menuStrip1
			// 
			this.menuStrip1.Items.AddRange(new System.Windows.Forms.ToolStripItem[] {
            this.toolStripMenuItem1});
			this.menuStrip1.Location = new System.Drawing.Point(0, 0);
			this.menuStrip1.Name = "menuStrip1";
			this.menuStrip1.Size = new System.Drawing.Size(715, 24);
			this.menuStrip1.TabIndex = 4;
			this.menuStrip1.Text = "menuStrip1";
			// 
			// toolStripMenuItem1
			// 
			this.toolStripMenuItem1.Name = "toolStripMenuItem1";
			this.toolStripMenuItem1.Size = new System.Drawing.Size(128, 20);
			this.toolStripMenuItem1.Text = "toolStripMenuItem1";
			// 
			// treeColumn1
			// 
			this.treeColumn1.Header = "Name";
			this.treeColumn1.SortOrder = System.Windows.Forms.SortOrder.None;
			this.treeColumn1.Width = 317;
			// 
			// button2
			// 
			this.button2.Location = new System.Drawing.Point(203, 35);
			this.button2.Name = "button2";
			this.button2.Size = new System.Drawing.Size(75, 23);
			this.button2.TabIndex = 6;
			this.button2.Text = "fill treeview";
			this.button2.UseVisualStyleBackColor = true;
			this.button2.Click += new System.EventHandler(this.button2_Click_1);
			// 
			// button4
			// 
			this.button4.Location = new System.Drawing.Point(284, 35);
			this.button4.Name = "button4";
			this.button4.Size = new System.Drawing.Size(75, 23);
			this.button4.TabIndex = 7;
			this.button4.Text = "button4";
			this.button4.UseVisualStyleBackColor = true;
			this.button4.Click += new System.EventHandler(this.button4_Click);
			// 
			// button5
			// 
			this.button5.Location = new System.Drawing.Point(365, 35);
			this.button5.Name = "button5";
			this.button5.Size = new System.Drawing.Size(75, 23);
			this.button5.TabIndex = 8;
			this.button5.Text = "button5";
			this.button5.UseVisualStyleBackColor = true;
			this.button5.Click += new System.EventHandler(this.button5_Click);
			// 
			// treeListView1
			// 
			this.treeListView1.Anchor = ((System.Windows.Forms.AnchorStyles)(((System.Windows.Forms.AnchorStyles.Top | System.Windows.Forms.AnchorStyles.Left)
						| System.Windows.Forms.AnchorStyles.Right)));
			this.treeListView1.BackColor = System.Drawing.SystemColors.Window;
			this.treeListView1.Columns.Add(this.treeColumn1);
			this.treeListView1.Cursor = System.Windows.Forms.Cursors.Default;
			this.treeListView1.DefaultToolTipProvider = null;
			this.treeListView1.DragDropMarkColor = System.Drawing.Color.Black;
			this.treeListView1.FitLastColumn = true;
			this.treeListView1.FullRowSelect = true;
			this.treeListView1.GridLines = true;
			this.treeListView1.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.Clickable;
			this.treeListView1.LineColor = System.Drawing.SystemColors.ControlDark;
			this.treeListView1.Location = new System.Drawing.Point(205, 64);
			this.treeListView1.Model = null;
			this.treeListView1.Name = "treeListView1";
			this.treeListView1.NodeControls.Add(this.nodeTextBox1);
			this.treeListView1.Search.BackColor = System.Drawing.Color.Pink;
			this.treeListView1.Search.FontColor = System.Drawing.Color.Black;
			this.treeListView1.SelectedNode = null;
			this.treeListView1.Size = new System.Drawing.Size(356, 287);
			this.treeListView1.TabIndex = 5;
			this.treeListView1.Text = "treeListView1";
			// 
			// nodeTextBox1
			// 
			this.nodeTextBox1.DataPropertyName = "Text";
			this.nodeTextBox1.ParentColumn = this.treeColumn1;
			// 
			// imageListBox1
			// 
			this.imageListBox1.DrawMode = System.Windows.Forms.DrawMode.OwnerDrawFixed;
			this.imageListBox1.FormattingEnabled = true;
			this.imageListBox1.ImageList = this.imageList1;
			this.imageListBox1.ItemHeight = 16;
			this.imageListBox1.Items.AddRange(new object[] {
            "aaa",
            "bbb",
            "ccc"});
			this.imageListBox1.Location = new System.Drawing.Point(12, 64);
			this.imageListBox1.Name = "imageListBox1";
			this.imageListBox1.Size = new System.Drawing.Size(73, 100);
			this.imageListBox1.TabIndex = 2;
			// 
			// Form1
			// 
			this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
			this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
			this.ClientSize = new System.Drawing.Size(715, 411);
			this.Controls.Add(this.button5);
			this.Controls.Add(this.button4);
			this.Controls.Add(this.button2);
			this.Controls.Add(this.treeListView1);
			this.Controls.Add(this.button3);
			this.Controls.Add(this.imageListBox1);
			this.Controls.Add(this.button1);
			this.Controls.Add(this.menuStrip1);
			this.MainMenuStrip = this.menuStrip1;
			this.Name = "Form1";
			this.Text = "Form1";
			this.menuStrip1.ResumeLayout(false);
			this.menuStrip1.PerformLayout();
			this.ResumeLayout(false);
			this.PerformLayout();

		}

		#endregion

		private System.Windows.Forms.Button button1;
		private DeadCode.WME.Controls.ImageListBox imageListBox1;
		private System.Windows.Forms.ImageList imageList1;
		private System.Windows.Forms.Button button3;
		private System.Windows.Forms.MenuStrip menuStrip1;
		private System.Windows.Forms.ToolStripMenuItem toolStripMenuItem1;
		private DeadCode.WME.Controls.TreeListView treeListView1;
		private Aga.Controls.Tree.NodeControls.NodeTextBox nodeTextBox1;
		private System.Windows.Forms.Button button2;
		private Aga.Controls.Tree.TreeColumn treeColumn1;
		private System.Windows.Forms.Button button4;
		private System.Windows.Forms.Button button5;


	}
}

