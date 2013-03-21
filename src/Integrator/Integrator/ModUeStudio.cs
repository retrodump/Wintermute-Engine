using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Integrator
{
	public partial class ModUeStudio : ModUltraEdit
	{
		protected override string ExeName
		{
			get
			{
				return "uestudio.exe";
			}
		}

		protected override string IniEnvVar
		{
			get
			{
				return "UESINIDIR";
			}
		}

		protected override string IniName
		{
			get
			{
				return "uestudio.ini";
			}
		}

		protected override string AppDataPath
		{
			get
			{
				return @"IDMComp\UEStudio\";
			}
		}


		public ModUeStudio()
		{
			InitializeComponent();
		}
	}
}

