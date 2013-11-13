using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global.Actions;
using DeadCode.WME.Global;

namespace ActionsTest
{
	public partial class Form2 : ActionForm, IActionProvider
	{
		TestProvider Test;
		public Form2()
		{
			ActionManager.LoadActions(typeof(Form2));

			Test = new TestProvider();			

			//ActContext.ActivateObject(this);
			InitializeComponent();
		}

		private void button1_Click(object sender, EventArgs e)
		{
			ActionStripItem i = ActContext.StripBuilder.DefineTestMenu();

			ActContext.StripBuilder.AddToolStrip(i, menuStrip1.Items, true);
			ActContext.StripBuilder.AddToolStrip(i, contextMenuStrip1.Items, true);
			ActContext.StripBuilder.AddToolStrip(i, toolStrip1.Items, false);

			ActContext.StripBuilder.RefreshManagedToolStrips();
		}

		bool Act = false;
		private void button2_Click(object sender, EventArgs e)
		{
			if(!Act) ActContext.ActivateObject(ActiveObjectSlot.Application, this);
			else ActContext.ActivateObject(ActiveObjectSlot.Application, null);
			Act = !Act;
		}


		void Item_Click(object sender, EventArgs e)
		{
			throw new Exception("The method or operation is not implemented.");
		}


		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Test.Action1",
			Caption = "Action 1",
			IconName = "Graphics.Icons.breakpoint.png")
		]
		public void TestAction(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					MessageBox.Show("Action 1 invoked\n" + Param.Sender.GetType().ToString());
					Param.Processed = true;
					break;

				case ActionParam.QueryType.Initialize:
					((ToolStripItem)Param.Sender).ForeColor = Color.Red;
					break;

				case ActionParam.QueryType.GetState:
					Param.State.Enabled = Action2Checked;
					break;
			}
		}

		private bool Action2Checked = true;
		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Test.Action2",
			Caption = "Action 2",
			IconName = "Graphics.Icons.bookmark.bmp",
			Type=ActionType.Toggle,
			DisplayType=DisplayType.ImageAndText,
			DisplayLabel=true,
			ShortcutKeys=Keys.F12)
		]				
		public void TestAction2(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					//MessageBox.Show("Action 2 invoked");
					Action2Checked = !Action2Checked;
					Param.Processed = true;
					break;

				case ActionParam.QueryType.GetState:					
					Param.State.Enabled = true;
					Param.State.Checked = Action2Checked;
					Param.State.Caption = Action2Checked ? "Checked" : "Unchecked";
					break;
			}
		}

		[ActionProp("Test.Action3", Caption = "Search", Tooltip = "Search something", Type = ActionType.Combo, DisplayLabel = true)]
		public void TestAction3(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					MessageBox.Show(((ToolStripItem)Param.Sender).Text);
					break;

				case ActionParam.QueryType.Initialize:
					ToolStripComboBox Combo = Param.Sender as ToolStripComboBox;
					if(Combo!=null)
					{
						//Combo.DropDownStyle = ComboBoxStyle.DropDownList;
						Combo.Items.Add("aaa");
						Combo.Items.Add("bbb");
						Combo.Items.Add("ccc");

						Combo.AutoCompleteMode = AutoCompleteMode.Append;
						Combo.AutoCompleteSource = AutoCompleteSource.ListItems;
					}
					break;
			}
		}

		private void cccToolStripMenuItem_Click(object sender, EventArgs e)
		{

		}

		private void toolStripComboBox1_SelectedIndexChanged(object sender, EventArgs e)
		{
			MessageBox.Show("combo");
		}

		private void toolStripComboBox1_KeyDown(object sender, KeyEventArgs e)
		{
			if(e.KeyCode == Keys.Enter)
			{
				//((ToolStripComboBox)sender).PerformClick();
			}
		}

		bool Act2;
		private void button3_Click(object sender, EventArgs e)
		{
			if (!Act2) ActContext.ActivateObject(ActiveObjectSlot.Control, Test);
			else ActContext.ActivateObject(ActiveObjectSlot.Control, null);
			Act2 = !Act2;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			menuStrip1.Visible = false;
		}

		private void toolStrip1_ItemClicked(object sender, ToolStripItemClickedEventArgs e)
		{

		}
	}


	internal class TestProvider : IActionProvider
	{
		public TestProvider()
		{
			ActionManager.LoadActions(typeof(TestProvider));
		}

		[ActionProp("Test.Action1",
			Caption = "Action 1 Test",
			IconName = "Graphics.Icons.breakpoint.png")
		]
		public void TestAction(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.Invoke:
					MessageBox.Show("Action 1 Test invoked\n" + Param.Sender.GetType().ToString());
					Param.Processed = true;
					break;

				//case ActionParam.QueryType.GetState:
				//	Param.State.Enabled = true;
				//	break;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		[ActionProp("Test.Action2", Caption = "Action 2")]
		public void TestAction2(ActionParam Param)
		{
			switch (Param.Type)
			{
				case ActionParam.QueryType.GetState:
					//Param.State.Visible = false;
					break;
			}
		}


	};
}
