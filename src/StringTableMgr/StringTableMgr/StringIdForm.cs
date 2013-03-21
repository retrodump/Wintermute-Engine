using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace DeadCode.WME.StringTableMgr
{
	public partial class StringIdForm : Form
	{
		public StringTableMgr Mgr = null;
		public string StringID = "";
		public int LastNum = -1;

		private StringTableMgr.StringID[] IdList = null;

		//////////////////////////////////////////////////////////////////////////
		public StringIdForm()
		{
			InitializeComponent();
		}

		//////////////////////////////////////////////////////////////////////////
		private void SetState()
		{
			BtnOK.Enabled = TxtStringID.Text != string.Empty;
		}


		//////////////////////////////////////////////////////////////////////////
		private void OnLoad(object sender, EventArgs e)
		{
			ListRecentIDs.Items.Clear();
			TxtStringID.AutoCompleteCustomSource.Clear();

			if (Mgr != null)
			{
				IdList = Mgr.GetUsedIDs();
				foreach(StringTableMgr.StringID ID in IdList)
				{
					ListRecentIDs.Items.Add(ID.ID);
					TxtStringID.AutoCompleteCustomSource.Add(ID.ID);
				}
			}

			TxtStringID.Text = StringID;
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnTextChanged(object sender, EventArgs e)
		{
			SetState();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnSelectedItem(object sender, EventArgs e)
		{
			if (ListRecentIDs.SelectedItem == null) return;
			TxtStringID.Text = ListRecentIDs.SelectedItem.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnItemDblClick(object sender, EventArgs e)
		{
			TxtStringID.Focus();
		}

		//////////////////////////////////////////////////////////////////////////
		private void OnOK(object sender, EventArgs e)
		{
			StringID = TxtStringID.Text;
			LastNum = -1;

			if (IdList != null)
			{
				foreach(StringTableMgr.StringID ID in IdList)
				{
					if(ID.ID.ToLower()==StringID.ToLower())
					{
						LastNum = ID.MaxNum;
						break;
					}
				}
			}

			this.DialogResult = DialogResult.OK;
			this.Close();
		}


	}
}