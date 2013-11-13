using System;
using System.Collections.Generic;
using System.Windows.Forms;
using System.Drawing;
using System.Text;

namespace DeadCode.WME.Controls
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ImageListBox : ListBox
	{
		private ImageList _ImageList;
		public ImageList ImageList
		{
			get
			{
				return _ImageList;
			}
			set
			{
				_ImageList = value;
				if (_ImageList != null) this.ItemHeight = _ImageList.ImageSize.Height;
			}
		}

		public ImageListBox()
		{
			// Set owner draw mode
			this.DrawMode = DrawMode.OwnerDrawFixed;
		}

		protected override void OnDrawItem(System.Windows.Forms.DrawItemEventArgs e)
		{
			
			e.DrawBackground();
			e.DrawFocusRectangle();
			Rectangle bounds = e.Bounds;

			// no item
			if (e.Index < 0 || e.Index >= Items.Count)
			{
				e.Graphics.DrawString(Text, e.Font, new SolidBrush(e.ForeColor),
					bounds.Left, bounds.Top);
			}
			else
			{
				int LabelOffset = 0;
				int ImageIndex = -1;
				string ItemText = Items[e.Index].ToString();
				Color ItemColor = Color.FromKnownColor(KnownColor.Transparent);
				bool ItemMark = false;

				if (ImageList != null) LabelOffset = ImageList.ImageSize.Width;
				
				ImageListBoxItem Item = Items[e.Index] as ImageListBoxItem;
				if (Item != null)
				{
					ImageIndex = Item.ImageIndex;
					ItemColor = Item.ForeColor;
					ItemMark = Item.Mark;
				}

				// draw image, if available
				if(ImageList!=null && ImageIndex>= 0 && ImageIndex < ImageList.Images.Count)
				{
					ImageList.Draw(e.Graphics, bounds.Left, bounds.Top, ImageIndex);
				}

				// draw label
				Color forecolor = (ItemColor != Color.FromKnownColor(KnownColor.Transparent)) ? ItemColor : e.ForeColor;
				Font font = ItemMark ? new Font(e.Font, FontStyle.Bold) : e.Font;


				e.Graphics.DrawString(ItemText, font, new SolidBrush(forecolor),
						bounds.Left + LabelOffset, bounds.Top);				
			}			
			base.OnDrawItem(e);
		}		
	}


	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ImageListBoxItem
	{
		private Color _ForeColor = Color.FromKnownColor(KnownColor.Transparent);
		private bool _Mark = false;
		private object _Tag = null;

		private string _Text;
		private int _ImageIndex;

		// properties 
		public string Text
		{
			get
			{
				return _Text;
			}
			set
			{
				_Text = value;
			}
		}

		public int ImageIndex
		{
			get
			{
				return _ImageIndex;
			}
			set
			{
				_ImageIndex = value;
			}
		}

		public Color ForeColor
		{
			get
			{
				return _ForeColor;
			}
			set
			{
				_ForeColor = value;
			}
		}

		public bool Mark
		{
			get
			{
				return _Mark;
			}
			set
			{
				_Mark = value;
			}
		}

		public object Tag
		{
			get
			{
				return _Tag;
			}
			set
			{
				_Tag = value;
			}
		}


		//constructor
		public ImageListBoxItem(string text, int index, bool mark, Color forecolor)
		{
			_Text = text;
			_ImageIndex = index;
			_Mark = mark;
			_ForeColor = forecolor;
		}

		public ImageListBoxItem(string text, int index, bool mark) : this(text, index, mark, Color.FromKnownColor(KnownColor.Transparent)) { }
		public ImageListBoxItem(string text, int index) : this(text, index, false) { }
		public ImageListBoxItem(string text) : this(text, -1) { }
		public ImageListBoxItem() : this("") { }

		public override string ToString()
		{
			return _Text;
		}
	}

}
