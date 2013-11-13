// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Core;
using System.Runtime.InteropServices;
using DeadCode.WME.Global;

namespace DeadCode.WME.Controls
{
	public partial class WmeCanvas : UserControl, IDisposable
	{
		//////////////////////////////////////////////////////////////////////////
		public WmeCanvas()
		{
			InitializeComponent();
			this.BackColor = Color.Black;
		}

		private WGame game;
		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public WGame Game
		{
			get
			{
				return game;
			}
		}

		private bool suspendedRendering = false;
		//////////////////////////////////////////////////////////////////////////
		public bool SuspendedRendering
		{
			get
			{
				return suspendedRendering;
			}
			set
			{
				suspendedRendering = value;
			}
		}

		private bool suspendedUpdate = false;
		//////////////////////////////////////////////////////////////////////////
		public bool SuspendedUpdate
		{
			get
			{
				return suspendedUpdate;
			}
			set
			{
				suspendedUpdate = value;
			}
		}

		private bool routeMouseClicks = false;
		public bool RouteMouseClicks
		{
			get
			{
				return routeMouseClicks;
			}
			set
			{
				routeMouseClicks = value;
				if(routeMouseClicks)
				{
					this.MouseDown += new MouseEventHandler(OnMouseDown);
					this.MouseUp += new MouseEventHandler(OnMouseUp);
				}
				else
				{
					this.MouseDown -= new MouseEventHandler(OnMouseDown);
					this.MouseUp -= new MouseEventHandler(OnMouseUp);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Create(WGame Game, string ProjectFile)
		{
			return Create(Game, ProjectFile, null);
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Create(WGame Game, string ProjectFile, string LogFile)
		{
			game = Game;
			bool Ret = this.Game.InitializeWindowed(this, true, ProjectFile, LogFile);
			if(Ret)
			{
				Application.Idle += new EventHandler(OnAppIdle);
				this.Resize += new EventHandler(OnResize);

				Form ParentForm = FindForm();
				if (ParentForm != null)
				{
					ParentForm.Activated += new EventHandler(OnFormActivate);
					ParentForm.Deactivate += new EventHandler(OnFormDeactivate);
				}
			}

			return Ret;
		}

		//////////////////////////////////////////////////////////////////////////
		public void Release()
		{
			Application.Idle -= new EventHandler(OnAppIdle);
			game = null;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAppIdle(object sender, EventArgs e)
		{
			if (IsFormActive && !SuspendedRendering)
			{
				while (AppStillIdle) PaintFrame(null);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnResize(object sender, EventArgs e)
		{
			if (Game != null && Game.Renderer != null)
				Game.Renderer.Resize(this.ClientSize.Width, this.ClientSize.Height);
		}

		bool IsFormActive = true;
		//////////////////////////////////////////////////////////////////////////
		private void OnFormActivate(object sender, EventArgs e)
		{
			IsFormActive = true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormDeactivate(object sender, EventArgs e)
		{
			IsFormActive = false;
		}

		//////////////////////////////////////////////////////////////////////////
		protected override void OnPaint(PaintEventArgs e)
		{
			if (!PaintFrame(e)) base.OnPaint(e);
		}

		//////////////////////////////////////////////////////////////////////////
		protected bool PaintFrame(PaintEventArgs e)
		{
			try
			{
				if (Game != null)
				{
					Game.FrameInit(!SuspendedUpdate, BackColor);

					if (PaintContent != null) PaintContent(this, new EventArgs());

					Game.FrameFinish();
					return true;
				}
				else return false;
			}
			catch(Exception Ex)
			{
				if (e != null)
				{
					RectangleF Rect = new RectangleF(0, 0, Width, Height);
					Font Fnt = new Font("Arial", 10);
					SolidBrush Brush = new SolidBrush(Color.White);

					e.Graphics.FillRectangle(Brushes.Black, 0, 0, Width, Height);
					e.Graphics.DrawString("Internal error occured while painting. You can try using the Undo command to fix this problem.\n\n" + Ex.Message, Fnt, Brush, Rect);

					#if DEBUG
						throw Ex;
					#else
						return true;
					#endif
				}
				else
				{
					Invalidate();
					return false;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected override void OnPaintBackground (System.Windows.Forms.PaintEventArgs e)
		{
			if (Game == null) base.OnPaintBackground(e);
		}

		//////////////////////////////////////////////////////////////////////////
		private bool AppStillIdle
		{
			get
			{
				NativeMethods.PeekMsg msg;
				return !NativeMethods.PeekMessage(out msg, IntPtr.Zero, 0, 0, 0);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public event PaintContentDelegate PaintContent;

		public delegate void PaintContentDelegate(object sender, EventArgs e);


		//////////////////////////////////////////////////////////////////////////
		private void OnMouseDown(object sender, MouseEventArgs e)
		{
			if (Game == null) return;
			switch(e.Button)
			{
				case MouseButtons.Left:
					Game.OnMouseLeftDown();
					break;

				case MouseButtons.Right:
					Game.OnMouseRightDown();
					break;

				case MouseButtons.Middle:
					Game.OnMouseMiddleDown();
					break;
			}				
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnMouseUp(object sender, MouseEventArgs e)
		{
			if (Game == null) return;
			switch (e.Button)
			{
				case MouseButtons.Left:
					Game.OnMouseLeftUp();
					break;

				case MouseButtons.Right:
					Game.OnMouseRightUp();
					break;

				case MouseButtons.Middle:
					Game.OnMouseMiddleUp();
					break;
			}				
		}

		private bool _WantArrowKeys;
		//////////////////////////////////////////////////////////////////////////
		public bool WantArrowKeys
		{
			get
			{
				return _WantArrowKeys;
			}
			set
			{
				_WantArrowKeys = value;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected override bool IsInputKey(Keys keyData)
		{
			if(WantArrowKeys)
			{
				switch (keyData & Keys.KeyCode)
				{
					case Keys.Left:
					case Keys.Up:
					case Keys.Down:
					case Keys.Right:
						return true;
				}
			}
			return base.IsInputKey(keyData);
		}


		#region IDisposable Members

		void IDisposable.Dispose()
		{
			Release();
		}

		#endregion
	}
}
