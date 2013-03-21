using System;
using System.Collections.Generic;
using System.Text;
using System.Reflection;

namespace DeadCode.WME.Controls.Actions
{	
	public class ActionManager
	{
		private static Dictionary<string, List<Action>> Actions = new Dictionary<string, List<Action>>();

		//////////////////////////////////////////////////////////////////////////
		public static void LoadActions(Type type)
		{
			foreach(MethodInfo mi in type.GetMethods())
			{
				foreach(Attribute Attr in mi.GetCustomAttributes(true))
				{
					ActionPropAttribute ActProp = Attr as ActionPropAttribute;
					if(ActProp!=null)
					{
						// TODO: check method parameters

						// create action
						Action Act = new Action(type, mi, ActProp.Name, ActProp);

						// create dictionary entry if necessary
						if (!Actions.ContainsKey(ActProp.Name)) Actions.Add(ActProp.Name, new List<Action>());

						// add action to the list of actions with this name
						Actions[ActProp.Name].Add(Act);
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static void GetActionByName(string ActionName, List<IActionProvider> ActiveObjects, out IActionProvider OutObj, out Action OutAct)
		{
			OutObj = null;
			OutAct = null;

			if(Actions.ContainsKey(ActionName))
			{
				List<Action> Acts = Actions[ActionName];

				// find nearest available object, if any
				foreach (IActionProvider Obj in ActiveObjects)
				{
					foreach(Action Act in Acts)
					{
						if (Act.ContextType == Obj.GetType())
						{
							OutObj = Obj;
							OutAct = Act;
							return;
						}
					}
				}
			}
			return;
		}

		//////////////////////////////////////////////////////////////////////////
		public static Action GetAction(string ActionName)
		{
			if (Actions.ContainsKey(ActionName))
			{
				List<Action> Acts = Actions[ActionName];
				if (Acts.Count > 0) return Acts[0];
			}
			return null;
		}

	}
}
