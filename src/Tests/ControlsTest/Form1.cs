using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Controls;
using Aga.Controls.Tree;

namespace ControlsTest
{
	public partial class Form1 : Form
	{
		FloatControl FC;
		public Form1()
		{
			InitializeComponent();


			this.FC = new FloatControl();
			this.FC.Name = "listBoxHover";
			//this.FC.TabStop = false;
			//this.FC.Visible = false;
			//this.FC.BackColor = SystemColors.Info;
			//this.FC.ForeColor = SystemColors.InfoText;
		}



		private void button1_Click(object sender, EventArgs e)
		{
			if (FC.Visible) FC.Hide();
			else
			{
				Point pt = button1.Location;
				pt = button1.PointToScreen(pt);

				FC.Size = button1.Size;
				FC.Location = pt;
				FC.Text = "Test message ffsdfdsfds fdssdfsfd";
				FC.ReceiveClicks = true;
				FC.ShowFloating();
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			ControlPaint.DrawScrollButton(
				System.Drawing.Graphics.FromHwnd(button1.Handle),
				0, 0,
				12, 12,
				ScrollButton.Down,
				ButtonState.Flat);

		}

		private void button3_Click(object sender, EventArgs e)
		{
			imageListBox1.Items.Clear();
			imageListBox1.Items.Add(new ImageListBoxItem("aaa", 0));
			imageListBox1.Items.Add(new ImageListBoxItem("bbb", 1));
			imageListBox1.Items.Add(new ImageListBoxItem("ccc", 2, true));
			imageListBox1.Items.Add(new ImageListBoxItem("dddfdsdfdfsfdsfsdsfdfdsfdsfdfdsfdsfds", 3, true, Color.Red));
			imageListBox1.Items.Add(new ImageListBoxItem("AyAyAy", 4));
		}

		private void button2_Click_1(object sender, EventArgs e)
		{
			TreeModel Model = new TreeModel();
			Node Root = new Node("Bla bla bla");

			for (int i = 0; i < 20; i++)
				Root.Nodes.Add(new Node("Child " + i.ToString()));
			
			Model.Nodes.Add(Root);
			treeListView1.Model = Model;

			treeListView1.SelectedNode = treeListView1.FindNode(Model.GetPath(Root));
			treeListView1.SelectedNode.IsExpanded = true;
			treeListView1.Focus();
		}


		private void button4_Click(object sender, EventArgs e)
		{
			TreePath Path = treeListView1.GetPath(treeListView1.SelectedNode);
			Node SelNode = Path.LastNode as Node;
			MessageBox.Show(SelNode.Text);
		}

		private void button5_Click(object sender, EventArgs e)
		{
			TreeModel Model = treeListView1.Model as TreeModel;

			Node NewNode = new Node("qwerty");
			Model.Nodes[0].Nodes.Add(NewNode);

			TreeNodeAdv Node = treeListView1.FindNode(Model.GetPath(NewNode));
			treeListView1.SelectedNode = Node;
			treeListView1.ScrollTo(Node);
			
		}


	}
}