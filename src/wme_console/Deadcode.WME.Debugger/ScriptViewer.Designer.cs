namespace DeadCode.WME.Debugger
{
	partial class ScriptViewer
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

		#region Component Designer generated code

		/// <summary>
		/// Required method for Designer support - do not modify 
		/// the contents of this method with the code editor.
		/// </summary>
		private void InitializeComponent()
		{
			this.components = new System.ComponentModel.Container();
			System.ComponentModel.ComponentResourceManager resources = new System.ComponentModel.ComponentResourceManager(typeof(ScriptViewer));
			this.columnHeader1 = new System.Windows.Forms.ColumnHeader();
			this.columnHeader2 = new System.Windows.Forms.ColumnHeader();
			this.ImgIcons = new System.Windows.Forms.ImageList(this.components);
			this.SuspendLayout();
			// 
			// columnHeader1
			// 
			this.columnHeader1.Text = "Line";
			this.columnHeader1.Width = -1;
			// 
			// columnHeader2
			// 
			this.columnHeader2.Text = "Code";
			this.columnHeader2.Width = -1;
			// 
			// ImgIcons
			// 
			this.ImgIcons.ImageStream = ((System.Windows.Forms.ImageListStreamer)(resources.GetObject("ImgIcons.ImageStream")));
			this.ImgIcons.TransparentColor = System.Drawing.Color.Fuchsia;
			this.ImgIcons.Images.SetKeyName(0, "breakpoint.bmp");
			// 
			// ScriptViewer
			// 
			this.Columns.AddRange(new System.Windows.Forms.ColumnHeader[] {
            this.columnHeader1,
            this.columnHeader2});
			this.Font = new System.Drawing.Font("Courier New", 9F, System.Drawing.FontStyle.Regular, System.Drawing.GraphicsUnit.Point, ((byte)(238)));
			this.FullRowSelect = true;
			this.HeaderStyle = System.Windows.Forms.ColumnHeaderStyle.None;
			this.MultiSelect = false;
			this.SmallImageList = this.ImgIcons;
			this.View = System.Windows.Forms.View.Details;
			this.ResumeLayout(false);

		}

		#endregion

		private System.Windows.Forms.ColumnHeader columnHeader1;
		private System.Windows.Forms.ColumnHeader columnHeader2;
		private System.Windows.Forms.ImageList ImgIcons;
	}
}
