// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Drawing;

namespace DeadCode.WME.Global
{
	public class SettingsNode
	{
		public string Name;
		private string Value = "";

		//////////////////////////////////////////////////////////////////////////
		protected SettingsNode()
		{
			this.Name = "";
		}

		//////////////////////////////////////////////////////////////////////////
		public SettingsNode(string Name)
		{
			this.Name = Name;
		}

		//////////////////////////////////////////////////////////////////////////
		public SettingsNode(string Name, string Value)
		{
			this.Name = Name;
			this.Value = Value;
		}

		//////////////////////////////////////////////////////////////////////////
		public override string ToString()
		{
			if (Children.Count > 0) return Name + "[" + Children.Count.ToString() + "]";
			else return Name + " = " + GetString();
		}

		//////////////////////////////////////////////////////////////////////////
		public void Clear()
		{
			Value = "";
			Children.Clear();
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetValue(object Val)
		{
			Value = Val.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetValue(string Path, object Val)
		{
			SettingsNode Node = GetNode(Path, false, true);
			if (Node != null) Node.SetValue(Val);
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetValue(Color Val)
		{
			Value = Val.R.ToString() + "," + Val.G.ToString() + "," + Val.B.ToString() + "," + Val.A.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		public void SetValue(string Path, Color Val)
		{
			SettingsNode Node = GetNode(Path, false, true);
			if (Node != null) Node.SetValue(Val);
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetString()
		{
			return Value;
		}

		//////////////////////////////////////////////////////////////////////////
		public string GetString(string Path, string InitValue)
		{
			SettingsNode Node = GetNode(Path, false, false);
			if (Node != null) return Node.GetString();
			else return InitValue;
		}
		
		//////////////////////////////////////////////////////////////////////////
		public string GetString(string Path)
		{
			return GetString(Path, "");
		}
		

		//////////////////////////////////////////////////////////////////////////
		public int GetInt()
		{
			int Ret;
			if (int.TryParse(Value, out Ret)) return Ret;
			else return 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public int GetInt(string Path, int InitValue)
		{
			SettingsNode Node = GetNode(Path, false, false);
			if (Node != null) return Node.GetInt();
			else return InitValue;
		}

		//////////////////////////////////////////////////////////////////////////
		public int GetInt(string Path)
		{
			return GetInt(Path, 0);
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GetBool()
		{
			try
			{
				return Convert.ToBoolean(Value);
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GetBool(string Path, bool InitValue)
		{
			SettingsNode Node = GetNode(Path, false, false);
			if (Node != null) return Node.GetBool();
			else return InitValue;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool GetBool(string Path)
		{
			return GetBool(Path, false);
		}

		//////////////////////////////////////////////////////////////////////////
		public Color GetColor()
		{
			try
			{
				string[] RGBA = Value.Split(new char[] { ',' });
				int R = 0, G = 0, B = 0, A = 0;
				if (RGBA.Length > 0) R = int.Parse(RGBA[0]);
				if (RGBA.Length > 1) G = int.Parse(RGBA[1]);
				if (RGBA.Length > 2) B = int.Parse(RGBA[2]);
				if (RGBA.Length > 3) A = int.Parse(RGBA[3]);

				return Color.FromArgb(A, R, G, B);
			}
			catch
			{
				return Color.Black;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public Color GetColor(string Path, Color InitValue)
		{
			SettingsNode Node = GetNode(Path, false, false);
			if (Node != null) return Node.GetColor();
			else return InitValue;
		}

		//////////////////////////////////////////////////////////////////////////
		public Color GetColor(string Path)
		{
			return GetColor(Path, Color.Black);
		}

		//////////////////////////////////////////////////////////////////////////
		private List<SettingsNode> _Children;		
		public List<SettingsNode> Children
		{
			get
			{
				if (_Children == null) _Children = new List<SettingsNode>();
				return _Children;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool LoadFromXmlNode(XmlNode Node)
		{
			try
			{
				this.Name = Node.Name;

				XmlNode ChildNode = Node.FirstChild;
				while (ChildNode != null)
				{
					if (ChildNode is XmlText)
					{
						SetValue(ChildNode.Value);
					}
					else if (ChildNode is XmlElement)
					{
						SettingsNode SetNode = new SettingsNode();
						if (SetNode.LoadFromXmlNode(ChildNode)) Children.Add(SetNode);
					}
					ChildNode = ChildNode.NextSibling;
				}
				return true;
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		protected XmlNode SaveToXmlNode(XmlDocument Doc)
		{
			XmlNode Node = Doc.CreateElement(Name);
			if (Children.Count > 0)
			{
				foreach(SettingsNode SetNode in Children)
				{
					XmlNode ChildNode = SetNode.SaveToXmlNode(Doc);
					if (ChildNode != null) Node.AppendChild(ChildNode);
				}
			}
			else
			{
				Node.AppendChild(Doc.CreateTextNode(GetString()));
			}
			return Node;
		}

		//////////////////////////////////////////////////////////////////////////
		protected SettingsNode GetChildByName(string Name, bool CaseSensitive)
		{
			foreach(SettingsNode Node in Children)
			{
				if (string.Compare(Node.Name, Name, !CaseSensitive)==0) return Node;
			}
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		protected SettingsNode GetChildByName(string Name)
		{
			return GetChildByName(Name, false);
		}



		//////////////////////////////////////////////////////////////////////////
		public SettingsNode GetNode(string Path, bool CaseSensitive, bool CreateIfDoesntExist)
		{
			string[] PathSplit = Path.Split(new char[] { '\\', '/' });

			SettingsNode Child = this;
			for(int i=0; i<PathSplit.Length; i++)
			{
				string Name = PathSplit[i];
				SettingsNode NewChild = Child.GetChildByName(Name, CaseSensitive);

				if (NewChild == null && CreateIfDoesntExist)
				{
					NewChild = new SettingsNode(Name);
					Child.Children.Add(NewChild);
				}
				Child = NewChild;

				if (Child == null) break;
			}
			return Child;
		}

		//////////////////////////////////////////////////////////////////////////
		public SettingsNode GetNode(string Path, bool CaseSensitive)
		{
			return GetNode(Path, CaseSensitive, false);
		}

		//////////////////////////////////////////////////////////////////////////
		public SettingsNode GetNode(string Path)
		{
			return GetNode(Path, false);
		}
	}
}
