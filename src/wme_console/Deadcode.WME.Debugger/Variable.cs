// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Design;
using System.ComponentModel;
using System.Globalization;

namespace DeadCode.WME.Debugger
{
	[TypeConverter(typeof(VariableObjectConverter))]
	[Editor(typeof(VariableEditor), typeof(UITypeEditor))]
	public class Variable : ICustomTypeDescriptor
	{
		private IWmeValue NativeValue;
		private int _Line = -1;
		public int Line
		{
			get
			{
				return _Line;
			}
		}

		
		//////////////////////////////////////////////////////////////////////////
		public Variable(IWmeValue NativeValue, int Line)
		{			
			this.NativeValue = NativeValue;
			this._Line = Line;
		}

		//////////////////////////////////////////////////////////////////////////
		public IWmeValue Value
		{
			get
			{
				return NativeValue;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool IsKnownConstant()
		{
			return Value.Name.StartsWith("VK_") ||
				   Value.Name.StartsWith("DI_") ||
				   Value.Name.StartsWith("TAL_") ||
				   Value.Name.StartsWith("VAL_");
		}

		public enum VariableScope
		{
			Local, Global
		}

		//////////////////////////////////////////////////////////////////////////
		public VariableScope Scope = VariableScope.Global;

		//////////////////////////////////////////////////////////////////////////
		public override string ToString()
		{
			//return Value.GetString();
			return Value.GetDebuggerDesc();
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

			if (Value.Type == EWmeValue.Native)
			{
				string[] Attrs = DebugClient.GetClassProperties(Value.GetNative().NativeClass);
				if(Attrs!=null)
				{
					foreach(string Attr in Attrs)
					{
						NativePropertyDescriptor Prop = GetNativeProp(Attr);
						if(Prop!=null) NewProps.Add(Prop);
					}
				}
			}

			if (Value.Properties.Count > 0)
			{
				foreach(IWmeValue Val in Value.Properties)
				{
					VariablePropertyDescriptor pd = new VariablePropertyDescriptor(new Variable(Val, this.Line));
					NewProps.Add(pd);
				}				
			}
			return NewProps;
		}

		//////////////////////////////////////////////////////////////////////////
		private NativePropertyDescriptor GetNativeProp(string Name)
		{
			try
			{
				if (Value.Type != EWmeValue.Native) return null;
				string Val = Value.GetNative().GetProperty(Name).GetString();
				return new NativePropertyDescriptor(Name, Val);
			}
			catch
			{
				return new NativePropertyDescriptor(Name, "<error retrieving property>");
			}
		}
#endregion
	}



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class VariablePropertyDescriptor : PropertyDescriptor
	{
		public Variable Var = null;

		public VariablePropertyDescriptor(Variable Var)
			: base(Var.Value.Name, null)
		{
			this.Var = Var;
		}

		public override object GetEditor(Type editorBaseType)
		{
			return base.GetEditor(editorBaseType);
		}

		public override AttributeCollection Attributes
		{
			get
			{
				return new AttributeCollection(null);
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
				return this.Var.GetType();
			}
		}

		public override string DisplayName
		{
			get
			{
				return Var.Value.Name;
			}
		}

		public override string Description
		{
			get
			{
				return string.Empty;
			}
		}

		public override object GetValue(object component)
		{
			return this.Var;
		}

		public override bool IsReadOnly
		{
			get
			{
				//return Var.Value.Properties.Count > 0 || Var.Value.Type==EWmeValue.Native;
				return true;
			}
		}

		public override string Name
		{
			get { return Var.Value.Name; }
		}

		public override Type PropertyType
		{
			get { return this.Var.GetType(); }
		}

		public override void ResetValue(object component) { }

		public override bool ShouldSerializeValue(object component)
		{
			return false;
		}

		public override void SetValue(object component, object value)
		{
			Var.Value.SetValue(value.ToString());
		}

		public override string Category
		{
			get
			{
				if(Var.IsKnownConstant()) return "Constants";
				else
				{
					switch(Var.Scope)
					{
						case Variable.VariableScope.Global:
							return "Global variables";
						case Variable.VariableScope.Local:
							return "Local variables";
					}
					return string.Empty;
				}
			}
		}

	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class NativePropertyDescriptor : PropertyDescriptor
	{
		private string PropName;
		private string PropValue;

		public NativePropertyDescriptor(string Name, string Value) : base(Name, null)
		{
			this.PropName = Name;
			this.PropValue = Value;
		}

		public override bool CanResetValue(object component)
		{
			return false;
		}

		public override Type ComponentType
		{
			get
			{
				return PropValue.GetType();
			}
		}

		public override object GetValue(object component)
		{
			return PropValue;
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
				return PropValue.GetType();
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


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class VariableEditor : UITypeEditor
	{
		public VariableEditor() { }

		public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
		{
			VariablePropertyDescriptor pd = context.PropertyDescriptor as VariablePropertyDescriptor;
			if (pd != null)
			{
				if (pd.Var.Value.Properties.Count > 0) return UITypeEditorEditStyle.None;
				else return UITypeEditorEditStyle.Modal;
			}
			return UITypeEditorEditStyle.None;
		}
	}



	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class VariableObjectConverter : ExpandableObjectConverter
	{
		//////////////////////////////////////////////////////////////////////////
		public override bool CanConvertFrom(ITypeDescriptorContext context, Type sourceType)
		{
			if (sourceType == typeof(string))
			{
				return true;
			}
			return base.CanConvertFrom(context, sourceType);
		}

		//////////////////////////////////////////////////////////////////////////
		public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
		{
			Variable Var = value as Variable;
			if (Var != null && destinationType == typeof(String))
			{
				return Var.ToString();
			}
			else return base.ConvertTo(context, culture, value, destinationType);
		}

		//////////////////////////////////////////////////////////////////////////
		public override object ConvertFrom(ITypeDescriptorContext context, CultureInfo culture, object value)
		{
			if (value is string)
			{
				VariablePropertyDescriptor pd = context.PropertyDescriptor as VariablePropertyDescriptor;
				if (pd != null)
				{
					pd.Var.Value.SetValue((string)value);
					return pd.Var;
				}
				else return null;
			}
			return base.ConvertFrom(context, culture, value);
		}

		public override bool IsValid(ITypeDescriptorContext context, object value)
		{
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public override bool GetPropertiesSupported(ITypeDescriptorContext context)
		{
			VariablePropertyDescriptor pd = context.PropertyDescriptor as VariablePropertyDescriptor;
			if (pd != null) return pd.Var.Value.Properties.Count > 0 || pd.Var.Value.Type==EWmeValue.Native;
			else return true;
		}
	}

}
