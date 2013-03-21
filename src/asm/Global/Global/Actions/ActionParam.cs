// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Global.Actions
{
	public class ActionParam
	{
		public enum QueryType
		{
			Invoke,
			GetState,
			Initialize,
			CustomBuild
		}

		public QueryType Type;
		public ActionState State;
		public bool Processed;
		public object Sender;

		public ToolStripItemCollection Items;
		public bool BuildAsMenu;
	}

	public class ActionState
	{
		public ActionState(bool Enabled, bool Visible, bool Checked)
		{
			this.Enabled = Enabled;
			this.Visible = Visible;
			this.Checked = Checked;
		}

		public ActionState(bool Enabled, bool Visible)
		{
			this.Enabled = Enabled;
			this.Visible = Visible;
			this.Checked = false;
		}

		public ActionState(bool Enabled)
		{
			this.Enabled = Enabled;
			this.Visible = true;
			this.Checked = false;
		}

		public bool Enabled;
		public bool Visible;
		public bool Checked;
		public string Caption = null;
		public string Tooltip = null;
	};
}
