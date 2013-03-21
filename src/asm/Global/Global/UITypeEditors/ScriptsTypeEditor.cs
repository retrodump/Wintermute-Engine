// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Design;
using System.ComponentModel;
using System.Windows.Forms;
using DeadCode.WME.Core;
using System.Media;

namespace DeadCode.WME.Global.UITypeEditors
{
	[TypeConverter(typeof(ScriptCollectionTypeConverter))]
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptCollection
	{
		public List<string> Scripts = new List<string>();
	}

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptCollectionTypeConverter : TypeConverter
	{
		//////////////////////////////////////////////////////////////////////////
		public override bool CanConvertTo(ITypeDescriptorContext context, Type destinationType)
		{
			if (destinationType == typeof(string))
				return true;
			else
				return base.CanConvertTo(context, destinationType);
		}

		//////////////////////////////////////////////////////////////////////////
		public override object ConvertTo(ITypeDescriptorContext context, System.Globalization.CultureInfo culture, object value, Type destinationType)
		{
			ScriptCollection Col = value as ScriptCollection;
			if (Col != null && destinationType == typeof(string))
			{
				if (Col.Scripts.Count == 1)
					return Col.Scripts.Count.ToString() + " script";
				else if (Col.Scripts.Count > 1)
					return Col.Scripts.Count.ToString() + " scripts";
				else
					return "";
			}
			else return base.ConvertTo(context, culture, value, destinationType);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ScriptsTypeEditor : UITypeEditor
	{

		//////////////////////////////////////////////////////////////////////////
		public override UITypeEditorEditStyle GetEditStyle(ITypeDescriptorContext context)
		{
			return UITypeEditorEditStyle.Modal;
		}

		//////////////////////////////////////////////////////////////////////////
		public override object EditValue(ITypeDescriptorContext context, IServiceProvider provider, object value)
		{
			if (context == null) return value;
			NativePropProxy Proxy = context.Instance as NativePropProxy;
			if (Proxy == null || Proxy.NativeObject == null)
			{
				SystemSounds.Asterisk.Play();
				return value;
			}
			ScriptCollection OrigCol = value as ScriptCollection;

			ScriptsForm dlg = new ScriptsForm();
			dlg.Game = Proxy.NativeObject.Game;
			dlg.ParentForm = Form.ActiveForm;

			foreach(string Scr in OrigCol.Scripts)
			{
				dlg.Scripts.Add(Scr);
			}

			if (dlg.ShowDialog() == DialogResult.OK)
			{
				ScriptCollection NewCol = new ScriptCollection();
				foreach (string Scr in dlg.Scripts)
				{
					NewCol.Scripts.Add(Scr);
				}
				return NewCol;
			}
			else return OrigCol;
		}
	}
}
