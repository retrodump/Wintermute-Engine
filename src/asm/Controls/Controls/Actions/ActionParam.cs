using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Controls.Actions
{
	public class ActionParam
	{
		public enum QueryType
		{
			Invoke,
			GetState,
			SaveData,
			LoadData
		}

		public QueryType Type;
		public Action.State State;
		public bool Processed;
		public ActionData Data;
	}
}
