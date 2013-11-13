// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace DeadCode.WME.Global
{
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	public class FileManager
	{
		private static List<String> Paths = new List<string>();

		//////////////////////////////////////////////////////////////////////////
		public static String GetFullPath(String PartPath)
		{
			if (File.Exists(PartPath)) return PartPath;
			foreach(String S in Paths)
			{
				String TestPath = S;
				if (!TestPath.EndsWith("\\")) TestPath += "\\";
				TestPath += PartPath;
				if (File.Exists(TestPath)) return TestPath;
			}
			return null;
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool ClearPaths()
		{
			Paths.Clear();
			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		public static bool AddPath(String Path)
		{
			foreach(String S in Paths)
			{
				if (S.ToUpper() == Path.ToUpper()) return true;
			}
			Paths.Add(Path);
			return true;
		}
	}
}
