using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using Aga.Controls.Tree;
using DeadCode.WME.Global;

namespace DeadCode.WME.Controls
{
	public partial class TreeListView : Aga.Controls.Tree.TreeViewAdv, ILayoutAwareControl
	{
		//////////////////////////////////////////////////////////////////////////
		public TreeListView()
		{
			this.Resize += new System.EventHandler(this.OnResize);
			this.ColumnWidthChanged += new System.EventHandler<TreeColumnEventArgs>(this.OnColumnResized);
		}

		private bool _fitLastColumn;
		[Category("Behavior")]
		//////////////////////////////////////////////////////////////////////////
		public bool FitLastColumn
		{
			get
			{
				return _fitLastColumn;
			}
			set
			{
				_fitLastColumn = value;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnColumnResized(object sender, TreeColumnEventArgs e)
		{
			FixLastColumnWidth();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnResize(object sender, EventArgs e)
		{
			FixLastColumnWidth();
		}

		//////////////////////////////////////////////////////////////////////////
		private void FixLastColumnWidth()
		{
			if (!FitLastColumn) return;

			if (!this.DesignMode) // necessary otherwise VS crashes on build :-/
			{
				int ColWidth = 0;
				for (int i = 0; i < this.Columns.Count - 1; i++)
				{
					ColWidth += this.Columns[i].Width;
				}
				ColWidth += SystemInformation.VerticalScrollBarWidth + 2 * SystemInformation.Border3DSize.Width;
				ColWidth = this.Width - ColWidth;
				if (ColWidth < 10) ColWidth = 10;

				this.Columns[this.Columns.Count - 1].Width = ColWidth;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public virtual void SaveControlLayout(SettingsNode RootNode)
		{
			SettingsNode Node = RootNode.GetNode(this.Name, false, true);
			if (Node != null)
			{
				for (int i = 0; i < Columns.Count; i++)
				{
					Node.SetValue("Col" + i.ToString() + "Width", Columns[i].Width);
				}
			}
		}
		
		//////////////////////////////////////////////////////////////////////////
		public virtual void LoadControlLayout(SettingsNode RootNode)
		{
			SettingsNode Node = RootNode.GetNode(this.Name, false, true);
			if (Node != null)
			{
				for (int i = 0; i < Columns.Count; i++)
				{
					TreeColumn Col = Columns[i];
					Col.Width = Node.GetInt("Col" + i.ToString() + "Width", Col.Width);
				}
			}
		}

	}
}
