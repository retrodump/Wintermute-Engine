using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.Controls.Actions
{
	public enum ActionType
	{
		Button,
		Toggle,
		Editor,
		Combo
	}

	public enum DisplayType
	{
		Image,
		Text,
		ImageAndText
	}

	public class ActionPropAttribute : Attribute
	{
		public ActionPropAttribute(string Name)
		{
			this.Name = Name;
		}

		public ActionPropAttribute(string Name, string Caption, string Tooltip)
		{
			this.Name = Name;
			this.Caption = Caption;
			this.Tooltip = Tooltip;
		}

		public ActionPropAttribute(string Name, string Caption, string Tooltip, string IconName)
		{
			this.Name = Name;
			this.Caption = Caption;
			this.Tooltip = Tooltip;
			this.IconName = IconName;
		}

		public ActionPropAttribute(string Name, string Caption, string Tooltip, string IconName, ActionType Type)
		{
			this.Name = Name;
			this.Caption = Caption;
			this.Tooltip = Tooltip;
			this.IconName = IconName;
			this.Type = Type;
		}


		private ActionType _Type = ActionType.Button;
		public ActionType Type
		{
			get
			{
				return _Type;
			}
			set
			{
				_Type = value;
			}
		}

		private DisplayType _DisplayType = DisplayType.Image;
		public DisplayType DisplayType
		{
			get
			{
				return _DisplayType;
			}
			set
			{
				_DisplayType = value;
			}
		}

		private string _Name;
		public string Name
		{
			get
			{
				return _Name;
			}
			set
			{
				_Name = value;
			}
		}

		private string _Caption;
		public string Caption
		{
			get
			{
				return _Caption;
			}
			set
			{
				_Caption = value;
			}
		}

		private string _Tooltip;
		public string Tooltip
		{
			get
			{
				return _Tooltip;
			}
			set
			{
				_Tooltip = value;
			}
		}

		private string _IconName;
		public string IconName
		{
			get
			{
				return _IconName;
			}
			set
			{
				_IconName = value;
			}
		}

		private bool _DisplayLabel;
		public bool DisplayLabel
		{
			get
			{
				return _DisplayLabel;
			}
			set
			{
				_DisplayLabel = value;
			}
		}

	}
}
