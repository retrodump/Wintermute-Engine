// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;

namespace DeadCode.WME.Debugger
{
	public class WatchProvider : ICustomTypeDescriptor
	{
		//////////////////////////////////////////////////////////////////////////
		private class WatchItem : PropertyDescriptor
		{
			public WatchItem(string WatchName) : base(WatchName, null)
			{
				this.WatchName = WatchName;
				this.Value = null;
			}

			public string WatchName;
			public Variable Value;

			public override string DisplayName
			{
				get
				{
					return WatchName;
				}
			}

			public override bool CanResetValue(object component)
			{
				return false;
			}

			public override Type ComponentType
			{
				get
				{
					return this.GetType();
				}
			}

			public override object GetValue(object component)
			{
				return "<variable name is not valid in current context>";
			}

			public override bool IsReadOnly
			{
				get
				{
					return true;
				}
			}

			public override Type PropertyType
			{
				get
				{
					return this.GetType();
				}
			}

			public override void ResetValue(object component)
			{
			}

			public override void SetValue(object component, object value)
			{
			}

			public override bool ShouldSerializeValue(object component)
			{
				return false;
			}
		};

		List<WatchItem> Items = new List<WatchItem>();

		//////////////////////////////////////////////////////////////////////////
		public bool AddWatch(string Name)
		{
			foreach(WatchItem Item in Items)
			{
				if (Item.Name == Name) return true;
			}
			Items.Add(new WatchItem(Name));
			Refresh();

			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool RemoveWatch(string Name)
		{
			foreach (WatchItem Item in Items)
			{
				if (Item.Name == Name)
				{
					Items.Remove(Item);
					return true;
				}
			}
			return false;
		}

		//////////////////////////////////////////////////////////////////////////
		public string[] GetWatchNames()
		{
			List<string> Names = new List<string>();
			foreach(WatchItem Item in Items)
			{
				Names.Add(Item.WatchName);
			}
			return Names.ToArray();
		}

		private ScriptScope GlobalScope;
		private Script Script;
		//////////////////////////////////////////////////////////////////////////
		public bool Refresh(ScriptScope GlobalScope)
		{
			this.GlobalScope = GlobalScope;
			return Refresh();
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Refresh(Script Script)
		{
			this.Script = Script;
			return Refresh();
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Refresh()
		{
			foreach(WatchItem Item in Items)
			{
				Variable Var = null;
				if (Script != null)
				{
					ScriptScope CurrScope = Script.GetCurrentScope();
					if (CurrScope == null) CurrScope = Script.GlobalScope;

					Var = CurrScope.GetVarByName(Item.WatchName);
				}
				if (GlobalScope != null && Var == null)
				{
					Var = GlobalScope.GetVarByName(Item.WatchName);
				}
				Item.Value = Var;
			}
			return true;
		}

		#region ICustomTypeDescriptor implementation
		//////////////////////////////////////////////////////////////////////////
		public String GetClassName()
		{
			return TypeDescriptor.GetClassName(this, true);
		}

		public AttributeCollection GetAttributes()
		{
			return TypeDescriptor.GetAttributes(this, true);
		}

		public String GetComponentName()
		{
			return TypeDescriptor.GetComponentName(this, true);
		}

		public TypeConverter GetConverter()
		{
			return TypeDescriptor.GetConverter(this, true);
		}

		public EventDescriptor GetDefaultEvent()
		{
			return TypeDescriptor.GetDefaultEvent(this, true);
		}

		public PropertyDescriptor GetDefaultProperty()
		{
			return TypeDescriptor.GetDefaultProperty(this, true);
		}

		public object GetEditor(Type editorBaseType)
		{
			return TypeDescriptor.GetEditor(this, editorBaseType, true);
		}

		public EventDescriptorCollection GetEvents(Attribute[] attributes)
		{
			return TypeDescriptor.GetEvents(this, attributes, true);
		}

		public EventDescriptorCollection GetEvents()
		{
			return TypeDescriptor.GetEvents(this, true);
		}

		public object GetPropertyOwner(PropertyDescriptor pd)
		{
			return this;
		}

		public PropertyDescriptorCollection GetProperties(Attribute[] attributes)
		{
			return GetProperties();
		}

		public PropertyDescriptorCollection GetProperties()
		{
			PropertyDescriptorCollection NewProps = new PropertyDescriptorCollection(null);
			foreach(WatchItem Item in Items)
			{
				if(Item.Value==null)
				{
					NewProps.Add(Item);
				}
				else
				{
					VariablePropertyDescriptor pd = new VariablePropertyDescriptor(Item.Value);
					NewProps.Add(pd);
				}
			}
			return NewProps;
		}
		#endregion
	}
}
