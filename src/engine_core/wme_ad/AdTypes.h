// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#ifndef ADTYPES_H
#define ADTYPES_H


typedef enum
{
	GAME_NORMAL, GAME_WAITING_RESPONSE
} TGameStateEx;


typedef enum
{
	OBJECT_ENTITY, OBJECT_REGION, OBJECT_ACTOR, OBJECT_NONE
} TObjectType;


typedef enum
{
	ENTITY_NORMAL, ENTITY_SOUND
} TEntityType;


typedef enum
{
	STATE_NONE,
	STATE_IDLE,
	STATE_PLAYING_ANIM,
	STATE_READY,
	STATE_FOLLOWING_PATH,
	STATE_SEARCHING_PATH,
	STATE_WAITING_PATH,
	STATE_TURNING_LEFT,
	STATE_TURNING_RIGHT,
	STATE_TURNING,
	STATE_TALKING,
	STATE_DIRECT_CONTROL,
	STATE_PLAYING_ANIM_SET,
} TObjectState;

typedef enum
{
	DIRECT_WALK_NONE, DIRECT_WALK_FW, DIRECT_WALK_BK
} TDirectWalkMode;

typedef enum
{
	DIRECT_TURN_NONE, DIRECT_TURN_CW, DIRECT_TURN_CCW
} TDirectTurnMode;

typedef enum
{
	RESPONSE_TEXT, RESPONSE_ICON
} TResponseStyle;

typedef enum
{
	RESPONSE_ALWAYS, RESPONSE_ONCE, RESPONSE_ONCE_GAME
} TResponseType;


typedef enum
{
	TALK_SKIP_LEFT=0, TALK_SKIP_RIGHT=1, TALK_SKIP_BOTH=2, TALK_SKIP_NONE=3
} TTalkSkipButton;


typedef enum
{
	VIDEO_SKIP_LEFT=0, VIDEO_SKIP_RIGHT=1, VIDEO_SKIP_BOTH=2, VIDEO_SKIP_NONE=3
} TVideoSkipButton;



typedef enum
{
	GEOM_WAYPOINT, GEOM_WALKPLANE, GEOM_BLOCKED, GEOM_GENERIC
} TGeomNodeType;


#endif // ADTYPES_H
