// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.Drawing;

namespace DeadCode.WME.Global.Actions
{
	#region ActionStripItem class
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ActionStripItem
	{
		public ActionStripItem()
		{
			this.ActionName = "";
			this.Caption = "";
		}

		public ActionStripItem(string ActionName)
		{
			this.ActionName = ActionName;
			this.Caption = "";
		}

		public ActionStripItem(string ActionName, string Caption)
		{
			this.ActionName = ActionName;
			this.Caption = Caption;
		}

		public ActionStripItem AddSub(string Name, string Caption)
		{
			ActionStripItem Item = new ActionStripItem(Name, Caption);
			Items.Add(Item);
			return Item;
		}

		public ActionStripItem AddItem(string ActionName)
		{
			ActionStripItem Item = new ActionStripItem(ActionName);
			Items.Add(Item);
			return Item;
		}

		public ActionStripItem AddItem()
		{
			ActionStripItem Item = new ActionStripItem();
			Items.Add(Item);
			return Item;
		}

		public string ActionName;
		public string Caption;
		public List<ActionStripItem> Items = new List<ActionStripItem>();
	};
	#endregion

	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class ActionStripBuilder
	{
		ActionContext ActContext = null;

		//////////////////////////////////////////////////////////////////////////
		public ActionStripBuilder(ActionContext Context)
		{
			ActContext = Context;
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionStripItem DefineStandardMenu()
		{
			ActionStripItem Root = new ActionStripItem();
			
				ActionStripItem FileMenu = Root.AddSub("FileMenu", "File");
					FileMenu.AddItem("File.New");
					FileMenu.AddItem("File.Open");
					FileMenu.AddItem();
					FileMenu.AddItem("File.Save");
					FileMenu.AddItem("File.SaveAs");
					FileMenu.AddItem();
					FileMenu.AddItem("File.Exit");

			return Root;
		}

		//////////////////////////////////////////////////////////////////////////
		public ActionStripItem DefineTestMenu()
		{
			ActionStripItem Root = new ActionStripItem();

				ActionStripItem FileMenu = Root.AddSub("FileMenu", "File");
					FileMenu.AddItem("Test.Action1");
					FileMenu.AddItem();
					FileMenu.AddItem();
					FileMenu.AddItem("Test.Action2");
					FileMenu.AddItem("Test.Action3");
					FileMenu.AddItem();
					FileMenu.AddItem();
				Root.AddItem("Test.Action2");
				Root.AddItem();
				Root.AddItem("Test.Action3");

			return Root;
		}

		private class ManagedToolStrip
		{
			public ManagedToolStrip(ActionStripItem RootItem, ToolStripItemCollection Items, bool AsMenu)
			{
				this.RootItem = RootItem;
				this.Items = Items;
				this.AsMenu = AsMenu;
			}
			public ActionStripItem RootItem;
			public ToolStripItemCollection Items;
			public bool AsMenu;
		};

		private List<ManagedToolStrip> ManagedStrips = new List<ManagedToolStrip>();

		//////////////////////////////////////////////////////////////////////////
		public void AddToolStrip(ActionStripItem Root, ToolStripItemCollection Items, bool AsMenu)
		{
			foreach(ManagedToolStrip Strip in ManagedStrips)
			{
				if(Strip.Items==Items)
				{
					ManagedStrips.Remove(Strip);
					break;
				}
			}
			ManagedStrips.Add(new ManagedToolStrip(Root, Items, AsMenu));
		}

		//////////////////////////////////////////////////////////////////////////
		public void RemoveToolStrip(ToolStripItemCollection Items)
		{
			foreach (ManagedToolStrip Strip in ManagedStrips)
			{
				if (Strip.Items == Items)
				{
					ManagedStrips.Remove(Strip);
					return;
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void PopulateToolStrip(ActionStripItem Root, ToolStripItemCollection Items, bool AsMenu)
		{
			Items.Clear();
			AddStripItems(ActContext.ActiveObjects, Root, Items, AsMenu);
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetManagedToolStripsState()
		{
			foreach(ManagedToolStrip Strip in ManagedStrips)
			{
				SetToolStripState(Strip.Items);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public void RefreshManagedToolStrips()
		{
			foreach (ManagedToolStrip Strip in ManagedStrips)
			{
				PopulateToolStrip(Strip.RootItem, Strip.Items, Strip.AsMenu);
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetToolStripState(ToolStripItemCollection Items)
		{
			foreach(ToolStripItem Item in Items)
			{
				string ActName = Item.Tag as string;
				if (ActName != null)
				{
					ActionState St = ActContext.GetState(ActName, Item);

					Item.Enabled = St.Enabled;
					Item.Available = St.Visible;

					if (St.Caption != null) Item.Text = St.Caption;
					if (St.Tooltip != null) Item.ToolTipText = St.Tooltip;

					if (Item is ToolStripButton)
						((ToolStripButton)Item).Checked = St.Checked;
					else if (Item is ToolStripMenuItem)
						((ToolStripMenuItem)Item).Checked = St.Checked;

				}

				ToolStripDropDownItem DropItem = Item as ToolStripDropDownItem;
				if(DropItem != null)
				{
					SetToolStripState(DropItem.DropDownItems);

					// hide menus with no items
					if (DropItem.DropDownItems.Count > 0)
					{
						bool HideItem = true;
						foreach (ToolStripItem It in DropItem.DropDownItems)
						{
							if (It.Available && !(It is ToolStripSeparator))
							{
								HideItem = false;
								break;
							}
						}
						if (HideItem) Item.Available = false;
					}
				}
			}
			WeedSeparators(Items, false);
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddStripItems(List<IActionProvider> ActiveObjects, ActionStripItem Root, ToolStripItemCollection Items, bool AsMenu)
		{
			foreach (ActionStripItem Item in Root.Items)
			{
				// is it a container?
				if (Item.Items.Count > 0)
				{
					// add subitem
					if (AsMenu)
					{
						ToolStripMenuItem Submenu = new ToolStripMenuItem(Item.Caption);
						AddStripItems(ActiveObjects, Item, Submenu.DropDownItems, true);

						Items.Add(Submenu);
					}
					else
					{
						// TODO: DropDown / SplitButton
						if(Item.Caption==String.Empty)
						{
							ToolStripSplitButton Btn = (ToolStripSplitButton)CreateButtonItem(Item.Items[0], ActionManager.GetAction(Item.Items[0].ActionName, ActiveObjects), AsMenu, true);
							AddStripItems(ActiveObjects, Item, Btn.DropDownItems, true);
							Items.Add(Btn);
						}
						else
						{
							ToolStripDropDownButton Btn = new ToolStripDropDownButton(Item.Caption);
							AddStripItems(ActiveObjects, Item, Btn.DropDownItems, true);
							Items.Add(Btn);
						}
					}
				}
				// normal item
				else
				{
					// separator
					if(Item.ActionName==String.Empty)
					{
						Items.Add(new ToolStripSeparator());
						continue;
					}

					// no action, no toolstrip item
					Action Act = ActionManager.GetAction(Item.ActionName, ActiveObjects);
					if (Act == null) continue;

					// regular action
					ToolStripItem NewItem = null;
					switch(Act.Attr.Type)
					{
						case ActionType.Button:
							NewItem = CreateButtonItem(Item, Act, AsMenu);
							break;

						case ActionType.Toggle:
							NewItem = CreateToggleItem(Item, Act, AsMenu);
							break;

						case ActionType.Combo:
							if(Act.Attr.DisplayLabel && !AsMenu)
							{
								ToolStripItem Label = CreateLabelItem(Item, Act, AsMenu);
								if (Label != null) Items.Add(Label);
							}
							NewItem = CreateComboItem(Item, Act, AsMenu);
							break;

						case ActionType.Editor:
							if (Act.Attr.DisplayLabel)
							{
								ToolStripItem Label = CreateLabelItem(Item, Act, AsMenu);
								if (Label != null) Items.Add(Label);
							}
							NewItem = CreateEditorItem(Item, Act, AsMenu);
							break;

						case ActionType.Custom:
							ActContext.CustomBuild(Act.Name, Items, AsMenu);
							break;
					}
					if (NewItem != null)
					{
						NewItem.Tag = Act.Name;
						Items.Add(NewItem);

						ActContext.Initialize(Act.Name, NewItem);
					}
				}
			}

			WeedSeparators(Items, true);
		}

		//////////////////////////////////////////////////////////////////////////
		private void WeedSeparators(ToolStripItemCollection Items, bool RemoveSeparators)
		{
			bool PrevSeparator = true;
			for (int i = 0; i < Items.Count; i++)
			{
				if (!Items[i].Available) continue;

				if (Items[i] is ToolStripSeparator)
				{
					if (PrevSeparator)
					{
						if (RemoveSeparators)
						{
							Items.RemoveAt(i);
							i--;
						}
						else Items[i].Available = false;
					}
					PrevSeparator = true;
				}
				else PrevSeparator = false;
			}

			PrevSeparator = true;
			for (int i = Items.Count - 1; i >= 0; i--)
			{
				if (!Items[i].Available) continue;

				if (Items[i] is ToolStripSeparator && PrevSeparator)
				{
					if (RemoveSeparators)
					{
						Items.RemoveAt(i);
					}
					else Items[i].Available = false;
				}
				else break;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateButtonItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			return CreateButtonItem(Item, Act, AsMenu, false);
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateButtonItem(ActionStripItem Item, Action Act, bool AsMenu, bool SplitButton)
		{
			if (AsMenu)
			{
				ToolStripMenuItem NewItem = new ToolStripMenuItem(Act.Attr.Caption);
				if (Act.Icon != null)
				{
					NewItem.Image = Act.Icon;
					NewItem.ImageTransparentColor = Color.Magenta;
				}
				NewItem.AutoToolTip = false;

				NewItem.ShortcutKeys = Act.Attr.ShortcutKeys;

				// click event
				NewItem.Click += delegate(object sender, EventArgs e)
				{
					ActContext.Invoke(Act.Name, sender);
				};
				
				return NewItem;
			}
			else
			{
				ToolStripItem NewItem;
				if (SplitButton) NewItem = new ToolStripSplitButton(Act.Attr.Caption);
				else NewItem = new ToolStripButton(Act.Attr.Caption);

				if (Act.Icon != null)
				{
					NewItem.Image = Act.Icon;
					NewItem.ImageTransparentColor = Color.Magenta;
				}
				NewItem.ToolTipText = Act.Attr.Tooltip;

				switch(Act.Attr.DisplayType)
				{
					case DisplayType.Image:
						NewItem.DisplayStyle = ToolStripItemDisplayStyle.Image;
						break;

					case DisplayType.Text:
						NewItem.DisplayStyle = ToolStripItemDisplayStyle.Text;
						break;

					case DisplayType.ImageAndText:
						NewItem.DisplayStyle = ToolStripItemDisplayStyle.ImageAndText;
						break;
				}

				if (NewItem is ToolStripSplitButton)
				{
					((ToolStripSplitButton)NewItem).ButtonClick += delegate(object sender, EventArgs e)
					{
						ActContext.Invoke(Act.Name, sender);
					};
				}
				else
				{
					NewItem.Click += delegate(object sender, EventArgs e)
					{
						ActContext.Invoke(Act.Name, sender);
					};
				}

				return NewItem;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateToggleItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			ToolStripItem NewItem = CreateButtonItem(Item, Act, AsMenu);
			if (NewItem!=null)
			{
				if (AsMenu)
				{
					((ToolStripMenuItem)NewItem).CheckOnClick = true;
				}
				else
				{
					((ToolStripButton)NewItem).CheckOnClick = true;
				}
			}
			return NewItem;
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateEditorItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			ToolStripTextBox NewItem = new ToolStripTextBox();
			NewItem.AutoToolTip = false;
			if (!AsMenu) NewItem.ToolTipText = Act.Attr.Tooltip;

			NewItem.KeyDown += delegate(object sender, KeyEventArgs e)
			{
				if (e.KeyCode == Keys.Enter)
				{
					ActContext.Invoke(Act.Name, sender);
					((ToolStripItem)sender).PerformClick();
				}
			};

			return NewItem;
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateComboItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			ToolStripComboBox NewItem = new ToolStripComboBox();
			NewItem.AutoToolTip = false;
			if(!AsMenu) NewItem.ToolTipText = Act.Attr.Tooltip;

			NewItem.SelectedIndexChanged += delegate(object sender, EventArgs e)
			{
				if (((ToolStripComboBox)sender).DropDownStyle == ComboBoxStyle.DropDownList)
				{
					ActContext.Invoke(Act.Name, sender);
					((ToolStripItem)sender).PerformClick();
				}
			};

			NewItem.KeyDown += delegate(object sender, KeyEventArgs e)
			{
				if (e.KeyCode == Keys.Enter)
				{
					ActContext.Invoke(Act.Name, sender);
					((ToolStripItem)sender).PerformClick();
				}
			};

			return NewItem;
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateLabelItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			if (AsMenu) return null;

			ToolStripLabel NewItem = new ToolStripLabel(Act.Attr.Caption + ":");
			NewItem.Tag = Act.Name;
			return NewItem;
		}

	}


}
