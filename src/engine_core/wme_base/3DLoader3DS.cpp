// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DLoader3DS.h"
#include "3DS_chunks.h"


//////////////////////////////////////////////////////////////////////////
C3DLoader3DS::C3DLoader3DS(CBGame* inGame):CBNamedObject(inGame)
{
	m_Filename = NULL;
	D3DXMatrixIdentity(&m_ViewMatrix);
}


//////////////////////////////////////////////////////////////////////////
C3DLoader3DS::~C3DLoader3DS()
{
	int i;

	for(i=0; i<m_Objects.GetSize(); i++) delete m_Objects[i];
	m_Objects.RemoveAll();

	SAFE_DELETE_ARRAY(m_Filename);
}



//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////
C3DLoader3DS::C3DFileObject3DS::C3DFileObject3DS()
{
	m_Name = NULL;
	m_Type = OBJ_3DS_NONE;

	m_NumVertices = 0;
	m_Vertices = NULL;

	m_NumFaces = 0;
	m_Faces = NULL;

	m_NumCoordinates = 0;
	m_Coordinates = NULL;

	D3DXMatrixIdentity(&m_LocalMatrix);

	m_LocalOrigin.x = m_LocalOrigin.y = m_LocalOrigin.z = 0;
	m_LightOff = false;
	m_LightSpotlight = false;
}


//////////////////////////////////////////////////////////////////////
C3DLoader3DS::C3DFileObject3DS::~C3DFileObject3DS()
{
	if(m_Name != NULL) delete [] m_Name;
	if(m_Vertices != NULL) delete [] m_Vertices;
	if(m_Faces != NULL) delete [] m_Faces;
	if(m_Coordinates != NULL) delete [] m_Coordinates;


	for(int i=0; i<m_MaterialFaces.GetSize(); i++)
	{
		delete [] m_MaterialFaces[i]->Material;
		delete [] m_MaterialFaces[i]->Faces;
		delete m_MaterialFaces[i];
	}
	m_MaterialFaces.RemoveAll();
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoader3DS::ParseFile(char *Filename)
{
	SAFE_DELETE_ARRAY(m_Filename);
	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	BYTE *pBuffer;
	DWORD fileSize;

	pBuffer = Game->m_FileManager->ReadWholeFile(Filename, &fileSize);

	if(!pBuffer) return E_FAIL;


	BYTE *pPtr = pBuffer;
	
	WORD chunk_id;
	DWORD chunk_length;
	BYTE* next_offset;
	WORD KeyframerSection = 0;
	char* KeyframerObject = NULL;

	C3DFileObject3DS* obj = NULL;
	WORD i;

	while(pPtr < pBuffer+fileSize)
	{
		chunk_id = *(WORD*)pPtr;
		pPtr += sizeof(WORD);

		chunk_length = *(DWORD*)pPtr;
		pPtr += sizeof(DWORD);

		next_offset = pPtr - 6 + chunk_length;

		bool handled = true;

		switch(chunk_id)
		{
			case MAIN3DS:
			break;

			case EDIT3DS:
			break;

			//////////////////////////////////////////////////////////////////////
			// object ////////////////////////////////////////////////////////////
			case NAMED_OBJECT:
				obj = new C3DFileObject3DS;
				obj->m_Name = new char [strlen((char*)pPtr)+1];
				m_Objects.Add(obj);
				strcpy(obj->m_Name, (char*)pPtr);
				pPtr += strlen(obj->m_Name)+1;
			break;


			// mesh //////////////////////////////////////////////////////////////
			case TRIANGLE_MESH:
				if(obj==NULL) break;
				obj->m_Type = OBJ_3DS_MESH;
			break;


			case TRIANGLE_VERTEXLIST:
				if(obj==NULL || obj->m_Type != OBJ_3DS_MESH) break;

				obj->m_NumVertices = *(WORD*) pPtr;
				pPtr += sizeof(WORD);

				obj->m_Vertices = new D3DXVECTOR3[obj->m_NumVertices];
				for(i=0; i<obj->m_NumVertices; i++){
					obj->m_Vertices[i].x = *(float*)pPtr;
					pPtr += sizeof(float);

					obj->m_Vertices[i].z = *(float*)pPtr;
					pPtr += sizeof(float);

					obj->m_Vertices[i].y = *(float*)pPtr;
					pPtr += sizeof(float);

					//obj->m_Vertices[i].y = -obj->m_Vertices[i].y;
				}
			break;


			case TRIANGLE_MAPPINGCOORS:
				if(obj==NULL || obj->m_Type != OBJ_3DS_MESH) break;

				obj->m_NumCoordinates = *(WORD*) pPtr;
				pPtr += sizeof(WORD);
				
				obj->m_Coordinates = new STextureCoordinate[obj->m_NumCoordinates];
				for(i=0; i<obj->m_NumCoordinates; i++){
					obj->m_Coordinates[i].u = *(float*)pPtr;
					pPtr += sizeof(float);

					obj->m_Coordinates[i].v = *(float*)pPtr;
					pPtr += sizeof(float);

					// correct coordinates
					obj->m_Coordinates[i].v = 1.0f - obj->m_Coordinates[i].v;
				}
			break;


			case TRIANGLE_FACELIST:
				if(obj==NULL || obj->m_Type != OBJ_3DS_MESH) break;

				obj->m_NumFaces = *(WORD*) pPtr;
				pPtr += sizeof(WORD);

				obj->m_Faces = new SFace[obj->m_NumFaces];
				for(i=0; i<obj->m_NumFaces; i++){
					obj->m_Faces[i].a = *(WORD*)pPtr;
					pPtr += sizeof(WORD);

					obj->m_Faces[i].c = *(WORD*)pPtr;
					pPtr += sizeof(WORD);

					obj->m_Faces[i].b = *(WORD*)pPtr;
					pPtr += sizeof(WORD);

					WORD dummy_word;
					dummy_word = *(WORD*)pPtr;
					pPtr += sizeof(WORD);
				}
			break;


			case TRIANGLE_MATERIAL:
				if(obj==NULL || obj->m_Type != OBJ_3DS_MESH) break;

				{
					SMaterialFaces* mf = new SMaterialFaces;

					mf->Material = new char [strlen((char*)pPtr)+1];
					strcpy(mf->Material, (char*)pPtr);
					pPtr += strlen(mf->Material)+1;

					mf->NumFaces = *(WORD*)pPtr;
					pPtr += sizeof(WORD);

					mf->Faces = new WORD [mf->NumFaces];

					for(i=0; i<mf->NumFaces; i++){
						mf->Faces[i] = *(WORD*)pPtr;
						pPtr += sizeof(WORD);
					}
					obj->m_MaterialFaces.Add(mf);
				}
			break;


			case TRI_LOCAL:
				if(obj==NULL || obj->m_Type != OBJ_3DS_MESH) break;
				{

					obj->m_LocalMatrix(0,0) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(0,1) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(0,2) = *(float*)pPtr; pPtr += sizeof(float);

					obj->m_LocalMatrix(1,0) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(1,1) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(1,2) = *(float*)pPtr; pPtr += sizeof(float);
					
					obj->m_LocalMatrix(2,0) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(2,1) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(2,2) = *(float*)pPtr; pPtr += sizeof(float);

					obj->m_LocalMatrix(3,0) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(3,1) = *(float*)pPtr; pPtr += sizeof(float);
					obj->m_LocalMatrix(3,2) = *(float*)pPtr; pPtr += sizeof(float);


					/*
					D3DXMATRIX matR, matS;
					D3DXMatrixRotationX(&matR, D3DXToRadian(-90));
					D3DXMatrixScaling(&matS, 1, 1, -1);
					
					D3DXMatrixMultiply(&obj->m_LocalMatrix, &matS, &obj->m_LocalMatrix);
					D3DXMatrixMultiply(&obj->m_LocalMatrix, &matR, &obj->m_LocalMatrix);
					*/
				}
			break;

			// camera //////////////////////////////////////////////////////////////
			case CHUNK_CAMERA:
				if(obj==NULL) break;
				obj->m_Type = OBJ_3DS_CAMERA;

				obj->m_CameraPos.x = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_CameraPos.z = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_CameraPos.y = *(float*)pPtr;
				pPtr += sizeof(float);

				obj->m_CameraTarget.x = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_CameraTarget.z = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_CameraTarget.y = *(float*)pPtr;
				pPtr += sizeof(float);				
				
				obj->m_CameraBank = *(float*)pPtr;
				pPtr += sizeof(float);

				obj->m_CameraLens = *(float*)pPtr;
				//if(obj->m_CameraLens > 0) obj->m_CameraFOV = 2400.0f / obj->m_CameraLens;
				//if(obj->m_CameraLens > 0) obj->m_CameraFOV = 1900.0f / obj->m_CameraLens;
				if(obj->m_CameraLens > 0) obj->m_CameraFOV = 1900.0f / obj->m_CameraLens;
				else obj->m_CameraFOV = 45.0f;

				pPtr += sizeof(float);
			break;


			// light //////////////////////////////////////////////////////////////
			case CHUNK_LIGHT:
				if(obj==NULL) break;
				obj->m_Type = OBJ_3DS_LIGHT;

				obj->m_LightPos.x = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_LightPos.z = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_LightPos.y = *(float*)pPtr;
				pPtr += sizeof(float);
			break;

			case LIGHT_SPOTLIGHT:
				if(obj==NULL || obj->m_Type != OBJ_3DS_LIGHT) break;

				obj->m_LightTarget.x = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_LightTarget.z = *(float*)pPtr;
				pPtr += sizeof(float);
				obj->m_LightTarget.y = *(float*)pPtr;
				pPtr += sizeof(float);				

				obj->m_LightHotspot = *(float*)pPtr;
				pPtr += sizeof(float);				

				obj->m_LightFalloff = *(float*)pPtr;
				pPtr += sizeof(float);

				obj->m_LightSpotlight = true;
			break;

			case LIGHT_IS_OFF:
				if(obj==NULL || obj->m_Type != OBJ_3DS_LIGHT) break;

				/*
				obj->m_LightOff = *(bool*)pPtr;
				pPtr += sizeof(bool);
				*/
				obj->m_LightOff = true;
			break;


			// colors ////////////////////////////////////////////////////////////////////////
			case RGB_FLOAT:
				if(obj && obj->m_Type==OBJ_3DS_LIGHT)
				{
					float R, G, B;
					R = *(float*)pPtr;
					pPtr += sizeof(float);

					G = *(float*)pPtr;
					pPtr += sizeof(float);

					B = *(float*)pPtr;
					pPtr += sizeof(float);

					obj->m_LightColor = DRGBA((int)(R*255), (int)(G*255), (int)(B*255), 255);
				}
				else handled = false;
			break;

			case RGB_BYTE:
				if(obj && obj->m_Type==OBJ_3DS_LIGHT)
				{
					BYTE R, G, B;
					R = *(BYTE*)pPtr;
					pPtr += sizeof(BYTE);

					G = *(BYTE*)pPtr;
					pPtr += sizeof(BYTE);

					B = *(BYTE*)pPtr;
					pPtr += sizeof(BYTE);

					obj->m_LightColor = DRGBA(R, G, B, 255);
				}
				else handled = false;
				break;

				// keyframer stuff
				case KEYFRAMER:
				break;

				case KEYFRAMER_AMBIENT_INFO:
				case KEYFRAMER_MESH_INFO:
				case KEYFRAMER_CAMERA_INFO:
				case KEYFRAMER_CAMERA_TARGET_INFO:
				case KEYFRAMER_OMNI_INFO:
				case KEYFRAMER_SPOT_TARGET_INFO:
				case KEYFRAMER_SPOT_INFO:
					KeyframerSection = chunk_id;
				break;

				case NODE_HDR:
					// object name
					SAFE_DELETE_ARRAY(KeyframerObject);
					KeyframerObject = new char [strlen((char*)pPtr)+1];
					strcpy(KeyframerObject, (char*)pPtr);
					pPtr += strlen(KeyframerObject)+1;

					// flags1
					pPtr += sizeof(WORD);

					// flags2
					pPtr += sizeof(WORD);

					// hierarchy
					pPtr += sizeof(WORD);
				break;

				case ROLL_TRACK_TAG:
					if(KeyframerSection==KEYFRAMER_CAMERA_INFO && KeyframerObject!=NULL)
					{
						// flags
						pPtr += sizeof(WORD);

						// unknown
						pPtr += sizeof(WORD);
						pPtr += sizeof(WORD);
						pPtr += sizeof(WORD);
						pPtr += sizeof(WORD);

						// keys
						WORD Keys = *(WORD*)pPtr;
						pPtr += sizeof(WORD);

						// unknown
						pPtr += sizeof(WORD);

						for(WORD Key=0; Key<Keys; Key++)
						{
							// frame number
							pPtr += sizeof(WORD);

							// unknown
							pPtr += sizeof(DWORD);

							// camera roll
							float CameraRoll = *(float*)pPtr;
							pPtr += sizeof(float);

							// inject this roll value to the camera
							if(Key==0)
							{
								for(int i=0; i<m_Objects.GetSize(); i++)
								{
									C3DFileObject3DS* Obj = m_Objects[i];
									if(m_Objects[i]->m_Type==OBJ_3DS_CAMERA && stricmp(m_Objects[i]->m_Name, KeyframerObject)==0)
									{
										m_Objects[i]->m_CameraBank = CameraRoll;
										break;
									}
								}
							}
						}

					}
					else handled = false;
				break;
			

			default:
				handled = false;
		} // switch(chunk_id)

		if(!handled) pPtr = next_offset;
	}

	SAFE_DELETE_ARRAY(KeyframerObject);

	delete [] pBuffer;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int C3DLoader3DS::GetNumMeshes()
{
	int ret = 0;

	for(int i=0; i<m_Objects.GetSize(); i++) if(m_Objects[i]->m_Type == OBJ_3DS_MESH) ret++;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
char* C3DLoader3DS::GetMeshName(int Index)
{
	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_MESH) pos++;
		if(pos==Index) return m_Objects[i]->m_Name;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoader3DS::LoadMesh(int Index, C3DMesh *Mesh)
{
	if(!Mesh) return E_POINTER;

	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_MESH) pos++;
		if(pos==Index){
			C3DFileObject3DS* obj = m_Objects[i];
			Mesh->Cleanup();

			Mesh->m_NumVertices = obj->m_NumVertices;
			Mesh->m_NumFaces = obj->m_NumFaces;

			int j;

			Mesh->m_Vertices = new C3DVertex[Mesh->m_NumVertices];
			for(j=0; j<Mesh->m_NumVertices; j++){
				Mesh->m_Vertices[j].m_Pos.x = obj->m_Vertices[j].x;
				Mesh->m_Vertices[j].m_Pos.y = obj->m_Vertices[j].y;
				Mesh->m_Vertices[j].m_Pos.z = obj->m_Vertices[j].z;
			}

			Mesh->m_Faces = new C3DFace[Mesh->m_NumFaces];
			for(j=0; j<Mesh->m_NumFaces; j++){
				Mesh->m_Faces[j].m_Vertices[0] = obj->m_Faces[j].a;
				Mesh->m_Faces[j].m_Vertices[1] = obj->m_Faces[j].b;
				Mesh->m_Faces[j].m_Vertices[2] = obj->m_Faces[j].c;
			}

			Mesh->SetName(obj->m_Name);

			return S_OK;
		}
	}
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int C3DLoader3DS::GetNumLights()
{
	int ret = 0;

	for(int i=0; i<m_Objects.GetSize(); i++) if(m_Objects[i]->m_Type == OBJ_3DS_LIGHT) ret++;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
char* C3DLoader3DS::GetLightName(int Index)
{
	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_LIGHT) pos++;
		if(pos==Index) return m_Objects[i]->m_Name;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoader3DS::LoadLight(int Index, C3DLight *Light)
{
	if(!Light) return E_POINTER;

	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_LIGHT)
		{
			pos++;
			if(pos==Index)
			{
				Light->SetName(m_Objects[i]->m_Name);
				Light->m_Pos = m_Objects[i]->m_LightPos;
				Light->m_Target = m_Objects[i]->m_LightTarget;
				Light->m_IsSpotlight = m_Objects[i]->m_LightSpotlight;
				Light->m_Active = !m_Objects[i]->m_LightOff;
				Light->m_DiffuseColor = m_Objects[i]->m_LightColor;
				Light->m_Falloff = m_Objects[i]->m_LightFalloff;
			}
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
int C3DLoader3DS::GetNumCameras()
{
	int ret = 0;

	for(int i=0; i<m_Objects.GetSize(); i++) if(m_Objects[i]->m_Type == OBJ_3DS_CAMERA) ret++;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
char* C3DLoader3DS::GetCameraName(int Index)
{
	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_CAMERA) pos++;
		if(pos==Index) return m_Objects[i]->m_Name;
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DLoader3DS::LoadCamera(int Index, C3DCamera *Camera)
{
	if(!Camera) return E_POINTER;

	int pos = -1;
	for(int i=0; i<m_Objects.GetSize(); i++){
		if(m_Objects[i]->m_Type == OBJ_3DS_CAMERA) pos++;
		if(pos==Index){
			Camera->SetupPos(m_Objects[i]->m_CameraPos, m_Objects[i]->m_CameraTarget, m_Objects[i]->m_CameraBank);
			Camera->SetName(m_Objects[i]->m_Name);
			Camera->m_FOV = Camera->m_OrigFOV = DegToRad(m_Objects[i]->m_CameraFOV);

			return S_OK;
		}
	}
	return E_FAIL;
}
