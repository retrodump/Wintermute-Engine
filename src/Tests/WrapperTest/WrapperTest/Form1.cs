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

		WAdGame Game;
		WSprite Sprite;
		WAdEntity Entity;
		WUIWindow Win;
		private void button1_Click(object sender, EventArgs e)
		{
			if(Game==null)
			{
				Game = new WAdGame();
				Canvas.Create(Game, @"F:\Program Files\WME DevKit\projects\wme_demo\wme_demo.wpr", @"c:\test.log");

				Sprite = new WSprite(Game);
				Sprite.LoadFromFile(@"actors\molly\rr\walk.sprite");

				Entity = new WAdEntity(Game);
				Entity.LoadFromFile(@"entities\oldguy\oldguy.entity");

				Win = new WUIWindow(Game);
				Win.LoadFromFile(@"interface\system\mainmenu.window");
				Game.Windows.Add(Win);
				Game.FocusedWindow = Win;
			}
		}

		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if(Game!=null)
			{
				Canvas.Release();
				Sprite.Dispose();
				Entity.Dispose();
				Win.Dispose();
				Game.Dispose();
				Game = null;
			}
		}

		private void button2_Click(object sender, EventArgs e)
		{
			Canvas.SuspendedUpdate = !Canvas.SuspendedUpdate;
		}

		private void OnPaintContent(object sender, EventArgs e)
		{
			Entity.X = 200;
			Entity.Y = 300;
			Entity.Update();

			//Sprite.GetCurrentFrame();
			//Sprite.Display(200, 300);
			//Sprite.Frames[0].Display(100, 200);
			
			
			Win.X = -Canvas.HorizontalScroll.Value;
			Win.Y = -Canvas.VerticalScroll.Value;
			
			
			
			//Entity.Display();
			Game.DisplayWindows(false);
			//Win.Display();

			Game.ActiveObject = Game.Renderer.GetObjectAt(Game.MousePos);
			
			//if(Game.ActiveObject!=null) button2.Text = Game.ActiveObject.ToString();
			button2.Text = Game.MousePos.ToString();
		}

		private void button3_Click(object sender, EventArgs e)
		{
			Canvas.RouteMouseClicks = !Canvas.RouteMouseClicks;
		}

	}
}