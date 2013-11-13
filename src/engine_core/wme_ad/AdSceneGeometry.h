// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADSCENEGEOMETRY_H__6043ACFC_3844_4218_BFA9_805F2E690AEB__INCLUDED_)
#define AFX_ADSCENEGEOMETRY_H__6043ACFC_3844_4218_BFA9_805F2E690AEB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class CAdSceneGeometry : public CBObject
{
public:
	bool m_MaxLightsWarning;
	HRESULT DropWaypoints();
	HRESULT SetLightColor(char* LightName, DWORD Color);
	DWORD GetLightColor(char* LightName);
	D3DXVECTOR3 GetLightPos(char* LightName);
	HRESULT EnableNode(char* NodeName, bool Enable=true);
	bool IsNodeEnabled(char* NodeName);
	HRESULT EnableLight(char* LightName, bool Enable=true);
	bool IsLightEnabled(char* LightName);
	DECLARE_PERSISTENT(CAdSceneGeometry, CBObject);
	HRESULT CorrectTargetPoint(D3DXVECTOR3 Source, D3DXVECTOR3* Target);

	bool m_LastValuesInitialized;
	D3DXMATRIX m_LastWorldMat;
	D3DXMATRIX m_LastViewMat;
	D3DXMATRIX m_LastProjMat;
	int m_LastOffsetX;
	int m_LastOffsetY;
	D3DVIEWPORT m_DrawingViewport;
	int m_LastScrollX;
	int m_LastScrollY;


	HRESULT CreateLights();
	HRESULT EnableLights(D3DXVECTOR3 Point, CBArray<char*, char*>& IgnoreLights);
	static int CompareLights(const void* Obj1, const void* Obj2);
	
	HRESULT InitLoop();
	float GetPointsDist(D3DXVECTOR3 p1, D3DXVECTOR3 p2);
	void PathFinderStep();
	bool GetPath(D3DXVECTOR3 source, D3DXVECTOR3 target, CAdPath3D* path, bool Rerun=false);
	bool Convert2Dto3D(int X, int Y, D3DXVECTOR3* Pos);
	bool Convert2Dto3DTolerant(int X, int Y, D3DXVECTOR3 *Pos);
	bool Convert3Dto2D(D3DXVECTOR3* Pos, int* X, int* Y);
	CBSprite* m_WptMarker;
	float m_WaypointHeight;
	bool DirectPathExists(D3DXVECTOR3* p1, D3DXVECTOR3* p2);
	float GetHeightAt(D3DXVECTOR3 Pos, float Tolerance=0.0f, bool* IntFound=NULL);

	HRESULT StoreDrawingParams();
	HRESULT Render(bool Render);
	HRESULT RenderShadowGeometry();

	D3DXMATRIX* GetViewMatrix();
	D3DXMATRIX m_ViewMatrix;
	HRESULT SetActiveCamera(char* Camera, float FOV, float NearClipPlane, float FarClipPlane);
	HRESULT SetActiveCamera(int Camera, float FOV, float NearClipPlane, float FarClipPlane);
	//HRESULT SetActiveCameraTwin(char* Camera);
	//HRESULT SetActiveCameraTwin(int Camera);
	C3DCamera* GetActiveCamera();
	int m_ActiveCamera;

	HRESULT SetActiveLight(char* Light);
	HRESULT SetActiveLight(int Light);
	int m_ActiveLight;

	void Cleanup();
	CAdSceneGeometry(CBGame* inGame);
	virtual ~CAdSceneGeometry();
	HRESULT CAdSceneGeometry::LoadFile(char* Filename);
	CBArray<CAdWalkplane*, CAdWalkplane*> m_Planes;
	CBArray<CAdBlock*, CAdBlock*> m_Blocks;
	CBArray<CAdGeneric*, CAdGeneric*> m_Generics;
	CBArray<C3DCamera*, C3DCamera*> m_Cameras;
	CBArray<C3DLight*, C3DLight*> m_Lights;
	CBArray<CAdWaypointGroup3D*, CAdWaypointGroup3D*> m_WaypointGroups;
	DWORD m_PFMaxTime;

private:
	CAdGeomExt* GetGeometryExtension(char* Filename);
	D3DXVECTOR3 GetBlockIntersection(D3DXVECTOR3* p1, D3DXVECTOR3* p2);	
	bool m_PFReady;
	D3DXVECTOR3 m_PFSource;
	D3DXVECTOR3 m_PFTarget;
	CAdPath3D* m_PFTargetPath;
	D3DXVECTOR3 m_PFAlternateTarget;
	float m_PFAlternateDist;
	bool m_PFRerun;
	CBArray<CAdPathPoint3D*, CAdPathPoint3D*> m_PFPath;

};

#endif // !defined(AFX_ADSCENEGEOMETRY_H__6043ACFC_3844_4218_BFA9_805F2E690AEB__INCLUDED_)
