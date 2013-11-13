// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifndef DCTYPES_H
#define DCTYPES_H


#include <string>
#include <list>
#include <vector>

using namespace std;


typedef std::string AnsiString;
typedef std::string Utf8String;
typedef std::wstring WideString;


typedef std::list<WideString> WideStringList;
typedef std::list<AnsiString> AnsiStringList;

typedef std::vector<WideString> WideStringArray;
typedef std::vector<AnsiString> AnsiStringArray;


typedef enum {
	GAME_RUNNING, GAME_FROZEN, GAME_SEMI_FROZEN
} TGameState;


typedef enum { IMG_PALETTED8, IMG_TRUECOLOR } TImageType;


typedef enum{
	TAL_LEFT=0, TAL_RIGHT, TAL_CENTER, NUM_TEXT_ALIGN
}TTextAlign;


typedef enum{
	VAL_TOP=0, VAL_CENTER, VAL_BOTTOM, NUM_VERTICAL_ALIGN
}TVerticalAlign;


typedef enum{
	DI_UP=0, DI_UPRIGHT, DI_RIGHT, DI_DOWNRIGHT, DI_DOWN, DI_DOWNLEFT, DI_LEFT, DI_UPLEFT, NUM_DIRECTIONS, DI_NONE
}TDirection;


typedef enum{
	EVENT_NONE			 = 0,
	EVENT_INIT			 = 1,
	EVENT_SHUTDOWN		 = 2,
	EVENT_LEFT_CLICK	 = 3,
	EVENT_RIGHT_CLICK	 = 4,
	EVENT_MIDDLE_CLICK	 = 5,
	EVENT_LEFT_DBLCLICK	 = 6,
	EVENT_PRESS			 = 7,
	EVENT_IDLE			 = 8,
	EVENT_MOUSE_OVER     = 9,
	EVENT_LEFT_RELEASE   = 10,
	EVENT_RIGHT_RELEASE  = 11,
	EVENT_MIDDLE_RELEASE = 12,
	NUM_EVENTS
}TEventType;


typedef enum{
	UI_UNKNOWN, UI_BUTTON, UI_WINDOW, UI_STATIC, UI_EDIT, UI_HTML, UI_CUSTOM
}TUIObjectType;


typedef enum{
	RSTATE_3D, RSTATE_2D, RSTATE_LINES, RSTATE_NONE
}TRendererState;


typedef enum{
	DYNAMIC_CONSTRUCTOR
}TDynamicConstructor;

typedef enum TSeek{
	SEEK_TO_BEGIN   = SEEK_SET,
	SEEK_TO_CURRENT = SEEK_CUR,
	SEEK_TO_END     = SEEK_END
};

typedef enum TSoundType{
	SOUND_SFX, SOUND_MUSIC, SOUND_SPEECH
};

typedef enum TVideoMode{
	VIDEO_WINDOW, VIDEO_FULLSCREEN, VIDEO_ANY
};


typedef enum TVideoPlayback{
	VID_PLAY_POS=0,
	VID_PLAY_STRETCH=1,
	VID_PLAY_CENTER=2
};


typedef enum TMouseEvent{
	MOUSE_CLICK, MOUSE_RELEASE, MOUSE_DBLCLICK
};


typedef enum TMouseButton{
	MOUSE_BUTTON_LEFT, MOUSE_BUTTON_RIGHT, MOUSE_BUTTON_MIDDLE
};


typedef enum {
	TRANS_MGR_RUNNING, TRANS_MGR_READY
} TTransMgrState;


typedef enum {
	TRANSITION_NONE=0,
	TRANSITION_FADE_OUT=1,
	TRANSITION_FADE_IN=2,
	NUM_TRANSITION_TYPES
} TTransitionType;


typedef enum {
	WINDOW_NORMAL, WINDOW_EXCLUSIVE, WINDOW_SYSTEM_EXCLUSIVE
} TWindowMode;

typedef enum {
	SFX_NONE, SFX_ECHO, SFX_REVERB
} TSFXType;


typedef enum {
	CACHE_ALL, CACHE_HALF
} TSpriteCacheType;

typedef enum {
	TEXT_ANSI=0, TEXT_UTF8=1, NUM_TEXT_ENCODINGS
} TTextEncoding;

typedef enum {
	BLEND_UNKNOWN=-1, BLEND_NORMAL=0, BLEND_ADDITIVE=1, BLEND_SUBTRACTIVE=2, NUM_BLEND_MODES
} TSpriteBlendMode;

typedef enum {
	TTS_CAPTION=0, TTS_TALK, TTS_KEYPRESS
} TTTSType;

typedef enum {
	SHADOW_NONE=0, SHADOW_SIMPLE=1, SHADOW_FLAT=2, SHADOW_STENCIL=3
} TShadowType;


#endif // DCTYPES_H
