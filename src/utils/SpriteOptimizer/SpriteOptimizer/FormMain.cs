using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using System.IO;

namespace DeadCode.WME.SpriteOptimizer
{
	public partial class FormMain : FormBase, ILogConsumer
	{
		//////////////////////////////////////////////////////////////////////////
		public FormMain()
		{
			InitializeComponent();
			AppMgr = new ApplicationMgr("SpriteOptimizer");
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;			
			AppMgr.Settings.LoadFromXmlFile();
			LoadLayout(AppMgr.Settings);
			TbProject.Text = AppMgr.Settings.GetString("ProjectFile");
			TbProject.Focus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			SaveLayout(AppMgr.Settings);
			AppMgr.Settings.SetValue("ProjectFile", TbProject.Text);
			AppMgr.Settings.SaveToXmlFile();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnProcess(object sender, EventArgs e)
		{
			string msg;
			msg = "All sprites in the project will be scanned and empty borders (i.e. pixels with zero alpha) will be cut from images.\n";
			msg += "BACKUP YOUR PROJECT FIRST!\n\n";
			msg += "Continue?";

			if (MessageBox.Show(msg, this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) == DialogResult.Yes)
			{
				SpriteOptimizer opt = new SpriteOptimizer(TbProject.Text, this);
				opt.Process();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnProjectChanged(object sender, EventArgs e)
		{
			BtnProcess.Enabled = File.Exists(TbProject.Text);
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddLog(string text)
		{
			TbLog.SelectionLength = 0;
			TbLog.AppendText(text.Replace("\n", "\r\n"));

			TbLog.SelectionStart = TbLog.Text.Length;
			TbLog.SelectionLength = 0;
			TbLog.ScrollToCaret();
		}

		//////////////////////////////////////////////////////////////////////////
		public void ClearLog()
		{
			TbLog.Text = "";
		}


	}
}