using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.Controls.Actions
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
					FileMenu.AddItem("Test.Action2");

			return Root;
		}

		//////////////////////////////////////////////////////////////////////////
		public void PopulateToolStrip(ActionStripItem Root, ToolStripItemCollection Items, bool AsMenu)
		{
			Items.Clear();
			AddStripItems(Root, Items, AsMenu);
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetToolStripState(ToolStripItemCollection Items)
		{
			foreach(ToolStripItem Item in Items)
			{
				string ActName = Item.Tag as string;
				if (ActName != null)
				{
					Action.State St = ActContext.GetState(ActName);

					Item.Enabled = ((St & Action.State.Disabled) != Action.State.Disabled);
					Item.Visible = ((St & Action.State.Hidden) != Action.State.Hidden);

					if (Item is ToolStripButton)
						((ToolStripButton)Item).Checked = ((St & Action.State.Checked) == Action.State.Checked);
					else if (Item is ToolStripMenuItem)
						((ToolStripMenuItem)Item).Checked = ((St & Action.State.Checked) == Action.State.Checked);


				}

				if(Item is ToolStripDropDownItem)
				{
					SetToolStripState(((ToolStripDropDownItem)Item).DropDownItems);
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void AddStripItems(ActionStripItem Root, ToolStripItemCollection Items, bool AsMenu)
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
						AddStripItems(Item, Submenu.DropDownItems, true);

						Items.Add(Submenu);
					}
					else
					{
						// TODO: DropDown / SplitButton
						if(Item.Caption==String.Empty)
						{
							ToolStripSplitButton Btn = (ToolStripSplitButton)CreateButtonItem(Item.Items[0], ActionManager.GetAction(Item.Items[0].ActionName), AsMenu, true);
							AddStripItems(Item, Btn.DropDownItems, true);
							Items.Add(Btn);
						}
						else
						{
							ToolStripDropDownButton Btn = new ToolStripDropDownButton(Item.Caption);
							AddStripItems(Item, Btn.DropDownItems, true);
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
					Action Act = ActionManager.GetAction(Item.ActionName);
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
							if(Act.Attr.DisplayLabel)
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
					}
					if (NewItem != null)
					{
						NewItem.Tag = Act.Name;
						Items.Add(NewItem);
					}
				}
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
				if (Act.Icon != null) NewItem.Image = Act.Icon;
				NewItem.AutoToolTip = false;

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
				
				if (Act.Icon != null) NewItem.Image = Act.Icon;
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
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateComboItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		private ToolStripItem CreateLabelItem(ActionStripItem Item, Action Act, bool AsMenu)
		{
			if (AsMenu) return null;

			ToolStripLabel NewItem = new ToolStripLabel(Act.Attr.Caption + ":");
			return NewItem;
		}

	}


}
