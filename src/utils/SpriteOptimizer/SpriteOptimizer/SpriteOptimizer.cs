using System;
using System.Collections.Generic;
using System.Text;
using DeadCode.WME.Core;
using System.IO;
using System.Windows.Forms;
using System.Drawing;

namespace DeadCode.WME.SpriteOptimizer
{
	public class SpriteOptimizer
	{
		private string ProjectFile;
		private ILogConsumer LogConsumer;
		private WGame Game;
		private int TotalSavedPixels;

		//////////////////////////////////////////////////////////////////////////
		public SpriteOptimizer(string projectFile, ILogConsumer logConsumer)
		{
			ProjectFile = projectFile;
			LogConsumer = logConsumer;

			LogConsumer.ClearLog();
			TotalSavedPixels = 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddLog(string text)
		{
			LogConsumer.AddLog(text + "\n");
		}

		//////////////////////////////////////////////////////////////////////////
		public void Process()
		{
			Game = new WGame();
			Game.InitializeSimple(ProjectFile);

			string rootDir = Path.GetDirectoryName(ProjectFile);
			ProcessDir(rootDir);

			Game.Dispose();

			AddLog(string.Format("Finished. Saved {0} pixels in total (that's approximately {1} MB of video memory).", TotalSavedPixels, TotalSavedPixels * 4 / 1024 / 1024));
		}

		//////////////////////////////////////////////////////////////////////////
		private void ProcessDir(string path)
		{
			foreach (string file in Directory.GetFiles(path, "*.sprite"))
			{
				Sprite sprite = new Sprite(Game, this);
				sprite.LoadFromFile(file);
				if (sprite.Process()) sprite.SaveToFile();
			}
			
			foreach (string dir in Directory.GetDirectories(path))
			{
				ProcessDir(dir);
			}
		}


		private Dictionary<string, Point> HotspotMap = new Dictionary<string, Point>();
		//////////////////////////////////////////////////////////////////////////
		public void CropImage(string fileName, out int modX, out int modY)
		{
			modX = modY = 0;

			string absPath = Game.MakeAbsolutePath(fileName);
			if (!File.Exists(absPath))
			{
				AddLog(string.Format("Skipping image '{0}' (file not found).", fileName));
				return;
			}

			absPath = absPath.ToLower();
			absPath.Replace("/", "\\");

			if (HotspotMap.ContainsKey(absPath))
			{
				modX = HotspotMap[absPath].X;
				modY = HotspotMap[absPath].Y;
				return;
			}
			else
			{
				ImageCropper ic = new ImageCropper(this);
				ic.Crop(absPath, out modX, out modY);

				HotspotMap[absPath] = new Point(modX, modY);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void AddSavedPixels(int savedPixels)
		{
			TotalSavedPixels += savedPixels;
		}
	}
}
