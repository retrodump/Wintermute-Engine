// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

#pragma once

namespace DeadCode { namespace WME { namespace Core
{

	//////////////////////////////////////////////////////////////////////////
	public interface class IEditorMovable
	{
		property bool EditorSelected
		{
			bool get();
			void set(bool value);
		};

		property int X
		{
			int get();
			void set(int value);
		};
		
		property int Y
		{
			int get();
			void set(int value);
		};

		property System::Drawing::Point OrigPos
		{
			System::Drawing::Point get();
			void set(System::Drawing::Point value);
		};
	}; 

	//////////////////////////////////////////////////////////////////////////
	public interface class IEditorResizable
	{
		property bool EditorSelected
		{
			bool get();
			void set(bool value);
		};

		property int X
		{
			int get();
			void set(int value);
		};
		
		property int Y
		{
			int get();
			void set(int value);
		};
		
		property int Width
		{
			int get();
			void set(int value);
		};
		
		property int Height
		{
			int get();
			void set(int value);
		};

		property System::Drawing::Rectangle OrigRect
		{
			System::Drawing::Rectangle get();
			void set(System::Drawing::Rectangle value);
		};
	}; 

}}};
