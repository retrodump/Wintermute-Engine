using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Controls.Actions
{
	public class ActionContext
	{
		private List<IActionProvider> ActiveObjects = new List<IActionProvider>();

		//////////////////////////////////////////////////////////////////////////
		public void ActivateObject(IActionProvider Obj)
		{
			if (!ActiveObjects.Contains(Obj)) ActiveObjects.Insert(0, Obj);
		}

		//////////////////////////////////////////////////////////////////////////
		public void DeactivateObject(IActionProvider Obj)
		{
			if (ActiveObjects.Contains(Obj)) ActiveObjects.Remove(Obj);
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
		public Action.State GetState(string ActionName)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return Action.State.Disabled;

			return Act.GetState(Obj);
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionData SaveData(string ActionName)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return null;

			return Act.SaveData(Obj);
		}

		//////////////////////////////////////////////////////////////////////////
		public bool LoadData(string ActionName, ActionData Data)
		{
			// get object of given type
			IActionProvider Obj;
			Action Act;
			ActionManager.GetActionByName(ActionName, ActiveObjects, out Obj, out Act);
			if (Obj == null || Act == null) return false;

			return Act.LoadData(Obj, Data);
		}

	}
}
