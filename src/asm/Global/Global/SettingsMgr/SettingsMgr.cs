// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;
using System.Reflection;
using System.IO;

namespace DeadCode.WME.Global
{
	public class SettingsMgr : SettingsNode
	{
		//////////////////////////////////////////////////////////////////////////
		public SettingsMgr(string RootName)
		{
			this.Name = RootName;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool LoadFromXmlFile()
		{
			return LoadFromXmlFile(GetSettingsFilePath());
		}

		//////////////////////////////////////////////////////////////////////////
		public bool LoadFromXmlFile(string Filename)
		{
			try
			{
				XmlDocument Doc = new XmlDocument();
				Doc.Load(Filename);

				XmlNode Root = Doc.DocumentElement;
				if (Root == null) return false;
				else
				{
					Children.Clear();
					return LoadFromXmlNode(Root);
				}
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public bool SaveToXmlFile()
		{
			return SaveToXmlFile(GetSettingsFilePath());
		}

		//////////////////////////////////////////////////////////////////////////
		public bool SaveToXmlFile(string Filename)
		{
			try
			{
				XmlDocument Doc = new XmlDocument();

				// header
				XmlDeclaration Decl = Doc.CreateXmlDeclaration("1.0", "utf-8", null);

				Assembly A = Assembly.GetExecutingAssembly();
				XmlComment Comment1 = Doc.CreateComment("Generated by: " + A.GetName());
				XmlComment Comment2 = Doc.CreateComment("Generated on: " + DateTime.Now.ToString());

				// root
				XmlNode RootNode = SaveToXmlNode(Doc);
				Doc.InsertBefore(Decl, Doc.DocumentElement);
				Doc.AppendChild(Comment1);
				Doc.AppendChild(Comment2);
				Doc.AppendChild(RootNode);

				// save to file
				XmlTextWriter Writer = new XmlTextWriter(Filename, null);
				Writer.Formatting = Formatting.Indented;
				Doc.Save(Writer);
				Writer.Close();

				return true;
			}
			catch
			{
				return false;
			}
		}


		//////////////////////////////////////////////////////////////////////////
		private string GetSettingsFilePath()
		{
			string SettingsPath = Path.Combine(Environment.GetFolderPath(Environment.SpecialFolder.ApplicationData), "Wintermute Engine");
			SettingsPath = Path.Combine(SettingsPath, this.Name);			
			Directory.CreateDirectory(SettingsPath);

			return Path.Combine(SettingsPath, "LastSettings.xml");
		}

	}
}