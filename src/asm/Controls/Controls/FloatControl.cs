using System;

using System.Drawing;
using System.Runtime.InteropServices;
using System.Security.Permissions;
using System.Windows.Forms;

// Based on vbAccelerator article:
// http://www.vbaccelerator.com/home/NET/Code/Controls/Popup_Windows/Floating_Controls/article.asp


// Requires unmanaged code
[assembly:SecurityPermissionAttribute(SecurityAction.RequestMinimum, UnmanagedCode=true)]
// Requires ability to create any window type
[assembly:UIPermissionAttribute(SecurityAction.RequestMinimum, Window=UIPermissionWindow.AllWindows)]
namespace DeadCode.WME.Controls
{
	/// <summary>
	/// An version of a <see cref="System.Windows.Forms.Control"/> 
	/// which can be shown floating, like a tooltip.  If you
	/// want to use this control in conjunction with mouse events
	/// then you must ensure that the mouse is never in any part of the 
	/// control when it is shown (like a tooltip). Otherwise, 
	/// <see cref="System.Windows.Forms.MouseEnter"/> and 
	/// <see cref="System.Windows.Forms.MouseLeave"/> events
	/// are broken, and the Forms Message Filter
	/// goes into a continuous loop when attempting to show
	/// the control.
	/// </summary>
	[System.ComponentModel.DesignTimeVisibleAttribute(false)]
	public class FloatControl : Control
	{				
		private const int HTTRANSPARENT = (-1);

		public bool ReceiveClicks = false;

		/// <summary>
		/// Shows the control as a floating Window child 
		/// of the desktop.  To hide the control again,
		/// use the <see cref="Visible"/> property.
		/// </summary>
		public void ShowFloating()
		{
			if (this.Handle == IntPtr.Zero)
			{
				base.CreateControl();
			}
			NativeMethods.SetParent(base.Handle, IntPtr.Zero);
			NativeMethods.ShowWindow(base.Handle, 1);
		}

		/// <summary>
		/// Get the <see cref="System.Windows.Forms.CreateParams"/>
		/// used to create the control.  This override adds the
		/// <code>WS_EX_NOACTIVATE</code>, <code>WS_EX_TOOLWINDOW</code>
		/// and <code>WS_EX_TOPMOST</code> extended styles to make
		/// the Window float on top.
		/// </summary>
		protected override CreateParams CreateParams
		{
			get
			{
				CreateParams p = base.CreateParams;
				p.ExStyle |= ((int)(NativeMethods.WindowStyleEx.NoActivate | NativeMethods.WindowStyleEx.ToolWindow | NativeMethods.WindowStyleEx.TopMost));
				p.Parent = IntPtr.Zero;
				return p;
			}
		}

		
		/// <summary>
		/// Overrides the standard Window Procedure to ensure the
		/// window is transparent to all mouse events.
		/// </summary>
		/// <param name="m">Windows message to process.</param>
		protected override void WndProc(ref Message m)
		{
			if (!ReceiveClicks && m.Msg == (int)NativeMethods.WindowMessage.NonClientHitTest)
			{
				m.Result = (IntPtr) HTTRANSPARENT;
			}
			else
			{
				base.WndProc(ref m);
			}
		}


		/// <summary>
		/// Overrides the standard painting procedure to render
		/// the text associated with the control.
		/// </summary>
		/// <param name="e">PaintEvent Arguments</param>
		protected override void OnPaint(PaintEventArgs e)
		{
			if (base.Text.Length > 0)
			{
				StringFormat sf = new StringFormat(StringFormat.GenericTypographic);
				Brush br = new SolidBrush(this.ForeColor);
				e.Graphics.DrawString(base.Text, this.Font, br, new PointF(2F, 2F), sf);
				br.Dispose();
			}
			Pen P = new Pen(this.ForeColor);
			e.Graphics.DrawRectangle(P /*SystemPens.ControlDarkDark*/, 
				0, 0, this.ClientRectangle.Width - 1, this.ClientRectangle.Height - 1);
			P.Dispose();
		}

		/// <summary>
		/// Constructs a new instance of this control.
		/// </summary>
		public FloatControl()
		{
			TabStop = false;
			Visible = false;
			BackColor = SystemColors.Info;
			ForeColor = SystemColors.InfoText;
		}
	}
	
}
