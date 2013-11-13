using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using System.Reflection;

namespace DeadCode.WME.StringTableMgr
{
	public partial class FormAbout : Form
	{
		public FormAbout()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			Assembly Asm = Assembly.GetExecutingAssembly();
			LblVersion.Text += " " + Asm.GetName().Version.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLinkClicked(object sender, LinkLabelLinkClickedEventArgs e)
		{
			System.Diagnostics.Process.Start(LblLink.Text);
		}
	}
}