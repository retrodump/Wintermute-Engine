// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

// Delarations of native Win32 functions/structs/constants
// Taken from DX SDK (modified)

namespace DeadCode.WME.Global
{
	public class NativeMethods
	{
		#region Win32 User Messages / Structures

		/// <summary>Window messages</summary>
		public enum WindowMessage : uint
		{
			// Misc messages
			Destroy = 0x0002,
			Close = 0x0010,
			Quit = 0x0012,
			Paint = 0x000F,
			SetCursor = 0x0020,
			Activate = 0x0006,
			ActivateApplication = 0x001C,
			NonClientActivate = 0x0086,
			EnterMenuLoop = 0x0211,
			ExitMenuLoop = 0x0212,
			NonClientHitTest = 0x0084,
			PowerBroadcast = 0x0218,
			SystemCommand = 0x0112,
			GetMinMax = 0x0024,

			// Keyboard messages
			KeyDown = 0x0100,
			KeyUp = 0x0101,
			Character = 0x0102,
			SystemKeyDown = 0x0104,
			SystemKeyUp = 0x0105,
			SystemCharacter = 0x0106,

			// Mouse messages
			MouseMove = 0x0200,
			LeftButtonDown = 0x0201,
			LeftButtonUp = 0x0202,
			LeftButtonDoubleClick = 0x0203,
			RightButtonDown = 0x0204,
			RightButtonUp = 0x0205,
			RightButtonDoubleClick = 0x0206,
			MiddleButtonDown = 0x0207,
			MiddleButtonUp = 0x0208,
			MiddleButtonDoubleClick = 0x0209,
			NcLeftButtonDown = 0x0A1,
			NcRightButtonDown = 0x0A4,
			NcMiddleButtonDown = 0x0A7,
			MouseWheel = 0x020a,
			XButtonDown = 0x020B,
			XButtonUp = 0x020c,
			XButtonDoubleClick = 0x020d,
			MouseFirst = LeftButtonDown, // Skip mouse move, it happens a lot and there is another message for that
			MouseLast = XButtonDoubleClick,

			// Sizing
			EnterSizeMove = 0x0231,
			ExitSizeMove = 0x0232,
			Size = 0x0005,
		}

		/// <summary>Mouse buttons</summary>
		public enum MouseButtons
		{
			Left = 0x0001,
			Right = 0x0002,
			Middle = 0x0010,
			Side1 = 0x0020,
			Side2 = 0x0040,
		}

		/// <summary>Keyboard events</summary>
		public enum KeyboardEvent
		{
			KeyUp = 0x0002, //KEYEVENTF_KEYUP
		}

		/// <summary>Extended window styles</summary>
		[Flags]
		public enum WindowStyleEx
		{
			ToolWindow = 0x00000080,
			NoActivate = 0x08000000,
			TopMost = 0x00000008,	
		}

		/// <summary>Windows Message</summary>
		[StructLayout(LayoutKind.Sequential)]
		public struct Message
		{
			public IntPtr hWnd;
			public WindowMessage msg;
			public IntPtr wParam;
			public IntPtr lParam;
			public uint time;
			public System.Drawing.Point p;
		}

		/// <summary>MinMax Info structure</summary>
		[StructLayout(LayoutKind.Sequential)]
		public struct MinMaxInformation
		{
			public System.Drawing.Point reserved;
			public System.Drawing.Point MaxSize;
			public System.Drawing.Point MaxPosition;
			public System.Drawing.Point MinTrackSize;
			public System.Drawing.Point MaxTrackSize;
		}

		/// <summary>Monitor Info structure</summary>
		[StructLayout(LayoutKind.Sequential)]
		public struct MonitorInformation
		{
			public uint Size; // Size of this structure
			public System.Drawing.Rectangle MonitorRectangle;
			public System.Drawing.Rectangle WorkRectangle;
			public uint Flags; // Possible flags
		}

		[StructLayout(LayoutKind.Sequential)]
		public struct PeekMsg
		{
			public IntPtr hWnd;
			public Message msg;
			public IntPtr wParam;
			public IntPtr lParam;
			public uint time;
			public System.Drawing.Point p;
		}
		#endregion

		#region Windows API calls
		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[System.Runtime.InteropServices.DllImport("winmm.dll")]
		public static extern IntPtr timeBeginPeriod(uint period);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("kernel32")]
		public static extern bool QueryPerformanceFrequency(ref long PerformanceFrequency);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("kernel32")]
		public static extern bool QueryPerformanceCounter(ref long PerformanceCount);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern bool GetMonitorInfo(IntPtr hWnd, ref MonitorInformation info);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern IntPtr MonitorFromWindow(IntPtr hWnd, uint flags);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern short GetAsyncKeyState(uint key);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern IntPtr SetCapture(IntPtr handle);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern bool ReleaseCapture();

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern int GetCaretBlinkTime();

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("User32.dll", CharSet = CharSet.Auto)]
		public static extern bool PeekMessage(out Message msg, IntPtr hWnd, uint messageFilterMin, uint messageFilterMax, uint flags);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32")]
		public static extern int SetParent(IntPtr hWndChild, IntPtr hWndNewParent);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32")]
		public static extern IntPtr GetParent(IntPtr hWndChild);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32")]
		public static extern int ShowWindow(IntPtr hWnd, int nCmdShow);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32", CharSet = CharSet.Auto)]
		public extern static int SendMessage(IntPtr handle, int msg, int wParam, IntPtr lParam);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32", CharSet = CharSet.Auto)]
		public extern static int PostMessage(IntPtr handle, int msg, int wParam, IntPtr lParam);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32")]
		public extern static void keybd_event(byte bVk, byte bScan, int dwFlags, int dwExtraInfo);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32.dll", SetLastError = true)]
		public static extern IntPtr FindWindowEx(IntPtr parentHandle, IntPtr childAfter, string className, string windowTitle);

		[System.Security.SuppressUnmanagedCodeSecurity] // We won't use this maliciously
		[DllImport("user32.dll", CharSet = CharSet.Auto)]
		public static extern bool PeekMessage(out PeekMsg msg, IntPtr hWnd, uint messageFilterMin, uint messageFilterMax, uint flags);

		#endregion

		#region Class Methods
		private NativeMethods() { } // No creation
		/// <summary>Returns the low word</summary>
		public static short LoWord(uint l)
		{
			return (short)(l & 0xffff);
		}
		/// <summary>Returns the high word</summary>
		public static short HiWord(uint l)
		{
			return (short)(l >> 16);
		}

		/// <summary>Makes two shorts into a long</summary>
		public static uint MakeUInt32(short l, short r)
		{
			return (uint)((l & 0xffff) | ((r & 0xffff) << 16));
		}

		/// <summary>Is this key down right now</summary>
		public static bool IsKeyDown(System.Windows.Forms.Keys key)
		{
			return (GetAsyncKeyState((int)System.Windows.Forms.Keys.ShiftKey) & 0x8000) != 0;
		}
		#endregion
	}
}
