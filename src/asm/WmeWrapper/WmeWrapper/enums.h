// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

namespace DeadCode { namespace WME { namespace Core
{
	using namespace System;
	using namespace DeadCode::WME::Core;


	public enum class WEGameState : int
	{
		Running = GAME_RUNNING,
		Frozen = GAME_FROZEN,
		SemiFrozen = GAME_SEMI_FROZEN
	};

	public enum class WETextAlign : int
	{
		Left = TAL_LEFT,
		Right = TAL_RIGHT,
		Center = TAL_CENTER
	};

	public enum class WEVerticalAlign : int
	{
		Top = VAL_TOP,
		Center = VAL_CENTER,
		Bottom = VAL_BOTTOM
	};

	public enum class WEDirection : int
	{
		Up = DI_UP,
		UpRight = DI_UPRIGHT,
		Right = DI_RIGHT,
		DownRight = DI_DOWNRIGHT,
		Down = DI_DOWN,
		DownLeft = DI_DOWNLEFT,
		Left = DI_LEFT,
		UpLeft = DI_UPLEFT,
		None = DI_NONE
	};

	public enum class WEUIObjectType : int
	{
		Unknown = UI_UNKNOWN,
		Button = UI_BUTTON,
		Window = UI_WINDOW,
		Static = UI_STATIC,
		Editor = UI_EDIT
	};

	public enum class WESoundType : int
	{
		Sfx = SOUND_SFX,
		Music = SOUND_MUSIC,
		Speech = SOUND_SPEECH
	};

	//////////////////////////////////////////////////////////////////////////
	public enum class WEGameStateEx : int
	{
		Normal = GAME_NORMAL,
		WaitingResponse = GAME_WAITING_RESPONSE
	};

	public enum class WEObjectType : int
	{
		Entity = OBJECT_ENTITY,
		Region = OBJECT_REGION,
		Actor = OBJECT_ACTOR,
		None = OBJECT_NONE
	};

	public enum class WEEntityType : int
	{
		Normal = ENTITY_NORMAL,
		Sound = ENTITY_SOUND
	};

	public enum class WEObjectState : int
	{
		None = STATE_NONE,
		Idle = STATE_IDLE,
		PlayingAnim = STATE_PLAYING_ANIM,
		Ready = STATE_READY,
		FollowingPath = STATE_FOLLOWING_PATH,
		SearchingPath = STATE_SEARCHING_PATH,
		AwaitingPath = STATE_WAITING_PATH,
		TurningLeft = STATE_TURNING_LEFT,
		TurningRight = STATE_TURNING_RIGHT,
		Turning = STATE_TURNING,
		Talking = STATE_TALKING
	};

	public enum class WETalkSkipButton : int
	{
		Left = TALK_SKIP_LEFT,
		Right = TALK_SKIP_RIGHT,
		Both = TALK_SKIP_BOTH
	};


	// value types
	public enum class WEValueType : int
	{
		Null = VAL_NULL,
		String = VAL_STRING,
		Int = VAL_INT,
		Bool = VAL_BOOL,
		Float = VAL_FLOAT,
		Object = VAL_OBJECT,
		Native = VAL_NATIVE,
		Reference = VAL_VARIABLE_REF
	};


	// script states
	public enum class WEScriptState : int
	{
		Running = SCRIPT_RUNNING,
		Waiting = SCRIPT_WAITING,
		Sleeping = SCRIPT_SLEEPING,
		Finished = SCRIPT_FINISHED,
		Persistent = SCRIPT_PERSISTENT,
		Error = SCRIPT_ERROR,
		Paused = SCRIPT_PAUSED,
		WaitingForScript = SCRIPT_WAITING_SCRIPT,
		ThreadFinished = SCRIPT_THREAD_FINISHED
	};


	public enum class WEWindowMode : int
	{
		Normal = WINDOW_NORMAL,
		Exclusive = WINDOW_EXCLUSIVE,
		SystemExclusive = WINDOW_SYSTEM_EXCLUSIVE
	};


	public enum class WESFXType : int
	{
		None = SFX_NONE,
		Echo = SFX_ECHO,
		Reverb = SFX_REVERB
	};


	public enum class WETextEncoding : int
	{
		Ansi = TEXT_ANSI,
		Utf8 = TEXT_UTF8
	};


	public enum class WEDirectWalkMode
	{
		None = DIRECT_WALK_NONE,
		Forward = DIRECT_WALK_FW,
		Backward = DIRECT_WALK_BK
	};

	public enum class WEDirectTurnMode
	{
		None = DIRECT_TURN_NONE,
		Clockwise = DIRECT_TURN_CW,
		CounterClockwise = DIRECT_TURN_CCW
	};

	public enum class WEShadowType
	{
		None = SHADOW_NONE,
		Simple = SHADOW_SIMPLE,
		Flat = SHADOW_FLAT,
		Stencil = SHADOW_STENCIL
	};

}}};