// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DEffect.h"
#include "3DEffectScript.h"
#include "3DRenderTarget.h"
#include "3DEffectTexture.h"
#include "BStringUtil.h"


#ifdef WME_D3D9


C3DEffect::SemanticStringMap C3DEffect::s_SemanticStrings;

//////////////////////////////////////////////////////////////////////////
C3DEffect::C3DEffect(CBGame* inGame) : CBBase(inGame)
{
	m_Filename = NULL;
	m_Effect = NULL;

	m_DefaultColorBuffer = NULL;
	m_DefaultDepthBuffer = NULL;

	BuildSemanticStringMap();
}

//////////////////////////////////////////////////////////////////////////
C3DEffect::~C3DEffect(void)
{
	for (ScriptVector::iterator it = m_PassScripts.begin(); it != m_PassScripts.end(); it++)
	{
		delete (*it);
	}
	m_PassScripts.clear();


	for (RenderTargetMap::iterator it = m_RenderTargets.begin(); it != m_RenderTargets.end(); it++)
	{
		delete it->second;
	}
	m_RenderTargets.clear();

	for (TextureMap::iterator it = m_Textures.begin(); it != m_Textures.end(); it++)
	{
		delete it->second;
	}
	m_Textures.clear();



	SAFE_DELETE_ARRAY(m_Filename);
	RELEASE(m_DefaultColorBuffer);
	RELEASE(m_DefaultDepthBuffer);
	RELEASE(m_Effect);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::InvalidateDeviceObjects()
{	
	HRESULT res = S_OK;

	if (m_Effect) m_Effect->OnLostDevice();

	for (RenderTargetMap::iterator it = m_RenderTargets.begin(); it != m_RenderTargets.end(); it++)
	{
		if (FAILED(it->second->InvalidateDeviceObjects())) res = E_FAIL;
	}
	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::RestoreDeviceObjects()
{
	HRESULT res = S_OK;

	if (m_Effect) m_Effect->OnResetDevice();

	for (RenderTargetMap::iterator it = m_RenderTargets.begin(); it != m_RenderTargets.end(); it++)
	{
		if (FAILED(it->second->RestoreDeviceObjects())) res = E_FAIL;
	}
	return res;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::CreateFromFile(char* Filename)
{
	HRESULT Ret;
	BYTE* Data;
	DWORD DataSize;

	Data = Game->m_FileManager->ReadWholeFile(Filename, &DataSize);
	if(!Data) return E_FAIL;

	CShaderIncludeHelper* IncHelper = new CShaderIncludeHelper(Game);
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	
	DWORD Flags = 0;
	//Flags |= D3DXSHADER_USE_LEGACY_D3DX9_31_DLL;
	//Flags |= D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT;
	//Flags |= D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT;
	//Flags |= D3DXSHADER_NO_PRESHADER;


	ID3DXBuffer* CompileErrors = NULL;


	CBUtils::SetString(&m_Filename, Filename);
	Ret = D3DXCreateEffect(Rend->m_Device, Data, DataSize, NULL, IncHelper, Flags, NULL, &m_Effect, &CompileErrors);

	if (FAILED(Ret))
	{
		Game->LOG(0, "Error compiling effect file '%s'", Filename);
		if (CompileErrors)
		{
			char* Buf = (char*)CompileErrors->GetBufferPointer();
			if(Buf) Game->LOG(0, "Effect compiler output: %s", Buf);
		}
	}

	SAFE_DELETE(IncHelper);
	RELEASE(CompileErrors);
	delete []  Data;

	if (FAILED(Ret)) return Ret;


	D3DXHANDLE tech;
	m_Effect->FindNextValidTechnique(NULL, &tech);
	if (tech == NULL)
	{
		Game->LOG(0, "No valid technique found in effect '%s'. Effect disabled.", Filename);
		return E_FAIL;
	}
	m_Effect->SetTechnique(tech);

	
	InitializeParams();
	InitializeScripts();

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::InitializeParams()
{
	D3DXEFFECT_DESC desc;
	m_Effect->GetDesc(&desc);


	for (int i = 0; i < desc.Parameters; i++)
	{
		D3DXHANDLE paramHandle = m_Effect->GetParameter(NULL, i);
		if (!paramHandle) continue;

		D3DXPARAMETER_DESC paramDesc;
		m_Effect->GetParameterDesc(paramHandle, &paramDesc);
	

		// handle textures and render targets
		switch (paramDesc.Type)
		{
		case D3DXPT_TEXTURE:
			if (paramDesc.Semantic && stricmp(paramDesc.Semantic, "RENDERCOLORTARGET") == 0)
				AddRenderTarget(paramHandle, false);
			if (paramDesc.Semantic && stricmp(paramDesc.Semantic, "RENDERDEPTHSTENCILTARGET") == 0)
				AddRenderTarget(paramHandle, true);
			else
				AddTexture(paramHandle);
			break;
		}

		// register handles of known semantics
		if (paramDesc.Semantic)
		{
			SemanticType type = GetSemanticType(paramDesc.Semantic);
			if (type != SEMANTIC_UNKNOWN)
			{
				m_Handles[type] = paramHandle;
			}
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::AddRenderTarget(D3DXHANDLE paramHandle, bool depthBuffer)
{
	D3DXPARAMETER_DESC paramDesc;
	m_Effect->GetParameterDesc(paramHandle, &paramDesc);

	// texture dimensions
	D3DXVECTOR2 dimensions;
	if (!GetAnnotationValue(paramHandle, "Dimensions", dimensions))
	{
		if (!GetAnnotationValue(paramHandle, "RenderTargetDimensions", dimensions))
		{
			float width = 0, height = 0;
			if (!GetAnnotationValue(paramHandle, "Width", width)) width = 512;
			if (!GetAnnotationValue(paramHandle, "Height", height)) height = 512;

			dimensions = D3DXVECTOR2(width, height);
		}
	}

	// texture format
	std::string formatStr;
	if (!GetAnnotationValue(paramHandle, "Format", formatStr))	formatStr = "D3DFMT_A8R8G8B8";
	D3DFORMAT format = ParseTextureFormat(formatStr);

	float clearDepth;
	bool shouldClearDepth;
	shouldClearDepth = GetAnnotationValue(paramHandle, "ClearDepth", clearDepth);

	int clearColor;
	bool shouldClearColor = GetAnnotationValue(paramHandle, "ClearColor", clearColor);

	int mipLevels;
	if (!GetAnnotationValue(paramHandle, "MipLevels", mipLevels)) mipLevels = 1;


	RenderTargetMap::iterator it;
	it = m_RenderTargets.find(paramDesc.Name);
	if (it != m_RenderTargets.end())
	{
		Game->LOG(0, "Duplicate render target name: %s", paramDesc.Name);
		return E_FAIL;
	}

	HRESULT res;
	C3DRenderTarget* target = new C3DRenderTarget(Game);
	if (depthBuffer)
		res = target->CreateRenderDepthTarget(format, dimensions.x, dimensions.y);
	else
		res = target->CreateRenderColorTarget(format, dimensions.x, dimensions.y, mipLevels);

	if (FAILED(res))
	{
		Game->LOG(res, "Error creating render target: %s", paramDesc.Name);
		SAFE_DELETE(target);
		return res;
	}

	if (shouldClearColor) target->SetClearColor(clearColor);
	if (shouldClearDepth) target->SetClearDepth(clearDepth);


	m_RenderTargets[paramDesc.Name] = target;
	
	m_Effect->SetTexture(paramHandle, target->GetTexture());

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::AddTexture(D3DXHANDLE paramHandle)
{
	D3DXPARAMETER_DESC paramDesc;
	m_Effect->GetParameterDesc(paramHandle, &paramDesc);

	std::string fileName;
	if (!GetAnnotationValue(paramHandle, "ResourceName", fileName))	return E_FAIL;
	
	C3DEffectTexture* texture = new C3DEffectTexture(Game);
	if (FAILED(texture->LoadFromFile(fileName)))
	{
		Game->LOG(0, "Error creating texture '%s' from file '%s'", paramDesc.Name, fileName.c_str());
		SAFE_DELETE(texture);
		return E_FAIL;
	}

	texture->SetHandle(paramHandle);
	m_Textures[paramDesc.Name] = texture;

	m_Effect->SetTexture(paramHandle, texture->GetTexture());

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::UpdateMatrices(D3DXMATRIX& matView, D3DXMATRIX& matProj, D3DXMATRIX& matWorld)
{
	D3DXMATRIX result;

	SemanticHandleMap::const_iterator it;
	for (it = m_Handles.begin(); it != m_Handles.end(); it++)
	{
		switch (it->first)
		{
		case SEMANTIC_WORLD:
			result = matWorld;
			break;
		case SEMANTIC_VIEW:
			result = matView;
			break;
		case SEMANTIC_PROJECTION:
			result = matProj;
			break;
		case SEMANTIC_WORLDTRANSPOSE:
			D3DXMatrixTranspose(&result, &matWorld);
			break;
		case SEMANTIC_VIEWTRANSPOSE:
			D3DXMatrixTranspose(&result, &matView);
			break;
		case SEMANTIC_PROJECTIONTRANSPOSE:
			D3DXMatrixTranspose(&result, &matProj);
			break;
		case SEMANTIC_WORLDVIEW:
			result = matWorld * matView;
			break;
		case SEMANTIC_WORLDVIEWPROJECTION:
			result = matWorld * matView * matProj;
			break;
		case SEMANTIC_WORLDINVERSE:
			D3DXMatrixInverse(&result, NULL, &matWorld);
			break;
		case SEMANTIC_VIEWINVERSE:
			D3DXMatrixInverse(&result, NULL, &matView);
			break;
		case SEMANTIC_PROJECTIONINVERSE:
			D3DXMatrixInverse(&result, NULL, &matProj);
			break;
		case SEMANTIC_WORLDINVERSETRANSPOSE:
			D3DXMatrixInverse(&result, NULL, &matWorld);
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_VIEWINVERSETRANSPOSE:
			D3DXMatrixInverse(&result, NULL, &matView);
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_PROJECTIONINVERSETRANSPOSE:
			D3DXMatrixInverse(&result, NULL, &matProj);
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_WORLDVIEWINVERSE:
			result = matWorld * matView;
			D3DXMatrixInverse(&result, NULL, &result);
			break;
		case SEMANTIC_WORLDVIEWTRANSPOSE:
			result = matWorld * matView;
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_WORLDVIEWINVERSETRANSPOSE:
			result = matWorld * matView;
			D3DXMatrixInverse(&result, NULL, &result);
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_WORLDVIEWPROJECTIONINVERSE:
			result = matWorld * matView * matProj;
			D3DXMatrixInverse(&result, NULL, &result);
			break;
		case SEMANTIC_WORLDVIEWPROJECTIONTRANSPOSE:
			result = matWorld * matView * matProj;
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_WORLDVIEWPROJECTIONINVERSETRANSPOSE:
			result = matWorld * matView * matProj;
			D3DXMatrixInverse(&result, NULL, &result);
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_VIEWPROJECTION:
			result = matView * matProj;
			break;
		case SEMANTIC_VIEWPROJECTIONTRANSPOSE:
			result = matView * matProj;
			D3DXMatrixTranspose(&result, &result);
			break;
		case SEMANTIC_VIEWPROJECTIONINVERSE:
			result = matView * matProj;
			D3DXMatrixInverse(&result, NULL, &result);
			break;
		case SEMANTIC_VIEWPROJECTIONINVERSETRANSPOSE:
			result = matView * matProj;
			D3DXMatrixInverse(&result, NULL, &result);
			D3DXMatrixTranspose(&result, &result);
			break;
		default:
			continue;
		}

		m_Effect->SetMatrix(it->second, &result);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::UpdateCamera(D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraTarget)
{
	SemanticHandleMap::const_iterator it;
	for (it = m_Handles.begin(); it != m_Handles.end(); it++)
	{
		switch (it->first)
		{
		case SEMANTIC_VIEWPOSITION:
			{
				D3DXVECTOR4 pos = cameraPos;
				pos.w = 0;
				m_Effect->SetVector(it->second, &pos);
			}
			break;
		case SEMANTIC_VIEWDIRECTION:
			{
				D3DXVECTOR4 dir = cameraTarget - cameraPos;
				dir.w = 0;
				m_Effect->SetVector(it->second, &dir);
			}
			break;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::UpdateMaterial(C3DMaterial* material)
{
	SemanticHandleMap::const_iterator it;
	for (it = m_Handles.begin(); it != m_Handles.end(); it++)
	{
		switch (it->first)
		{
		case SEMANTIC_DIFFUSEMAP:
			{
				CBSurfaceD3D* surface = (CBSurfaceD3D*)material->GetSurface();
				if (surface && surface->m_Texture)
					m_Effect->SetTexture(it->second, surface->m_Texture);
				else
					m_Effect->SetTexture(it->second, NULL);

			}
			break;

		case SEMANTIC_AMBIENT:
			m_Effect->SetVector(it->second, &ColorToVector(material->m_Material.Ambient));
			break;

		case SEMANTIC_DIFFUSE:
			m_Effect->SetVector(it->second, &ColorToVector(material->m_Material.Diffuse));
			break;

		case SEMANTIC_SPECULAR:
			m_Effect->SetVector(it->second, &ColorToVector(material->m_Material.Specular));
			break;

		case SEMANTIC_EMISSIVE:
			m_Effect->SetVector(it->second, &ColorToVector(material->m_Material.Emissive));
			break;

		case SEMANTIC_POWER:
			m_Effect->SetFloat(it->second, material->m_Material.Power);
			break;

		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::UpdateGlobals()
{
	SemanticHandleMap::const_iterator it;
	for (it = m_Handles.begin(); it != m_Handles.end(); it++)
	{
		switch (it->first)
		{
		case SEMANTIC_TIME:
			m_Effect->SetFloat(it->second, (float)Game->m_CurrentTime / 1000.0);
			break;

		case SEMANTIC_ELAPSEDTIME:
			m_Effect->SetFloat(it->second, (float)Game->m_DeltaTime / 1000.0);
			break;
		}
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::UpdateLights(CBObject* object)
{
	SemanticHandleMap::const_iterator it;
	for (it = m_Handles.begin(); it != m_Handles.end(); it++)
	{
		switch (it->first)
		{
		case SEMANTIC_ACTORLIGHTPOS:
			if (object)
				m_Effect->SetVector(it->second, &D3DXVECTOR4(object->m_ShadowLightPos.x, object->m_ShadowLightPos.y, object->m_ShadowLightPos.z, 0));
			break;

		case SEMANTIC_LIGHTPOS0:
			SetLightPos(0, it->second);
			break;
		case SEMANTIC_LIGHTDIR0:
			SetLightDir(0, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE0:
			SetLightDiffuse(0, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT0:
			SetLightAmbient(0, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR0:
			SetLightSpecular(0, it->second);
			break;

		case SEMANTIC_LIGHTPOS1:
			SetLightPos(1, it->second);
			break;
		case SEMANTIC_LIGHTDIR1:
			SetLightDir(1, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE1:
			SetLightDiffuse(1, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT1:
			SetLightAmbient(1, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR1:
			SetLightSpecular(1, it->second);
			break;

		case SEMANTIC_LIGHTPOS2:
			SetLightPos(2, it->second);
			break;
		case SEMANTIC_LIGHTDIR2:
			SetLightDir(2, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE2:
			SetLightDiffuse(2, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT2:
			SetLightAmbient(2, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR2:
			SetLightSpecular(2, it->second);
			break;

		case SEMANTIC_LIGHTPOS3:
			SetLightPos(3, it->second);
			break;
		case SEMANTIC_LIGHTDIR3:
			SetLightDir(3, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE3:
			SetLightDiffuse(3, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT3:
			SetLightAmbient(3, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR3:
			SetLightSpecular(3, it->second);
			break;

		case SEMANTIC_LIGHTPOS4:
			SetLightPos(4, it->second);
			break;
		case SEMANTIC_LIGHTDIR4:
			SetLightDir(4, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE4:
			SetLightDiffuse(4, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT4:
			SetLightAmbient(4, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR4:
			SetLightSpecular(4, it->second);
			break;

		case SEMANTIC_LIGHTPOS5:
			SetLightPos(5, it->second);
			break;
		case SEMANTIC_LIGHTDIR5:
			SetLightDir(5, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE5:
			SetLightDiffuse(5, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT5:
			SetLightAmbient(5, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR5:
			SetLightSpecular(5, it->second);
			break;

		case SEMANTIC_LIGHTPOS6:
			SetLightPos(6, it->second);
			break;
		case SEMANTIC_LIGHTDIR6:
			SetLightDir(6, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE6:
			SetLightDiffuse(6, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT6:
			SetLightAmbient(6, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR6:
			SetLightSpecular(6, it->second);
			break;

		case SEMANTIC_LIGHTPOS7:
			SetLightPos(7, it->second);
			break;
		case SEMANTIC_LIGHTDIR7:
			SetLightDir(7, it->second);
			break;
		case SEMANTIC_LIGHTDIFFUSE7:
			SetLightDiffuse(7, it->second);
			break;
		case SEMANTIC_LIGHTAMBIENT7:
			SetLightAmbient(7, it->second);
			break;
		case SEMANTIC_LIGHTSPECULAR7:
			SetLightSpecular(7, it->second);
			break;

		}
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
void C3DEffect::SetLightPos(int index, D3DXHANDLE paramHandle)
{
	D3DLIGHT light;
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->GetLight(index, &light);
	m_Effect->SetVector(paramHandle, &D3DXVECTOR4(light.Position.x, light.Position.y, light.Position.z, 0));
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::SetLightDir(int index, D3DXHANDLE paramHandle)
{
	D3DLIGHT light;
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->GetLight(index, &light);
	m_Effect->SetVector(paramHandle, &D3DXVECTOR4(light.Direction.x, light.Direction.y, light.Direction.z, 0));
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::SetLightAmbient(int index, D3DXHANDLE paramHandle)
{
	D3DLIGHT light;
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->GetLight(index, &light);
	m_Effect->SetVector(paramHandle, &ColorToVector(light.Ambient));
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::SetLightDiffuse(int index, D3DXHANDLE paramHandle)
{
	D3DLIGHT light;
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->GetLight(index, &light);
	m_Effect->SetVector(paramHandle, &ColorToVector(light.Diffuse));
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::SetLightSpecular(int index, D3DXHANDLE paramHandle)
{
	D3DLIGHT light;
	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;

	Rend->m_Device->GetLight(index, &light);
	m_Effect->SetVector(paramHandle, &ColorToVector(light.Specular));
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::SetRenderColorTarget(const std::string& name)
{
	RenderTargetMap::iterator it;

	if (name.empty()) it = m_RenderTargets.find("__DefaultRenderColorTarget");
	else it = m_RenderTargets.find(name);

	if (it != m_RenderTargets.end() && !it->second->IsDepthTarget()) return it->second->Activate();
	else
	{
		return E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::SetRenderDepthTarget(const std::string& name)
{
	RenderTargetMap::iterator it;

	if (name.empty()) it = m_RenderTargets.find("__DefaultRenderDepthTarget");
	else it = m_RenderTargets.find(name);

	if (it != m_RenderTargets.end() && it->second->IsDepthTarget()) return it->second->Activate();
	else
	{
		return E_FAIL;
	}
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::InitializeScripts()
{
	D3DXEFFECT_DESC desc;
	m_Effect->GetDesc(&desc);

	D3DXHANDLE techHandle = m_Effect->GetCurrentTechnique();
	if (!techHandle) return E_FAIL;

	D3DXTECHNIQUE_DESC techDesc;
	m_Effect->GetTechniqueDesc(techHandle, &techDesc);

	for (int i = 0; i < techDesc.Passes; i++)
	{
		C3DEffectScript* script = new C3DEffectScript(Game, this);
		m_PassScripts.push_back(script);

		D3DXHANDLE passHandle = m_Effect->GetPass(techHandle, i);
		if (!passHandle) continue;

		std::string strScript;
		if (!GetAnnotationValue(passHandle, "Script", strScript)) strScript.clear();

		script->ParseString(strScript);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
C3DEffectScript* C3DEffect::GetScriptForPass(int pass)
{
	if (pass < 0 || pass >= m_PassScripts.size()) return NULL;
	else return m_PassScripts[pass];
}


//////////////////////////////////////////////////////////////////////////
// helpers
//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, std::string& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	const char* annVal;
	m_Effect->GetString(annHandle, &annVal);
	val = std::string(annVal);
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, float& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	m_Effect->GetFloat(annHandle, &val);
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, int& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	m_Effect->GetInt(annHandle, &val);
	return true;
}


//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR2& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	float array[2];
	m_Effect->GetFloatArray(annHandle, array, 2);

	val = D3DXVECTOR2(array[0], array[1]);

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR3& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	float array[3];
	m_Effect->GetFloatArray(annHandle, array, 3);

	val = D3DXVECTOR3(array[0], array[1], array[2]);

	return true;
}

//////////////////////////////////////////////////////////////////////////
bool C3DEffect::GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR4& val)
{
	D3DXHANDLE annHandle = m_Effect->GetAnnotationByName(paramHandle, annName.c_str());
	if (!annHandle) return false;

	m_Effect->GetVector(annHandle, &val);
	return true;
}

//////////////////////////////////////////////////////////////////////////
D3DFORMAT C3DEffect::ParseTextureFormat(const std::string& format)
{
	if      (format == "A2R10G10B10")          return D3DFMT_A2R10G10B10;
	else if (format == "A8R8G8B8")             return D3DFMT_A8R8G8B8;
	else if (format == "X8R8G8B8")             return D3DFMT_X8R8G8B8;
	else if (format == "A1R5G5B5")             return D3DFMT_A1R5G5B5;
	else if (format == "X1R5G5B5")             return D3DFMT_X1R5G5B5;
	else if (format == "R5G6B5")               return D3DFMT_R5G6B5;
	else if (format == "D3DFMT_D16_LOCKABLE")  return D3DFMT_D16_LOCKABLE;
	else if (format == "D3DFMT_D32")           return D3DFMT_D32;
	else if (format == "D3DFMT_D15S1")         return D3DFMT_D15S1;
	else if (format == "D3DFMT_D24S8")         return D3DFMT_D24S8;
	else if (format == "D3DFMT_D24X8")         return D3DFMT_D24X8;
	else if (format == "D3DFMT_D24X4S4")       return D3DFMT_D24X4S4;
	else if (format == "D3DFMT_D32F_LOCKABLE") return D3DFMT_D32F_LOCKABLE;
	else if (format == "D3DFMT_D24FS8")        return D3DFMT_D24FS8;
	else if (format == "D3DFMT_D32_LOCKABLE")  return D3DFMT_D32_LOCKABLE;
	else if (format == "D3DFMT_S8_LOCKABLE")   return D3DFMT_S8_LOCKABLE;
	else if (format == "D3DFMT_D16")           return D3DFMT_D16;
	else if (format == "D3DFMT_VERTEXDATA")    return D3DFMT_VERTEXDATA;
	else if (format == "D3DFMT_INDEX16")       return D3DFMT_INDEX16;
	else if (format == "D3DFMT_INDEX32")       return D3DFMT_INDEX32;
	else if (format == "D3DFMT_R16F")          return D3DFMT_R16F;
	else if (format == "D3DFMT_G16R16F")       return D3DFMT_G16R16F;
	else if (format == "D3DFMT_A16B16G16R16F") return D3DFMT_A16B16G16R16F;
	else if (format == "D3DFMT_R8G8B8")        return D3DFMT_R8G8B8;
	else if (format == "D3DFMT_A8R8G8B8")      return D3DFMT_A8R8G8B8;
	else if (format == "D3DFMT_X8R8G8B8")      return D3DFMT_X8R8G8B8;
	else if (format == "D3DFMT_R5G6B5")        return D3DFMT_R5G6B5;
	else if (format == "D3DFMT_X1R5G5B5")      return D3DFMT_X1R5G5B5;
	else if (format == "D3DFMT_A1R5G5B5")      return D3DFMT_A1R5G5B5;
	else if (format == "D3DFMT_A4R4G4B4")      return D3DFMT_A4R4G4B4;
	else if (format == "D3DFMT_R3G3B2")        return D3DFMT_R3G3B2;
	else if (format == "D3DFMT_A8")            return D3DFMT_A8;
	else if (format == "D3DFMT_A8R3G3B2")      return D3DFMT_A8R3G3B2;
	else if (format == "D3DFMT_X4R4G4B4")      return D3DFMT_X4R4G4B4;
	else if (format == "D3DFMT_A2B10G10R10")   return D3DFMT_A2B10G10R10;
	else if (format == "D3DFMT_A8B8G8R8")      return D3DFMT_A8B8G8R8;
	else if (format == "D3DFMT_X8B8G8R8")      return D3DFMT_X8B8G8R8;
	else if (format == "D3DFMT_G16R16")        return D3DFMT_G16R16;
	else if (format == "D3DFMT_A2R10G10B10")   return D3DFMT_A2R10G10B10;
	else if (format == "D3DFMT_A16B16G16R16")  return D3DFMT_A16B16G16R16;
	else if (format == "D3DFMT_A8P8")          return D3DFMT_A8P8;
	else if (format == "D3DFMT_P8")            return D3DFMT_P8;
	else if (format == "D3DFMT_L8")            return D3DFMT_L8;
	else if (format == "D3DFMT_L16")           return D3DFMT_L16;
	else if (format == "D3DFMT_A8L8")          return D3DFMT_A8L8;
	else if (format == "D3DFMT_A4L4")          return D3DFMT_A4L4;
	else
	{
		Game->LOG(0, "Unknown texture format: %s, using default.", format.c_str());
		return D3DFMT_A8R8G8B8;
	}
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::BuildSemanticStringMap()
{
	if (!s_SemanticStrings.empty()) return;

	AddSemanticString("World",									SEMANTIC_WORLD);
	AddSemanticString("View",									SEMANTIC_VIEW);
	AddSemanticString("Projection",								SEMANTIC_PROJECTION);
	AddSemanticString("WorldTranspose",							SEMANTIC_WORLDTRANSPOSE);
	AddSemanticString("ViewTranspose",							SEMANTIC_VIEWTRANSPOSE);
	AddSemanticString("ProjectionTranspose",					SEMANTIC_PROJECTIONTRANSPOSE);
	AddSemanticString("WorldView",								SEMANTIC_WORLDVIEW);
	AddSemanticString("WorldViewProjection",					SEMANTIC_WORLDVIEWPROJECTION);
	AddSemanticString("WorldInverse",							SEMANTIC_WORLDINVERSE);
	AddSemanticString("ViewInverse",							SEMANTIC_VIEWINVERSE);
	AddSemanticString("ProjectionInverse",						SEMANTIC_PROJECTIONINVERSE);
	AddSemanticString("WorldInverseTranspose",					SEMANTIC_WORLDINVERSETRANSPOSE);
	AddSemanticString("ViewInverseTranspose",					SEMANTIC_VIEWINVERSETRANSPOSE);
	AddSemanticString("ProjectionInverseTranspose",				SEMANTIC_PROJECTIONINVERSETRANSPOSE);
	AddSemanticString("WorldViewInverse",						SEMANTIC_WORLDVIEWINVERSE);
	AddSemanticString("WorldViewTranspose",						SEMANTIC_WORLDVIEWTRANSPOSE);
	AddSemanticString("WorldViewInverseTranspose",				SEMANTIC_WORLDVIEWINVERSETRANSPOSE);
	AddSemanticString("WorldViewProjectionInverse",				SEMANTIC_WORLDVIEWPROJECTIONINVERSE);
	AddSemanticString("WorldViewProjectionTranspose",			SEMANTIC_WORLDVIEWPROJECTIONTRANSPOSE);
	AddSemanticString("WorldViewProjectionInverseTranspose",	SEMANTIC_WORLDVIEWPROJECTIONINVERSETRANSPOSE);
	AddSemanticString("ViewProjection",							SEMANTIC_VIEWPROJECTION);
	AddSemanticString("ViewProjectionTranspose",				SEMANTIC_VIEWPROJECTIONTRANSPOSE);
	AddSemanticString("ViewProjectionInverse",					SEMANTIC_VIEWPROJECTIONINVERSE);
	AddSemanticString("ViewProjectionInverseTranspose",			SEMANTIC_VIEWPROJECTIONINVERSETRANSPOSE);

	AddSemanticString("ViewPosition",							SEMANTIC_VIEWPOSITION);
	AddSemanticString("ViewDirection",							SEMANTIC_VIEWDIRECTION);

	AddSemanticString("DiffuseMap",								SEMANTIC_DIFFUSEMAP);
	AddSemanticString("Ambient",								SEMANTIC_AMBIENT);
	AddSemanticString("Diffuse",								SEMANTIC_DIFFUSE);
	AddSemanticString("Specular",								SEMANTIC_SPECULAR);
	AddSemanticString("Emissive",								SEMANTIC_EMISSIVE);
	AddSemanticString("Power",									SEMANTIC_POWER);

	AddSemanticString("Time",									SEMANTIC_TIME);
	AddSemanticString("ElapsedTime",							SEMANTIC_ELAPSEDTIME);

	AddSemanticString("ActorLightPos",							SEMANTIC_ACTORLIGHTPOS);

	AddSemanticString("LightPos0",								SEMANTIC_LIGHTPOS0);
	AddSemanticString("LightDir0",								SEMANTIC_LIGHTDIR0);
	AddSemanticString("LightDiffuse0",							SEMANTIC_LIGHTDIFFUSE0);
	AddSemanticString("LightAmbient0",							SEMANTIC_LIGHTAMBIENT0);
	AddSemanticString("LightSpecular0",							SEMANTIC_LIGHTSPECULAR0);

	AddSemanticString("LightPos1",								SEMANTIC_LIGHTPOS1);
	AddSemanticString("LightDir1",								SEMANTIC_LIGHTDIR1);
	AddSemanticString("LightDiffuse1",							SEMANTIC_LIGHTDIFFUSE1);
	AddSemanticString("LightAmbient1",							SEMANTIC_LIGHTAMBIENT1);
	AddSemanticString("LightSpecular1",							SEMANTIC_LIGHTSPECULAR1);

	AddSemanticString("LightPos2",								SEMANTIC_LIGHTPOS2);
	AddSemanticString("LightDir2",								SEMANTIC_LIGHTDIR2);
	AddSemanticString("LightDiffuse2",							SEMANTIC_LIGHTDIFFUSE2);
	AddSemanticString("LightAmbient2",							SEMANTIC_LIGHTAMBIENT2);
	AddSemanticString("LightSpecular2",							SEMANTIC_LIGHTSPECULAR2);

	AddSemanticString("LightPos3",								SEMANTIC_LIGHTPOS3);
	AddSemanticString("LightDir3",								SEMANTIC_LIGHTDIR3);
	AddSemanticString("LightDiffuse3",							SEMANTIC_LIGHTDIFFUSE3);
	AddSemanticString("LightAmbient3",							SEMANTIC_LIGHTAMBIENT3);
	AddSemanticString("LightSpecular3",							SEMANTIC_LIGHTSPECULAR3);

	AddSemanticString("LightPos4",								SEMANTIC_LIGHTPOS4);
	AddSemanticString("LightDir4",								SEMANTIC_LIGHTDIR4);
	AddSemanticString("LightDiffuse4",							SEMANTIC_LIGHTDIFFUSE4);
	AddSemanticString("LightAmbient4",							SEMANTIC_LIGHTAMBIENT4);
	AddSemanticString("LightSpecular4",							SEMANTIC_LIGHTSPECULAR4);

	AddSemanticString("LightPos5",								SEMANTIC_LIGHTPOS5);
	AddSemanticString("LightDir5",								SEMANTIC_LIGHTDIR5);
	AddSemanticString("LightDiffuse5",							SEMANTIC_LIGHTDIFFUSE5);
	AddSemanticString("LightAmbient5",							SEMANTIC_LIGHTAMBIENT5);
	AddSemanticString("LightSpecular5",							SEMANTIC_LIGHTSPECULAR5);

	AddSemanticString("LightPos6",								SEMANTIC_LIGHTPOS6);
	AddSemanticString("LightDir6",								SEMANTIC_LIGHTDIR6);
	AddSemanticString("LightDiffuse6",							SEMANTIC_LIGHTDIFFUSE6);
	AddSemanticString("LightAmbient6",							SEMANTIC_LIGHTAMBIENT6);
	AddSemanticString("LightSpecular6",							SEMANTIC_LIGHTSPECULAR6);

	AddSemanticString("LightPos7",								SEMANTIC_LIGHTPOS7);
	AddSemanticString("LightDir7",								SEMANTIC_LIGHTDIR7);
	AddSemanticString("LightDiffuse7",							SEMANTIC_LIGHTDIFFUSE7);
	AddSemanticString("LightAmbient7",							SEMANTIC_LIGHTAMBIENT7);
	AddSemanticString("LightSpecular7",							SEMANTIC_LIGHTSPECULAR7);
}


//////////////////////////////////////////////////////////////////////////
void C3DEffect::AddSemanticString(const std::string& name, SemanticType type)
{
	std::string nameUC = name;
	CBStringUtil::ToUpperCase(nameUC);
	
	s_SemanticStrings[nameUC] = type;
}


//////////////////////////////////////////////////////////////////////////
C3DEffect::SemanticType C3DEffect::GetSemanticType(const std::string& name)
{
	std::string nameUC = name;
	CBStringUtil::ToUpperCase(nameUC);

	SemanticStringMap::const_iterator it = s_SemanticStrings.find(nameUC);
	if (it == s_SemanticStrings.end()) return SEMANTIC_UNKNOWN;
	else return it->second;
}


//////////////////////////////////////////////////////////////////////////
D3DXVECTOR4 C3DEffect::ColorToVector(const D3DCOLORVALUE& color)
{
	return D3DXVECTOR4(color.r, color.g, color.b, color.a);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::StoreDefaultBuffers()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	m_Renderer->m_Device->GetRenderTarget(0, &m_DefaultColorBuffer);
	m_Renderer->m_Device->GetDepthStencilSurface(&m_DefaultDepthBuffer);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::RestoreDefaultBuffers()
{
	RestoreColorBuffer();
	RestoreDepthBuffer();

	RELEASE(m_DefaultColorBuffer);
	RELEASE(m_DefaultDepthBuffer);

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::RestoreColorBuffer()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	if (m_DefaultColorBuffer)
		return m_Renderer->m_Device->SetRenderTarget(0, m_DefaultColorBuffer);
	else
		return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DEffect::RestoreDepthBuffer()
{
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	if (m_DefaultDepthBuffer)
		return m_Renderer->m_Device->SetDepthStencilSurface(m_DefaultDepthBuffer);
	else
		return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
// CShaderIncludeHelper
//////////////////////////////////////////////////////////////////////////
CShaderIncludeHelper::CShaderIncludeHelper(CBGame* Game)
{
	this->Game = Game;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CShaderIncludeHelper::Open(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes)
{
	*ppData = Game->m_FileManager->ReadWholeFile((char*)pFileName, (DWORD*)pBytes, true);

	if(*ppData) return S_OK;
	else return E_FAIL;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CShaderIncludeHelper::Close(LPCVOID pData)
{
	if(pData) delete [] pData;
	return S_OK;
}

#endif // WME_D3D9
