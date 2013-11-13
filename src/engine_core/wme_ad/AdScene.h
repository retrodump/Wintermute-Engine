// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSCENE_H__1AA57255_740A_4EC0_942D_C3FDCB424A38__INCLUDED_)
#define AFX_ADSCENE_H__1AA57255_740A_4EC0_942D_C3FDCB424A38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CAdScene : public CBObject
{
public:
	DWORD m_AmbientLightColor;
	TShadowType m_MaxShadowType;
	bool m_Scroll3DCompatibility;

	bool m_FogEnabled;
	DWORD m_FogColor;
	float m_FogStart;
	float m_FogEnd;

	CBObject* GetNextAccessObject(CBObject* CurrObject);
	CBObject* GetPrevAccessObject(CBObject* CurrObject);
	HRESULT GetSceneObjects(CBArray<CAdObject*, CAdObject*>& Objects, bool InteractiveOnly);
	HRESULT GetRegionObjects(CAdRegion* Region, CBArray<CAdObject*, CAdObject*>& Objects, bool InteractiveOnly);
	HRESULT Display3DContent(D3DXMATRIX* ViewMat, D3DXMATRIX* ProjMat);

	bool m_2DPathfinding;
	HRESULT AfterLoad();

	float m_NearClipPlane;
	float m_FarClipPlane;
	float m_FOV;
	int m_EditorResolutionWidth;
	int m_EditorResolutionHeight;
	C3DLight* GetActiveLight();
	HRESULT GetRegionsAt(int X, int Y, CAdRegion** RegionList, int NumRegions);
	HRESULT HandleItemAssociations(char* ItemName, bool Show);
	CUIWindow* m_ShieldWindow;
	float GetRotationAt(int X, int Y);
	HRESULT LoadState();
	HRESULT SaveState();
	bool m_PersistentState;
	bool m_PersistentStateSprites;
	CBObject* GetNodeByName(char* Name);
	void SetOffset(int OffsetLeft, int OffsetTop);
	bool PointInViewport(int X, int Y);
	int GetOffsetTop();
	int GetOffsetLeft();
	HRESULT GetViewportSize(int* Width=NULL, int* Height=NULL);
	HRESULT GetViewportOffset(int* OffsetX=NULL, int* OffsetY=NULL);
	CBViewport* m_Viewport;
	CBFader* m_Fader;
	int m_PFPointsNum;
	void PFPointsAdd(int X, int Y, int Distance);
	void PFPointsStart();
	bool m_Initialized;
	HRESULT CorrectTargetPoint(int StartX, int StartY, int* X, int* Y, bool CheckFreeObjects=false, CBObject* Requester=NULL);
	HRESULT CorrectTargetPoint2(int StartX, int StartY, int* TargetX, int* TargetY, bool CheckFreeObjects, CBObject* Requester);
	DECLARE_PERSISTENT(CAdScene, CBObject);
	HRESULT DisplayRegionContent(CAdRegion* Region=NULL, bool Display3DOnly=false);
	HRESULT DisplayRegionContentOld(CAdRegion* Region=NULL);
	static int CompareObjs(const void* Obj1, const void* Obj2);

	HRESULT UpdateFreeObjects();
	HRESULT CAdScene::TraverseNodes(bool Update=false);
	float GetScaleAt(int Y);
	HRESULT SortScaleLevels();
	HRESULT SortRotLevels();
	virtual HRESULT SaveAsText(CBDynBuffer* Buffer, int Indent);
	bool m_ShowGeometry;
	CAdSceneGeometry* m_Geom;
	DWORD GetAlphaAt(int X, int Y, bool ColorCheck = false);
	bool m_ParalaxScrolling;
	void SkipTo(int OffsetX, int OffsetY);
	void SetDefaults();
	void Cleanup();
	void SkipToObject(CBObject* Object);
	void ScrollToObject(CBObject* Object);
	void ScrollTo(int OffsetX, int OffsetY);
	virtual HRESULT Update();
	bool m_AutoScroll;
	int m_TargetOffsetTop;
	int m_TargetOffsetLeft;

	int m_ScrollPixelsV;
	DWORD m_ScrollTimeV;
	DWORD m_LastTimeV;

	int m_ScrollPixelsH;
	DWORD m_ScrollTimeH;
	DWORD m_LastTimeH;

	virtual HRESULT Display();
	DWORD m_PFMaxTime;
	HRESULT InitLoop();
	void PathFinderStep();
	bool IsBlockedAt(int X, int Y, bool CheckFreeObjects=false, CBObject* Requester=NULL);
	bool IsWalkableAt(int X, int Y, bool CheckFreeObjects=false, CBObject* Requester=NULL);
	CAdLayer* m_MainLayer;
	float GetZoomAt(int X, int Y);
	bool GetPath(CBPoint source, CBPoint target, CAdPath* path, CBObject* requester=NULL);
	CAdScene(CBGame* inGame);
	virtual ~CAdScene();
	CBArray<CAdLayer*, CAdLayer*> m_Layers;
	CBArray<CAdObject*, CAdObject*> m_Objects;
	CBArray<CAdWaypointGroup*, CAdWaypointGroup*> m_WaypointGroups;
	HRESULT LoadFile(char* Filename);
	HRESULT LoadBuffer(BYTE* Buffer, bool Complete=true);
	int m_Width;
	int m_Height;
	HRESULT AddObject(CAdObject *Object);
	HRESULT RemoveObject(CAdObject *Object);
	int m_EditorMarginH;
	int m_EditorMarginV;
	DWORD m_EditorColFrame;
	DWORD m_EditorColEntity;
	DWORD m_EditorColRegion;
	DWORD m_EditorColBlocked;
	DWORD m_EditorColWaypoints;
	DWORD m_EditorColEntitySel;
	DWORD m_EditorColRegionSel;
	DWORD m_EditorColBlockedSel;
	DWORD m_EditorColWaypointsSel;
	DWORD m_EditorColScale;
	DWORD m_EditorColDecor;
	DWORD m_EditorColDecorSel;

	bool m_EditorShowRegions;
	bool m_EditorShowBlocked;
	bool m_EditorShowDecor;
	bool m_EditorShowEntities;
	bool m_EditorShowScale;
	CBArray<CAdScaleLevel*, CAdScaleLevel*> m_ScaleLevels;
	CBArray<CAdRotLevel*, CAdRotLevel*> m_RotLevels;
	
	virtual HRESULT RestoreDeviceObjects();
	HRESULT SetMaxShadowType(TShadowType MaxShadowType);
	int GetPointsDist(CBPoint p1, CBPoint p2, CBObject* requester=NULL);	

	void OnLayerResized(CAdLayer* layer);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);
	virtual char* ScToString();


private:
	HRESULT PersistState(bool Saving=true);
	void PFAddWaypointGroup(CAdWaypointGroup* Wpt, CBObject* Requester = NULL);	
	bool m_PFReady;
	CBPoint* m_PFTarget;
	CAdPath* m_PFTargetPath;
	CBObject* m_PFRequester;
	CBArray<CAdPathPoint*, CAdPathPoint*> m_PFPath;

	int m_OffsetTop;
	int m_OffsetLeft;

};

#endif // !defined(AFX_ADSCENE_H__1AA57255_740A_4EC0_942D_C3FDCB424A38__INCLUDED_)
