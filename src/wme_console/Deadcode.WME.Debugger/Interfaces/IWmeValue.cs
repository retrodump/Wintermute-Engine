// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;


namespace DeadCode.WME.Debugger
{
	public enum EWmeValue { Int, Float, Bool, String, Native, Object, Unknown };

	public interface IWmeValue
	{
		IntPtr NativeID { get; }

		string Name { get; }
		EWmeValue Type { get; }		
		
		int GetInt();
		double GetFloat();
		bool GetBool();
		string GetString();
		IWmeObject GetNative();

		string GetDebuggerDesc();

		bool SetValue(int Value);
		bool SetValue(double Value);
		bool SetValue(bool Value);
		bool SetValue(string Value);
		bool SetValue();

		ICollection<IWmeValue> Properties { get; }
	}


	//////////////////////////////////////////////////////////////////////////
	public class IWmeValueComparer : IEqualityComparer<IWmeValue>
	{
		public bool Equals(IWmeValue val1, IWmeValue val2)
		{
			if (val1 != null && val2 != null && val1.NativeID == val2.NativeID) return true;
			else return false;
		}

		public int GetHashCode(IWmeValue val)
		{
			return val.NativeID.GetHashCode();
		}
	};
}
