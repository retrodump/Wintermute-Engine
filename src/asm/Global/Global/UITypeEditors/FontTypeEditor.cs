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
	public class FontTypeEditor : UITypeEditor
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

			WFont OrigFont = value as WFont;

			FileAssignForm dlg = new FileAssignForm();
			dlg.Game = Proxy.NativeObject.Game;
			dlg.ParentForm = Form.ActiveForm;
			dlg.Filter = "Fonts|*.font";
			if (OrigFont != null && OrigFont.Filename != null)
				dlg.Filename = OrigFont.Filename;

			DialogResult Res = dlg.ShowDialog();

			switch(Res)
			{
				case DialogResult.OK:
					try
					{
						WFont NewFont = Proxy.NativeObject.Game.CreateFont(dlg.Filename);
						if (NewFont != null) return NewFont;
						else return value;
					}
					catch
					{
						return value;
					}
				case DialogResult.Abort:
					return null;
				default:
					return value;
			}

		}
	}
}
