// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using Microsoft.Win32;
using System.IO;

namespace DeadCode.WME.Global
{
	public static class WmeUtils
	{
		//////////////////////////////////////////////////////////////////////////
		public static string ToolsPath
		{
			get
			{
				string Ret = "";
				try
				{
					using (RegistryKey Key = Registry.CurrentUser.OpenSubKey(@"Software\DEAD:CODE\Wintermute Tools\Settings"))
					{
						Ret = Key.GetValue("ToolsPath", "").ToString();
					};
				}
				catch
				{
					Ret = "";
				}
				return Ret;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static string XmlDocsPath
		{
			get
			{
				return Path.Combine(ToolsPath, "wme_docs_core_en.xml");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static string RuntimePath
		{
			get
			{
				return Path.Combine(ToolsPath, "wme.exe");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static string CompilerPath
		{
			get
			{
				return Path.Combine(ToolsPath, "wme_comp.exe");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool MakeFileAssociation(string extension, string progId, string description, string executeable, string iconFile, int iconIdx)
		{
			try
			{
				if (extension.Length != 0)
				{
					if (extension[0] != '.')
					{
						extension = "." + extension;
					}

					using (RegistryKey extKey = Registry.ClassesRoot.CreateSubKey(extension))
					{
						extKey.SetValue(string.Empty, progId);
					}

					// register the progId, if necessary
					using (RegistryKey progIdKey = Registry.ClassesRoot.CreateSubKey(progId))
					{
						progIdKey.SetValue(string.Empty, description);
						progIdKey.SetValue("DefaultIcon", String.Format("\"{0}\",{1}", iconFile, iconIdx));

						using (RegistryKey command = progIdKey.CreateSubKey("shell\\open\\command"))
						{
							command.SetValue(string.Empty, String.Format("\"{0}\" \"%1\"", executeable));
						}

						using (RegistryKey command = progIdKey.CreateSubKey("shell\\edit\\command"))
						{
							command.SetValue(string.Empty, String.Format("\"{0}\" \"%1\"", executeable));
						}
					}

					// delete custom explorer assoctiation if any
					try
					{
						using (RegistryKey explorerKey = Registry.CurrentUser.OpenSubKey(@"Software\Microsoft\Windows\CurrentVersion\Explorer\FileExts\" + extension, true))
						{
							explorerKey.DeleteValue("Application", false);
						}
					}
					catch
					{
					}

					return true;
				}
				return false;
			}
			catch
			{
				return false;
			}
		}

		//////////////////////////////////////////////////////////////////////////
		public static string GetRelativePath(string BasePath, string FullPath)
		{
			string RelativePath = "";

			string[] BaseArray = Path.GetDirectoryName(BasePath).Split(new char[] { '/', '\\' });
			string[] FullArray = Path.GetDirectoryName(FullPath).Split(new char[] { '/', '\\' });

			int i;
			int NumCommon = 0;
			for (i = 0; i < Math.Min(BaseArray.Length, FullArray.Length); i++)
			{
				if (BaseArray[i] == FullArray[i]) NumCommon++;
				else break;
			}
			if (NumCommon == 0) return FullPath;

			int NumExtra = BaseArray.Length - NumCommon;
			for (i = 0; i < NumExtra; i++) RelativePath += "..\\";

			for (i = NumCommon; i < FullArray.Length; i++)
			{
				RelativePath += FullArray[i] + "\\";
			}
			RelativePath += Path.GetFileName(FullPath);

			return RelativePath;
		}

		//////////////////////////////////////////////////////////////////////////
		// code by Joe Woodbury (http://www.codeproject.com/csharp/MruToolStripMenu.asp)
		static public string ShortenPathname(string pathname, int maxLength)
		{
			if (pathname.Length <= maxLength)
				return pathname;

			string root = Path.GetPathRoot(pathname);
			if (root.Length > 3)
				root += Path.DirectorySeparatorChar;

			string[] elements = pathname.Substring(root.Length).Split(Path.DirectorySeparatorChar, Path.AltDirectorySeparatorChar);

			int filenameIndex = elements.GetLength(0) - 1;

			if (elements.GetLength(0) == 1) // pathname is just a root and filename
			{
				if (elements[0].Length > 5) // long enough to shorten
				{
					// if path is a UNC path, root may be rather long
					if (root.Length + 6 >= maxLength)
					{
						return root + elements[0].Substring(0, 3) + "...";
					}
					else
					{
						return pathname.Substring(0, maxLength - 3) + "...";
					}
				}
			}
			else if ((root.Length + 4 + elements[filenameIndex].Length) > maxLength) // pathname is just a root and filename
			{
				root += "...\\";

				int len = elements[filenameIndex].Length;
				if (len < 6)
					return root + elements[filenameIndex];

				if ((root.Length + 6) >= maxLength)
				{
					len = 3;
				}
				else
				{
					len = maxLength - root.Length - 3;
				}
				return root + elements[filenameIndex].Substring(0, len) + "...";
			}
			else if (elements.GetLength(0) == 2)
			{
				return root + "...\\" + elements[1];
			}
			else
			{
				int len = 0;
				int begin = 0;

				for (int i = 0; i < filenameIndex; i++)
				{
					if (elements[i].Length > len)
					{
						begin = i;
						len = elements[i].Length;
					}
				}

				int totalLength = pathname.Length - len + 3;
				int end = begin + 1;

				while (totalLength > maxLength)
				{
					if (begin > 0)
						totalLength -= elements[--begin].Length - 1;

					if (totalLength <= maxLength)
						break;

					if (end < filenameIndex)
						totalLength -= elements[++end].Length - 1;

					if (begin == 0 && end == filenameIndex)
						break;
				}

				// assemble final string

				for (int i = 0; i < begin; i++)
				{
					root += elements[i] + '\\';
				}

				root += "...\\";

				for (int i = end; i < filenameIndex; i++)
				{
					root += elements[i] + '\\';
				}

				return root + elements[filenameIndex];
			}
			return pathname;
		}

	}
}
