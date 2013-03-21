// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DLOADERMS3D_H__A76EAADE_C083_4EE3_A858_AD29AC169940__INCLUDED_)
#define AFX_3DLOADERMS3D_H__A76EAADE_C083_4EE3_A858_AD29AC169940__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BBase.h"

class C3DModel;

class C3DLoaderMS3D : public CBBase  
{
public:
	#pragma pack(push, packing)
	#pragma pack(1)
	
	// File header
	struct MS3DHeader{
		char m_ID[10];
		int m_Version;
	};
	
	// Vertex information
	struct MS3DVertex{
		BYTE m_Flags;
		float m_Vertex[3];
		char m_BoneID;
		BYTE m_RefCount;
	};
	
	// Triangle information
	struct MS3DTriangle{
		WORD m_Flags;
		WORD m_VertexIndices[3];
		float m_VertexNormals[3][3];
		float m_s[3], m_t[3];
		BYTE m_SmoothingGroup;
		BYTE m_GroupIndex;
	};

	// Material groups
	struct MS3DGroup{
		BYTE m_Flags;            // SELECTED | HIDDEN
		char m_Name[32];         
		WORD m_NumTriangles;     
		WORD* m_TriangleIndices; // the groups group the triangles
		char m_MaterialIndex;    // -1 = no material
	};
	
	// Material information
	struct MS3DMaterial{
		char m_Name[32];
		float m_Ambient[4];
		float m_Diffuse[4];
		float m_Specular[4];
		float m_Emissive[4];
		float m_Shininess;	// 0.0f - 128.0f
		float m_Transparency;	// 0.0f - 1.0f
		BYTE m_Mode;	// 0, 1, 2 is unused now
		char m_Texture[128];
		char m_Alphamap[128];
	};


	// Keyframe data
	struct MS3DKeyframe{
		float m_Time;
		float m_Parameter[3];
	};


	//	Joint information
	struct MS3DJoint{
		BYTE m_Flags;
		char m_Name[32];
		char m_ParentName[32];
		float m_Rotation[3];
		float m_Translation[3];
		WORD m_NumRotKeyframes;
		WORD m_NumTransKeyframes;
		struct MS3DKeyframe* m_RotationKeyframes;
		struct MS3DKeyframe* m_TranslationKeyframes;
	};

	class MS3DAnimation{
	public:
		MS3DAnimation(){
			m_Name = NULL;
			m_StartFrame = m_EndFrame = -1;
			m_Looping = true;
		}
		~MS3DAnimation(){
			if(m_Name) delete [] m_Name;
		}
		char* m_Name;
		int m_StartFrame;
		int m_EndFrame;
		bool m_Looping;
	};
		
	// Default alignment
	#pragma pack(pop, packing)
	
	
	//HRESULT ParseSingleAnim(MS3DAnimation* Anim, BYTE* Buffer);
	//HRESULT ParseAnimationsFile(char* Filename);
	void Cleanup();	
	HRESULT LoadModel(C3DModel* Model);
	HRESULT MergeMesh(C3DModel *Model, char* MeshName, int BoneID);
	HRESULT LoadAnimation(C3DSkelAnim* Anim, int StartFrame, int EndFrame);
	HRESULT ParseFile(char* Filename);
	C3DLoaderMS3D(CBGame* inGame);
	virtual ~C3DLoaderMS3D();
	char* m_Filename;
	double m_TotalTime;
	double m_FrameTime;
	
	MS3DHeader m_Header;

	MS3DVertex* m_Vertices;
	int m_NumVertices;

	MS3DTriangle* m_Faces;
	int m_NumFaces;

	MS3DGroup* m_Meshes;
	int m_NumMeshes;

	MS3DMaterial* m_Materials;
	int m_NumMaterials;

	MS3DJoint* m_Joints;
	int m_NumJoints;

	//CBArray<MS3DAnimation*, MS3DAnimation*> m_Animations;
	
private:
	HRESULT LoadMesh(C3DMesh* Mesh, MS3DGroup* MSGroup, int MaterialOffset=0, int BoneID=-1);
	HRESULT LoadMaterial(C3DMaterial* Material, MS3DMaterial* MSMaterial);

};

#endif // !defined(AFX_3DLOADERMS3D_H__A76EAADE_C083_4EE3_A858_AD29AC169940__INCLUDED_)
