using System;
using System.Collections.Generic;
using System.Text;
using System.IO;
using DeadCode.WME.HtmlParser;
using System.Web;


namespace hhc2html
{
	class Program
	{
		static int Main(string[] args)
		{
			if(args.Length < 2)
			{
				Console.WriteLine("hhc2html, (c) 2013 dead:code");
				Console.WriteLine("");
				Console.WriteLine("usage:");
				Console.WriteLine("  hhc2html path_to_hhc path_to_html [base_path]");

				return 1;
			}

			Program P = new Program();
			//if (P.Generate("wme.hhc", @"web\contents.html", "../")) return 0;
			if (P.Generate(args[0], args[1], args.Length > 2 ? "../" : ""))
			{
				Console.WriteLine("File {0} has been generated.", args[1]);
				return 0;
			}
			else return 1;
		}

		//////////////////////////////////////////////////////////////////////////
		private bool Generate(string HHCPath, string HtmlPath, string BasePath)
		{
			if(!File.Exists(HHCPath))
			{
				Console.WriteLine("File {0} doesn't exit.", HHCPath);
				return false;
			}
			try
			{
				Directory.CreateDirectory(Path.GetDirectoryName(HtmlPath));
			}
			catch(Exception e)
			{
				Console.WriteLine("Error creating output directory.");
				Console.WriteLine(e.Message);
				return false;
			}

			try
			{
				HtmlParser Parser = new HtmlParser();
				HtmlElement[] Elements = Parser.ParseHtml(HHCPath);

				Topic RootTopic = ParseHHC(Elements);
				GenerateContentsHtml(RootTopic, HtmlPath, BasePath);
			}
			catch (Exception e)
			{
				Console.WriteLine("Error parsing file {0}.", HHCPath);
				Console.WriteLine(e.Message);
				return false;
			}



			return true;
		}

		//////////////////////////////////////////////////////////////////////////
		private void GenerateContentsHtml(Topic RootTopic, string HtmlPath, string BasePath)
		{
			using(StreamWriter sw = new StreamWriter(HtmlPath, false, Encoding.UTF8))
			{
				sw.WriteLine(@"<html>");
				sw.WriteLine(@"  <head>");
				sw.WriteLine(@"    <META http-equiv='Content-Type' content='text/html; charset=utf-8'>");
				sw.WriteLine(@"    <title>Contents</title>");
				sw.WriteLine(@"    <meta name='GENERATOR' content='hhc2html'>");
				sw.WriteLine(@"    <link rel='stylesheet' type='text/css' href='tree.css'>");
				sw.WriteLine(@"    <script src='tree.js' language='javascript' type='text/javascript'>");
				sw.WriteLine(@"    </script>");
				sw.WriteLine(@"  </head>");
				sw.WriteLine(@"  <body id='docBody' style='background-color: #f1f1f1; color: White; margin: 0px 0px 0px 0px;' onload='resizeTree()' onresize='resizeTree()' onselectstart='return false;'>");
				//sw.WriteLine(@"    <div id='synctoc'><div style='font-family: verdana; font-size: 8pt; cursor: pointer; margin: 6 4 8 2; text-align: right' onmouseover='this.style.textDecoration='underline'' onmouseout='this.style.textDecoration='none'' onclick='syncTree(window.parent.frames[1].document.URL)'>sync toc</div></div>");
				sw.WriteLine(@"    <div id=""synctoc""><div style=""font-family: verdana; font-size: 8pt; cursor: pointer; text-align: right"" onmouseover=""this.style.textDecoration='underline'"" onmouseout=""this.style.textDecoration='none'"" onclick=""syncTree(window.parent.frames[1].document.URL)"">sync toc</div></div>");
				sw.WriteLine(@"    <div id='tree' style='top: 35px; left: 0px;' class='treeDiv'>");
				sw.WriteLine(@"      <div id='treeRoot' onselectstart='return false' ondragstart='return false'>");

				foreach(Topic SubTopic in RootTopic.SubTopics)
				{
					GenerateNode(sw, SubTopic, BasePath, 8);
				}

				sw.WriteLine(@"      </div>");
				sw.WriteLine(@"    </div>");
				sw.WriteLine(@"  </body>");
				sw.WriteLine(@"</html>");
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void GenerateNode(StreamWriter sw, Topic SubTopic, string BasePath, int Nest)
		{
			string N = "";
			for(int i=0; i<Nest; i++) N += " ";

			string Link = SubTopic.Link.Replace('\\', '/');
			if (!Link.StartsWith("http://")) Link = BasePath + Link;

			sw.WriteLine(N + @"<div class='treeNode'>");

			if(SubTopic.SubTopics.Count > 0)
			{
				sw.WriteLine(N + @"  <img src='treenodeplus.gif' class='treeLinkImage' onclick='expandCollapse(this.parentNode)'>");
				sw.WriteLine(N + @"  <a href='" + Link + "' target='main' class='treeUnselected' onclick='clickAnchor(this)'>" + SubTopic.Name + "</a>");

				sw.WriteLine(N + @"  <div class='treeSubnodesHidden'>");
				foreach(Topic SubSubTopic in SubTopic.SubTopics)
				{
					GenerateNode(sw, SubSubTopic, BasePath, Nest + 4);
				}
				sw.WriteLine(N + @"  </div>");
			}
			else
			{
				sw.WriteLine(N + @"  <img src='treenodedot.gif' class='treeNoLinkImage'>");
				sw.WriteLine(N + @"  <a href='" + Link + "' target='main' class='treeUnselected' onclick='clickAnchor(this)'>" + SubTopic.Name + "</a>");
			}

			sw.WriteLine(N + @"</div>");
		}


		//////////////////////////////////////////////////////////////////////////
		private Topic ParseHHC(HtmlElement[] Elements)
		{
			Topic Root = new Topic();
			
			Topic LastTopic = Root;
			Topic Parent = null;

			foreach(HtmlElement El in Elements)
			{
				HtmlTag Tag = El as HtmlTag;
				if (Tag == null) continue;

				// go down one level
				if(Tag.Name.ToUpper() == "UL")
				{
					Parent = LastTopic;
				}

				// go up one level
				if (Tag.Name.ToUpper() == "/UL")
				{
					Parent = Parent.Parent;
					if (Parent == null) break;
				}

				// current item
				if(Tag.Name.ToUpper() == "LI")
				{
					LastTopic = new Topic();
					LastTopic.Parent = Parent;
				}

				if (Tag.Name.ToUpper() == "/OBJECT")
				{
					if (LastTopic.Parent != null && LastTopic.Name != "")
						LastTopic.Parent.SubTopics.Add(LastTopic);
				}

				// topic param
				if(Tag.Name.ToUpper() == "PARAM")
				{
					string Name = Tag.GetParamValue("Name");
					string Value = Tag.GetParamValue("Value");

					switch(Name.ToUpper())
					{
						case "NAME":
							//LastTopic.Name = Value;
							LastTopic.Name = HttpUtility.HtmlDecode(Value);
							break;

						case "LOCAL":
							LastTopic.Link = Value;
							break;

						case "IMAGENUMBER":
							LastTopic.Image = int.Parse(Value);
							break;
					}
				}				
			}
			return Root;
		}

		
		class Topic
		{
			public string Name = "";
			public string Link = "";
			public int Image = -1;

			public Topic Parent = null;
			public List<Topic> SubTopics = new List<Topic>();
		};

	}

}
