using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Core;

namespace WrapperTest
{
	public partial class Form1 : Form
	{
		public Form1()
		{
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			WGame Game = new WGame();
			Game.SetLogCallback(LogCallback);
			//Game.SetScriptCompilerCallback(CompilerErrorCallback);

			/*
			WFrame Frame = new WFrame(Game);
			Frame.Events.Add("aaa");
			Frame.Events.Add("bbb");
			Frame.Events.Add("ccc");

			MessageBox.Show(Frame.Events.IndexOf("bbb").ToString());

			foreach(String S in Frame.Events)
			{
				MessageBox.Show(S);
			}
			Frame.Events.Clear();
			*/
			Game.InitializeWindowed(this, true, @"C:\Program Files\WME DevKit\projects\wme_demo\wme_demo.wpr");
			Game.FrameInit();
			Game.FrameFinish();

			//WAdScene Scene;

			Game.Dispose();
		}

		private void LogCallback(String Text, int Result)
		{
			//MessageBox.Show(Text);
		}

		private void CompilerErrorCallback(int Line, String Text)
		{
			MessageBox.Show("Error at line " + Line.ToString() + ": " + Text);
		}
	}

	class Test
	{
		public int i;
	}
}