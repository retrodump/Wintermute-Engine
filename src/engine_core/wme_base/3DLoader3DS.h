// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DLOADER3DS_H__99A2BA02_A2DF_4B8B_A93A_DBDAC57F6C8C__INCLUDED_)
#define AFX_3DLOADER3DS_H__99A2BA02_A2DF_4B8B_A93A_DBDAC57F6C8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DLoader3DS : public CBNamedObject
{
public:
	enum E3DSFileObjectType{
		OBJ_3DS_NONE, OBJ_3DS_MESH, OBJ_3DS_CAMERA, OBJ_3DS_LIGHT
	};

	struct SFace{
		WORD a;
		WORD b;
		WORD c;
	};

	struct STextureCoordinate{
		float u;
		float v;
	};

	struct SMaterialFaces{
		char* Material;
		WORD NumFaces;
		WORD* Faces;
	};

	
	class C3DFileObject3DS
	{
	public:
		D3DXVECTOR3 m_LocalOrigin;
		D3DXMATRIX m_LocalMatrix;
		D3DXVECTOR3 m_CameraTarget;
		float m_CameraBank;
		float m_CameraLens;
		float m_CameraFOV;
		D3DXVECTOR3 m_CameraPos;
		D3DXVECTOR3 m_LightTarget;
		D3DXVECTOR3 m_LightPos;
		D3DCOLOR m_LightColor;
		float m_LightHotspot;
		float m_LightFalloff;
		bool m_LightOff;
		bool m_LightSpotlight;
		CBArray<SMaterialFaces*, SMaterialFaces*> m_MaterialFaces;
		bool m_Hidden;
		WORD m_NumCoordinates;
		STextureCoordinate* m_Coordinates;
		WORD m_NumFaces;
		SFace* m_Faces;
		D3DXVECTOR3* m_Vertices;
		WORD m_NumVertices;
		char* m_Name;
		E3DSFileObjectType m_Type;
		virtual  ~C3DFileObject3DS();
		C3DFileObject3DS();
	};

public:	
	char* GetCameraName(int Index);
	char* GetLightName(int Index);
	char* GetMeshName(int Index);
	HRESULT LoadCamera(int Index, C3DCamera* Camera);
	int GetNumCameras();
	HRESULT LoadLight(int Index, C3DLight* Light);
	int GetNumLights();
	HRESULT LoadMesh(int Index, C3DMesh* Mesh);
	int GetNumMeshes();
	HRESULT ParseFile(char* Filename);
	char* m_Filename;
	C3DLoader3DS(CBGame* inGame);
	virtual ~C3DLoader3DS();
	CBArray<C3DFileObject3DS*, C3DFileObject3DS*> m_Objects;
	D3DXMATRIX m_ViewMatrix;
};

#endif // !defined(AFX_3DLOADER3DS_H__99A2BA02_A2DF_4B8B_A93A_DBDAC57F6C8C__INCLUDED_)
