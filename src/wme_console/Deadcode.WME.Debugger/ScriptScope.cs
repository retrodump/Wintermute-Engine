// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.ComponentModel;


namespace DeadCode.WME.Debugger
{
	public class ScriptScope : ICustomTypeDescriptor
	{
		public Dictionary<IWmeValue, Variable> Vars = new Dictionary<IWmeValue, Variable>(new IWmeValueComparer());

		//////////////////////////////////////////////////////////////////////////
		public void AddVar(IWmeValue Var, int Line)
		{
			if (!Vars.ContainsKey(Var)) Vars.Add(Var, new Variable(Var, Line));
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddVar(IWmeValue Var)
		{
			AddVar(Var, -1);
		}

		//////////////////////////////////////////////////////////////////////////
		public Variable GetVarByName(string Name)
		{
			foreach (KeyValuePair<IWmeValue, Variable> kvp in Vars)
			{
				if (kvp.Value.Value.Name == Name) return kvp.Value;
			}
			return null;
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
			if (Vars.Count > 0)
			{
				foreach(KeyValuePair<IWmeValue, Variable> kvp in Vars)
				{
					VariablePropertyDescriptor pd = new VariablePropertyDescriptor(kvp.Value);
					NewProps.Add(pd);
				}
			}
			return NewProps;
		}
#endregion
	}

}
