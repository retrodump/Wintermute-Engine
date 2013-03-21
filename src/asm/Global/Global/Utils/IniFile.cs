// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace DeadCode.WME.Global
{
	public class IniFile
	{
		private string IniFilename;

		[DllImport("kernel32")]
        private static extern long WritePrivateProfileString(string section, string key, string val, string filePath);

		[DllImport("kernel32")]
        private static extern int GetPrivateProfileString(string section, string key, string defaut, StringBuilder retVal, int size, string filePath);

		[DllImport("kernel32")]
		private static extern int GetPrivateProfileInt(string section, string key, int defaut, string filePath);
		
		//////////////////////////////////////////////////////////////////////////
		public IniFile(string Filename)
		{
			IniFilename = Filename;
		}

		//////////////////////////////////////////////////////////////////////////
		public bool WriteString(string Section, string Key, string Value)
		{
			return WritePrivateProfileString(Section, Key, Value, IniFilename) != 0;
		}

		//////////////////////////////////////////////////////////////////////////
		public string ReadString(string Section, string Key)
		{
			return ReadString(Section, Key, "");
		}

		//////////////////////////////////////////////////////////////////////////
		public string ReadString(string Section, string Key, string DefaultVal)
		{
			const int BufferSize = 32768;

			StringBuilder Buffer = new StringBuilder(BufferSize);
			int i = GetPrivateProfileString(Section, Key, DefaultVal, Buffer, BufferSize, IniFilename);
			return Buffer.ToString();
		}

		//////////////////////////////////////////////////////////////////////////
		public int ReadInt(string Section, string Key)
		{
			return ReadInt(Section, Key, 0);
		}

		//////////////////////////////////////////////////////////////////////////
		public int ReadInt(string Section, string Key, int DefaultVal)
		{
			return GetPrivateProfileInt(Section, Key, DefaultVal, IniFilename);
		}

	}
}
