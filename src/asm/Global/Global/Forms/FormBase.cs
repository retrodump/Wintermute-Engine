// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;
using System.ComponentModel;

namespace DeadCode.WME.Global
{
	public class FormBase : System.Windows.Forms.Form
	{
		private Rectangle FormStateRect = Rectangle.Empty;
		public ApplicationMgr AppMgr;

		//////////////////////////////////////////////////////////////////////////		
		public FormBase()
		{
			Resize += new EventHandler(OnResize);
			Move += new EventHandler(OnMove);
		}

		//////////////////////////////////////////////////////////////////////////
		protected void LoadLayout(SettingsNode RootNode)
		{
			SettingsNode LayoutNode = RootNode.GetNode("Layout\\" + this.Name);
			if (LayoutNode != null)
			{
				// load position
				Point FormLoc = new Point(LayoutNode.GetInt("FormPosX", this.Location.X), LayoutNode.GetInt("FormPosY", this.Location.Y));

				// load size
				if (this.FormBorderStyle == FormBorderStyle.Sizable || this.FormBorderStyle == FormBorderStyle.SizableToolWindow)
				{
					this.Size = new Size(LayoutNode.GetInt("FormWidth", this.Size.Width), LayoutNode.GetInt("FormHeight", this.Size.Height));
				}

				// make sure the position/size is valid on current monitor setup
				bool FoundScreen = false;
				Point LowerRight = new Point(FormLoc.X + this.Size.Width, FormLoc.Y + this.Size.Height);
				foreach (Screen Scr in Screen.AllScreens)
				{
					if (Scr.Bounds.Contains(FormLoc) || Scr.Bounds.Contains(LowerRight))
						FoundScreen = true;
				}
				if (FoundScreen) this.Location = FormLoc;


				// remember window position
				FormStateRect.Location = this.Location;
				FormStateRect.Size = this.Size;

				this.WindowState = (FormWindowState)LayoutNode.GetInt("FormState", (int)FormWindowState.Normal);
				LoadControls(this, LayoutNode);
			}
			else
			{
				FormStateRect.Location = this.Location;
				FormStateRect.Size = this.Size;
			}			
		}

		//////////////////////////////////////////////////////////////////////////
		protected void LoadControls(Control ParentControl, SettingsNode RootNode)
		{
			foreach (Control C in ParentControl.Controls)
			{
				if (C is ILayoutAwareControl)
				{
					((ILayoutAwareControl)C).LoadControlLayout(RootNode);
				}
				else
				{
					if (C is SplitContainer)
					{
						SplitContainer Ctrl = C as SplitContainer;
						if (!Ctrl.IsSplitterFixed)
						{
							SettingsNode Node = RootNode.GetNode(Ctrl.Name);
							if (Node != null)
							{
								try
								{
									Ctrl.SplitterDistance = Node.GetInt("SplitterDistance", Ctrl.SplitterDistance);
								}
								catch
								{
								}
							}
						}
					}

					if (C is ListView)
					{
						ListView Ctrl = C as ListView;
						SettingsNode Node = RootNode.GetNode(Ctrl.Name, false, true);
						if (Node != null)
						{
							for (int i = 0; i < Ctrl.Columns.Count; i++)
							{
								ColumnHeader Col = Ctrl.Columns[i];
								Col.Width = Node.GetInt("Col" + i.ToString() + "Width", Col.Width);
							}
						}
					}

					if(C is TabControl)
					{
						TabControl Ctrl = C as TabControl;
						SettingsNode Node = RootNode.GetNode(Ctrl.Name, false, true);
						if (Node != null)
						{
							int SelIndex = Node.GetInt("SelectedIndex", Ctrl.SelectedIndex);
							if (SelIndex >= 0 && SelIndex < Ctrl.TabCount) Ctrl.SelectedIndex = SelIndex;
						}
					}
				}

				if (C.Controls.Count > 0) LoadControls(C, RootNode);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		protected void SaveLayout(SettingsNode RootNode)
		{
			SettingsNode LayoutNode = RootNode.GetNode("Layout\\" + this.Name, false, true);
			if (LayoutNode != null)
			{
				LayoutNode.SetValue("FormState", (int)this.WindowState);
				LayoutNode.SetValue("FormPosX", FormStateRect.Location.X);
				LayoutNode.SetValue("FormPosY", FormStateRect.Location.Y);
				LayoutNode.SetValue("FormWidth", FormStateRect.Size.Width);
				LayoutNode.SetValue("FormHeight", FormStateRect.Size.Height);

				SaveControls(this, LayoutNode);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected void SaveControls(Control ParentControl, SettingsNode RootNode)
		{
			foreach (Control C in ParentControl.Controls)
			{
				if (C is ILayoutAwareControl)
				{
					((ILayoutAwareControl)C).SaveControlLayout(RootNode);
				}
				else
				{
					if (C is SplitContainer)
					{
						SplitContainer Ctrl = C as SplitContainer;
						SettingsNode Node = RootNode.GetNode(Ctrl.Name, false, true);
						if (Node != null)
						{
							Node.SetValue("SplitterDistance", Ctrl.SplitterDistance);
						}
					}

					if (C is ListView)
					{
						ListView Ctrl = C as ListView;
						SettingsNode Node = RootNode.GetNode(Ctrl.Name, false, true);
						if (Node != null)
						{
							for (int i = 0; i < Ctrl.Columns.Count; i++)
							{
								Node.SetValue("Col" + i.ToString() + "Width", Ctrl.Columns[i].Width);
							}
						}
					}

					if (C is TabControl)
					{
						TabControl Ctrl = C as TabControl;
						SettingsNode Node = RootNode.GetNode(Ctrl.Name, false, true);
						if (Node != null)
						{
							Node.SetValue("SelectedIndex", Ctrl.SelectedIndex);
						}
					}

				}
				if (C.Controls.Count > 0) SaveControls(C, RootNode);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnMove(object sender, System.EventArgs e) 
		{
			// Save position.
			if (WindowState == FormWindowState.Normal)				
				FormStateRect.Location = Location;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnResize(object sender, System.EventArgs e) 
		{
			// Save width and height.
			if (WindowState == FormWindowState.Normal)
				FormStateRect.Size = Size;
		}

	}
}
