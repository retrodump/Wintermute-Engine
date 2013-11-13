// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme

// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently,
// but are changed infrequently

#pragma once

#include "dcgf.h"
#include "dcgf_ad.h"

#define SAFE_CONTROL_HWND( c ) \
	c != nullptr ? (HWND)c->Handle.ToInt32() : NULL

#include "enums.h"

// utility classes
#include "WDynArray.h"
#include "WVector3.h"
#include "W3DJoint.h"

// base classes
#include "WBase.h"
#include "WNamedObject.h"
#include "WDynBuffer.h"
#include "WScriptable.h"
#include "WScriptHolder.h"
#include "WPlugin.h"
#include "WPluginMgr.h"
#include "WPoint.h"
#include "WObject.h"
#include "WRegion.h"
#include "WSound.h"
#include "WSubFrame.h"
#include "WFrame.h"
#include "WSprite.h"
#include "WFont.h"
#include "WFontBitmap.h"
#include "WFontTT.h"
#include "WRenderer.h"

// UI classes
#include "WUITiledImage.h"
#include "WUIObject.h"
#include "WUIButton.h"
#include "WUIEdit.h"
#include "WUIStatic.h"
#include "WUIWindow.h"

// scripting classes
#include "WScValue.h"
#include "WScStack.h"
#include "WScScript.h"
#include "WScEngine.h"

// 3d classes
#include "W3DCamera.h"
#include "W3DMesh.h"
#include "W3DSkelAnim.h"
#include "W3DModel.h"

// adventure classes
#include "WAdSpriteSet.h"
#include "WAdTalkNode.h"
#include "WAdTalkDef.h"
#include "WAdResponse.h"
#include "WAdResponseBox.h"
#include "WAdInventory.h"
#include "WAdObject.h"
#include "WAdTalkHolder.h"
#include "WAdItem.h"
#include "WAdObject3D.h"
#include "WAdActor.h"
#include "WAdEntity.h"
#include "WAdSceneNode.h"
#include "WAdRegion.h"
#include "WAdLayer.h"
#include "WAdWaypointGroup.h"
#include "WAdScaleLevel.h"
#include "WAdRotLevel.h"
#include "WAdWalkplane.h"
#include "WAdBlock.h"
#include "WAdWaypointGroup3d.h"
#include "WAdSceneGeometry.h"
#include "WAdInventoryBox.h"
#include "WAdScene.h"
#include "WAdGame.h"
#include "WAdActor3d.h"
#include "WUIEntity.h"
