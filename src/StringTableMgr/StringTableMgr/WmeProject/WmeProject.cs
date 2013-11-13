using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using DeadCode.WME.Global;

namespace DeadCode.WME.StringTableMgr
{
	//////////////////////////////////////////////////////////////////////////
	public class ProjectPackage
	{
		public readonly string FullPath;
		public readonly string Name;
		public readonly string Description;
		public readonly int Priority;

		public ProjectPackage(string FullPath, string Name, string Description, int Priority)
		{
			this.FullPath = FullPath;
			this.Name = Name;
			this.Description = Description;
			this.Priority = Priority;
		}
	};


	//////////////////////////////////////////////////////////////////////////
	public class WmeProject
	{
		public ProjectPackage[] Packages;

		//////////////////////////////////////////////////////////////////////////
		public WmeProject(string ProjectFilename)
		{
			List<ProjectPackage> Packages = new List<ProjectPackage>();

			if (ProjectFilename != string.Empty)
			{
				// get the project package directories and scan them
				IniFile ProjectFile = new IniFile(ProjectFilename);
				int NumPackages = ProjectFile.ReadInt("General", "NumPackages", 0);

				string BasePath = Path.GetDirectoryName(ProjectFilename) + Path.DirectorySeparatorChar;
				
				for (int i = 1; i <= NumPackages; i++)
				{
					string Name = ProjectFile.ReadString("Package" + i.ToString(), "Folder");
					if (Name == string.Empty) continue;

					string Description = ProjectFile.ReadString("Package" + i.ToString(), "Description");

					int Priority = ProjectFile.ReadInt("Package" + i.ToString(), "Priority");

					Packages.Add(new ProjectPackage(BasePath + Name, Name, Description, Priority));
				}
				// sort packages by priority ascending
				Packages.Sort(ComparePriorityAscending);				
			}
			this.Packages = Packages.ToArray();
		}

		//////////////////////////////////////////////////////////////////////////
		public bool ContainsFile(string Filename)
		{
			foreach(ProjectPackage Package in Packages)
			{
				string FullFilename = Package.FullPath + Path.DirectorySeparatorChar + Filename;
				if (File.Exists(FullFilename)) return true;
			}
			return false;
		}


		//////////////////////////////////////////////////////////////////////////
		private static int ComparePriorityAscending(ProjectPackage p1, ProjectPackage p2)
		{
			if (p1 == null)
			{
				if (p2 == null) return 0;
				else return -1;
			}
			else
			{
				if (p2 == null) return 1;
				else
				{
					if (p1.Priority < p2.Priority) return -1;
					else if (p1.Priority > p2.Priority) return 1;
					else return 0;
				}
			}
		}

	}
}
