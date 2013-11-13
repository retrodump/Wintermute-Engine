using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using System.IO;
using DeadCode.WME.DocMaker;


namespace DocMaker2
{
	public partial class FrmMain
	{
		//////////////////////////////////////////////////////////////////////////
		private void WriteHtml(string Filename, ScriptObject Obj)
		{
			using (StreamWriter sw = new StreamWriter(Filename, false, GetEncodingByName(CmbHtmlOutputCodepage.Text)))
			{
				// header
				sw.WriteLine("<html>");
				sw.WriteLine("<meta http-equiv=\"Content-Language\" content=\"" + TxtTermLanguage.Text + "\">");
				sw.WriteLine("<meta name=\"GENERATOR\" content=\"WME DocMaker 2.0\">");
				sw.WriteLine("<meta http-equiv=\"Content-Type\" content=\"text/html; charset=" + GetEncodingByName(CmbHtmlOutputCodepage.Text).WebName + "\">");
				sw.WriteLine("<title>" + Obj.Name + "</title>");
				sw.WriteLine("<link rel=\"stylesheet\" type=\"text/css\" href=\"../../styles.css\">");
				sw.WriteLine("</head>");
				sw.WriteLine();
				sw.WriteLine("<body>");

				sw.WriteLine("<!-- Generated on " + DateTime.Now.ToString() + ", settings file: " + CurrentFilename + " -->");
				sw.WriteLine();


				// title
				sw.WriteLine("<h3>" + Obj.Name + "</h3>");
				sw.WriteLine("<p>");
				sw.WriteLine(Obj.Desc);
				sw.WriteLine("</p>");
				sw.WriteLine();

				sw.WriteLine("<br>");
				sw.WriteLine();


				// methods
				WriteHtmlMethods(sw, Obj, false, true);
				WriteHtmlMethods(sw, Obj, false, false);
				WriteHtmlMethods(sw, Obj, true, false);

				// attributes
				WriteHtmlAttributes(sw, Obj, false);
				WriteHtmlAttributes(sw, Obj, true);

				// events
				WriteHtmlEvents(sw, Obj);


				// methods detailed
				int ConstructorCount = 0;
				foreach (ScriptMethod Method in Obj.Methods)
				{
					if (Method.Constructor) ConstructorCount++;
					string Suffix = "";
					if (ConstructorCount > 0) Suffix = ConstructorCount.ToString();

					WriteHtmlMethodDetailed(sw, Method, Method.Constructor?Suffix:"");
				}

				// footer
				sw.WriteLine();
				sw.WriteLine("</body>");
				sw.WriteLine();
				sw.WriteLine("</html>");
			}

		}

		//////////////////////////////////////////////////////////////////////////
		private void WriteHtmlMethods(StreamWriter sw, ScriptObject Obj, bool Globals, bool Constructors)
		{
			bool FoundType = false;
			foreach (ScriptMethod Method in Obj.Methods)
			{
				if (!Globals && !Constructors && !Method.IsGlobal && !Method.Constructor)
				{
					FoundType = true;
					break;
				}
				if (Globals && Method.IsGlobal)
				{
					FoundType = true;
					break;
				}
				if (Constructors && Method.Constructor)
				{
					FoundType = true;
					break;
				}
			}
			if (!FoundType) return;


			// title
			if (Globals) sw.WriteLine("<h4>" + TxtTermGlobalFunctions.Text + "</h4>");
			else if (Constructors) sw.WriteLine("<h4>" + TxtTermConstructors.Text + "</h4>");
			else sw.WriteLine("<h4>" + TxtTermMethods.Text + "</h4>");
			sw.WriteLine("<table border=\"2\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse: collapse\" bordercolor=\"#111111\">");

			string LastGroup = "";
			int ConstructorCount = 0;
			foreach (ScriptMethod Method in Obj.Methods)
			{
				if (Globals && !Method.IsGlobal) continue;
				if (Constructors && !Method.Constructor) continue;
				if (!Constructors && !Globals && (Method.IsGlobal || Method.Constructor)) continue;

				if (Method.Constructor) ConstructorCount++;

				// group
				if (Method.Group != LastGroup)
				{
					sw.WriteLine("  <tr>");
					sw.WriteLine("    <td nowrap width=\"25%\"><b>" + Method.Group + "</b></td>");
					sw.WriteLine("    <td></td>");
					sw.WriteLine("  </tr>");
					LastGroup = Method.Group;
				}

				// method
				string Suffix = "";
				if (ConstructorCount > 0) Suffix = ConstructorCount.ToString();

				string MethodName = GetBracketName(Method.Headers[0]);
				sw.WriteLine("  <tr>");
				sw.WriteLine("    <td nowrap width=\"25%\"><a href=\"#" + MethodName + Suffix + "\">" + MethodName + "</a></td>");
				sw.WriteLine("    <td>" + Method.Desc + "</td>");
				sw.WriteLine("  </tr>");
			}

			sw.WriteLine("</table>");
			sw.WriteLine("");
		}

		//////////////////////////////////////////////////////////////////////////
		private void WriteHtmlAttributes(StreamWriter sw, ScriptObject Obj, bool Globals)
		{
			bool FoundType = false;
			foreach (ScriptAttribute Attr in Obj.Attributes)
			{
				if (Globals == Attr.IsGlobal)
				{
					FoundType = true;
					break;
				}
			}
			if (!FoundType) return;

			if (Globals)
				sw.WriteLine("<h4>" + TxtTermGlobalVariables.Text + "</h4>");
			else
				sw.WriteLine("<h4>" + TxtTermAttributes.Text + "</h4>");
			sw.WriteLine("<table border=\"2\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse: collapse\" bordercolor=\"#111111\">");

			string LastGroup = "";
			foreach (ScriptAttribute Attr in Obj.Attributes)
			{
				if (Attr.IsGlobal != Globals) continue;

				// group
				if (Attr.Group != LastGroup)
				{
					sw.WriteLine("  <tr>");
					sw.WriteLine("    <td nowrap width=\"25%\"><b>" + Attr.Group + "</b></td>");
					sw.WriteLine("    <td></td>");
					sw.WriteLine("  </tr>");
					LastGroup = Attr.Group;
				}

				// attribute
				sw.WriteLine("  <tr>");
				sw.WriteLine("    <td nowrap width=\"25%\">" + Attr.Name + (Attr.ReadOnly ? " (" + TxtTermReadOnly.Text + ")" : "") + "</td>");
				sw.WriteLine("    <td>" + Attr.Desc + "</td>");
				sw.WriteLine("  </tr>");
			}
			sw.WriteLine("</table>");
			sw.WriteLine();
		}

		//////////////////////////////////////////////////////////////////////////
		private void WriteHtmlEvents(StreamWriter sw, ScriptObject Obj)
		{
			if (Obj.Events.Count > 0)
			{
				sw.WriteLine("<h4>" + TxtTermEvents.Text + "</h4>");
				sw.WriteLine("<table border=\"2\" cellspacing=\"0\" cellpadding=\"2\" style=\"border-collapse: collapse\" bordercolor=\"#111111\">");

				string LastGroup = "";
				foreach (ScriptEvent Event in Obj.Events)
				{
					// group
					if (Event.Group != LastGroup)
					{
						sw.WriteLine("  <tr>");
						sw.WriteLine("    <td nowrap width=\"25%\"><b>" + Event.Group + "</b></td>");
						sw.WriteLine("    <td></td>");
						sw.WriteLine("  </tr>");
						LastGroup = Event.Group;
					}

					// event
					sw.WriteLine("  <tr>");
					sw.WriteLine("    <td nowrap width=\"25%\">" + Event.Name + "</td>");
					sw.WriteLine("    <td>" + Event.Desc + "</td>");
					sw.WriteLine("  </tr>");
				}
				sw.WriteLine("</table>");
				sw.WriteLine();
			}
		}

		//////////////////////////////////////////////////////////////////////////
		private void WriteHtmlMethodDetailed(StreamWriter sw, ScriptMethod Method, string Suffix)
		{
			string MethodName = GetBracketName(Method.Headers[0]);

			sw.WriteLine("<hr>");

			// headers
			sw.Write("<h3>");
			for (int i = 0; i < Method.Headers.Count; i++)
			{
				if (i == 0) sw.Write("<a name=\"" + MethodName + Suffix + "\"></a>");
				else sw.Write("<br>");
				sw.Write(Method.Headers[i]);
			}
			sw.WriteLine("</h3>");

			// description
			sw.WriteLine("<p>" + Method.Desc + "</p>");

			// params
			if (Method.Parameters.Count > 0)
			{
				sw.WriteLine("<h4>" + TxtTermParameters.Text + "</h4>");
				sw.WriteLine("<dl>");
				foreach (ScriptParameter Param in Method.Parameters)
				{
					sw.WriteLine("<dt><i>" + Param.Name + "</i></dt>");
					sw.WriteLine("<dd>" + Param.Desc + "</dd>");
				}

				sw.WriteLine("</dl>");
			}

			// return
			if (Method.Return != "")
			{
				sw.WriteLine("<h4>" + TxtTermReturn.Text + "</h4>");
				sw.WriteLine("<p>" + Method.Return + "</p>");
			}

			// remarks
			if (Method.Remark != "")
			{
				sw.WriteLine("<h4>" + TxtTermRemarks.Text + "</h4>");
				sw.WriteLine("<p>" + Method.Remark + "</p>");
			}

			// examples
			if (Method.Examples.Count > 0)
			{
				int i = 0;
				foreach (string Example in Method.Examples)
				{
					i++;
					sw.Write("<h4>" + TxtTermExample.Text);
					if (Method.Examples.Count > 1)
					{
						sw.Write(" " + i.ToString());
					}
					sw.WriteLine("</h4>");

					sw.WriteLine("<table border=\"0\" cellpadding=\"0\" cellspacing=\"0\" width=\"100%\">");
					sw.WriteLine("<tr>");
					sw.WriteLine("<td class=\"code\"><pre>");
					sw.WriteLine(Example);
					sw.WriteLine("</pre></td>");
					sw.WriteLine("</tr>");
					sw.WriteLine("</table>");
				}
			}
			sw.WriteLine();
		}

		//////////////////////////////////////////////////////////////////////////
		private string GetBracketName(string Line)
		{
			string Token = "";
			for (int i = 0; i < Line.Length; i++)
			{
				if (Line[i] == ' ' || Line[i] == '\t' || Line[i] == '(') break;
				else Token += Line[i];
			}
			return Token;
		}
	}
}
