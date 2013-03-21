using System;
using System.Drawing;
using System.Runtime.InteropServices;
using System.Windows.Forms;

// Based on vbAccelerator article:
// http://www.vbaccelerator.com/home/NET/Code/Controls/Popup_Windows/Popup_Windows/article.asp

namespace DeadCode.WME.Controls
{
	#region Event Argument Classes
	/// <summary>
	/// Contains event information for a <see cref="PopupClosed"/> event.
	/// </summary>
	public class PopupClosedEventArgs : EventArgs
	{
		/// <summary>
		/// The popup form.
		/// </summary>
		private Form popup = null;

		/// <summary>
		/// Gets the popup form which is being closed.
		/// </summary>
		public Form Popup
		{
			get
			{
				return this.popup;
			}
		}

		/// <summary>
		/// Constructs a new instance of this class for the specified
		/// popup form.
		/// </summary>
		/// <param name="popup">Popup Form which is being closed.</param>
		public PopupClosedEventArgs(Form popup)
		{
			this.popup = popup;
		}
	}

	/// <summary>
	/// Arguments to a <see cref="PopupCancelEvent"/>.  Provides a
	/// reference to the popup form that is to be closed and 
	/// allows the operation to be cancelled.
	/// </summary>
	public class PopupCancelEventArgs : EventArgs
	{
		/// <summary>
		/// Whether to cancel the operation
		/// </summary>
		private bool cancel = false;
		/// <summary>
		/// Mouse down location
		/// </summary>
		private Point location;
		/// <summary>
		/// Popup form.
		/// </summary>
		private Form popup = null;

		/// <summary>
		/// Constructs a new instance of this class.
		/// </summary>
		/// <param name="popup">The popup form</param>
		/// <param name="location">The mouse location, if any, where the
		/// mouse event that would cancel the popup occured.</param>
		public PopupCancelEventArgs(Form popup, Point location)
		{
			this.popup = popup;
			this.location = location;
			this.cancel = false;
		}

		/// <summary>
		/// Gets the popup form
		/// </summary>
		public Form Popup
		{
			get
			{
				return this.popup;
			}
		}

		/// <summary>
		/// Gets the location that the mouse down which would cancel this 
		/// popup occurred
		/// </summary>
		public Point CursorLocation
		{
			get
			{
				return this.location;
			}
		}

		/// <summary>
		/// Gets/sets whether to cancel closing the form. Set to
		/// <c>true</c> to prevent the popup from being closed.
		/// </summary>
		public bool Cancel
		{
			get
			{
				return this.cancel;
			}
			set
			{
				this.cancel = value;
			}
		}

	}
	#endregion

	#region Delegates
	/// <summary>
	/// Represents the method which responds to a <see cref="PopupClosed"/> event.
	/// </summary>
	public delegate void PopupClosedEventHandler(object sender, PopupClosedEventArgs e);

	/// <summary>
	/// Represents the method which responds to a <see cref="PopupCancel"/> event.
	/// </summary>
	public delegate void PopupCancelEventHandler(object sender, PopupCancelEventArgs e);
	#endregion

	#region PopupWindowHelper
	/// <summary>
	/// A class to assist in creating popup windows like Combo Box drop-downs and Menus.
	/// This class includes functionality to keep the title bar of the popup owner form
	/// active whilst the popup is displayed, and to automatically cancel the popup
	/// whenever the user clicks outside the popup window or shifts focus to another 
	/// application.
	/// </summary>
	public class PopupWindowHelper : NativeWindow
	{
		#region Member Variables
		/// <summary>
		/// Event Handler to detect when the popup window is closed
		/// </summary>
		private EventHandler popClosedHandler = null;
		/// <summary>
		/// Message filter to detect mouse clicks anywhere in the application
		/// whilst the popup window is being displayed.
		/// </summary>
		private PopupWindowHelperMessageFilter filter = null;
		/// <summary>
		/// The popup form that is being shown.
		/// </summary>
		private Form popup = null;
		/// <summary>
		/// The owner of the popup form that is being shown:
		/// </summary>
		private Form owner = null;
		/// <summary>
		/// Whether the popup is showing or not.
		/// </summary>
		private bool popupShowing = false;
		/// <summary>
		/// Whether the popup has been cancelled, notified by PopupCancel,
		/// rather than closed.
		/// </summary>
		private bool skipClose = false;
		#endregion

		/// <summary>
		/// Raised when the popup form is closed.
		/// </summary>
		public event PopupClosedEventHandler PopupClosed;
		/// <summary>
		/// Raised when the Popup Window is about to be cancelled.  The
		/// <see cref="PopupCancelEventArgs.Cancel"/> property can be
		/// set to <c>true</c> to prevent the form from being cancelled.
		/// </summary>
		public event PopupCancelEventHandler PopupCancel;

		/// <summary>
		/// Shows the specified Form as a popup window, keeping the
		/// Owner's title bar active and preparing to cancel the popup
		/// should the user click anywhere outside the popup window.
		/// <para>Typical code to use this message is as follows:</para>
		/// <code>
		///    frmPopup popup = new frmPopup();
		///    Point location = this.PointToScreen(new Point(button1.Left, button1.Bottom));
		///    popupHelper.ShowPopup(this, popup, location);
		/// </code>
		/// <para>Put as much initialisation code as possible
		/// into the popup form's constructor, rather than the <see cref="System.Windows.Forms.Load"/>
		/// event as this will improve visual appearance.</para>
		/// </summary>
		/// <param name="owner">Main form which owns the popup</param>
		/// <param name="popup">Window to show as a popup</param>
		/// <param name="location">Location relative to the screen to show the popup at.</param>
		public void ShowPopup(Form owner, Form popup, Point location)
		{
			ShowPopup(owner, popup, location, true);
		}
		public void ShowPopup(Form owner, Form popup, Point location, bool TabOrderFix)
		{

			this.owner = owner;
			this.popup = popup;

			// Start checking for the popup being cancelled
			Application.AddMessageFilter(filter);

			// Set the location of the popup form:
			popup.StartPosition = FormStartPosition.Manual;
			popup.Location = location; 
			// Make it owned by the window that's displaying it:
			owner.AddOwnedForm(popup);			
			// Respond to the Closed event in case the popup
			// is closed by its own internal means
			popClosedHandler = new EventHandler(popup_Closed);
			popup.Closed += popClosedHandler;

			// Show the popup:
			this.popupShowing = true;
			popup.Show();
			popup.Activate();
			
			// A little bit of fun.  We've shown the popup,
			// but because we've kept the main window's
			// title bar in focus the tab sequence isn't quite
			// right.  This can be fixed by sending a tab,
			// but that on its own would shift focus to the
			// second control in the form.  So send a tab,
			// followed by a reverse-tab.

			if (TabOrderFix)
			{
				// Send a Tab command:
				NativeMethods.keybd_event((byte)Keys.Tab, 0, 0, 0);
				NativeMethods.keybd_event((byte)Keys.Tab, 0, (int)NativeMethods.KeyboardEvent.KeyUp, 0);

				// Send a reverse Tab command:
				NativeMethods.keybd_event((byte)Keys.ShiftKey, 0, 0, 0);
				NativeMethods.keybd_event((byte)Keys.Tab, 0, 0, 0);
				NativeMethods.keybd_event((byte)Keys.Tab, 0, (int)NativeMethods.KeyboardEvent.KeyUp, 0);
				NativeMethods.keybd_event((byte)Keys.ShiftKey, 0, (int)NativeMethods.KeyboardEvent.KeyUp, 0);
			}


			// Start filtering for mouse clicks outside the popup
			filter.Popup = popup;
					
		}

		/// <summary>
		/// Responds to the <see cref="System.Windows.Forms.Form.Closed"/>
		/// event from the popup form.
		/// </summary>
		/// <param name="sender">Popup form that has been closed.</param>
		/// <param name="e">Not used.</param>
		private void popup_Closed(object sender, EventArgs e)
		{
			ClosePopup();
		}

		/// <summary>
		/// Subclasses the owning form's existing Window Procedure to enables the 
		/// title bar to remain active when a popup is show, and to detect if
		/// the user clicks onto another application whilst the popup is visible.
		/// </summary>
		/// <param name="m">Window Procedure Message</param>
		protected override void WndProc(ref Message m)
		{
			base.WndProc(ref m);
			if (this.popupShowing)
			{
				// check for WM_ACTIVATE and WM_NCACTIVATE
				if (m.Msg == (int)NativeMethods.WindowMessage.NonClientActivate)
				{
					// Check if the title bar will made inactive:
					if (((int) m.WParam) == 0)
					{
						// If so reactivate it.
						NativeMethods.SendMessage(this.Handle, (int)NativeMethods.WindowMessage.NonClientActivate, 1, IntPtr.Zero);
						
						// Note it's no good to try and consume this message;
						// if you try to do that you'll end up with windows
						// that don't respond.
					}

				}
				else if (m.Msg == (int)NativeMethods.WindowMessage.ActivateApplication)
				{
					// Check if the application is being deactivated.
					if ((int)m.WParam == 0)
					{
						// It is so cancel the popup:
						ClosePopup();
						// And put the title bar into the inactive state:
						NativeMethods.PostMessage(this.Handle, (int)NativeMethods.WindowMessage.ActivateApplication, 0, IntPtr.Zero);
					}
				}
			}
		}

		public void CancelPopup()
		{
			skipClose = true;
			ClosePopup();
		}

		/// <summary>
		/// Called when the popup is being hidden.
		/// </summary>
		public void ClosePopup()
		{
			if (this.popupShowing)
			{
				if (!skipClose)
				{
					// Raise event to owner
					OnPopupClosed(new PopupClosedEventArgs(this.popup));					
				}
				skipClose = false;

				// Make sure the popup is closed and we've cleaned
				// up:
				this.owner.RemoveOwnedForm(this.popup);
				this.popupShowing = false;
				this.popup.Closed -= popClosedHandler;
				this.popClosedHandler = null;
				this.popup.Close();
				// No longer need to filter for clicks outside the
				// popup.
				Application.RemoveMessageFilter(filter);

				// If we did something from the popup which shifted
				// focus to a new form, like showing another popup
				// or dialog, then Windows won't know how to bring
				// the original owner back to the foreground, so
				// force it here:
				this.owner.Activate();
				
				// Null out references for GC
				this.popup = null;
				this.owner = null;
								
			}
		}

		/// <summary>
		/// Raises the <see cref="PopupClosed"/> event.
		/// </summary>
		/// <param name="e"><see cref="PopupClosedEventArgs"/> describing the
		/// popup form that is being closed.</param>
		protected virtual void OnPopupClosed(PopupClosedEventArgs e)
		{
			if (this.PopupClosed != null)
			{
				this.PopupClosed(this, e);
			}
		}

		private void popup_Cancel(object sender, PopupCancelEventArgs e)
		{
			OnPopupCancel(e);
		}

		/// <summary>
		/// Raises the <see cref="PopupCancel"/> event.
		/// </summary>
		/// <param name="e"><see cref="PopupCancelEventArgs"/> describing the
		/// popup form that about to be cancelled.</param>
		protected virtual void OnPopupCancel(PopupCancelEventArgs e)
		{
			if (this.PopupCancel != null)
			{
				this.PopupCancel(this, e);
				if (!e.Cancel)
				{
					skipClose = true;
				}
			}
		}

		/// <summary>
		/// Default constructor.
		/// </summary>
		/// <remarks>Use the <see cref="System.Windows.Forms.NativeWindow.AssignHandle"/>
		/// method to attach this class to the form you want to show popups from.</remarks>
		public PopupWindowHelper()
		{
			filter = new PopupWindowHelperMessageFilter(this);
			filter.PopupCancel += new PopupCancelEventHandler(popup_Cancel);
		}


	}
	#endregion

	#region PopupWindowHelperMessageFilter
	/// <summary>
	/// A Message Loop filter which detect mouse events whilst the popup form is shown
	/// and notifies the owning <see cref="PopupWindowHelper"/> class when a mouse
	/// click outside the popup occurs.
	/// </summary>
	public class PopupWindowHelperMessageFilter : IMessageFilter
	{
		/// <summary>
		/// Raised when the Popup Window is about to be cancelled.  The
		/// <see cref="PopupCancelEventArgs.Cancel"/> property can be
		/// set to <c>true</c> to prevent the form from being cancelled.
		/// </summary>
		public event PopupCancelEventHandler PopupCancel;
		
		/// <summary>
		/// The popup form
		/// </summary>
		private Form popup = null;
		/// <summary>
		/// The owning <see cref="PopupWindowHelper"/> object.
		/// </summary>
		private PopupWindowHelper owner = null;

		/// <summary>
		/// Constructs a new instance of this class and sets the owning
		/// object.
		/// </summary>
		/// <param name="owner">The <see cref="PopupWindowHelper"/> object
		/// which owns this class.</param>
		public PopupWindowHelperMessageFilter(PopupWindowHelper owner)
		{
			this.owner = owner;
		}

		/// <summary>
		/// Gets/sets the popup form which is being displayed.
		/// </summary>
		public Form Popup
		{
			get
			{
				return this.popup;
			}
			set
			{
				this.popup = value;
			}
		}

		/// <summary>
		/// Checks the message loop for mouse messages whilst the popup
		/// window is displayed.  If one is detected the position is
		/// checked to see if it is outside the form, and the owner
		/// is notified if so.
		/// </summary>
		/// <param name="m">Windows Message about to be processed by the
		/// message loop</param>
		/// <returns><c>true</c> to filter the message, <c>false</c> otherwise.
		/// This implementation always returns <c>false</c>.</returns>
		public bool PreFilterMessage(ref Message m)
		{
			if (this.popup != null)
			{
				switch (m.Msg)
				{				
					case (int)NativeMethods.WindowMessage.LeftButtonDown:
					case (int)NativeMethods.WindowMessage.RightButtonDown:
					case (int)NativeMethods.WindowMessage.MiddleButtonDown:
					case (int)NativeMethods.WindowMessage.NcLeftButtonDown:
					case (int)NativeMethods.WindowMessage.NcRightButtonDown:
					case (int)NativeMethods.WindowMessage.NcMiddleButtonDown:
						OnMouseDown();
						break;
				}
			}
			return false;
		}

		/// <summary>
		/// Checks the mouse location and calls the OnCancelPopup method
		/// if the mouse is outside the popup form.		
		/// </summary>
		private void OnMouseDown()
		{
			// Get the cursor location
			Point cursorPos = Cursor.Position;
			// Check if it is within the popup form
			if (!popup.Bounds.Contains(cursorPos))
			{
				// If not, then call to see if it should be closed
				OnCancelPopup(new PopupCancelEventArgs(popup, cursorPos));
			}
		}

		/// <summary>
		/// Raises the <see cref="PopupCancel"/> event.
		/// </summary>
		/// <param name="e">The <see cref="PopupCancelEventArgs"/> associated 
		/// with the cancel event.</param>
		protected virtual void OnCancelPopup(PopupCancelEventArgs e)
		{
			if (this.PopupCancel != null)
			{
				this.PopupCancel(this, e);
			}
			if (!e.Cancel)
			{
				owner.ClosePopup();
				// Clear reference for GC
				popup = null;
			}
		}


	}
	#endregion

}
