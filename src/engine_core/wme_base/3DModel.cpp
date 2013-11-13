// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DModel.h"
#include "3DUtils.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_PERSISTENT(C3DModel, false);

//////////////////////////////////////////////////////////////////////////
C3DModel::C3DModel(CBGame* inGame, CBObject* Owner):CBObject(inGame)
{
	m_Owner = Owner;
	SetDefaults();
}


//////////////////////////////////////////////////////////////////////////
C3DModel::~C3DModel()
{
	Cleanup();
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::SetDefaults()
{
	m_NumMeshes = m_NumJoints = m_NumMaterials = 0;
	m_Joints = NULL;
	
	for(int i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)
	{
		m_CurrAnims[i] = NULL;
		m_StartTime[i] = 0;
	}

	m_BBoxStart = m_BBoxEnd = D3DXVECTOR3(0,0,0);
	SetRectEmpty(&m_BoundingRect);

	memset(&m_DrawingViewport, 0, sizeof(D3DVIEWPORT));

	D3DXMatrixIdentity(&m_LastWorldMat);
	D3DXMatrixIdentity(&m_LastViewMat);
	D3DXMatrixIdentity(&m_LastProjMat);

	m_LastOffsetX = m_LastOffsetY = 0;
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::Cleanup()
{
	int i;

	ClearLoaderCache();

	for(i=0; i<m_Meshes.GetSize(); i++)	delete m_Meshes[i];
	m_Meshes.RemoveAll();

	for(i=0; i<m_Materials.GetSize(); i++)	delete m_Materials[i];
	m_Materials.RemoveAll();

	SAFE_DELETE_ARRAY(m_Joints);

	for(i=0; i<m_Animations.GetSize(); i++)	delete m_Animations[i];
	m_Animations.RemoveAll();

	for(i=0; i<m_AttachedMeshes.GetSize(); i++)	delete m_AttachedMeshes[i];
	m_AttachedMeshes.RemoveAll();

	for(i=0; i<m_DeletedMeshes.GetSize(); i++)	delete m_DeletedMeshes[i];
	m_DeletedMeshes.RemoveAll();


	for(i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)
		m_CurrAnims[i] = NULL; // ref only

	SetDefaults();
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::ClearLoaderCache()
{
	for(int i=0; i<m_LoaderCache.GetSize(); i++)
	{
		delete m_LoaderCache[i];
	}
	m_LoaderCache.RemoveAll();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::LoadFile(char *Filename)
{
	Cleanup();

	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);

	if(_stricmp(ext, ".ms3d")==0)
	{
		C3DLoaderMS3D* loader = ParseFileMS3D(Filename);
		if(!loader) return E_FAIL;

		loader->LoadModel(this);
	}
	else{
		Game->LOG(0, "Error: no suitable loader found for file '%s'", Filename);
		return E_FAIL;
	}

	if(m_Filename!=Filename) CBUtils::SetString(&m_Filename, Filename);


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::LoadFinalize()
{
	ClearLoaderCache();
	AnimSetupJoints();
	AnimRestart();
	
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::Update()
{
	HRESULT res;

	// update joints
	AnimAdvance();

	// fill morphed vertex buffers
	if(FAILED(res=PrepareMeshes())) return res;

	UpdateBoundingRect();
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::Render()
{
	HRESULT res;
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	// set culling
	if(m_Owner && !m_Owner->m_DrawBackfaces)
		m_Renderer->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	else
		m_Renderer->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	C3DUtils::SetFixedVertexShader(m_Renderer->m_Device, D3DFVF_MODELVERTEX);

	// Render each VB
	for(int i=0; i<m_NumMeshes; i++)
	{
		if(!m_Meshes[i]->m_Visible) continue;

		// set material
		if(m_Meshes[i]->m_MaterialIndex>=0 && m_Meshes[i]->m_MaterialIndex < m_Materials.GetSize())
		{
			m_Renderer->m_Device->SetMaterial(&m_Materials[m_Meshes[i]->m_MaterialIndex]->m_Material);

			CBSurfaceD3D* surf = (CBSurfaceD3D*)m_Materials[m_Meshes[i]->m_MaterialIndex]->GetSurface();
			if(surf && surf->m_Texture){
				//D3DXSaveTextureToFile("c:\\!test.bmp", D3DXIFF_BMP, surf->m_Texture, NULL);
				m_Renderer->m_Device->SetTexture(0, surf->m_Texture);				
			}
			else m_Renderer->m_Device->SetTexture(0, NULL);
		}
		else m_Renderer->m_Device->SetTexture(0, NULL);


		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Meshes[i]->m_VB, sizeof(MODELVERTEX));
		res = m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Meshes[i]->m_NumFaces);
		if(FAILED(res)){
			Game->LOG(res, "Error rendering model '%s' (mesh %d)", m_Filename, i);
			return res;
		}

		m_Renderer->m_NumPolygons += m_Meshes[i]->m_NumFaces;
	}
	m_Renderer->m_Device->GetTransform(D3DTS_WORLD, &m_LastWorldMat);
	m_Renderer->m_Device->GetTransform(D3DTS_VIEW, &m_LastViewMat);
	m_Renderer->m_Device->GetTransform(D3DTS_PROJECTION, &m_LastProjMat);

	RECT rc;
	bool CustomViewport;
	Game->GetCurrentViewportRect(&rc, &CustomViewport);
	float Width = (float)rc.right - (float)rc.left;
	float Height = (float)rc.bottom - (float)rc.top;

	// margins
	int mleft = rc.left;
	int mright = m_Renderer->m_Width - Width - rc.left;
	int mtop = rc.top;
	int mbottom = m_Renderer->m_Height - Height - rc.top;


	m_LastOffsetX = Game->m_OffsetX + (mleft - mright)/2;
	m_LastOffsetY = Game->m_OffsetY + (mtop - mbottom)/2;


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::PrepareMeshes()
{
	HRESULT res;

	m_BBoxStart = D3DXVECTOR3(FLT_MAX, FLT_MAX, FLT_MAX);
	m_BBoxEnd = D3DXVECTOR3(FLT_MIN, FLT_MIN, FLT_MIN);

	// fill vertex buffers
	for (int i = 0; i < m_NumMeshes; i++ ){
		if(!m_Meshes[i]->m_Visible) continue;

		MODELVERTEX* Verts;
		if(FAILED(res = m_Meshes[i]->m_VB->LockVB(0, 0, &Verts, 0))){
			Game->LOG(0, "Error locking vertex buffer");
			return res;
		}

		for (int j = 0; j < m_Meshes[i]->m_NumFaces; j++){
			const C3DFace* Face = &m_Meshes[i]->m_Faces[j];

			// For each vert on this face
			for(int k=0; k<3; k++){

				int iVert;
				float u, v;
				D3DXVECTOR3 Normal;

				iVert = Face->m_Vertices[k];
				Normal = Face->m_Normals[k];

				u = Face->m_u[k];
				v = Face->m_v[k];

				// Get the index of the vertex we're editing
				int iOutVert = j*3+k;

				Verts[iOutVert].u = u;
				Verts[iOutVert].v = v;

				// no bone, just copy verbatim
				if(m_Meshes[i]->m_Vertices[iVert].m_BoneID == -1){
					// Set this vertex's data
					Verts[iOutVert].x  = m_Meshes[i]->m_Vertices[iVert].m_Pos.x;
					Verts[iOutVert].y  = m_Meshes[i]->m_Vertices[iVert].m_Pos.y;
					Verts[iOutVert].z  = m_Meshes[i]->m_Vertices[iVert].m_Pos.z;

					
					Verts[iOutVert].nx = Normal.x;
					Verts[iOutVert].ny = Normal.y;
					Verts[iOutVert].nz = Normal.z;
				}
				else{
					D3DXMATRIX MatFinal = m_Joints[m_Meshes[i]->m_Vertices[iVert].m_BoneID].m_Final;


					D3DXVECTOR4 NewVertex;
					D3DXVec3Transform(&NewVertex, &m_Meshes[i]->m_Vertices[iVert].m_Pos, &MatFinal);

					Verts[iOutVert].x  = NewVertex.x;
					Verts[iOutVert].y  = NewVertex.y;
					Verts[iOutVert].z  = NewVertex.z;

					D3DXVECTOR4 NewNormal;
					MatFinal._41 = MatFinal._42 = MatFinal._43 = 0;
					D3DXVec3Transform(&NewNormal, &Normal, &MatFinal);
					D3DXVec3Normalize((D3DXVECTOR3*)&NewNormal, (D3DXVECTOR3*)&NewNormal);
					
					Verts[iOutVert].nx = NewNormal.x;
					Verts[iOutVert].ny = NewNormal.y;
					Verts[iOutVert].nz = NewNormal.z;
				}

				m_BBoxStart.x = min(m_BBoxStart.x, Verts[iOutVert].x);
				m_BBoxStart.y = min(m_BBoxStart.y, Verts[iOutVert].y);
				m_BBoxStart.z = min(m_BBoxStart.z, Verts[iOutVert].z);
				
				m_BBoxEnd.x = max(m_BBoxEnd.x, Verts[iOutVert].x);
				m_BBoxEnd.y = max(m_BBoxEnd.y, Verts[iOutVert].y);
				m_BBoxEnd.z = max(m_BBoxEnd.z, Verts[iOutVert].z);
			}
		}
		m_Meshes[i]->m_VB->Unlock();
	}
	return S_OK;

}


//////////////////////////////////////////////////////////////////////////
void C3DModel::AnimSetupJoints()
{
	int i,j;

	// set joint matrices
	for (i=0; i<m_NumJoints; i++){

		C3DJoint* joint = &m_Joints[i];

		D3DXMatrixIdentity(&joint->m_Relative);
		C3DUtils::MatrixSetRotation(&joint->m_Relative, &joint->m_LocalRotation);
		C3DUtils::MatrixSetTranslation(&joint->m_Relative, &joint->m_LocalTranslation);

		if(joint->m_Parent != -1){
			D3DXMatrixMultiply(&joint->m_Absolute, &joint->m_Relative, &m_Joints[joint->m_Parent].m_Absolute); // poradi???
		}
		else{
			joint->m_Absolute = joint->m_Relative;
		}
  }

	float det;

	// setup vertices and normals
	for(i=0; i<m_NumMeshes; i++){

		// vertices
		for(j=0; j<m_Meshes[i]->m_NumVertices; j++){
			C3DVertex* vertex = &m_Meshes[i]->m_Vertices[j];
			if(vertex->m_BoneID != -1){
				D3DXVECTOR4 NewVertex;
				D3DXMATRIX InvAbsolute;
				D3DXMatrixInverse(&InvAbsolute, &det, &m_Joints[vertex->m_BoneID].m_Absolute);
				D3DXVec3Transform(&NewVertex, &vertex->m_Pos, &InvAbsolute); // ???

				vertex->m_Pos.x = NewVertex.x;
				vertex->m_Pos.y = NewVertex.y;
				vertex->m_Pos.z = NewVertex.z;
			}
		}

		// normals
		for(j=0; j<m_Meshes[i]->m_NumFaces; j++){
			C3DFace* face = &m_Meshes[i]->m_Faces[j];
			for(int k=0; k<3; k++){
				C3DVertex* vertex = &m_Meshes[i]->m_Vertices[face->m_Vertices[k]];
				if(vertex->m_BoneID != -1){
					
					D3DXVECTOR4 NewNormal;
					D3DXMATRIX InvAbsolute;
					D3DXMatrixInverse(&InvAbsolute, &det, &m_Joints[vertex->m_BoneID].m_Absolute);
					InvAbsolute._41 = InvAbsolute._42 = InvAbsolute._43 = 0;
					D3DXVec3Transform(&NewNormal, &face->m_Normals[k], &InvAbsolute);

					face->m_Normals[k].x = NewNormal.x;
					face->m_Normals[k].y = NewNormal.y;
					face->m_Normals[k].z = NewNormal.z;
				}
			}
		}
	}
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::AnimRestart()
{
	for(int i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)
		AnimRestart(i);
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::AnimRestart(int Channel)
{
	for (int i=0; i<m_NumJoints; i++){
		m_Joints[i].m_CurrentRotationKeyframes[Channel] = m_Joints[i].m_CurrentTranslationKeyframes[Channel] = 0;
		m_Joints[i].m_Final =  m_Joints[i].m_Absolute;
	}

	m_StartTime[Channel] = Game->m_Timer;
	if(m_CurrAnims[Channel]) m_CurrAnims[Channel]->m_Finished = false;
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::AnimAdvance()
{
	int Channel;

	bool FoundAny = false;
	for(Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
	{
		if(m_CurrAnims[Channel])
		{
			m_CurrAnims[Channel]->m_Finished = false;
			FoundAny = true;
		}
	}
	if(!FoundAny) return;


	double time[NUM_SKEL_ANIM_CHANNELS];

	// end of animation
	for(Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
	{
		if(!m_CurrAnims[Channel]) continue;

		time[Channel] = (double)(Game->m_Timer - m_StartTime[Channel]);
	
		if(time[Channel] > m_CurrAnims[Channel]->m_TotalTime){
			if(m_CurrAnims[Channel]->m_Looping){
				AnimRestart(Channel);
				time[Channel] = 0;
			}
			else {
				m_CurrAnims[Channel]->m_Finished = true;
				time[Channel] = m_CurrAnims[Channel]->m_TotalTime;
				if(Channel!=0) m_CurrAnims[Channel] = NULL;
			}
		}
	} 

	

	for(int i=0; i<m_NumJoints; i++)
	{
		D3DXVECTOR3 TransVec[NUM_SKEL_ANIM_CHANNELS];
		D3DXVECTOR3 RotVec[NUM_SKEL_ANIM_CHANNELS];

		C3DJoint* joint = &m_Joints[i];

		for(Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
		{
			// only used channels
			if(!m_CurrAnims[Channel])
			{
				TransVec[Channel] = D3DXVECTOR3(0, 0, 0);
				RotVec[Channel] = D3DXVECTOR3(0, 0, 0);
				continue;
			}

			int frame;			
			C3DSkelAnim::C3DJointAnim* anim = NULL;
			if(i<m_CurrAnims[Channel]->m_NumJointAnims)
			{
				anim = &m_CurrAnims[Channel]->m_JointAnims[i];
			}

			if(!anim || (anim->m_NumRotationKeyframes==0 && anim->m_NumTranslationKeyframes==0)){
				TransVec[Channel] = D3DXVECTOR3(0, 0, 0);
				RotVec[Channel] = D3DXVECTOR3(0, 0, 0);
				//joint->m_Final = joint->m_Absolute;
				continue;
			}

			// animation events
			if(m_CurrAnims[Channel]->m_CurrentFrame != (int)(time[Channel] / m_CurrAnims[Channel]->m_FrameTime))
			{
				m_CurrAnims[Channel]->m_CurrentFrame = (int)(time[Channel] / m_CurrAnims[Channel]->m_FrameTime);
				for(int i=0; i<m_CurrAnims[Channel]->m_Events.GetSize(); i++)
				{
					C3DSkelAnim::C3DAnimEvent* Event = m_CurrAnims[Channel]->m_Events[i];
					if(Event->m_Frame==m_CurrAnims[Channel]->m_CurrentFrame)
					{
						if(m_Owner) m_Owner->ApplyEvent(Event->m_EventName);
					}
				}
			}

			// translation
			if(anim->m_NumTranslationKeyframes>0)
			{
				frame = joint->m_CurrentTranslationKeyframes[Channel];
				while(frame < anim->m_NumTranslationKeyframes && anim->m_TranslationKeyframes[frame].m_Time < time[Channel])
				{
					frame++;
				}
				joint->m_CurrentTranslationKeyframes[Channel] = frame;


				if(frame==0)
					TransVec[Channel] = anim->m_TranslationKeyframes[0].m_Parameter;
				else if(frame==anim->m_NumTranslationKeyframes)
					TransVec[Channel] = anim->m_TranslationKeyframes[frame-1].m_Parameter;
				else
				{
					const C3DKeyframe* CurFrame = &anim->m_TranslationKeyframes[frame];
					const C3DKeyframe* PrevFrame = &anim->m_TranslationKeyframes[frame-1];

					float TimeDelta = CurFrame->m_Time - PrevFrame->m_Time;
					float InterpValue = (float)((time[Channel] - PrevFrame->m_Time) / TimeDelta);

					TransVec[Channel].x = PrevFrame->m_Parameter.x + (CurFrame->m_Parameter.x - PrevFrame->m_Parameter.x) * InterpValue;
					TransVec[Channel].y = PrevFrame->m_Parameter.y + (CurFrame->m_Parameter.y - PrevFrame->m_Parameter.y) * InterpValue;
					TransVec[Channel].z = PrevFrame->m_Parameter.z + (CurFrame->m_Parameter.z - PrevFrame->m_Parameter.z) * InterpValue;
				}
			}

			// rotation
			if(anim->m_NumRotationKeyframes>0)
			{
				frame = joint->m_CurrentRotationKeyframes[Channel];		
				while(frame < anim->m_NumRotationKeyframes && anim->m_RotationKeyframes[frame].m_Time < time[Channel])
				{
					frame++;
				}
				joint->m_CurrentRotationKeyframes[Channel] = frame;


				D3DXVECTOR3 par;
				if(frame==0)
				{
					par = anim->m_RotationKeyframes[0].m_Parameter;
					RotVec[Channel] = par;
				}
				else if(frame==anim->m_NumRotationKeyframes)
				{
					par = anim->m_RotationKeyframes[frame-1].m_Parameter;
					RotVec[Channel] = par;
				}
				else
				{
					const C3DKeyframe* CurFrame = &anim->m_RotationKeyframes[frame];
					const C3DKeyframe* PrevFrame = &anim->m_RotationKeyframes[frame-1];

					float TimeDelta = CurFrame->m_Time - PrevFrame->m_Time;
					float InterpValue = (float)((time[Channel] - PrevFrame->m_Time) / TimeDelta);
			
					RotVec[Channel].x = PrevFrame->m_Parameter.x + (CurFrame->m_Parameter.x - PrevFrame->m_Parameter.x) * InterpValue;
					RotVec[Channel].y = PrevFrame->m_Parameter.y + (CurFrame->m_Parameter.y - PrevFrame->m_Parameter.y) * InterpValue;
					RotVec[Channel].z = PrevFrame->m_Parameter.z + (CurFrame->m_Parameter.z - PrevFrame->m_Parameter.z) * InterpValue;			
				}
			}
		}


		D3DXMATRIX transform;
		D3DXMatrixIdentity(&transform);
		D3DXMATRIX RelativeFinal;

		D3DXVECTOR3 RotFinal = D3DXVECTOR3(0, 0, 0);
		D3DXVECTOR3 TransFinal = D3DXVECTOR3(0, 0, 0);

		for(Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
		{
			if(m_CurrAnims[Channel] && (fabs(D3DXVec3Length(&RotVec[Channel]))>0 || fabs(D3DXVec3Length(&TransVec[Channel]))>0))
			{
				// TODO: interpolation
				RotFinal = RotVec[Channel];
				TransFinal = TransVec[Channel];
			}
		}

/*
		if(RotFinal.x != 0 && RotFinal.y != 0 && RotFinal.z != 0)
		{
			Game->QuickMessageForm("%f %f %f", RadToDeg(RotFinal.x), RadToDeg(RotFinal.y), RadToDeg(RotFinal.z));
			Game->LOG(0, "%f %f %f", RadToDeg(RotFinal.x), RadToDeg(RotFinal.y), RadToDeg(RotFinal.z));
		}
*/
		//D3DXMatrixRotationYawPitchRoll(&transform, RotFinal.x, RotFinal.y, RotFinal.z);
		//D3DXMATRIX transl;
		//D3DXMatrixTranslation(&transl, TransFinal.x, TransFinal.y, TransFinal.z);
		//D3DXMatrixMultiply(&transform, &transform, &transl);
		
		C3DUtils::MatrixSetRotation(&transform, &RotFinal);		
		C3DUtils::MatrixSetTranslation(&transform, &TransFinal);

		D3DXMatrixMultiply(&RelativeFinal, &transform, &joint->m_Relative);
		//RelativeFinal = transform;


		if(joint->m_Parent == -1)
			joint->m_Final = RelativeFinal;
		else
		{
			D3DXMatrixMultiply(&joint->m_Final, &RelativeFinal, &m_Joints[joint->m_Parent].m_Final);
		}
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::SetAnimation(int Channel, char *Name, bool ForceReset)
{
	C3DSkelAnim* OldAnim = m_CurrAnims[Channel];

	m_CurrAnims[Channel] = NULL;
	if(Name){
		for(int i=0; i<m_Animations.GetSize(); i++){
			if(_stricmp(m_Animations[i]->m_Name, Name)==0){
				m_CurrAnims[Channel] = m_Animations[i];
				break;
			}
		}
	}

	if(m_CurrAnims[Channel] != OldAnim || ForceReset) AnimRestart(Channel);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void C3DModel::UpdateBoundingRect()
{
	m_BoundingRect.left = m_BoundingRect.top = INT_MAX;
	m_BoundingRect.right = m_BoundingRect.bottom = INT_MIN;


	LPDIRECT3DDEVICE dev = ((CBRenderD3D*)Game->m_Renderer)->m_Device;

	D3DXMATRIX viewMat, projMat, worldMat;
	D3DXVECTOR3 vec2d(0,0,0);
	dev->GetTransform(D3DTS_VIEW, &viewMat);
	dev->GetTransform(D3DTS_PROJECTION, &projMat);
	dev->GetTransform(D3DTS_WORLD, &worldMat);

	dev->GetViewport(&m_DrawingViewport);

	float x1 = m_BBoxStart.x;
	float x2 = m_BBoxEnd.x;
	float y1 = m_BBoxStart.y;
	float y2 = m_BBoxEnd.y;
	float z1 = m_BBoxStart.z;
	float z2 = m_BBoxEnd.z;

	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y1,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y1,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y1,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y1,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);

	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y2,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y2,z1), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x1,y2,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
	D3DXVec3Project(&vec2d, &D3DXVECTOR3(x2,y2,z2), &m_DrawingViewport, &projMat, &viewMat, &worldMat);
	UpdateRect(&m_BoundingRect, &vec2d);
}

//////////////////////////////////////////////////////////////////////////
void C3DModel::UpdateRect(RECT *rc, D3DXVECTOR3 *vec)
{
	rc->left   = min(rc->left,   vec->x);
	rc->right  = max(rc->right,  vec->x);
	rc->top    = min(rc->top,    vec->y);
	rc->bottom = max(rc->bottom, vec->y);
}


//////////////////////////////////////////////////////////////////////////
bool C3DModel::IsTransparentAt(int X, int Y)
{
	X += m_LastOffsetX;
	Y += m_LastOffsetY;

	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;
	if(!rend->m_Camera) return true;

	X-=m_DrawingViewport.X;
	Y-=m_DrawingViewport.Y;

    D3DXVECTOR3 vPickRayDir;
    D3DXVECTOR3 vPickRayOrig;

	/*
	D3DVIEWPORT8 viewport;
	rend->m_Device->GetViewport(&viewport);

	RECT rc;
	Game->GetCurrentViewportRect(&rc);

	viewport.Width = rc.right - rc.left;
	viewport.Height = rc.bottom - rc.top;
	viewport.X = rc.left;
	viewport.Y = rc.top;
	*/

	/*
    D3DXMATRIX matProj;
	rend->m_Device->GetTransform(D3DTS_PROJECTION, &matProj);
	*/
		
	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 vec;
	vec.x =  ((( 2.0f * X) / m_DrawingViewport.Width ) - 1) / m_LastProjMat._11;
	vec.y = -((( 2.0f * Y) / m_DrawingViewport.Height) - 1) / m_LastProjMat._22;
	vec.z =  1.0f;
	
	// Get the inverse view matrix
	D3DXMATRIX m;
	//rend->m_Camera->GetViewMatrix(&m);
	//D3DXMatrixInverse(&m, NULL, &m);
	D3DXMatrixInverse(&m, NULL, &m_LastViewMat);
	
	// Transform the screen space pick ray into 3D space
	vPickRayDir.x  = vec.x*m._11 + vec.y*m._21 + vec.z*m._31;
	vPickRayDir.y  = vec.x*m._12 + vec.y*m._22 + vec.z*m._32;
	vPickRayDir.z  = vec.x*m._13 + vec.y*m._23 + vec.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;

	// transform to model space
	D3DXVECTOR3 end = vPickRayOrig + vPickRayDir;
	D3DXMatrixInverse(&m, NULL, &m_LastWorldMat);
	D3DXVec3TransformCoord(&vPickRayOrig, &vPickRayOrig, &m);
	D3DXVec3TransformCoord(&end, &end, &m);
	vPickRayDir = end - vPickRayOrig;

	D3DXVECTOR3 intersection;
	bool int_found;
	for(int i=0; i<m_Meshes.GetSize(); i++){
		MODELVERTEX* Verts;
		if(FAILED(m_Meshes[i]->m_VB->LockVB(0, 0, &Verts, D3DLOCK_READONLY))) continue;
		
		for(int j=0; j<m_Meshes[i]->m_NumVertices; j+=3){
			D3DXVECTOR3 v0 = D3DXVECTOR3(Verts[j+0].x, Verts[j+0].y, Verts[j+0].z);
			D3DXVECTOR3 v1 = D3DXVECTOR3(Verts[j+1].x, Verts[j+1].y, Verts[j+1].z);
			D3DXVECTOR3 v2 = D3DXVECTOR3(Verts[j+2].x, Verts[j+2].y, Verts[j+2].z);

			int_found = C3DUtils::IntersectTriangle(vPickRayOrig, vPickRayDir, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z) != FALSE;

			if(int_found) break;
		}

		m_Meshes[i]->m_VB->Unlock();
		if(int_found) return false;
	}	
	return true;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::ShowMesh(char* MeshName, bool Show)
{
	if(!MeshName) return E_POINTER;

	bool Found = false;
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		if(stricmp(m_Meshes[i]->m_Name, MeshName)==0)
		{
			m_Meshes[i]->m_Visible = Show;
			Found = true;
		}
	}
	if(!Found) Game->LOG(0, "Error showing/hiding mesh '%s' (model: %s)", MeshName, m_Filename);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool C3DModel::IsMeshVisible(char* MeshName)
{
	if(!MeshName) return false;

	bool Found = false;
	for(int i=0; i<m_Meshes.GetSize(); i++)
	{
		if(stricmp(m_Meshes[i]->m_Name, MeshName)==0)
		{
			return m_Meshes[i]->m_Visible;
		}
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::DeleteMesh(char* MeshName)
{
	if(!MeshName) return E_POINTER;

	int i;
	bool Found = false;
	for(i=0; i<m_Meshes.GetSize(); i++)
	{
		if(stricmp(m_Meshes[i]->m_Name, MeshName)==0)
		{
			delete m_Meshes[i];
			m_Meshes.RemoveAt(i);
			m_NumMeshes--;
			i--;
			Found = true;
		}
	}

	bool FoundAttached = false;
	for(int i=0; i<m_AttachedMeshes.GetSize(); i++)
	{
		if(stricmp(m_AttachedMeshes[i]->m_Name, MeshName)==0)
		{
			delete m_AttachedMeshes[i];
			m_AttachedMeshes.RemoveAt(i);
			i--;
			FoundAttached = true;
		}
	}
	if(!FoundAttached)
	{
		// add to deleted list (only once)
		FoundAttached = false;
		for(int i=0; i<m_DeletedMeshes.GetSize(); i++)
		{
			if(stricmp(m_DeletedMeshes[i]->m_Name, MeshName)==0)
			{
				FoundAttached = true;
				break;
			}
		}
		if(!FoundAttached)
		{
			C3DMeshInfo* MI = new C3DMeshInfo(MeshName, NULL, NULL);
			m_DeletedMeshes.Add(MI);
		}
	}

	if(!Found) Game->LOG(0, "Error deleting mesh '%s' (model: %s)", MeshName, m_Filename);
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::AddMesh(char* Filename, char* MeshName, char* JointName)
{
	// find joint by name
	int BoneID = -1;
	for(int i=0; i<m_NumJoints; i++)
	{
		if(stricmp(m_Joints[i].m_Name, JointName)==0)
		{
			BoneID = i;
			break;
		}
	}
	if(BoneID==-1)
	{
		Game->LOG(0, "Error: cannot find bone '%s'", JointName);
		return E_FAIL;
	}

	C3DLoaderMS3D* loader = ParseFileMS3D(Filename);
	if(loader)
	{
		HRESULT Ret = loader->MergeMesh(this, MeshName, BoneID);
		if(SUCCEEDED(Ret))
		{
			C3DMeshInfo* MI = new C3DMeshInfo(MeshName, Filename, JointName);
			m_AttachedMeshes.Add(MI);
		}
		return Ret;
	}
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
C3DLoaderMS3D* C3DModel::ParseFileMS3D(char* Filename)
{
	if(!Filename) return NULL;

	for(int i=0; i<m_LoaderCache.GetSize(); i++)
	{
		if(m_LoaderCache[i]->m_Filename && stricmp(m_LoaderCache[i]->m_Filename, Filename)==0)
		{
			return m_LoaderCache[i]->m_Loader;
		}
	}

	C3DLoaderMS3D* loader = new C3DLoaderMS3D(Game);
	if(FAILED(loader->ParseFile(Filename)))
	{
		delete loader;
		return NULL;
	}
	else 
	{
		// add to cache
		C3DCachedLoader* Cached = new C3DCachedLoader();
		Cached->m_Loader = loader;
		CBUtils::SetString(&Cached->m_Filename, Filename);
		m_LoaderCache.Add(Cached);

		return loader;
	}
}


TOKEN_DEF_START
	TOKEN_DEF (NAME)
	TOKEN_DEF (LOOPING)
	TOKEN_DEF (START_FRAME)
	TOKEN_DEF (END_FRAME)
	TOKEN_DEF (MODEL)
	TOKEN_DEF (EVENT)
	TOKEN_DEF (FRAME)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::ParseAnim(BYTE *Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (LOOPING)
		TOKEN_TABLE (START_FRAME)
		TOKEN_TABLE (END_FRAME)
		TOKEN_TABLE (MODEL)
		TOKEN_TABLE (EVENT)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	char* Name = NULL;
	char* ModelName = NULL;
	bool Looping = false;
	int StartFrame = 0;
	int EndFrame = 0;

	C3DSkelAnim* Anim = new C3DSkelAnim(Game);


	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_NAME:
			CBUtils::SetString(&Name, (char*)params);
			break;

		case TOKEN_MODEL:
			CBUtils::SetString(&ModelName, (char*)params);
			break;

		case TOKEN_LOOPING:
			parser.ScanStr((char*)params, "%b", &Looping);
			break;

		case TOKEN_START_FRAME:
			parser.ScanStr((char*)params, "%d", &StartFrame);
			//StartFrame--;
			break;

		case TOKEN_END_FRAME:
			parser.ScanStr((char*)params, "%d", &EndFrame);
			//EndFrame--;
			break;

		case TOKEN_EVENT:
			ParseEvent(Anim, params);
			break;
		}
	}
	if (cmd != PARSERR_EOF) return E_FAIL;

	HRESULT Ret = S_OK;

	if(!ModelName && m_Filename) CBUtils::SetString(&ModelName, m_Filename);
	C3DLoaderMS3D* Loader = ParseFileMS3D(ModelName);
	if(!Loader)
	{
		SAFE_DELETE(Anim);
		Game->LOG(0, "Error loading animations from file '%s'.", ModelName);
		Ret = E_FAIL;
	}
	else
	{		
		CBUtils::SetString(&Anim->m_Name, Name);
		Anim->m_Looping = Looping;
		
		if(SUCCEEDED(Loader->LoadAnimation(Anim, StartFrame, EndFrame)))
		{
			m_Animations.Add(Anim);
			Ret = S_OK;
		}
		else
		{
			SAFE_DELETE(Anim);
			Ret = E_FAIL;
		}
	}
	
	SAFE_DELETE_ARRAY(Name);
	SAFE_DELETE_ARRAY(ModelName);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::ParseEvent(C3DSkelAnim* Anim, BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (FRAME)
	TOKEN_TABLE_END

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	C3DSkelAnim::C3DAnimEvent* Event = new C3DSkelAnim::C3DAnimEvent();
	if(!Event) return E_FAIL;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_NAME:
				CBUtils::SetString(&Event->m_EventName, (char*)params);
				break;

			case TOKEN_FRAME:
				parser.ScanStr((char*)params, "%d", &Event->m_Frame);
				Event->m_Frame--;
				break;
		}
	}
	if (cmd != PARSERR_EOF)
	{
		SAFE_DELETE(Event);
		return E_FAIL;
	}

	if(Event->m_EventName) Anim->m_Events.Add(Event);
	else
	{
		SAFE_DELETE(Event);
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool C3DModel::IsAnimPending(int Channel, char* AnimName)
{
	if(!AnimName)
	{
		if(m_CurrAnims[Channel] && !m_CurrAnims[Channel]->m_Finished) return true;
	}
	else
	{
		if(m_CurrAnims[Channel] && m_CurrAnims[Channel]->m_Name && stricmp(AnimName, m_CurrAnims[Channel]->m_Name)==0 && !m_CurrAnims[Channel]->m_Finished) return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
bool C3DModel::IsAnimPending(char* AnimName)
{
	for(int Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
	{
		if(IsAnimPending(Channel, AnimName)) return true;
	}
	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::AnimStop(int Channel)
{
	m_CurrAnims[Channel] = NULL;
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::AnimStop()
{
	for(int Channel=0; Channel<NUM_SKEL_ANIM_CHANNELS; Channel++)
		AnimStop(Channel);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DModel::Persist(CBPersistMgr *PersistMgr)
{
	int i;

	CBObject::Persist(PersistMgr);
	
	PersistMgr->Transfer(TMEMBER(m_BBoxStart));
	PersistMgr->Transfer(TMEMBER(m_BBoxEnd));
	PersistMgr->Transfer(TMEMBER(m_BoundingRect));

	if(!PersistMgr->m_Saving)
	{
		memset(&m_DrawingViewport, 0, sizeof(D3DVIEWPORT));
	}

	PersistMgr->Transfer(TMEMBER(m_LastOffsetX));
	PersistMgr->Transfer(TMEMBER(m_LastOffsetY));

	PersistMgr->Transfer(TMEMBER(m_LastProjMat));
	PersistMgr->Transfer(TMEMBER(m_LastViewMat));
	PersistMgr->Transfer(TMEMBER(m_LastWorldMat));


	PersistMgr->Transfer(TMEMBER(m_Owner));
		
	// load model
	if(!PersistMgr->m_Saving)
	{
		m_NumJoints = 0;
		m_Joints = NULL;

		m_NumMaterials = 0;
		m_NumMeshes = 0;

		if(m_Filename) LoadFile(m_Filename);
	}

	for(i=0; i<m_NumJoints; i++)
	{
		m_Joints[i].Persist(PersistMgr);
	}

	m_Animations.Persist(PersistMgr);
	if(!PersistMgr->m_Saving) LoadFinalize();



	for(i=0; i<NUM_SKEL_ANIM_CHANNELS; i++)		
	{
		if(!PersistMgr->m_Saving) m_CurrAnims[i] = NULL;
		PersistMgr->Transfer(TMEMBER(m_CurrAnims[i]));
		PersistMgr->Transfer(TMEMBER(m_StartTime[i]));
	}

	int NumDeleted = m_DeletedMeshes.GetSize();
	PersistMgr->Transfer(TMEMBER(NumDeleted));
	for(i=0; i<NumDeleted; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_DeletedMeshes[i]->m_Name));
		}
		else
		{
			char* MeshName;
			PersistMgr->Transfer(TMEMBER(MeshName));
			DeleteMesh(MeshName);
		}
	}

	int NumAttached = m_AttachedMeshes.GetSize();
	PersistMgr->Transfer(TMEMBER(NumAttached));
	for(i=0; i<NumAttached; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_AttachedMeshes[i]->m_Name));
			PersistMgr->Transfer(TMEMBER(m_AttachedMeshes[i]->m_Filename));
			PersistMgr->Transfer(TMEMBER(m_AttachedMeshes[i]->m_ParentBone));
		}
		else
		{
			char* Name;
			char* Filename;
			char* ParentBone;
			PersistMgr->Transfer(TMEMBER(Name));
			PersistMgr->Transfer(TMEMBER(Filename));
			PersistMgr->Transfer(TMEMBER(ParentBone));
			AddMesh(Filename, Name, ParentBone);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	int NumMeshes = m_Meshes.GetSize();
	PersistMgr->Transfer(TMEMBER(NumMeshes));
	for(i=0; i<NumMeshes; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_Meshes[i]->m_Name));
			m_Meshes[i]->Persist(PersistMgr);
		}
		else
		{
			char* Name = NULL;
			PersistMgr->Transfer(TMEMBER(Name));
			bool Found = false;
			for(int j=0; j<m_Meshes.GetSize(); j++)
			{
				if(stricmp(Name, m_Meshes[j]->m_Name)==0)
				{
					m_Meshes[j]->Persist(PersistMgr);
					Found = true;
					break;
				}
			}
			if(!Found)
			{
				C3DMesh* Mesh = new C3DMesh(Game);
				Mesh->Persist(PersistMgr);
				delete Mesh;
			}
		}
	}

	return S_OK;
}
