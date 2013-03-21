using System.Drawing;
namespace DeadCode.WME.Controls
{
	partial class ColorButton
	{
		/// <summary> 
		/// Clean up any resources being used.
		/// </summary>
		/// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
		protected override void Dispose(bool disposing)
		{
			if (disposing)
			{
				//Make sure our brushes are cleaned up            
				if (buttonColorBrush != null)
					buttonColorBrush.Dispose();
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
			this.Name = "ColorButton";
			this.Size = new Size(75, 22);
		}

		#endregion
	}
}
