// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DLoaderMS3D.h"


//////////////////////////////////////////////////////////////////////////
C3DLoaderMS3D::C3DLoaderMS3D(CBGame* inGame):CBBase(inGame)
{
	m_Filename = NULL;
	m_TotalTime = m_FrameTime = 0.0f;

	m_NumVertices = 0;
	m_Vertices = NULL;

	m_NumFaces = 0;
	m_Faces = NULL;

	m_NumMeshes = 0;
	m_Meshes = NULL;

	m_NumMaterials = 0;
	m_Materials = NULL;

	m_NumJoints = 0;
	m_Joints = NULL;
}


//////////////////////////////////////////////////////////////////////////
C3DLoaderMS3D::~C3DLoaderMS3D()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void C3DLoaderMS3D::Cleanup()
{
	int i;

	SAFE_DELETE_ARRAY(m_Filename);

	SAFE_DELETE_ARRAY(m_Vertices);
	SAFE_DELETE_ARRAY(m_Faces);

	for(i=0; i<m_NumMeshes; i++) delete [] m_Meshes[i].m_TriangleIndices;
	SAFE_DELETE_ARRAY(m_Meshes);

	SAFE_DELETE_ARRAY(m_Materials);

	for(i=0; i<m_NumJoints; i++){
		delete [] m_Joints[i].m_RotationKeyframes;
		delete [] m_Joints[i].m_TranslationKeyframes;
	}
	SAFE_DELETE_ARRAY(m_Joints);

	/*
	for(i=0; i<m_Animations.GetSize(); i++) delete m_Animations[i];
	m_Animations.RemoveAll();
	*/
}



#pragma pack(push, packing)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::ParseFile(char *Filename)
{
	Cleanup();

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);


	BYTE *Buffer;
	DWORD FileSize;

	Buffer = Game->m_FileManager->ReadWholeFile(Filename, &FileSize);
	if(!Buffer) return E_FAIL;

	BYTE *pPtr = Buffer;

	int i,j;


	// header
	memcpy(&m_Header, pPtr, sizeof(MS3DHeader));
	pPtr += sizeof(MS3DHeader);

	if(strncmp(m_Header.m_ID, "MS3D000000", 10) != 0){
		Game->LOG(0, "Not a valid Milkshape3D model (%s)", m_Filename);
		delete [] Buffer;
		return E_FAIL;
	}

	if(m_Header.m_Version < 3 || m_Header.m_Version > 4){
		Game->LOG(0, "Unknown Milkshape3D model version (%s)", m_Filename);
		delete [] Buffer;
		return E_FAIL;
	}
	

	// vertices
	m_NumVertices = *(WORD*)pPtr;
	m_Vertices = new MS3DVertex[m_NumVertices];
	pPtr += sizeof(WORD);

	for(i=0; i<m_NumVertices; i++){
		memcpy(&m_Vertices[i], pPtr, sizeof(MS3DVertex));
		pPtr += sizeof(MS3DVertex);
	}


	// faces
	m_NumFaces = *(WORD*)pPtr;
	m_Faces = new MS3DTriangle[m_NumFaces];
	pPtr += sizeof(WORD);

	for(i=0; i<m_NumFaces; i++){
		memcpy(&m_Faces[i], pPtr, sizeof(MS3DTriangle));
		pPtr += sizeof(MS3DTriangle);
	}


	// meshes
	m_NumMeshes = *(WORD*)pPtr;
	m_Meshes = new MS3DGroup[m_NumMeshes];
	pPtr += sizeof(WORD);

	for(i=0; i<m_NumMeshes; i++){
		m_Meshes[i].m_Flags = *(BYTE*)pPtr;
		pPtr += sizeof(BYTE);

		memcpy(m_Meshes[i].m_Name, pPtr, 32);
		pPtr += 32;

		m_Meshes[i].m_NumTriangles = *(WORD*)pPtr;
		pPtr += sizeof(WORD);
		
		m_Meshes[i].m_TriangleIndices = new WORD[m_Meshes[i].m_NumTriangles];
		for(j=0; j<m_Meshes[i].m_NumTriangles; j++){
			m_Meshes[i].m_TriangleIndices[j] = *(WORD*)pPtr;
			pPtr += sizeof(WORD);
		}

		m_Meshes[i].m_MaterialIndex = *(char*)pPtr;
		pPtr += sizeof(char);
	}


	// materials
	m_NumMaterials = *(WORD*)pPtr;
	m_Materials = new MS3DMaterial[m_NumMaterials];
	pPtr += sizeof(WORD);

	for(i=0; i<m_NumMaterials; i++){
		memcpy(&m_Materials[i], pPtr, sizeof(MS3DMaterial));
		pPtr += sizeof(MS3DMaterial);
	}
	

	// animation info
	float animFPS = *(float*)pPtr;
	pPtr += sizeof(float);

	// skip currentTime
	pPtr += sizeof(float);

	int totalFrames = *(int*)pPtr;
	pPtr += sizeof(int);

	m_FrameTime = 1000.0f/animFPS;
	m_TotalTime = totalFrames*m_FrameTime;


	/*
	// animations (external file)
	char PathTemp[MAX_PATH+1];
	int PathLength;
	for (PathLength = strlen(m_Filename); PathLength--; ) {
		if(m_Filename[PathLength] == '.') break;
	}
	
	strncpy(PathTemp, m_Filename, PathLength);
	strcpy(PathTemp + PathLength, ".ska");
	ParseAnimationsFile(PathTemp);
	*/


	// joints
	m_NumJoints = *(WORD*)pPtr;
	m_Joints = new MS3DJoint[m_NumJoints];
	pPtr += sizeof(WORD);

	for(i=0; i<m_NumJoints; i++){
		m_Joints[i].m_Flags = *(BYTE*)pPtr;
		pPtr += sizeof(BYTE);

		memcpy(m_Joints[i].m_Name, pPtr, 32);
		pPtr += 32;

		memcpy(m_Joints[i].m_ParentName, pPtr, 32);
		pPtr += 32;

		memcpy(m_Joints[i].m_Rotation, pPtr, 3*sizeof(float));
		pPtr += 3*sizeof(float);

		memcpy(m_Joints[i].m_Translation, pPtr, 3*sizeof(float));
		pPtr += 3*sizeof(float);

		m_Joints[i].m_NumRotKeyframes = *(WORD*)pPtr;
		pPtr += sizeof(WORD);

		m_Joints[i].m_NumTransKeyframes = *(WORD*)pPtr;
		pPtr += sizeof(WORD);

		m_Joints[i].m_RotationKeyframes = new MS3DKeyframe[m_Joints[i].m_NumRotKeyframes];
		m_Joints[i].m_TranslationKeyframes = new MS3DKeyframe[m_Joints[i].m_NumTransKeyframes];

		// rotation keyframes
		for(j=0; j<m_Joints[i].m_NumRotKeyframes; j++){
			memcpy(&m_Joints[i].m_RotationKeyframes[j], pPtr, sizeof(MS3DKeyframe));
			
			
			float t = m_Joints[i].m_RotationKeyframes[j].m_Time;
			float x = RadToDeg(m_Joints[i].m_RotationKeyframes[j].m_Parameter[0]);
			float y = RadToDeg(m_Joints[i].m_RotationKeyframes[j].m_Parameter[1]);
			float z = RadToDeg(m_Joints[i].m_RotationKeyframes[j].m_Parameter[2]);
			//Game->LOG(0, "Keyframe %d: %f -> %f, %f, %f", j, t, x, y, z);
			
			pPtr += sizeof(MS3DKeyframe);
		}

		// translation keyframes
		for(j=0; j<m_Joints[i].m_NumTransKeyframes; j++){
			memcpy(&m_Joints[i].m_TranslationKeyframes[j], pPtr, sizeof(MS3DKeyframe));
			pPtr += sizeof(MS3DKeyframe);
		}
	}

	delete [] Buffer;

	return S_OK;
}
#pragma pack(pop, packing)

//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::MergeMesh(C3DModel *Model, char* MeshName, int BoneID)
{		
	int i;

	// load a mesh
	bool Found = false;
	for(i=0; i<m_NumMeshes; i++)
	{
		if(stricmp(m_Meshes[i].m_Name, MeshName)==0)
		{
			C3DMesh* Mesh = new C3DMesh(Game);
			LoadMesh(Mesh, &m_Meshes[i], Model->m_NumMaterials, BoneID);
			Model->m_Meshes.Add(Mesh);

			Model->m_NumMeshes++;
			Found = true;
			break;
		}
	}
	if(!Found)
	{
		Game->LOG(0, "Error: Mesh '%s' not found in model '%s'", MeshName, m_Filename);
		return E_FAIL;
	}

	// load materials
	for(i=0; i<m_NumMaterials; i++)
	{
		C3DMaterial* Material = new C3DMaterial(Game);
		LoadMaterial(Material, &m_Materials[i]);
		Model->m_Materials.Add(Material);

		Model->m_NumMaterials++;
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::LoadModel(C3DModel *Model)
{		
	int i,j;

	// load meshes
	Model->m_NumMeshes = m_NumMeshes;

	for(i=0; i<m_NumMeshes; i++){
		C3DMesh* Mesh = new C3DMesh(Game);
		LoadMesh(Mesh, &m_Meshes[i], 0);
		Model->m_Meshes.Add(Mesh);
	}

	// load materials
	Model->m_NumMaterials = m_NumMaterials;
	
	for(i=0; i<m_NumMaterials; i++){
		C3DMaterial* Material = new C3DMaterial(Game);
		LoadMaterial(Material, &m_Materials[i]);
		Model->m_Materials.Add(Material);
	}


	// load joints
	int parent;

	Model->m_NumJoints = m_NumJoints;
	Model->m_Joints = new C3DJoint[m_NumJoints];

	// conversion matrix from right-handed to left-handed
	D3DXMATRIX convert;
	D3DXMatrixRotationYawPitchRoll(&convert, DegToRad(180), DegToRad(180), 0);

	for(i=0; i<m_NumJoints; i++){
		parent = -1;
		
		// find parent (if any)
		if(strlen(m_Joints[i].m_ParentName) > 0){
			for(j=0; j<m_NumJoints; j++){
				if(stricmp(m_Joints[j].m_Name, m_Joints[i].m_ParentName)== 0){
					parent = j;
					break;
				}
			}
			if(parent==-1) Game->LOG(0, "Warning: cannot find joint '%s' (model '%s')", m_Joints[i].m_ParentName, m_Filename);
		}
		Model->m_Joints[i].m_Parent = parent;

		// local rotation
		Model->m_Joints[i].m_LocalRotation.x =  m_Joints[i].m_Rotation[0];
		Model->m_Joints[i].m_LocalRotation.y =  m_Joints[i].m_Rotation[1];
		Model->m_Joints[i].m_LocalRotation.z =  m_Joints[i].m_Rotation[2];

		D3DXVec3TransformCoord(&Model->m_Joints[i].m_LocalRotation, &Model->m_Joints[i].m_LocalRotation, &convert);

		// local translation
		Model->m_Joints[i].m_LocalTranslation.x =  m_Joints[i].m_Translation[0];
		Model->m_Joints[i].m_LocalTranslation.y =  m_Joints[i].m_Translation[1];
		Model->m_Joints[i].m_LocalTranslation.z = -m_Joints[i].m_Translation[2];

		CBUtils::SetString(&Model->m_Joints[i].m_Name, m_Joints[i].m_Name);
	}

	// load anim info
	//Model->m_TotalTime = m_TotalTime;


	// load animations
	/*
	Model->m_NumAnimations = m_Animations.GetSize();
	if(Model->m_NumAnimations > 0)
		Model->m_Animations = new C3DSkelAnim[Model->m_NumAnimations];

	for(i=0; i<m_Animations.GetSize(); i++){
		C3DSkelAnim* anim = &Model->m_Animations[i];
		anim->SetName(m_Animations[i]->m_Name);
		anim->m_Looping = m_Animations[i]->m_Looping;

		anim->m_JointAnims = new C3DSkelAnim::C3DJointAnim[m_NumJoints];

		double StartTime = m_FrameTime * m_Animations[i]->m_StartFrame;
		double EndTime = m_FrameTime * m_Animations[i]->m_EndFrame;

		anim->m_TotalTime = EndTime - StartTime;

		int count;
		for(j=0; j<m_NumJoints; j++){
			//////////////////////////////////////////////////////////////////////////			
			// count valid translation keyframes
			for(k=0; k<m_Joints[j].m_NumTransKeyframes; k++){
				if(m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f <= EndTime)
					anim->m_JointAnims[j].m_NumTranslationKeyframes++;
			}
			// ...and load them
			if(anim->m_JointAnims[j].m_NumTranslationKeyframes > 0){
				anim->m_JointAnims[j].m_TranslationKeyframes = new C3DKeyframe[anim->m_JointAnims[j].m_NumTranslationKeyframes];
				count=0;
				for(k=0; k<m_Joints[j].m_NumTransKeyframes; k++){
					if(m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f <= EndTime){
						C3DKeyframe* Keyframe = &anim->m_JointAnims[j].m_TranslationKeyframes[count++];
						
						Keyframe->m_JointIndex = j;
						Keyframe->m_Time = m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f - StartTime;
			
						Keyframe->m_Parameter.x =  m_Joints[j].m_TranslationKeyframes[k].m_Parameter[0];
						Keyframe->m_Parameter.y =  m_Joints[j].m_TranslationKeyframes[k].m_Parameter[1];
						Keyframe->m_Parameter.z = -m_Joints[j].m_TranslationKeyframes[k].m_Parameter[2];
					}
				}
			}

			//////////////////////////////////////////////////////////////////////////
			// count valid rotation keyframes
			for(k=0; k<m_Joints[j].m_NumRotKeyframes; k++){
				if(m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f <= EndTime)
					anim->m_JointAnims[j].m_NumRotationKeyframes++;
			}
			// ...and load them
			if(anim->m_JointAnims[j].m_NumRotationKeyframes > 0){
				anim->m_JointAnims[j].m_RotationKeyframes = new C3DKeyframe[anim->m_JointAnims[j].m_NumRotationKeyframes];
				count=0;
				for(k=0; k<m_Joints[j].m_NumRotKeyframes; k++){
					if(m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f <= EndTime){
						C3DKeyframe* Keyframe = &anim->m_JointAnims[j].m_RotationKeyframes[count++];
						
						Keyframe->m_JointIndex = j;
						Keyframe->m_Time = m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f - StartTime;
			
						Keyframe->m_Parameter.x = m_Joints[j].m_RotationKeyframes[k].m_Parameter[0];
						Keyframe->m_Parameter.y = m_Joints[j].m_RotationKeyframes[k].m_Parameter[1];
						Keyframe->m_Parameter.z = m_Joints[j].m_RotationKeyframes[k].m_Parameter[2];

						D3DXVec3TransformCoord(&Keyframe->m_Parameter, &Keyframe->m_Parameter, &convert);
					}
				}
			}
		}

	}
	*/

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::LoadAnimation(C3DSkelAnim* Anim, int StartFrame, int EndFrame)
{
	int j, k;

	// conversion matrix from right-handed to left-handed
	D3DXMATRIX convert;
	D3DXMatrixRotationYawPitchRoll(&convert, DegToRad(180), DegToRad(180), 0);

	Anim->m_JointAnims = new C3DSkelAnim::C3DJointAnim[m_NumJoints];
	Anim->m_NumJointAnims = m_NumJoints;

	double StartTime = m_FrameTime * (StartFrame);
	double EndTime = m_FrameTime * (EndFrame);

	if(EndTime==0) EndTime = m_TotalTime;

	Anim->m_TotalTime = EndTime - StartTime;
	Anim->m_FrameTime = m_FrameTime;

	int count;
	for(j=0; j<m_NumJoints; j++)
	{
		//////////////////////////////////////////////////////////////////////////			
		// count valid translation keyframes
		for(k=0; k<m_Joints[j].m_NumTransKeyframes; k++){
			if(m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f <= EndTime)
				Anim->m_JointAnims[j].m_NumTranslationKeyframes++;
		}
		// ...and load them
		if(Anim->m_JointAnims[j].m_NumTranslationKeyframes > 0){
			Anim->m_JointAnims[j].m_TranslationKeyframes = new C3DKeyframe[Anim->m_JointAnims[j].m_NumTranslationKeyframes];
			count=0;
			for(k=0; k<m_Joints[j].m_NumTransKeyframes; k++){
				if(m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f <= EndTime){
					C3DKeyframe* Keyframe = &Anim->m_JointAnims[j].m_TranslationKeyframes[count++];

					Keyframe->m_JointIndex = j;
					Keyframe->m_Time = m_Joints[j].m_TranslationKeyframes[k].m_Time*1000.0f - StartTime;

					Keyframe->m_Parameter.x =  m_Joints[j].m_TranslationKeyframes[k].m_Parameter[0];
					Keyframe->m_Parameter.y =  m_Joints[j].m_TranslationKeyframes[k].m_Parameter[1];
					Keyframe->m_Parameter.z = -m_Joints[j].m_TranslationKeyframes[k].m_Parameter[2];
				}
			}
		}

		//////////////////////////////////////////////////////////////////////////
		// count valid rotation keyframes
		for(k=0; k<m_Joints[j].m_NumRotKeyframes; k++){
			if(m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f <= EndTime)
				Anim->m_JointAnims[j].m_NumRotationKeyframes++;
		}
		// ...and load them
		if(Anim->m_JointAnims[j].m_NumRotationKeyframes > 0){
			Anim->m_JointAnims[j].m_RotationKeyframes = new C3DKeyframe[Anim->m_JointAnims[j].m_NumRotationKeyframes];
			count=0;
			for(k=0; k<m_Joints[j].m_NumRotKeyframes; k++){
				if(m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f >= StartTime && m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f <= EndTime){
					C3DKeyframe* Keyframe = &Anim->m_JointAnims[j].m_RotationKeyframes[count++];

					Keyframe->m_JointIndex = j;
					Keyframe->m_Time = m_Joints[j].m_RotationKeyframes[k].m_Time*1000.0f - StartTime;

					Keyframe->m_Parameter.x = m_Joints[j].m_RotationKeyframes[k].m_Parameter[0];
					Keyframe->m_Parameter.y = m_Joints[j].m_RotationKeyframes[k].m_Parameter[1];
					Keyframe->m_Parameter.z = m_Joints[j].m_RotationKeyframes[k].m_Parameter[2];
					

					D3DXVec3TransformCoord(&Keyframe->m_Parameter, &Keyframe->m_Parameter, &convert);
				}
			}
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::LoadMesh(C3DMesh* Mesh, MS3DGroup* MSGroup, int MaterialOffset, int BoneID)
{
	int i, j;

	Mesh->m_NumFaces = MSGroup->m_NumTriangles;
	Mesh->m_NumVertices = Mesh->m_NumFaces * 3;

	Mesh->m_Faces = new C3DFace[Mesh->m_NumFaces];
	Mesh->m_Vertices = new C3DVertex[Mesh->m_NumVertices];


	// for each face
	for(i=0; i<MSGroup->m_NumTriangles; i++){
		C3DFace* ModFace = &Mesh->m_Faces[i];
		MS3DTriangle* MSFace = &m_Faces[MSGroup->m_TriangleIndices[i]];

		// for each vertex
		int k;
		for(j=0; j<3; j++){
			k = j;
			if(k==1) k = 2;
			else if(k==2) k = 1;

			int iVertex = i*3+k;

			// vertices
			Mesh->m_Vertices[iVertex].m_Pos.x =  m_Vertices[MSFace->m_VertexIndices[j]].m_Vertex[0];
			Mesh->m_Vertices[iVertex].m_Pos.y =  m_Vertices[MSFace->m_VertexIndices[j]].m_Vertex[1];
			Mesh->m_Vertices[iVertex].m_Pos.z = -m_Vertices[MSFace->m_VertexIndices[j]].m_Vertex[2];

			// override bone id?
			if(BoneID>=0) Mesh->m_Vertices[iVertex].m_BoneID = BoneID;
			else Mesh->m_Vertices[iVertex].m_BoneID = m_Vertices[MSFace->m_VertexIndices[j]].m_BoneID;

			// texture coordinates
			ModFace->m_u[k] = MSFace->m_s[j];
			if(MSFace->m_t[j]<0) ModFace->m_v[k] = 1+MSFace->m_t[j];
			else ModFace->m_v[k] = MSFace->m_t[j];

			// normals
			ModFace->m_Normals[k].x =  MSFace->m_VertexNormals[j][0];
			ModFace->m_Normals[k].y =  MSFace->m_VertexNormals[j][1];
			ModFace->m_Normals[k].z = -MSFace->m_VertexNormals[j][2];

			// vertex indices
			ModFace->m_Vertices[k] = iVertex;
		}
	}

	// misc properties
	Mesh->m_MaterialIndex = MSGroup->m_MaterialIndex + MaterialOffset;
	Mesh->SetName(MSGroup->m_Name);
	Mesh->m_Visible = (MSGroup->m_Flags & 0x02)==0;

	Mesh->CreateVertexBuffer();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoaderMS3D::LoadMaterial(C3DMaterial* Material, MS3DMaterial* MSMaterial)
{
	// texture relative path base
	char PathTemp[MAX_PATH+1];
	int PathLength;
	for (PathLength = strlen(m_Filename); PathLength--; ) {
		if(m_Filename[PathLength] == '/' || m_Filename[PathLength] == '\\' ) break;
	}
	strncpy(PathTemp, m_Filename, PathLength );

	// colors
	memcpy(&Material->m_Material.Ambient,  MSMaterial->m_Ambient,  4*sizeof(float));
	memcpy(&Material->m_Material.Diffuse,  MSMaterial->m_Diffuse,  4*sizeof(float));
	memcpy(&Material->m_Material.Specular, MSMaterial->m_Specular, 4*sizeof(float));
	memcpy(&Material->m_Material.Emissive, MSMaterial->m_Emissive, 4*sizeof(float));
	Material->m_Material.Power = MSMaterial->m_Shininess;

	// textures
	if(strncmp(MSMaterial->m_Texture, ".\\", 2) == 0){
		// MS3D 1.5.x relative path
		strcpy(PathTemp + PathLength, MSMaterial->m_Texture + 1);
		Material->SetTexture(PathTemp);
	}
	else if(strchr(MSMaterial->m_Texture, '\\')==NULL)
	{
		// MS3D 1.7.x no slashes
		strcpy(PathTemp + PathLength, "\\");
		strcpy(PathTemp + PathLength + 1, MSMaterial->m_Texture);
		Material->SetTexture(PathTemp);
	}
	else {
		// MS3D 1.4.x or earlier - absolute path
		Material->SetTexture(MSMaterial->m_Texture);
	}

	return S_OK;
}
