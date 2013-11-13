// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Reflection;
using System.Windows.Forms;

namespace DeadCode.WME.Global.Actions
{
	public interface IActionProvider
	{

	}

	public class Action
	{
		public Type ContextType;
		public MethodInfo Method;
		public string Name;
		public readonly ActionPropAttribute Attr;

		public Image Icon;


		//////////////////////////////////////////////////////////////////////////
		public Action(Type ContextType, MethodInfo Method, string Name, ActionPropAttribute Attr)
		{
			if (ContextType == null || Method == null) throw new NullReferenceException();

			this.ContextType = ContextType;
			this.Method = Method;
			this.Name = Name;
			this.Attr = Attr;

			if (this.Attr.Caption == "") this.Attr.Caption = this.Name;
			if (this.Attr.Tooltip == "") this.Attr.Tooltip = this.Attr.Caption;

			if(Attr.IconName!="")
			{
				try
				{
					Icon = new Bitmap(ContextType, Attr.IconName);
				}
				catch
				{
					Icon = null;
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public bool Invoke(IActionProvider Provider, object Sender)
		{
#if !DEBUG
			try
			{
#endif
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.Invoke;
				Param.Sender = Sender;
				Param.Processed = false;

				Method.Invoke(Provider, new object[] { Param });
				return Param.Processed;
#if !DEBUG
			}
			catch(Exception e)
			{				
				MessageBox.Show("Error invoking command:\n\n" + e.InnerException.Message, "", MessageBoxButtons.OK, MessageBoxIcon.Error);
				return false;
			}
#endif
		}


		//////////////////////////////////////////////////////////////////////////
		public ActionState GetState(IActionProvider Provider, object Sender)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.GetState;
				Param.Sender = Sender;
				Param.State = new ActionState(true, true);

				Method.Invoke(Provider, new object[] { Param });
				return Param.State;
			}
			catch
			{
				return new ActionState(false);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public bool Initialize(IActionProvider Provider, object Sender)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.Initialize;
				Param.Sender = Sender;
				Param.Processed = false;

				Method.Invoke(Provider, new object[] { Param });
				return Param.Processed;
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool CustomBuild(IActionProvider Provider, ToolStripItemCollection Items, bool AsMenu)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.CustomBuild;
				Param.Items = Items;
				Param.BuildAsMenu = AsMenu;
				Param.Processed = false;

				Method.Invoke(Provider, new object[] { Param });
				return Param.Processed;
			}
			catch
			{
				return false;
			}
		}
	}
}
