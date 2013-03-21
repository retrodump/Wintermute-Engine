// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdSceneGeometry.h"
#include "3DUtils.h"


IMPLEMENT_PERSISTENT(CAdSceneGeometry, false);

//////////////////////////////////////////////////////////////////////////
CAdSceneGeometry::CAdSceneGeometry(CBGame* inGame):CBObject(inGame)
{
	m_ActiveCamera = m_ActiveLight = -1;
	D3DXMatrixIdentity(&m_ViewMatrix);
	//m_WaypointHeight = 5.0f;
	//m_WaypointHeight = 1.0f;
	m_WaypointHeight = 10.0f;
	m_WptMarker = NULL;

	m_PFReady = true;
	m_PFTargetPath = NULL;
	m_PFMaxTime = 15;
	m_PFRerun = false;

	m_PFSource = m_PFTarget = m_PFAlternateTarget = D3DXVECTOR3(0, 0, 0);
	m_PFAlternateDist = FLT_MAX;


	memset(&m_DrawingViewport, 0, sizeof(D3DVIEWPORT));

	D3DXMatrixIdentity(&m_LastWorldMat);
	D3DXMatrixIdentity(&m_LastViewMat);
	D3DXMatrixIdentity(&m_LastProjMat);

	m_LastOffsetX = m_LastOffsetY = 0;
	m_LastScrollX = m_LastScrollY = 0;

	m_LastValuesInitialized = false;
	m_MaxLightsWarning = false;
}


//////////////////////////////////////////////////////////////////////////
CAdSceneGeometry::~CAdSceneGeometry()
{	
	Cleanup();
	SAFE_DELETE(m_WptMarker);
}


//////////////////////////////////////////////////////////////////////////
void CAdSceneGeometry::Cleanup()
{
	int i;

	for(i=0; i<m_Planes.GetSize(); i++) delete m_Planes[i];
	m_Planes.RemoveAll();

	for(i=0; i<m_Blocks.GetSize(); i++) delete m_Blocks[i];
	m_Blocks.RemoveAll();

	for(i=0; i<m_Generics.GetSize(); i++) delete m_Generics[i];
	m_Generics.RemoveAll();

	for(i=0; i<m_WaypointGroups.GetSize(); i++) delete m_WaypointGroups[i];
	m_WaypointGroups.RemoveAll();

	for(i=0; i<m_Cameras.GetSize(); i++)
	{
		CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
		if(m_Renderer->m_Camera == m_Cameras[i]) m_Renderer->m_Camera = NULL;

		delete m_Cameras[i];
	}
	m_Cameras.RemoveAll();

	for(i=0; i<m_Lights.GetSize(); i++) delete m_Lights[i];
	m_Lights.RemoveAll();

	m_ActiveCamera = m_ActiveLight = -1;
	D3DXMatrixIdentity(&m_ViewMatrix);

	for(i=0; i<m_PFPath.GetSize(); i++)
		delete m_PFPath[i];
	m_PFPath.RemoveAll();

	m_PFTargetPath = NULL;
}

//////////////////////////////////////////////////////////////////////////
CAdGeomExt* CAdSceneGeometry::GetGeometryExtension(char* Filename)
{
	CAdGeomExt* Ret = new CAdGeomExt(Game);

	char GeomExtFile[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];

	_splitpath(Filename, drive, dir, fname, NULL);
	_makepath(GeomExtFile, drive, dir, fname, ".geometry");

	bool LoadOK = false;
	CBFile* TestFile = Game->m_FileManager->OpenFile(GeomExtFile);
	if(TestFile)
	{
		Game->m_FileManager->CloseFile(TestFile);
		LoadOK = SUCCEEDED(Ret->LoadFile(GeomExtFile));
	}

	// no ext file found, just use the defaults
	if(!LoadOK) Ret->AddStandardNodes();

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::LoadFile(char* Filename)
{
	Cleanup();

	// load waypoint graphics from resources
	if(!m_WptMarker)
	{
		m_WptMarker = new CBSprite(Game);
		if(m_WptMarker){
			if(FAILED(m_WptMarker->LoadFile("wpt.sprite")))
			{
				delete m_WptMarker;
				m_WptMarker = NULL;
			}
		}
	}

	CAdGeomExt* GeomExt = GetGeometryExtension(Filename);

	char ext[_MAX_EXT];
	_splitpath(Filename, NULL, NULL, NULL, ext);

	if(_stricmp(ext, ".3ds")==0)
	{
		C3DLoader3DS* loader = new C3DLoader3DS(Game);
		if(FAILED(loader->ParseFile(Filename)))
		{
			delete loader;
			delete GeomExt;
			return E_FAIL;
		}

		int i;
		
		CSysClassRegistry::GetInstance()->m_Disabled = true;

		// load meshes
		for(i=0; i<loader->GetNumMeshes(); i++)
		{
			CAdGeomExtNode* ExtNode = GeomExt->MatchName(loader->GetMeshName(i));
			if(!ExtNode) continue;

			switch(ExtNode->m_Type)
			{
				case GEOM_WALKPLANE:
				{
					CAdWalkplane* plane = new CAdWalkplane(Game);
					plane->SetName(loader->GetMeshName(i));
					plane->m_Mesh = new C3DMesh(Game);
					if(FAILED(loader->LoadMesh(i, plane->m_Mesh)))
					{
						delete plane->m_Mesh;
						delete plane;
						delete loader;
						delete GeomExt;
						return E_FAIL;
					}
					else
					{
						plane->m_Mesh->ComputeNormals();
						plane->m_Mesh->FillVertexBuffer(0x700000FF);
						plane->m_ReceiveShadows = ExtNode->m_ReceiveShadows;
						m_Planes.Add(plane);
					}
				}
				break;

				case GEOM_BLOCKED:
				{
					CAdBlock* block = new CAdBlock(Game);
					block->SetName(loader->GetMeshName(i));
					block->m_Mesh = new C3DMesh(Game);
					if(FAILED(loader->LoadMesh(i, block->m_Mesh)))
					{
						delete block->m_Mesh;
						delete block;
						delete loader;
						delete GeomExt;
						return E_FAIL;
					}
					else
					{
						block->m_Mesh->ComputeNormals();
						block->m_Mesh->FillVertexBuffer(0x70FF0000);
						block->m_ReceiveShadows = ExtNode->m_ReceiveShadows;
						m_Blocks.Add(block);
					}
				}
				break;

				case GEOM_WAYPOINT:
				{
					C3DMesh* mesh = new C3DMesh(Game);
					if(FAILED(loader->LoadMesh(i, mesh)))
					{
						delete mesh;
						delete loader;
						delete GeomExt;
						return E_FAIL;
					}
					else
					{
						// TODO: groups
						if(m_WaypointGroups.GetSize()==0) m_WaypointGroups.Add(new CAdWaypointGroup3D(Game));
						m_WaypointGroups[0]->AddFromMesh(mesh);
						delete mesh;
					}
				}
				break;

				case GEOM_GENERIC:
				{
					CAdGeneric* generic = new CAdGeneric(Game);
					generic->SetName(loader->GetMeshName(i));
					generic->m_Mesh = new C3DMesh(Game);
					if(FAILED(loader->LoadMesh(i, generic->m_Mesh)))
					{
						delete generic->m_Mesh;
						delete generic;
						delete loader;
						delete GeomExt;
						return E_FAIL;
					}
					else
					{
						generic->m_Mesh->ComputeNormals();
						generic->m_Mesh->FillVertexBuffer(0x7000FF00);
						generic->m_ReceiveShadows = ExtNode->m_ReceiveShadows;
						m_Generics.Add(generic);
					}
				}
				break;

			}


			/*
			// walk planes
			if(CBUtils::StrBeginsI(loader->GetMeshName(i), "walk_"))
			{
				CAdWalkplane* plane = new CAdWalkplane(Game);
				plane->SetName(loader->GetMeshName(i));
				plane->m_Mesh = new C3DMesh(Game);
				if(FAILED(loader->LoadMesh(i, plane->m_Mesh)))
				{
					delete plane->m_Mesh;
					delete plane;
					delete loader;
					delete GeomExt;
					return E_FAIL;
				}
				else
				{
					plane->m_Mesh->ComputeNormals();
					plane->m_Mesh->FillVertexBuffer(0x700000FF);
					m_Planes.Add(plane);
				}
			}

			// blocks
			else if(CBUtils::StrBeginsI(loader->GetMeshName(i), "blk_"))
			{
				CAdBlock* block = new CAdBlock(Game);
				block->SetName(loader->GetMeshName(i));
				block->m_Mesh = new C3DMesh(Game);
				if(FAILED(loader->LoadMesh(i, block->m_Mesh)))
				{
					delete block->m_Mesh;
					delete block;
					delete loader;
					delete GeomExt;
					return E_FAIL;
				}
				else
				{
					block->m_Mesh->ComputeNormals();
					block->m_Mesh->FillVertexBuffer(0x70FF0000);
					m_Blocks.Add(block);
				}
			}

			// waypoints
			else if(CBUtils::StrBeginsI(loader->GetMeshName(i), "wpt_"))
			{
				C3DMesh* mesh = new C3DMesh(Game);
				if(FAILED(loader->LoadMesh(i, mesh)))
				{
					delete mesh;
					delete loader;
					delete GeomExt;
					return E_FAIL;
				}
				else
				{
					// TODO: groups
					if(m_WaypointGroups.GetSize()==0) m_WaypointGroups.Add(new CAdWaypointGroup3D(Game));
					m_WaypointGroups[0]->AddFromMesh(mesh);
					delete mesh;
				}
			}
			*/
		}		

		// load cameras
		for(i=0; i<loader->GetNumCameras(); i++)
		{
			C3DCamera* camera = new C3DCamera(Game);
			if(FAILED(loader->LoadCamera(i, camera)))
			{
				delete camera;
				delete loader;
				delete GeomExt;
				return E_FAIL;
			}
			else m_Cameras.Add(camera);
		}

		// load lights
		for(i=0; i<loader->GetNumLights(); i++)
		{
			C3DLight* light = new C3DLight(Game);
			if(FAILED(loader->LoadLight(i, light)))
			{
				delete light;
				delete loader;
				delete GeomExt;
				return E_FAIL;
			}
			else m_Lights.Add(light);
		}
		CSysClassRegistry::GetInstance()->m_Disabled = false;

		if(m_Cameras.GetSize()>0) SetActiveCamera(0, -1.0f, -1.0f, -1.0f);
		CreateLights();

		if(m_Lights.GetSize()>0) SetActiveLight(0);

		delete loader;
		delete GeomExt;
	}
	else
	{
		Game->LOG(0, "Error: no suitable loader found for file '%s'", Filename);
		delete GeomExt;
		return E_FAIL;
	}

	// drop waypoints to the ground
	DropWaypoints();


	if(m_Filename!=Filename) SetFilename(Filename);


	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::DropWaypoints()
{
	for(int i=0; i<m_WaypointGroups.GetSize(); i++)
	{
		for(int j=0; j<m_WaypointGroups[i]->m_Points.GetSize(); j++)
		{
			D3DXVECTOR3* point = m_WaypointGroups[i]->m_Points[j];
			point->y = GetHeightAt(*point) + m_WaypointHeight;
		}
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveCamera(int Camera, float FOV, float NearClipPlane, float FarClipPlane)
{
	if(Camera<0 || Camera>= m_Cameras.GetSize())
	{
		Game->LOG(0, "Warning: Camera %d is out of bounds.", Camera);
		return E_FAIL;
	}
	else
	{
		m_ActiveCamera = Camera;
		
		if(FOV>=0.0f) m_Cameras[Camera]->m_FOV = FOV;
		else m_Cameras[Camera]->m_FOV = m_Cameras[Camera]->m_OrigFOV;
		
		m_Cameras[Camera]->m_NearClipPlane = NearClipPlane;
		m_Cameras[Camera]->m_FarClipPlane = FarClipPlane;
		
		m_Cameras[Camera]->GetViewMatrix(&m_ViewMatrix);
		return S_OK;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveCamera(char* Camera, float FOV, float NearClipPlane, float FarClipPlane)
{
	for(int i=0; i<m_Cameras.GetSize(); i++){
		if(_stricmp(m_Cameras[i]->m_Name, Camera)==0) return SetActiveCamera(i, FOV, NearClipPlane, FarClipPlane);
	}

	Game->LOG(0, "Warning: Camera '%s' not found.", Camera);
	return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
C3DCamera* CAdSceneGeometry::GetActiveCamera()
{
	if(m_ActiveCamera>=0 && m_ActiveCamera<m_Cameras.GetSize())
		return m_Cameras[m_ActiveCamera];
	else return NULL;
}

/*
//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveCameraTwin(int Camera)
{
	C3DCamera* ActiveCam = GetActiveCamera();
	if(!ActiveCam) return E_FAIL;

	if(Camera<0 || Camera>= m_Cameras.GetSize()){
		Game->LOG(0, "Warning: Camera %d is out of bounds.", Camera);
		return E_FAIL;
	}
	else{
		m_ActiveCamera2 = Camera;
		ActiveCam->SetTwin(m_Cameras[Camera]);
		ActiveCam->GetViewMatrix(&m_ViewMatrix);
		return S_OK;
	}
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveCameraTwin(char* Camera)
{
	for(int i=0; i<m_Cameras.GetSize(); i++){
		if(_stricmp(m_Cameras[i]->m_Name, Camera)==0) return SetActiveCameraTwin(i);
	}

	Game->LOG(0, "Warning: Camera '%s' not found.", Camera);
	return E_FAIL;
}
*/

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveLight(int Light)
{
	if(Light<0 || Light>= m_Lights.GetSize()){
		Game->LOG(0, "Warning: Light %d is out of bounds.", Light);
		return E_FAIL;
	}
	else{
		m_ActiveLight = Light;
		return S_OK;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetActiveLight(char* Light)
{
	for(int i=0; i<m_Lights.GetSize(); i++){
		if(_stricmp(m_Lights[i]->m_Name, Light)==0) return SetActiveLight(i);
	}

	Game->LOG(0, "Warning: Light '%s' not found.", Light);
	return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
D3DXMATRIX* CAdSceneGeometry::GetViewMatrix()
{
	return &m_ViewMatrix;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::StoreDrawingParams()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	// store values
	m_Renderer->m_Device->GetViewport(&m_DrawingViewport);

	m_Renderer->m_Device->GetTransform(D3DTS_WORLD, &m_LastWorldMat);
	m_Renderer->m_Device->GetTransform(D3DTS_VIEW, &m_LastViewMat);
	m_Renderer->m_Device->GetTransform(D3DTS_PROJECTION, &m_LastProjMat);

	CAdScene* Scene = ((CAdGame*)Game)->m_Scene;
	if(Scene)
	{
		m_LastScrollX = Scene->GetOffsetLeft();
		m_LastScrollY = Scene->GetOffsetTop();
	}
	else
	{
		m_LastScrollX = 0;
		m_LastScrollY = 0;
	}

	RECT rc;
	Game->GetCurrentViewportRect(&rc);
	float Width = (float)rc.right - (float)rc.left;
	float Height = (float)rc.bottom - (float)rc.top;

	// margins
	int mleft = rc.left;
	int mright = m_Renderer->m_Width - Width - rc.left;
	int mtop = rc.top;
	int mbottom = m_Renderer->m_Height - Height - rc.top;


	m_LastOffsetX = Game->m_OffsetX + (mleft - mright)/2;
	m_LastOffsetY = Game->m_OffsetY + (mtop - mbottom)/2;

	m_LastValuesInitialized = true;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::Render(bool Render)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	// store values
	StoreDrawingParams();
	if(!Render) return S_OK;


	// render the geometry
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	if(m_ActiveCamera>=0 && m_ActiveCamera<m_Cameras.GetSize())
		m_Renderer->Setup3D(m_Cameras[m_ActiveCamera]);

	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &matIdentity);

	C3DUtils::SetFixedVertexShader(m_Renderer->m_Device, D3DFVF_MODELVERTEXCOLOR);

	m_Renderer->m_Device->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);
	m_Renderer->m_Device->SetRenderState(D3DRS_LIGHTING, FALSE);
	m_Renderer->m_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_Renderer->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_Renderer->m_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	m_Renderer->m_Device->SetRenderState(D3DRS_ALPHATESTENABLE,  FALSE);
	m_Renderer->m_Device->SetTexture(0, NULL);

	int i;

	// render walk planes
	for(i=0; i<m_Planes.GetSize(); i++)
	{
		if(!m_Planes[i]->m_Active) continue;
		if(!m_Planes[i]->m_Mesh->m_VB) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Planes[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Planes[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Planes[i]->m_Mesh->m_NumFaces;
	}

	// render blocks
	for(i=0; i<m_Blocks.GetSize(); i++)
	{
		if(!m_Blocks[i]->m_Active) continue;
		if(!m_Blocks[i]->m_Mesh->m_VB) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Blocks[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Blocks[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Blocks[i]->m_Mesh->m_NumFaces;
	}

	// render generic objects
	for(i=0; i<m_Generics.GetSize(); i++)
	{
		if(!m_Generics[i]->m_Active) continue;
		if(!m_Generics[i]->m_Mesh->m_VB) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Generics[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Generics[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Generics[i]->m_Mesh->m_NumFaces;
	}


	m_Renderer->m_Device->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID );

	// render waypoints
	if(m_WptMarker)
	{
		D3DXMATRIX viewMat, projMat, worldMat;
		D3DXVECTOR3 vec2d(0,0,0);
		m_Renderer->m_Device->GetTransform(D3DTS_VIEW, &viewMat);
		m_Renderer->m_Device->GetTransform(D3DTS_PROJECTION, &projMat);
		D3DXMatrixIdentity(&worldMat);
		D3DVIEWPORT vport;
		m_Renderer->m_Device->GetViewport(&vport);

		m_Renderer->Setup2D();

		CAdScene* Scene = ((CAdGame*)Game)->m_Scene;

		for(i=0; i<m_WaypointGroups.GetSize(); i++)
		{
			for(int j=0; j<m_WaypointGroups[i]->m_Points.GetSize(); j++)
			{
				D3DXVECTOR3 *vect = m_WaypointGroups[i]->m_Points[j];
				D3DXVec3Project(&vec2d, m_WaypointGroups[i]->m_Points[j], &vport, &projMat, &viewMat, &worldMat);
				m_WptMarker->Display(vec2d.x + Scene->GetOffsetLeft() - m_Renderer->m_DrawOffsetX, vec2d.y + Scene->GetOffsetTop() - m_Renderer->m_DrawOffsetY);
			}
		}
	}
	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::RenderShadowGeometry()
{
	StoreDrawingParams();

	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	// render the geometry
	D3DXMATRIX matIdentity;
	D3DXMatrixIdentity(&matIdentity);

	if(m_ActiveCamera>=0 && m_ActiveCamera<m_Cameras.GetSize())
		m_Renderer->Setup3D(m_Cameras[m_ActiveCamera]);

	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &matIdentity);


	// disable color write
	m_Renderer->SetSpriteBlendMode(BLEND_UNKNOWN);
	m_Renderer->m_Device->SetRenderState(D3DRS_SRCBLEND,  D3DBLEND_ZERO);
	m_Renderer->m_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);
	
	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &matIdentity);
	C3DUtils::SetFixedVertexShader(m_Renderer->m_Device, D3DFVF_MODELVERTEXCOLOR);

	// no texture
	m_Renderer->m_LastTexture = NULL;
	m_Renderer->m_Device->SetTexture(0, NULL);

	
	m_Renderer->m_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	// render blocks
	for(int i=0; i<m_Blocks.GetSize(); i++)
	{
		if(!m_Blocks[i]->m_Active) continue;
		if(!m_Blocks[i]->m_Mesh->m_VB) continue;
		if(!m_Blocks[i]->m_ReceiveShadows) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Blocks[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Blocks[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Blocks[i]->m_Mesh->m_NumFaces;
	}

	// render walkplanes
	for(int i=0; i<m_Planes.GetSize(); i++)
	{
		if(!m_Planes[i]->m_Active) continue;
		if(!m_Planes[i]->m_Mesh->m_VB) continue;
		if(!m_Planes[i]->m_ReceiveShadows) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Planes[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Planes[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Planes[i]->m_Mesh->m_NumFaces;
	}

	// render generic meshes
	for(int i=0; i<m_Generics.GetSize(); i++)
	{
		if(!m_Generics[i]->m_Active) continue;
		if(!m_Generics[i]->m_Mesh->m_VB) continue;
		if(!m_Generics[i]->m_ReceiveShadows) continue;
		C3DUtils::SetStreamSource(m_Renderer->m_Device, 0, m_Generics[i]->m_Mesh->m_VB, sizeof(MODELVERTEXCOLOR));
		m_Renderer->m_Device->DrawPrimitive(D3DPT_TRIANGLELIST, 0, m_Generics[i]->m_Mesh->m_NumFaces);

		m_Renderer->m_NumPolygons += m_Generics[i]->m_Mesh->m_NumFaces;
	}

	m_Renderer->SetSpriteBlendMode(BLEND_NORMAL);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
float CAdSceneGeometry::GetHeightAt(D3DXVECTOR3 Pos, float Tolerance, bool* IntFound)
{
	float ret = Pos.y;
	D3DXVECTOR3 intersection;
	D3DXVECTOR3 dir = D3DXVECTOR3(0, -1, 0);

	Pos.y += Tolerance;

	bool int_found = false;
	for(int i=0; i<m_Planes.GetSize(); i++){
		for(int j=0; j<m_Planes[i]->m_Mesh->m_NumFaces; j++){
			if(C3DUtils::IntersectTriangle(
						Pos, dir,
				        m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[0]].m_Pos,
						m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[1]].m_Pos,
						m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[2]].m_Pos,
						&intersection.x, &intersection.y, &intersection.z)){
				if(intersection.y > Pos.y+Tolerance) continue; // only fall down
				if(!int_found || fabs(ret - Pos.y) > fabs(intersection.y - Pos.y)) ret = intersection.y;
				int_found = true;
			}

		}
	}

	if(IntFound) *IntFound = int_found;

	return ret;
}


//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::DirectPathExists(D3DXVECTOR3* p1, D3DXVECTOR3* p2)
{
	D3DXVECTOR3 v0, v1, v2;
	int i,j;

	// test walkplanes
	for(i=0; i<m_Planes.GetSize(); i++){
		for(j=0; j<m_Planes[i]->m_Mesh->m_NumFaces; j++){
			v0 = m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[0]].m_Pos;
			v1 = m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[1]].m_Pos;
			v2 = m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[2]].m_Pos;
			
			D3DXPLANE plane;
			D3DXVECTOR3 intersection;
			float dist;

			if(C3DUtils::PickGetIntersect(*p1, *p2, v0, v1, v2, &intersection, &dist)){
				if(C3DUtils::IntersectTriangle(*p1, *p1-*p2, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return false;
				if(C3DUtils::IntersectTriangle(*p2, *p2-*p1, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return false;
			}
		}
	}

	// test blocks
	for(i=0; i<m_Blocks.GetSize(); i++)
	{
		if(!m_Blocks[i]->m_Active) continue;
		for(j=0; j<m_Blocks[i]->m_Mesh->m_NumFaces; j++)
		{
			v0 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[0]].m_Pos;
			v1 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[1]].m_Pos;
			v2 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[2]].m_Pos;
			
			D3DXPLANE plane;
			D3DXVECTOR3 intersection;
			float dist;

			if(C3DUtils::PickGetIntersect(*p1, *p2, v0, v1, v2, &intersection, &dist))
			{
				if(C3DUtils::IntersectTriangle(*p1, *p1-*p2, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return false;
				if(C3DUtils::IntersectTriangle(*p2, *p2-*p1, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return false;
			}
		}
	}


	return true;
}

//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CAdSceneGeometry::GetBlockIntersection(D3DXVECTOR3* p1, D3DXVECTOR3* p2)
{
	D3DXVECTOR3 v0, v1, v2;

	// test blocks
	for(int i=0; i<m_Blocks.GetSize(); i++)
	{
		if(!m_Blocks[i]->m_Active) continue;
		for(int j=0; j<m_Blocks[i]->m_Mesh->m_NumFaces; j++)
		{
			v0 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[0]].m_Pos;
			v1 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[1]].m_Pos;
			v2 = m_Blocks[i]->m_Mesh->m_Vertices[m_Blocks[i]->m_Mesh->m_Faces[j].m_Vertices[2]].m_Pos;

			D3DXPLANE plane;
			D3DXVECTOR3 intersection;
			float dist;

			if(C3DUtils::PickGetIntersect(*p1, *p2, v0, v1, v2, &intersection, &dist))
			{
				if(C3DUtils::IntersectTriangle(*p1, *p1-*p2, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return intersection;
				if(C3DUtils::IntersectTriangle(*p2, *p2-*p1, v0, v1, v2, &intersection.x, &intersection.y, &intersection.z)) return intersection;
			}
		}
	}
	return D3DXVECTOR3(0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::Convert2Dto3DTolerant(int X, int Y, D3DXVECTOR3 *Pos)
{
	bool Ret = Convert2Dto3D(X, Y, Pos);
	if(Ret) return Ret;


	int LenLeft = 0;
	int LenRight = 0;
	int LenDown = 0;
	int LenUp = 0;

	int i;

	// left
	for(i=0; i<1000; i+=10)
	{
		if(Convert2Dto3D(X-i, Y, Pos))
		{
			LenLeft = i;
			break;
		}
	}

	// right
	for(i=0; i<1000; i+=10)
	{
		if(Convert2Dto3D(X+i, Y, Pos))
		{
			LenRight = i;
			break;
		}
	}

	// up
	for(i=0; i<1000; i+=10)
	{
		if(Convert2Dto3D(X, Y-i, Pos))
		{
			LenUp = i;
			break;
		}
	}

	// down
	for(i=0; i<1000; i+=10)
	{
		if(Convert2Dto3D(X, Y+i, Pos))
		{
			LenDown = i;
			break;
		}
	}

	if(!LenLeft && !LenRight && !LenUp && !LenDown)	return false;


	int OffsetX = INT_MAX;
	int OffsetY = INT_MAX;

	if(LenLeft || LenRight)
	{
		if(LenRight)
		{
			if(LenLeft && LenLeft<LenRight) OffsetX = -LenLeft;
			else OffsetX = LenRight;
		}
		else OffsetX = -LenLeft;
	}

	if(LenUp || LenDown)
	{
		if(LenDown)
		{
			if(LenUp && LenUp<LenDown) OffsetY = -LenUp;
			else OffsetY = LenDown;
		}
		else OffsetY = -LenUp;
	}


	if(abs(OffsetX) < abs(OffsetY)) X += OffsetX;
	else Y += OffsetY;

	return Convert2Dto3D(X, Y, Pos);
}

//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::Convert2Dto3D(int X, int Y, D3DXVECTOR3 *Pos)
{
	CBRenderD3D* rend = (CBRenderD3D*)Game->m_Renderer;

	if(!m_LastValuesInitialized)
	{
		rend->m_Device->GetViewport(&m_DrawingViewport);
		rend->m_Device->GetTransform(D3DTS_PROJECTION, &m_LastProjMat);
	}


	float ResWidth, ResHeight;
	float LayerWidth, LayerHeight;
	float ModWidth, ModHeight;
	bool CustomViewport;
	rend->GetProjectionParams(&ResWidth, &ResHeight, &LayerWidth, &LayerHeight, &ModWidth, &ModHeight, &CustomViewport);

	// modify coordinates according to viewport settings
	int mleft = m_DrawingViewport.X;
	int mright = ResWidth - m_DrawingViewport.Width - m_DrawingViewport.X;
	int mtop = m_DrawingViewport.Y;
	int mbottom = ResHeight - m_DrawingViewport.Height - m_DrawingViewport.Y;

	X-=(mleft + mright)/2 + ModWidth;
	Y-=(mtop + mbottom)/2 + ModHeight;

	

	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;

	// Compute the vector of the pick ray in screen space
	D3DXVECTOR3 vec;
	vec.x =  ((( 2.0f * X) / m_DrawingViewport.Width ) - 1) / m_LastProjMat._11;
	vec.y = -((( 2.0f * Y) / m_DrawingViewport.Height) - 1) / m_LastProjMat._22;
	vec.z =  1.0f;
	
	// Get the inverse view matrix
	D3DXMATRIX m;
	D3DXMatrixInverse(&m, NULL, &m_ViewMatrix);
	
	// Transform the screen space pick ray into 3D space
	vPickRayDir.x  = vec.x*m._11 + vec.y*m._21 + vec.z*m._31;
	vPickRayDir.y  = vec.x*m._12 + vec.y*m._22 + vec.z*m._32;
	vPickRayDir.z  = vec.x*m._13 + vec.y*m._23 + vec.z*m._33;
	vPickRayOrig.x = m._41;
	vPickRayOrig.y = m._42;
	vPickRayOrig.z = m._43;
	

	/*
	D3DXMatrixIdentity(&m_LastWorldMat);
	D3DXVECTOR3 vPickRayDir;
	D3DXVECTOR3 vPickRayOrig;

	D3DXVECTOR3 VecMouse1 = D3DXVECTOR3(X, Y, 0.0f);
	D3DXVECTOR3 VecMouse2 = D3DXVECTOR3(X, Y, 1.0f);
	D3DXVec3Unproject(&vPickRayOrig, &VecMouse1, &m_DrawingViewport, &m_LastProjMat, &m_LastViewMat, &m_LastWorldMat);
	D3DXVec3Unproject(&vPickRayDir, &VecMouse2, &m_DrawingViewport, &m_LastProjMat, &m_LastViewMat, &m_LastWorldMat);

	vPickRayDir = vPickRayDir - vPickRayOrig;
	*/



	bool int_found = false;
	float min_dist = FLT_MAX;
	D3DXVECTOR3 intersection, ray;
	for(int i=0; i<m_Planes.GetSize(); i++){
		for(int j=0; j<m_Planes[i]->m_Mesh->m_NumFaces; j++){
			if(C3DUtils::IntersectTriangle(
								vPickRayOrig, vPickRayDir,
				                m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[0]].m_Pos,
								m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[1]].m_Pos,
								m_Planes[i]->m_Mesh->m_Vertices[m_Planes[i]->m_Mesh->m_Faces[j].m_Vertices[2]].m_Pos,
								&intersection.x, &intersection.y, &intersection.z))
			{
				ray = intersection - vPickRayOrig;
				float dist = D3DXVec3Length(&ray);
				if(dist < min_dist){
					*Pos = intersection;
					min_dist = dist;
				}
				int_found = true;
			}

		}
	}
	
	/* TODO: modify X/Y to hit walkplane */
	
	/*
	if(!int_found)
	{
		D3DXVECTOR3 Point = D3DXVECTOR3(X, Y, 1.0f);
		D3DVIEWPORT8 vp;
		D3DXMATRIX mProj;
		D3DXMATRIX mView;
		D3DXMATRIX mWorld;

		((CBRenderD3D*)Game->m_Renderer)->m_Device->GetTransform(D3DTS_PROJECTION, &mProj);
		((CBRenderD3D*)Game->m_Renderer)->m_Device->GetTransform(D3DTS_VIEW, &mView);
		((CBRenderD3D*)Game->m_Renderer)->m_Device->GetTransform(D3DTS_WORLD, &mWorld);
		((CBRenderD3D*)Game->m_Renderer)->m_Device->GetViewport(&vp);

		D3DXVec3Unproject(Pos, &Point, &vp, &mProj, &mView, &mWorld);

		//D3DXVec3Unproject(Pos, &Point, &m_DrawingViewport, &m_LastProjMat, &m_LastViewMat, &m_LastWorldMat);
		
		//Pos->y = m_WaypointHeight;
		Game->LOG(0, "Unpr:  %f, %f, %f", Pos->x, Pos->y, Pos->z);

		D3DXVECTOR3 NearestWpt = D3DXVECTOR3(0, 0, 0);
		float NearestLen = FLT_MAX;

		for(int i=0; i<m_WaypointGroups.GetSize(); i++){
			for(int j=0; j<m_WaypointGroups[i]->m_Points.GetSize(); j++){
				D3DXVECTOR3 Dir = *(m_WaypointGroups[i]->m_Points[j]) - *Pos;
				float Len = D3DXVec3Length(&Dir);
				if(fabs(Len)<fabs(NearestLen))
				{
					NearestLen = Len;
					NearestWpt = *m_WaypointGroups[i]->m_Points[j];
				}
			}
		}

		if(NearestWpt!=D3DXVECTOR3(0, 0, 0))
		{
			Game->LOG(0, "Orig: %f, %f", X, Y);
			Game->LOG(0, "New:  %f, %f, %f", NearestWpt.x, NearestWpt.y, NearestWpt.z);


			int_found = true;
			*Pos = NearestWpt;
		}		
	}
	*/
	
	return int_found;
}


//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::GetPath(D3DXVECTOR3 source, D3DXVECTOR3 target, CAdPath3D *path, bool Rerun)
{
	if(!m_PFReady) return false;
	else{
		source.y = GetHeightAt(source, m_WaypointHeight) + m_WaypointHeight;
		target.y = GetHeightAt(target, m_WaypointHeight) + m_WaypointHeight;

		m_PFReady = false;
		m_PFSource = source;
		m_PFTarget = target;
		m_PFTargetPath = path;
		m_PFAlternateTarget = D3DXVECTOR3(0, 0, 0);
		m_PFAlternateDist = FLT_MAX;

		m_PFTargetPath->Reset();
		m_PFTargetPath->SetReady(false);
		m_PFRerun = Rerun;

		// prepare working path
		int i,j;
		for(i=0; i<m_PFPath.GetSize(); i++)
			delete m_PFPath[i];
		m_PFPath.RemoveAll();

		// first point
		m_PFPath.Add(new CAdPathPoint3D(source, 0));

		// last point
		m_PFPath.Add(new CAdPathPoint3D(target, FLT_MAX));

		// add all active waypoints
		for(i=0; i<m_WaypointGroups.GetSize(); i++){
			if(m_WaypointGroups[i]->m_Active){
				for(j=0; j<m_WaypointGroups[i]->m_Points.GetSize(); j++){
					m_PFPath.Add(new CAdPathPoint3D(*m_WaypointGroups[i]->m_Points[j], FLT_MAX));
				}
			}
		}

		return true;
	}
}


//////////////////////////////////////////////////////////////////////////
void CAdSceneGeometry::PathFinderStep()
{
	int i;

	// get lowest unmarked
	float lowest_dist = FLT_MAX;
	CAdPathPoint3D* lowest_pt=NULL;

	for(i=0; i<m_PFPath.GetSize(); i++)
		if(!m_PFPath[i]->m_Marked && m_PFPath[i]->m_Distance < lowest_dist){
			lowest_dist = m_PFPath[i]->m_Distance;
			lowest_pt = m_PFPath[i];
		}

	if(lowest_pt == NULL){ // no path -> terminate PathFinder
		m_PFReady = true;
		if(!m_PFRerun)
		{
			/*
			D3DXVECTOR3 NewTarget = m_PFTarget;
			CorrectTargetPoint(m_PFSource, &NewTarget);
			if(NewTarget!=m_PFTarget)
			{
				GetPath(m_PFSource, NewTarget, m_PFTargetPath, true);
			}
			else m_PFTargetPath->SetReady(true);
			*/
			if(m_PFAlternateTarget!=D3DXVECTOR3(0, 0, 0))
			{
				GetPath(m_PFSource, m_PFAlternateTarget, m_PFTargetPath, true);
			}
			else m_PFTargetPath->SetReady(true);
		}
		else m_PFTargetPath->SetReady(true);
		return;
	}

	lowest_pt->m_Marked = true;

	// target point marked, generate path and terminate
	if(lowest_pt->m_Pos == m_PFTarget){
		while(lowest_pt!=NULL){
			m_PFTargetPath->m_Points.InsertAt(0, new D3DXVECTOR3(lowest_pt->m_Pos));
			lowest_pt = lowest_pt->m_Origin;
		}
		// remove current position
		if(m_PFTargetPath->m_Points.GetSize()>0)
		{
			delete m_PFTargetPath->m_Points[0];
			m_PFTargetPath->m_Points.RemoveAt(0);
		}
		

		m_PFReady = true;
		m_PFTargetPath->SetReady(true);
		return;
	}

	// otherwise keep on searching
	for(i=0; i<m_PFPath.GetSize(); i++)
		if(!m_PFPath[i]->m_Marked){
			float dist = GetPointsDist(lowest_pt->m_Pos, m_PFPath[i]->m_Pos);
			if(dist>=0 && lowest_pt->m_Distance + dist < m_PFPath[i]->m_Distance){
				m_PFPath[i]->m_Distance = lowest_pt->m_Distance + dist;
				m_PFPath[i]->m_Origin = lowest_pt;
			}
			else
			{
				if(!m_PFRerun && m_PFPath[i]->m_Pos == m_PFTarget)
				{
					D3DXVECTOR3 Line = m_PFPath[i]->m_Pos - lowest_pt->m_Pos;
					float Len = D3DXVec3Length(&Line);

					if(Len < m_PFAlternateDist)
					{
						m_PFAlternateDist = Len;
						m_PFAlternateTarget = GetBlockIntersection(&lowest_pt->m_Pos, &m_PFPath[i]->m_Pos);

						D3DXVECTOR3 Dir = m_PFAlternateTarget - lowest_pt->m_Pos;
						D3DXVec3Normalize(&Dir, &Dir);
						m_PFAlternateTarget -= Dir * 30;
					}
				}
			}
		}
}


//////////////////////////////////////////////////////////////////////////
float CAdSceneGeometry::GetPointsDist(D3DXVECTOR3 p1, D3DXVECTOR3 p2)
{
	if(!DirectPathExists(&p1, &p2)) return -1;

	D3DXVECTOR3 vect = p2 - p1;
	return D3DXVec3Length(&vect);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::InitLoop()
{
#ifdef _DEBUG
	int num_steps=0;
	DWORD start = Game->m_CurrentTime;
	while(!m_PFReady && Game->m_CurrentTime - start <= m_PFMaxTime){
		PathFinderStep();
		num_steps++;
	}
	//if(num_steps>0) Game->LOG(0, "STAT: PathFinder3D iterations in one loop: %d (%s)  m_PFMaxTime=%d", num_steps, m_PFReady?"finished":"not yet done", m_PFMaxTime);
#else
	DWORD start = Game->m_CurrentTime;
	while(!m_PFReady && Game->m_CurrentTime - start <= m_PFMaxTime) PathFinderStep();
#endif

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::CreateLights()
{
	int i;
	
	// disable all lights
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	int MaxLights = m_Renderer->GetMaxActiveLights();

	for(i=0; i<100; i++)
		m_Renderer->m_Device->LightEnable(i, FALSE);
	
	for(i=0; i<m_Lights.GetSize(); i++)
	{
		if(i >= 100) break;
		m_Lights[i]->SetLight(i);
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::EnableLights(D3DXVECTOR3 Point, CBArray<char*, char*>& IgnoreLights)
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	int MaxLights = m_Renderer->GetMaxActiveLights();

	int NumActiveLights = 0;
	for(int i=0; i<m_Lights.GetSize(); i++)
	{
		m_Lights[i]->m_IsAvailable = false;
		if(m_Lights[i]->m_Active) NumActiveLights++;
	}
	if(NumActiveLights <= MaxLights)
	{
		for(int i=0; i<m_Lights.GetSize(); i++)
		{
			m_Lights[i]->m_IsAvailable = true;
		}
	}
	else
	{
		if(!m_MaxLightsWarning)
		{
			Game->LOG(0, "Warning: Using more lights than the hardware supports (%d)", MaxLights);
			m_MaxLightsWarning = true;
		}

		CBArray<C3DLight*, C3DLight*> ActiveLights;

		// compute distance to point
		for(int i=0; i<m_Lights.GetSize(); i++)
		{
			if(!m_Lights[i]->m_Active) continue;

			D3DXVECTOR3 Dif;
			if(m_Lights[i]->m_IsSpotlight)
			{
				//Dif = m_Lights[i]->m_Target - Point;
				D3DXVECTOR3 Dir = m_Lights[i]->m_Target - m_Lights[i]->m_Pos;
				Dif = (m_Lights[i]->m_Pos + Dir * 0.75f) - Point;
			}
			else
				Dif = m_Lights[i]->m_Pos - Point;

			m_Lights[i]->m_Distance = fabs(D3DXVec3Length(&Dif));

			ActiveLights.Add(m_Lights[i]);
		}

		// sort by distance
		if(ActiveLights.GetSize() > 0)
		{
			qsort(ActiveLights.GetData(), ActiveLights.GetSize(), sizeof(C3DLight*), CAdSceneGeometry::CompareLights);

			for(int i=0; i<ActiveLights.GetSize(); i++)
			{
				ActiveLights[i]->m_IsAvailable = i < MaxLights;
			}
		}
	}


	// light all available lights
	for(int i=0; i<100; i++)
	{
		m_Renderer->m_Device->LightEnable(i, FALSE);
	}

	NumActiveLights = 0;
	for(int i=0; i<m_Lights.GetSize(); i++)
	{
		if(NumActiveLights >= MaxLights) break;

		if(IgnoreLights.GetSize())
		{
			bool Ignore = false;
			for(int j=0; j<IgnoreLights.GetSize(); j++)
			{
				char* c1 = m_Lights[i]->m_Name;
				char* c2 = IgnoreLights[j];
				if(stricmp(m_Lights[i]->m_Name, IgnoreLights[j])==0)
				{
					Ignore = true;
					break;
				}
			}
			if(Ignore) continue; // ship this light
		}

		if(m_Lights[i]->m_IsAvailable)
		{
			m_Renderer->m_Device->LightEnable(i, m_Lights[i]->m_Active);
			if(m_Lights[i]->m_Active) NumActiveLights++;
		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
int CAdSceneGeometry::CompareLights(const void* Obj1, const void* Obj2)
{
	C3DLight* Light1 = *(C3DLight**)Obj1;
	C3DLight* Light2 = *(C3DLight**)Obj2;

	if(Light1->m_Distance < Light2->m_Distance) return -1;
	else if(Light1->m_Distance > Light2->m_Distance) return 1;
	else return 0;
}



//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::CorrectTargetPoint(D3DXVECTOR3 Source, D3DXVECTOR3* Target)
{
	int i;
	int MaxLen = 1000;
	int Step = 10;
	D3DXVECTOR3 NewTarget;

	int LenLeft = 0;
	int LenRight = 0;
	int LenUp = 0;
	int LenDown = 0;


	// left
	NewTarget = *Target;
	for(i=1; i<=MaxLen; i+=Step)
	{
		NewTarget.x -= i;
		if(!DirectPathExists(Target, &NewTarget))
		{
			LenLeft = i;
			break;
		}
	}

	// right
	NewTarget = *Target;
	for(i=1; i<=MaxLen; i+=Step)
	{
		NewTarget.x += i;
		if(!DirectPathExists(Target, &NewTarget))
		{
			LenRight = i;
			break;
		}
	}

	// up
	NewTarget = *Target;
	for(i=1; i<=MaxLen; i+=Step)
	{
		NewTarget.z -= i;
		if(!DirectPathExists(Target, &NewTarget))
		{
			LenUp = i;
			break;
		}
	}

	// down
	NewTarget = *Target;
	for(i=1; i<=MaxLen; i+=Step)
	{
		NewTarget.z += i;
		if(!DirectPathExists(Target, &NewTarget))
		{
			LenDown = i;
			break;
		}
	}

	if(!LenLeft && !LenRight && !LenUp && !LenDown)	return S_OK;


	int OffsetX = INT_MAX;
	int OffsetZ = INT_MAX;
	
	if(LenLeft || LenRight)
	{
		if(LenRight)
		{
			if(LenLeft && LenLeft<LenRight) OffsetX = -LenLeft;
			else OffsetX = LenRight;
		}
		else OffsetX = -LenLeft;
	}
	
	if(LenUp || LenDown)
	{
		if(LenDown)
		{
			if(LenUp && LenUp<LenDown) OffsetZ = -LenUp;
			else OffsetZ = LenDown;
		}
		else OffsetZ = -LenUp;
	}

	//Game->QuickMessageForm("%f,%f,%f", Target->x, Target->y, Target->z);
	if(abs(OffsetX) < abs(OffsetZ)) Target->x += OffsetX;
	else Target->z += OffsetZ;

	//Game->QuickMessageForm("%f,%f,%f", Target->x, Target->y, Target->z);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::EnableNode(char* NodeName, bool Enable)
{
	HRESULT Ret = E_FAIL;

	int i;
	for(i=0; i<m_Blocks.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Blocks[i]->m_Name)==0)
		{
			m_Blocks[i]->m_Active = Enable;
			Ret = S_OK;
		}
	}

	for(i=0; i<m_Planes.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Planes[i]->m_Name)==0)
		{
			m_Planes[i]->m_Active = Enable;
			Ret = S_OK;
		}
	}

	for(i=0; i<m_Generics.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Generics[i]->m_Name)==0)
		{
			m_Generics[i]->m_Active = Enable;
			Ret = S_OK;
		}
	}

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::IsNodeEnabled(char* NodeName)
{
	for(int i=0; i<m_Blocks.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Blocks[i]->m_Name)==0)
		{
			return m_Blocks[i]->m_Active;
		}
	}
	for(int i=0; i<m_Planes.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Planes[i]->m_Name)==0)
		{
			return m_Planes[i]->m_Active;
		}
	}

	for(int i=0; i<m_Generics.GetSize(); i++)
	{
		if(stricmp(NodeName, m_Generics[i]->m_Name)==0)
		{
			return m_Generics[i]->m_Active;
		}
	}

	return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::EnableLight(char* LightName, bool Enable)
{
	HRESULT Ret = E_FAIL;

	int i;
	for(i=0; i<m_Lights.GetSize(); i++)
	{
		if(stricmp(LightName, m_Lights[i]->m_Name)==0)
		{
			m_Lights[i]->m_Active = Enable;
			Ret = S_OK;
		}
	}
	CreateLights();

	return Ret;
}

//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::IsLightEnabled(char* LightName)
{
	for(int i=0; i<m_Lights.GetSize(); i++)
	{
		if(stricmp(LightName, m_Lights[i]->m_Name)==0)
		{
			return m_Lights[i]->m_Active;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::SetLightColor(char* LightName, DWORD Color)
{
	HRESULT Ret = E_FAIL;

	int i;
	for(i=0; i<m_Lights.GetSize(); i++)
	{
		if(stricmp(LightName, m_Lights[i]->m_Name)==0)
		{
			m_Lights[i]->m_DiffuseColor = Color;
			Ret = S_OK;
		}
	}
	CreateLights();

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
DWORD CAdSceneGeometry::GetLightColor(char* LightName)
{
	for(int i=0; i<m_Lights.GetSize(); i++)
	{
		if(stricmp(LightName, m_Lights[i]->m_Name)==0)
		{
			return m_Lights[i]->m_DiffuseColor;
		}
	}
	return 0;
}

//////////////////////////////////////////////////////////////////////////
D3DXVECTOR3 CAdSceneGeometry::GetLightPos(char* LightName)
{
	for(int i=0; i<m_Lights.GetSize(); i++)
	{
		if(stricmp(LightName, m_Lights[i]->m_Name)==0)
		{
			return m_Lights[i]->m_Pos;
		}
	}
	return D3DXVECTOR3(0, 0, 0);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdSceneGeometry::Persist(CBPersistMgr* PersistMgr)
{
	CBObject::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_WaypointHeight));
	PersistMgr->Transfer(TMEMBER(m_WptMarker));

	if(!PersistMgr->m_Saving)
	{
		//m_WptMarker = NULL;
		LoadFile(m_Filename);
		m_LastValuesInitialized = false;
	}

	PersistMgr->Transfer(TMEMBER(m_ActiveCamera));
	PersistMgr->Transfer(TMEMBER(m_ActiveLight));
	PersistMgr->Transfer(TMEMBER(m_ViewMatrix));

	m_PFPath.Persist(PersistMgr);
	PersistMgr->Transfer(TMEMBER(m_PFReady));
	PersistMgr->Transfer(TMEMBER(m_PFSource));
	PersistMgr->Transfer(TMEMBER(m_PFTarget));
	PersistMgr->Transfer(TMEMBER(m_PFAlternateTarget));
	PersistMgr->Transfer(TMEMBER(m_PFTargetPath));
	PersistMgr->Transfer(TMEMBER(m_PFMaxTime));
	PersistMgr->Transfer(TMEMBER(m_PFRerun));

	// now save/load light/blocks/walkplanes/generic node states by name
	int i;

	//////////////////////////////////////////////////////////////////////////
	int NumLights = m_Lights.GetSize();
	PersistMgr->Transfer(TMEMBER(NumLights));
	for(i=0; i<NumLights; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_Lights[i]->m_Name));
			m_Lights[i]->Persist(PersistMgr);
		}
		else
		{
			char* Name = NULL;
			PersistMgr->Transfer(TMEMBER(Name));
			bool Found = false;
			for(int j=0; j<m_Lights.GetSize(); j++)
			{
				if(stricmp(Name, m_Lights[j]->m_Name)==0)
				{
					m_Lights[j]->Persist(PersistMgr);
					Found = true;
					break;
				}
			}
			if(!Found)
			{
				C3DLight* Light = new C3DLight(Game);
				Light->Persist(PersistMgr);
				delete Light;
			}
			SAFE_DELETE_ARRAY(Name);
		}
	}
	CreateLights();


	//////////////////////////////////////////////////////////////////////////
	int NumBlocks = m_Blocks.GetSize();
	PersistMgr->Transfer(TMEMBER(NumBlocks));
	for(i=0; i<NumBlocks; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_Blocks[i]->m_Name));
			m_Blocks[i]->Persist(PersistMgr);
		}
		else
		{
			char* Name = NULL;
			PersistMgr->Transfer(TMEMBER(Name));
			bool Found = false;
			for(int j=0; j<m_Blocks.GetSize(); j++)
			{
				if(stricmp(Name, m_Blocks[j]->m_Name)==0)
				{
					m_Blocks[j]->Persist(PersistMgr);
					Found = true;
					break;
				}
			}
			if(!Found)
			{
				CAdBlock* Block = new CAdBlock(Game);
				Block->Persist(PersistMgr);
				delete Block;
			}
			SAFE_DELETE_ARRAY(Name);
		}
	}

	//////////////////////////////////////////////////////////////////////////
	int NumPlanes = m_Planes.GetSize();
	PersistMgr->Transfer(TMEMBER(NumPlanes));
	for(i=0; i<NumPlanes; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_Planes[i]->m_Name));
			m_Planes[i]->Persist(PersistMgr);
		}
		else
		{
			char* Name = NULL;
			PersistMgr->Transfer(TMEMBER(Name));
			bool Found = false;
			for(int j=0; j<m_Planes.GetSize(); j++)
			{
				if(stricmp(Name, m_Planes[j]->m_Name)==0)
				{
					m_Planes[j]->Persist(PersistMgr);
					Found = true;
					break;
				}
			}
			if(!Found)
			{
				CAdWalkplane* Plane = new CAdWalkplane(Game);
				Plane->Persist(PersistMgr);
				delete Plane;
			}
			SAFE_DELETE_ARRAY(Name);
		}
	}


	//////////////////////////////////////////////////////////////////////////
	int NumGenerics = m_Generics.GetSize();
	PersistMgr->Transfer(TMEMBER(NumGenerics));
	for(i=0; i<NumGenerics; i++)
	{
		if(PersistMgr->m_Saving)
		{
			PersistMgr->Transfer(TMEMBER(m_Generics[i]->m_Name));
			m_Generics[i]->Persist(PersistMgr);
		}
		else
		{
			char* Name = NULL;
			PersistMgr->Transfer(TMEMBER(Name));
			bool Found = false;
			for(int j=0; j<m_Generics.GetSize(); j++)
			{
				if(stricmp(Name, m_Generics[j]->m_Name)==0)
				{
					m_Generics[j]->Persist(PersistMgr);
					Found = true;
					break;
				}
			}
			if(!Found)
			{
				CAdGeneric* Generic = new CAdGeneric(Game);
				Generic->Persist(PersistMgr);
				delete Generic;
			}
			SAFE_DELETE_ARRAY(Name);
		}
	}

	if(!PersistMgr->m_Saving) m_MaxLightsWarning = false;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
bool CAdSceneGeometry::Convert3Dto2D(D3DXVECTOR3* Pos, int* X, int* Y)
{
	D3DXMATRIX worldMat;
	D3DXMatrixIdentity(&worldMat);

	D3DXVECTOR3 Vect2D;
	D3DXVec3Project(&Vect2D, Pos, &m_DrawingViewport, &m_LastProjMat, &m_LastViewMat, &worldMat);
	*X = Vect2D.x + m_LastScrollX;
	*Y = Vect2D.y + m_LastScrollY;

	return true;
}