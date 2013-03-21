using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing;
using System.Windows.Forms;

namespace DeadCode.WME.SpriteOptimizer
{
	public class ImageCropper
	{
		private SpriteOptimizer Optimizer;

		//////////////////////////////////////////////////////////////////////////
		public ImageCropper(SpriteOptimizer optimizer)
		{
			Optimizer = optimizer;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool Crop(string fileName, out int modX, out int modY)
		{
			try
			{
				Bitmap image = new Bitmap(fileName, false);

				int origWidth = image.Width;
				int origHeight = image.Height;

				int cutLeft, cutRight, cutTop, cutBottom;
				GetBorders(image, out cutLeft, out cutRight, out cutTop, out cutBottom);

				if (cutLeft == image.Width)
				{
					// empty image
					cutLeft = cutTop = 0;
					cutRight = image.Width - 1;
					cutBottom = image.Height - 1;
				}
				else
				{
					cutLeft = Math.Max(0, cutLeft - 1);
					cutTop = Math.Max(0, cutTop - 1);
					cutRight = Math.Max(0, cutRight - 1);
					cutBottom = Math.Max(0, cutBottom - 1);
				}

				if (cutLeft == 0 && cutRight == 0 && cutTop == 0 && cutBottom == 0)
				{
					modX = modY = 0;
					return false;
				}

				Rectangle rect = new Rectangle(cutLeft, cutTop, image.Width - cutRight - cutLeft, image.Height - cutBottom - cutTop);
				Bitmap newImage = image.Clone(rect, image.PixelFormat);
				image.Dispose();

				newImage.Save(fileName);

				modX = -cutLeft;
				modY = -cutTop;

				int savedPixels = origWidth * origHeight - newImage.Width * newImage.Height;
				Optimizer.AddLog(string.Format("Cropped image '{0}' (saved {1} pixels).", fileName, savedPixels.ToString()));
				Optimizer.AddSavedPixels(savedPixels);

				newImage.Dispose();

				return true;
			}
			catch (Exception e)
			{
				Optimizer.AddLog("Error cropping image: " + e.Message);
				modX = modY = 0;
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void GetBorders(Bitmap image, out int cutLeft, out int cutRight, out int cutTop, out int cutBottom)
		{
			cutLeft = GetLeftBorder(image);
			cutRight = GetRightBorder(image);
			cutTop = GetTopBorder(image);
			cutBottom = GetBottomBorder(image);
		}

		//////////////////////////////////////////////////////////////////////////
		int GetLeftBorder(Bitmap image)
		{
			for (int x = 0; x < image.Width; x++)
			{
				for (int y = 0; y < image.Height; y++)
				{
					Color pix = image.GetPixel(x, y);
					if (pix.A > 0) return x;
				}
			}
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		int GetRightBorder(Bitmap image)
		{
			for (int x = 0; x < image.Width; x++)
			{
				for (int y = 0; y < image.Height; y++)
				{
					Color pix = image.GetPixel(image.Width - x - 1, y);
					if (pix.A > 0) return x;
				}
			}
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		int GetTopBorder(Bitmap image)
		{
			for (int y = 0; y < image.Height; y++)
			{
				for (int x = 0; x < image.Width; x++)
				{
					Color pix = image.GetPixel(x, y);
					if (pix.A > 0) return y;
				}
			}
			return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		int GetBottomBorder(Bitmap image)
		{
			for (int y = 0; y < image.Height; y++)
			{
				for (int x = 0; x < image.Width; x++)
				{
					Color pix = image.GetPixel(x, image.Height - y - 1);
					if (pix.A > 0) return y;
				}
			}
			return 0;
		}

	}
}
