using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Reflection;
using System.Windows.Forms;

namespace DeadCode.WME.Controls.Actions
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

		[Flags]
		public enum State
		{
			Sensitive = 0,
			Disabled  = 1,
			Hidden    = 2,
			Checked   = 4
		}


		//////////////////////////////////////////////////////////////////////////
		public bool Invoke(IActionProvider Provider, object Sender)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.Invoke;
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
		public State GetState(IActionProvider Provider)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.GetState;
				Param.Processed = false;

				Method.Invoke(Provider, new object[] { Param });
				if (Param.Processed) return Param.State;
				else return State.Sensitive;
			}
			catch
			{
				return State.Sensitive;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionData SaveData(IActionProvider Provider)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.SaveData;
				Param.Processed = false;
				Param.Data = null;

				Method.Invoke(Provider, new object[] { Param });
				if (Param.Processed) return Param.Data;
				else return null;
			}
			catch
			{
				return null;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool LoadData(IActionProvider Provider, ActionData Data)
		{
			try
			{
				ActionParam Param = new ActionParam();
				Param.Type = ActionParam.QueryType.SaveData;
				Param.Processed = false;
				Param.Data = Data;

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
