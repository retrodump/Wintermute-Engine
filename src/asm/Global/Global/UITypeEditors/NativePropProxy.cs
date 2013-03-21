// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.ComponentModel;
using DeadCode.WME.Global.UITypeEditors;
using System.Drawing.Design;

namespace DeadCode.WME.Global
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	[TypeConverter(typeof(PropertySorter))]
	[DefaultProperty("Name")]
	public class NativePropProxy
	{
		//////////////////////////////////////////////////////////////////////////
		public NativePropProxy(WObject NativeObject)
		{
			_NativeObject = NativeObject;
		}

		protected WObject _NativeObject;
		//////////////////////////////////////////////////////////////////////////
		[Browsable(false)]
		public WObject NativeObject
		{
			get
			{
				return _NativeObject;
			}
		}

		[Category(CategoryName.Behavior), PropertyOrder(100)]
		[Description("Scripts attached to this object")]
		[EditorAttribute(typeof(ScriptsTypeEditor), typeof(UITypeEditor))]
		//////////////////////////////////////////////////////////////////////////
		public ScriptCollection Scripts
		{
			get
			{
				ScriptCollection Ret = new ScriptCollection();
				for(int i=0; i< NativeObject.Scripts.Count; i++)
				{
					Ret.Scripts.Add(NativeObject.Scripts[i].Filename);
				}
				return Ret;
			}
			set
			{
				OnPropertyChanging("Scripts");

				NativeObject.EditorRemoveAllScripts();
				foreach(string Scr in value.Scripts)
				{
					NativeObject.AddScript(Scr);
				}
			}
		}


		//////////////////////////////////////////////////////////////////////////
		public event PropertyChangedEventHandler PropertyChanging;
		protected void OnPropertyChanging(string PropName)
		{
			if (PropertyChanging != null) PropertyChanging(this, new PropertyChangedEventArgs(PropName));
		}

		//////////////////////////////////////////////////////////////////////////
		public event PropertyChangedEventHandler PropertyChanged;
		protected void OnPropertyChanged(string PropName)
		{
			if (PropertyChanged != null) PropertyChanged(this, new PropertyChangedEventArgs(PropName));
		}
	}

}
