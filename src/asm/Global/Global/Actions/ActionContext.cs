// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Global.Actions
{
	public enum ActiveObjectSlot
	{
		Application,
		Form,
		Document,
		Panel,
		Control,
	}

	public class ActionContext
	{		
		private Form OwnerForm = null;
		private IActionProvider[] ActiveObjectSlots;

		//////////////////////////////////////////////////////////////////////////
		public ActionContext(Form OwnerForm)
		{
			this.OwnerForm = OwnerForm;

			ActiveObjectSlots = new IActionProvider[GetNumSlots()];
		}

		//////////////////////////////////////////////////////////////////////////
		private int GetNumSlots()
		{
			return Enum.GetValues(typeof(ActiveObjectSlot)).Length;
		}

		//////////////////////////////////////////////////////////////////////////
		public List<IActionProvider> ActiveObjects
		{
			get
			{
				List<IActionProvider> Ret = new List<IActionProvider>();
				for (int i = GetNumSlots() - 1; i >= 0; i--)
				{
					if (ActiveObjectSlots[i] != null) Ret.Add(ActiveObjectSlots[i]);
				}
				return Ret;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void ActivateObject(ActiveObjectSlot Slot, IActionProvider Obj)
		{
			ActiveObjectSlots[(int)Slot] = Obj;
			StripBuilder.RefreshManagedToolStrips();
		}


		//////////////////////////////////////////////////////////////////////////
		public void AttachIdleEvent()
		{
			Application.Idle += new EventHandler(OnIdle);
		}

		//////////////////////////////////////////////////////////////////////////
		public void DetachIdleEvent()
		{
			Application.Idle -= new EventHandler(OnIdle);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnIdle(object sender, EventArgs e)
		{
			if (OwnerForm == null || Form.ActiveForm == OwnerForm)
			{
				StripBuilder.SetManagedToolStripsState();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Invoke(string ActionName, object Sender)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return false;

			return Act.Invoke(Obj, Sender);
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionState GetState(string ActionName, object Sender)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return new ActionState(false, false);

			return Act.GetState(Obj, Sender);
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Initialize(string ActionName, object Sender)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return false;

			return Act.Initialize(Obj, Sender);
		}

		//////////////////////////////////////////////////////////////////////////
		public bool CustomBuild(string ActionName, ToolStripItemCollection Items, bool AsMenu)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return false;

			return Act.CustomBuild(Obj, Items, AsMenu);
		}


		ActionStripBuilder _Builder = null;
		//////////////////////////////////////////////////////////////////////////
		public ActionStripBuilder StripBuilder
		{
			get
			{
				if (_Builder == null) _Builder = new ActionStripBuilder(this);
				return _Builder;
			}
		}

	}
}
