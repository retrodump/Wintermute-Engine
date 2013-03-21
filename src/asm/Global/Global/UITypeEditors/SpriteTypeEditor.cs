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
	public class SpriteTypeEditor : UITypeEditor
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

			WSprite OrigSprite = value as WSprite;

			FileAssignForm dlg = new FileAssignForm();
			dlg.Game = Proxy.NativeObject.Game;
			dlg.ParentForm = Form.ActiveForm;
			dlg.Filter = "Sprites|*.sprite;*.png;*.bmp;*.jpg;*.tga";
			if (OrigSprite != null && OrigSprite.Filename != null)
				dlg.Filename = OrigSprite.Filename;

			DialogResult Res = dlg.ShowDialog();

			switch (Res)
			{
				case DialogResult.OK:
					try
					{
						WSprite NewSprite = new WSprite(Proxy.NativeObject.Game);
						if (!NewSprite.LoadFromFile(dlg.Filename))
						{
							NewSprite.Dispose();
							return value;
						}
						else return NewSprite;
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
