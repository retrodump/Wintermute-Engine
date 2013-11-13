using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;
using DeadCode.WME.Global;
using System.Runtime.InteropServices;
using System.IO;

namespace DeadCode.WME.StringTableMgr
{
	public partial class ClassifyForm : FormBase
	{
		public ClassifyForm()
		{
			InitializeComponent();

			LblContext.Text = "";
			LblStatusIgnored.Text = "";
		}

		public StringTableMgr Mgr;

		private bool HideStringsWithID = false;

		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			this.MinimumSize = this.Size;

			if (AppMgr != null)
			{
				LoadLayout(AppMgr.Settings);
			}
            Reload();
   		}

        //////////////////////////////////////////////////////////////////////////
        private void Reload()
        {
            UpdateStatusLocked = true;

			int OrigSelIndex = -1;
			if (ListStrings.SelectedIndices.Count > 0) OrigSelIndex = ListStrings.SelectedIndices[0];
			
			if (Mgr != null)
            {
                if (Mgr.ProjectStrings != null)
                {
                    ListStrings.BeginUpdate();
                    ListStrings.Items.Clear();
                    foreach (StringItem StrItem in Mgr.ProjectStrings)
                    {
						if (HideStringsWithID && StrItem.ID != string.Empty) continue;
                        AddStringToList(StrItem);
                    }
                    ListStrings.EndUpdate();
                }

                if (Mgr.IgnoredStrings != null)
                {
                    ListIgnored.BeginUpdate();
                    ListIgnored.Items.Clear();
                    foreach (StringItem StrItem in Mgr.IgnoredStrings)
                    {
                        if (StrItem.IgnoreReason == IgnoreReason.AlreadyInTable) continue;
                        AddStringToIgnoreList(StrItem);
                    }
                    ListIgnored.EndUpdate();
                }

            }

			int SelIndex = Math.Min(ListStrings.Items.Count - 1, OrigSelIndex);
			if (SelIndex >= 0)
			{
				ListStrings.Items[SelIndex].Selected = true;
				ListStrings.EnsureVisible(SelIndex);
			}

            UpdateStatusLocked = false;
            UpdateStringStatus();
        }

		//////////////////////////////////////////////////////////////////////////
		private ListViewItem AddStringToList(StringItem StrItem)
		{
			return AddStringToList(StrItem, false);
		}


		//////////////////////////////////////////////////////////////////////////
		private ListViewItem AddStringToList(StringItem StrItem, bool RespectOrder)
		{
			ListViewItem Item = new ListViewItem(StrItem.ID);
			Item.SubItems.Add(StrItem.Value);
			Item.Tag = StrItem;

			bool Found = false;

			if(RespectOrder)
			{
				foreach(ListViewItem It in ListStrings.Items)
				{
					StringItem Str = It.Tag as StringItem;
					if(Str != null && Str.Order > StrItem.Order)
					{
						ListStrings.Items.Insert(It.Index, Item);
						Found = true;
						break;
					}
				}
			}

			if (!Found)
			{
				ListStrings.Items.Add(Item);
			}

			return Item;
		}

		//////////////////////////////////////////////////////////////////////////
		private ListViewItem AddStringToIgnoreList(StringItem StrItem)
		{
			ListViewItem Item = new ListViewItem(StrItem.Value);

			Item.SubItems.Add(IgnoreReasonToString(StrItem.IgnoreReason));
			Item.Tag = StrItem;

			ListIgnored.Items.Add(Item);

			return Item;
		}

		//////////////////////////////////////////////////////////////////////////
		private string IgnoreReasonToString(IgnoreReason Reason)
		{
			switch (Reason)
			{
				case IgnoreReason.AlreadyInTable:
					return "Already in string table";
				case IgnoreReason.InIgnoreList:
					return "In ignore list";
				case IgnoreReason.IsFilename:
					return "File name";
				case IgnoreReason.KnownCodePattern:
					return "Known code pattern";
				case IgnoreReason.SelectedByUser:
					return "Removed by user";
				default:
					return "Unknown";
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnFormClosed(object sender, FormClosedEventArgs e)
		{
			if (AppMgr != null)
			{
				SaveLayout(AppMgr.Settings);
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnColumnWidthChanged(object sender, ColumnWidthChangedEventArgs e)
		{
			ListView List = sender as ListView;
			if (List == null) return;

			if (e.ColumnIndex == List.Columns.Count - 1) return;
			else FixLastColumn(List);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnListResize(object sender, EventArgs e)
		{
			ListView List = sender as ListView;
			if (List == null) return;

			if (List.Columns.Count > 0) FixLastColumn(List);
		}

		//////////////////////////////////////////////////////////////////////////
		private void FixLastColumn(ListView List)
		{
			int ColWidth = 0;
			for(int i=0; i<List.Columns.Count - 1; i++)
			{
				ColWidth += List.Columns[i].Width;
			}
			ColWidth += SystemInformation.VerticalScrollBarWidth + 2 * SystemInformation.Border3DSize.Width;
			ColWidth = List.Width - ColWidth;
			if (ColWidth < 10) ColWidth = 10;
			
			List.Columns[List.Columns.Count - 1].Width = ColWidth;

			/*
			if (List.Columns.Count > 0)
			{
				ColumnHeader Col = List.Columns[List.Columns.Count - 1];
				Col.Width = -2;
			}
			*/
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnClose(object sender, EventArgs e)
		{
			this.Close();
		}

		private int PrevSelectedIndex = -1;
		//////////////////////////////////////////////////////////////////////////
		private void OnStringSelected(object sender, EventArgs e)
		{
			ListView List = sender as ListView;
			if (List == null || List.SelectedItems.Count != 1) return;

			StringItem Item = List.SelectedItems[0].Tag as StringItem;
			if (Item != null) DisplayContext(Item);

			// handle multi selection
			if (List == ListStrings)
			{
				if (PrevSelectedIndex >= 0 && (ModifierKeys & Keys.Shift) == Keys.Shift)
				{
					int Start, End;
					if (PrevSelectedIndex < List.SelectedIndices[0])
					{
						Start = PrevSelectedIndex;
						End = List.SelectedIndices[0];
					}
					else
					{
						Start = List.SelectedIndices[0];
						End = PrevSelectedIndex;
					}

					bool Select = (ModifierKeys & Keys.Control) != Keys.Control;

					UpdateStatusLocked = true;
					ListStrings.BeginUpdate();
					for (int i = Start; i <= End; i++)
					{
						List.Items[i].Checked = Select;
					}
					ListStrings.EndUpdate();
					UpdateStatusLocked = false;
					UpdateStringStatus();

				}
				PrevSelectedIndex = List.SelectedIndices[0];
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void DisplayContext(StringItem Item)
		{
			LblContext.Text = "";
			TxtContext.Text = "";

			if (Item == null) return;

			LblContext.Text = "Line " + Item.Line.ToString() + ", " + Item.Filename;

			if (Item.IsScriptItem)
			{
				try
				{
					string FileText = "";
					using (StreamReader sr = new StreamReader(Item.Filename, Encoding.Default, true))
					{
						string Line;
						while ((Line = sr.ReadLine()) != null)
						{
							FileText += Line + "\r\n";
						}
					}
					TxtContext.Text = FileText;
					int SelStart = TxtContext.GetFirstCharIndexFromLine(Item.Line);
					if (SelStart >= 0)
					{
						TxtContext.Select(SelStart, 0);
						TxtContext.ScrollToCaret();

						SelStart = TxtContext.GetFirstCharIndexFromLine(Item.Line - 1);
						if (SelStart >= 0)
						{
							TxtContext.Select(SelStart, TxtContext.Lines[Item.Line - 1].Length);
						}
					}
				}
				catch (Exception e)
				{
					TxtContext.Text = "Error loading file\n" + e.Message;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnIgnoreItems(object sender, EventArgs e)
		{
			IgnoreItems(false);
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAlwaysIgnoreItems(object sender, EventArgs e)
		{
			IgnoreItems(true);
		}

		//////////////////////////////////////////////////////////////////////////
		private void IgnoreItems(bool AddToIgnoreList)
		{
			StringItem[] SelectedItems = GetSelectedStrings();
			if (SelectedItems.Length == 0) return;

			if(AddToIgnoreList)
			{
				if (MessageBox.Show("Selected item(s) will be ignored and added to the ignore list. Continue?", this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) return;
			}

			ListStrings.BeginUpdate();
			ListIgnored.BeginUpdate();

			int OrigSelIndex = -1;
			if (ListStrings.SelectedIndices.Count > 0) OrigSelIndex = ListStrings.SelectedIndices[0];

			if (ListStrings.CheckedItems.Count > 0)
			{
				foreach (ListViewItem Item in ListStrings.CheckedItems)
				{
					IgnoreSingleItem(Item, AddToIgnoreList);
				}
			}
			else if (ListStrings.SelectedIndices.Count > 0)
			{
				foreach (ListViewItem Item in ListStrings.SelectedItems)
				{
					IgnoreSingleItem(Item, AddToIgnoreList);
				}
			}

			// select ignored
			int SelIndex;
			SelIndex = ListIgnored.Items.Count - 1;
			if (SelIndex >= 0)
			{
				ListIgnored.Items[SelIndex].Selected = true;
				ListIgnored.EnsureVisible(SelIndex);
			}

			// select string
			SelIndex = Math.Min(ListStrings.Items.Count - 1, OrigSelIndex);
			if (SelIndex >= 0)
			{
				ListStrings.Items[SelIndex].Selected = true;
				ListStrings.EnsureVisible(SelIndex);
			}


			ListStrings.EndUpdate();
			ListIgnored.EndUpdate();

			UpdateStringStatus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void IgnoreSingleItem(ListViewItem Item, bool AddToIgnoreList)
		{
			StringItem StrItem = Item.Tag as StringItem;
			if (StrItem == null) return;

			Mgr.ProjectStrings.Remove(StrItem);
			Mgr.IgnoredStrings.Add(StrItem);

			if(AddToIgnoreList)
			{
				if (!Mgr.IgnoreList.Contains(StrItem.Value)) Mgr.IgnoreList.Add(StrItem.Value);
			}

			StrItem.IgnoreReason = AddToIgnoreList ? IgnoreReason.InIgnoreList : IgnoreReason.SelectedByUser;
			StrItem.Ignored = true;

			ListStrings.Items.Remove(Item);			
			AddStringToIgnoreList(StrItem);
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateSelectionMenu()
		{
			ToolStripMenuItem Item = new ToolStripMenuItem("Selection");
			
			Item.DropDownItems.Add("Select items with no ID", null, new EventHandler(OnSelectWithoutID));
			Item.DropDownItems.Add("-");
			Item.DropDownItems.Add("Select all", null, new EventHandler(OnSelectAll));
			Item.DropDownItems.Add("Select none", null, new EventHandler(OnSelectNone));
			Item.DropDownItems.Add("Invert selection", null, new EventHandler(OnSelectInvert));

			return Item;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectWithoutID(object sender, EventArgs e)
		{
			UpdateStatusLocked = true;
			ListStrings.BeginUpdate();
			foreach (ListViewItem Item in ListStrings.Items)
			{
				StringItem StrItem = Item.Tag as StringItem;
				Item.Checked = StrItem != null && StrItem.ID == string.Empty;
			}
			ListStrings.EndUpdate();
			UpdateStatusLocked = false;
			UpdateStringStatus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectAll(object sender, EventArgs e)
		{
			UpdateStatusLocked = true;
			ListStrings.BeginUpdate();
			foreach(ListViewItem Item in ListStrings.Items)
			{
				Item.Checked = true;
			}
			ListStrings.EndUpdate();
			UpdateStatusLocked = false;
			UpdateStringStatus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectNone(object sender, EventArgs e)
		{
            SelectNone();
		}

        //////////////////////////////////////////////////////////////////////////
        private void SelectNone()
        {
            UpdateStatusLocked = true;
            ListStrings.BeginUpdate();
            foreach (ListViewItem Item in ListStrings.Items)
            {
                Item.Checked = false;
            }
            ListStrings.EndUpdate();
            UpdateStatusLocked = false;
            UpdateStringStatus();
        }

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectInvert(object sender, EventArgs e)
		{
			UpdateStatusLocked = true;
			ListStrings.BeginUpdate();
			foreach (ListViewItem Item in ListStrings.Items)
			{
				Item.Checked = !Item.Checked;
			}
			ListStrings.EndUpdate();
			UpdateStatusLocked = false;
			UpdateStringStatus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnStringMenu(object sender, CancelEventArgs e)
		{
			StringItem[] SelectedItems = GetSelectedStrings();

			StringMenu.Items.Clear();

			if (SelectedItems.Length > 0)
			{
				if (ListStrings.CheckedIndices.Count == 0)
				{
					StringMenu.Items.Add("Ignore item", null, new EventHandler(OnIgnoreItems));
					StringMenu.Items.Add("Always ignore item", null, new EventHandler(OnAlwaysIgnoreItems));
				}
				else
				{
					StringMenu.Items.Add("Ignore " + SelectedItems.Length.ToString() + " selected item(s)", null, new EventHandler(OnIgnoreItems));
					StringMenu.Items.Add("Always ignore " + SelectedItems.Length.ToString() + " selected item(s)", null, new EventHandler(OnAlwaysIgnoreItems));
				}
			}

			if (StringMenu.Items.Count > 0) StringMenu.Items.Add("-");
			StringMenu.Items.Add(CreateSelectionMenu());

			if (SelectedItems.Length > 0)
			{
				StringMenu.Items.Add("-");
				StringMenu.Items.Add("Assign string ID...", null, new EventHandler(OnAssignID));
				StringMenu.Items.Add("Remove string ID", null, new EventHandler(OnRemoveID));
				StringMenu.Items.Add("-");

				ToolStripMenuItem btn = new ToolStripMenuItem("Hide strings with assigned ID", null, new EventHandler(OnHide));
				btn.Checked = HideStringsWithID;
				StringMenu.Items.Add(btn);
			}
									
			e.Cancel = StringMenu.Items.Count == 0;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAssignID(object sender, EventArgs e)
		{
			UpdateStatusLocked = true;
			ListStrings.BeginUpdate();


			string InitVal = "";
			int Dummy;
			if (ListStrings.SelectedItems.Count > 0)
			{
				InitVal = ListStrings.SelectedItems[0].Text;
				Mgr.GetIDBase(InitVal, out InitVal, out Dummy);
			}

			StringIdForm Form = new StringIdForm();
			Form.Mgr = Mgr;
			Form.StringID = InitVal;
			if (Form.ShowDialog() != DialogResult.OK) return;

			string Msg = "";
			if (ListStrings.CheckedItems.Count > 0) Msg = ListStrings.CheckedItems.Count.ToString() + " selected item(s)";
			else Msg = "Selected item";

			Msg += " will be assigned ID '" + Form.StringID + "' starting with number " + (Form.LastNum + 1).ToString("0000") + ".";
			Msg += "\n\nDo you want to continue?";

			if (MessageBox.Show(Msg, this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) return;

			StringItem[] SelItems = GetSelectedStrings();

			int NumID = Form.LastNum;
			foreach(StringItem StrItem in SelItems)
			{
				NumID++;
				string FinalID = Form.StringID + NumID.ToString("0000");

				StrItem.ID = FinalID;
			}
			RefreshStringIDs();
            SelectNone();
			
			ListStrings.EndUpdate();
			UpdateStatusLocked = false;

			if (HideStringsWithID) Reload();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRemoveID(object sender, EventArgs e)
		{
			if (MessageBox.Show("Do you really want to remove string ID from selected items?", this.Text, MessageBoxButtons.YesNo, MessageBoxIcon.Question) != DialogResult.Yes) return;

			StringItem[] SelItems = GetSelectedStrings();
			foreach (StringItem StrItem in SelItems)
			{
				StrItem.ID = "";
			}
			RefreshStringIDs();			
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnHide(object sender, EventArgs e)
		{
			HideStringsWithID = !HideStringsWithID;

			int OrigSelIndex = -1;
			if (ListStrings.SelectedIndices.Count > 0) OrigSelIndex = ListStrings.SelectedIndices[0];

			Reload();

			int SelIndex = Math.Min(ListStrings.Items.Count - 1, OrigSelIndex);
			if (SelIndex >= 0)
			{
				ListStrings.Items[SelIndex].Selected = true;
				ListStrings.EnsureVisible(SelIndex);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void RefreshStringIDs()
		{
			ListStrings.BeginUpdate();
			foreach (ListViewItem Item in ListStrings.Items)
			{
				StringItem StrItem = Item.Tag as StringItem;
				if (StrItem != null) Item.Text = StrItem.ID;
			}
			ListStrings.EndUpdate();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnIgnoreMenu(object sender, CancelEventArgs e)
		{
			IgnoreMenu.Items.Clear();
			StringItem StrItem = null;
			if (ListIgnored.SelectedItems.Count > 0)
				StrItem = ListIgnored.SelectedItems[0].Tag as StringItem;

			if(StrItem!=null)
			{
				IgnoreMenu.Items.Add("Unignore item", null, new EventHandler(OnUnignoreItem));
				IgnoreMenu.Items.Add("-");
				
				if(Mgr.IgnoreList.Contains(StrItem.Value))
					IgnoreMenu.Items.Add("Remove from ignore list", null, new EventHandler(OnRemoveFromIgnoreList));
				else
					IgnoreMenu.Items.Add("Add to ignore list", null, new EventHandler(OnAddToIgnoreList));

				e.Cancel = false;
			}
			else e.Cancel = true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnUnignoreItem(object sender, EventArgs e)
		{
			StringItem StrItem = null;
			if (ListIgnored.SelectedItems.Count > 0)
			{
				ListViewItem Item = ListIgnored.SelectedItems[0];
				StrItem = Item.Tag as StringItem;

				if (StrItem != null)
				{
					StrItem.Ignored = false;
					Mgr.ProjectStrings.Add(StrItem);
					Mgr.IgnoredStrings.Remove(StrItem);

					int OrigSelIndex = Item.Index;
					ListIgnored.Items.Remove(Item);
					
					int SelIndex = Math.Min(ListIgnored.Items.Count - 1, OrigSelIndex);
					if (SelIndex >= 0)
					{
						ListIgnored.Items[SelIndex].Selected = true;
						ListIgnored.EnsureVisible(SelIndex);
					}

					ListViewItem ListItem = AddStringToList(StrItem, true);
					SelIndex = ListItem.Index;
					if (SelIndex >= 0)
					{
						ListStrings.Items[SelIndex].Selected = true;
						ListStrings.EnsureVisible(SelIndex);
					}
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnAddToIgnoreList(object sender, EventArgs e)
		{
			StringItem StrItem = null;
			if (ListIgnored.SelectedItems.Count > 0)
			{
				ListViewItem Item = ListIgnored.SelectedItems[0];
				StrItem = Item.Tag as StringItem;
				if (StrItem != null)
				{
					if (!Mgr.IgnoreList.Contains(StrItem.Value))
						Mgr.IgnoreList.Add(StrItem.Value);

					StrItem.IgnoreReason = IgnoreReason.InIgnoreList;
					Item.SubItems[1].Text = IgnoreReasonToString(StrItem.IgnoreReason);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnRemoveFromIgnoreList(object sender, EventArgs e)
		{
			StringItem StrItem = null;
			if (ListIgnored.SelectedItems.Count > 0)
			{
				ListViewItem Item = ListIgnored.SelectedItems[0];
				StrItem = Item.Tag as StringItem;
				if (StrItem != null)
				{
					if (Mgr.IgnoreList.Contains(StrItem.Value))
						Mgr.IgnoreList.Remove(StrItem.Value);

					StrItem.IgnoreReason = IgnoreReason.SelectedByUser;
					Item.SubItems[1].Text = IgnoreReasonToString(StrItem.IgnoreReason);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		StringItem[] GetSelectedStrings()
		{
			List<StringItem> Strings = new List<StringItem>();
			if (ListStrings.CheckedIndices.Count > 0)
			{
				foreach (int Index in ListStrings.CheckedIndices)
				{
					Strings.Add(ListStrings.Items[Index].Tag as StringItem);
				}
			}
			else if (ListStrings.SelectedIndices.Count > 0)
			{
				foreach (int Index in ListStrings.SelectedIndices)
				{
					Strings.Add(ListStrings.Items[Index].Tag as StringItem);
				}
			}
			return Strings.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnStringChecked(object sender, ItemCheckedEventArgs e)
		{
			UpdateStringStatus();
		}

		bool UpdateStatusLocked = false;
		//////////////////////////////////////////////////////////////////////////
		private void UpdateStringStatus()
		{
			if (!UpdateStatusLocked)
			{
				int Count = ListStrings.CheckedItems.Count;
				if (Count > 0)
					LblStatusStrings.Text = Count.ToString() + " item(s) selected";
				else
					LblStatusStrings.Text = "";
			}
		}

	}
}