using System;
using System.Collections.Generic;
using System.Text;

namespace DeadCode.WME.SpriteOptimizer
{
	public interface ILogConsumer
	{
		void AddLog(string text);
		void ClearLog();
	}
}
