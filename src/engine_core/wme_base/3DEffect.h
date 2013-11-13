// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#ifdef WME_D3D9

#include "BBase.h"
#include <vector>


class C3DEffectScript;
class C3DRenderTarget;
class C3DEffectTexture;

//////////////////////////////////////////////////////////////////////////
class C3DEffect : public CBBase
{
public:
	enum SemanticType
	{
		SEMANTIC_UNKNOWN,
		SEMANTIC_WORLD,
		SEMANTIC_VIEW,
		SEMANTIC_PROJECTION,
		SEMANTIC_WORLDTRANSPOSE,
		SEMANTIC_VIEWTRANSPOSE,
		SEMANTIC_PROJECTIONTRANSPOSE,
		SEMANTIC_WORLDVIEW,
		SEMANTIC_WORLDVIEWPROJECTION,
		SEMANTIC_WORLDINVERSE,
		SEMANTIC_VIEWINVERSE,
		SEMANTIC_PROJECTIONINVERSE,
		SEMANTIC_WORLDINVERSETRANSPOSE,
		SEMANTIC_VIEWINVERSETRANSPOSE,
		SEMANTIC_PROJECTIONINVERSETRANSPOSE,
		SEMANTIC_WORLDVIEWINVERSE,
		SEMANTIC_WORLDVIEWTRANSPOSE,
		SEMANTIC_WORLDVIEWINVERSETRANSPOSE,
		SEMANTIC_WORLDVIEWPROJECTIONINVERSE,
		SEMANTIC_WORLDVIEWPROJECTIONTRANSPOSE,
		SEMANTIC_WORLDVIEWPROJECTIONINVERSETRANSPOSE,
		SEMANTIC_VIEWPROJECTION,
		SEMANTIC_VIEWPROJECTIONTRANSPOSE,
		SEMANTIC_VIEWPROJECTIONINVERSE,
		SEMANTIC_VIEWPROJECTIONINVERSETRANSPOSE,

		SEMANTIC_VIEWPOSITION,
		SEMANTIC_VIEWDIRECTION,

		SEMANTIC_DIFFUSEMAP,
		SEMANTIC_AMBIENT,
		SEMANTIC_DIFFUSE,
		SEMANTIC_SPECULAR,
		SEMANTIC_EMISSIVE,
		SEMANTIC_POWER,

		SEMANTIC_TIME,
		SEMANTIC_ELAPSEDTIME,

		SEMANTIC_ACTORLIGHTPOS,

		SEMANTIC_LIGHTPOS0,
		SEMANTIC_LIGHTDIR0,
		SEMANTIC_LIGHTDIFFUSE0,
		SEMANTIC_LIGHTAMBIENT0,
		SEMANTIC_LIGHTSPECULAR0,

		SEMANTIC_LIGHTPOS1,
		SEMANTIC_LIGHTDIR1,
		SEMANTIC_LIGHTDIFFUSE1,
		SEMANTIC_LIGHTAMBIENT1,
		SEMANTIC_LIGHTSPECULAR1,

		SEMANTIC_LIGHTPOS2,
		SEMANTIC_LIGHTDIR2,
		SEMANTIC_LIGHTDIFFUSE2,
		SEMANTIC_LIGHTAMBIENT2,
		SEMANTIC_LIGHTSPECULAR2,

		SEMANTIC_LIGHTPOS3,
		SEMANTIC_LIGHTDIR3,
		SEMANTIC_LIGHTDIFFUSE3,
		SEMANTIC_LIGHTAMBIENT3,
		SEMANTIC_LIGHTSPECULAR3,

		SEMANTIC_LIGHTPOS4,
		SEMANTIC_LIGHTDIR4,
		SEMANTIC_LIGHTDIFFUSE4,
		SEMANTIC_LIGHTAMBIENT4,
		SEMANTIC_LIGHTSPECULAR4,

		SEMANTIC_LIGHTPOS5,
		SEMANTIC_LIGHTDIR5,
		SEMANTIC_LIGHTDIFFUSE5,
		SEMANTIC_LIGHTAMBIENT5,
		SEMANTIC_LIGHTSPECULAR5,

		SEMANTIC_LIGHTPOS6,
		SEMANTIC_LIGHTDIR6,
		SEMANTIC_LIGHTDIFFUSE6,
		SEMANTIC_LIGHTAMBIENT6,
		SEMANTIC_LIGHTSPECULAR6,

		SEMANTIC_LIGHTPOS7,
		SEMANTIC_LIGHTDIR7,
		SEMANTIC_LIGHTDIFFUSE7,
		SEMANTIC_LIGHTAMBIENT7,
		SEMANTIC_LIGHTSPECULAR7,

	};


	C3DEffect(CBGame* inGame);
	~C3DEffect(void);
	HRESULT CreateFromFile(char* Filename);

	ID3DXEffect* GetEffect() { return m_Effect; }

	HRESULT InvalidateDeviceObjects();
	HRESULT RestoreDeviceObjects();
	
	C3DEffectScript* GetScriptForPass(int pass);
	HRESULT UpdateMatrices(D3DXMATRIX& matView, D3DXMATRIX& matProj, D3DXMATRIX& matWorld);
	HRESULT UpdateCamera(D3DXVECTOR3& cameraPos, D3DXVECTOR3& cameraTarget);
	HRESULT UpdateMaterial(C3DMaterial* material);
	HRESULT UpdateGlobals();
	HRESULT UpdateLights(CBObject* object);

	HRESULT SetRenderColorTarget(const std::string& name);
	HRESULT SetRenderDepthTarget(const std::string& name);

	char* GetFileName() { return m_Filename; }

	HRESULT StoreDefaultBuffers();
	HRESULT RestoreDefaultBuffers();

	HRESULT RestoreColorBuffer();
	HRESULT RestoreDepthBuffer();

private:	
	HRESULT InitializeParams();
	HRESULT InitializeScripts();
	HRESULT AddRenderTarget(D3DXHANDLE paramHandle, bool depthBuffer = false);
	HRESULT AddTexture(D3DXHANDLE paramHandle);
	
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, std::string& val);
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, float& val);
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, int& val);
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR2& val);
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR3& val);
	bool GetAnnotationValue(D3DXHANDLE paramHandle, const std::string& annName, D3DXVECTOR4& val);

	void SetLightPos(int index, D3DXHANDLE paramHandle);
	void SetLightDir(int index, D3DXHANDLE paramHandle);
	void SetLightAmbient(int index, D3DXHANDLE paramHandle);
	void SetLightDiffuse(int index, D3DXHANDLE paramHandle);
	void SetLightSpecular(int index, D3DXHANDLE paramHandle);

	D3DFORMAT ParseTextureFormat(const std::string& format);

	char* m_Filename;
	ID3DXEffect* m_Effect;

	typedef std::vector<C3DEffectScript*> ScriptVector;
	ScriptVector m_PassScripts;

	typedef std::map<std::string, C3DRenderTarget*> RenderTargetMap;
	RenderTargetMap m_RenderTargets;

	typedef std::map<std::string, C3DEffectTexture*> TextureMap;
	TextureMap m_Textures;

	typedef std::map<SemanticType, D3DXHANDLE> SemanticHandleMap;
	SemanticHandleMap m_Handles;

	
	LPDIRECT3DSURFACE m_DefaultColorBuffer;
	LPDIRECT3DSURFACE m_DefaultDepthBuffer;


	D3DXVECTOR4 ColorToVector(const D3DCOLORVALUE& color);

	// known semantics
	typedef std::map<std::string, SemanticType> SemanticStringMap;
	static SemanticStringMap s_SemanticStrings;
	
	static void BuildSemanticStringMap();
	static void AddSemanticString(const std::string& name, SemanticType type);
	static SemanticType GetSemanticType(const std::string& name);
};






//////////////////////////////////////////////////////////////////////////
class CShaderIncludeHelper : public ID3DXInclude
{
public:
	/// constructor
	CShaderIncludeHelper(CBGame* Game);
	/// open an include file and read its contents
	STDMETHOD(Open)(D3DXINCLUDE_TYPE IncludeType, LPCSTR pFileName, LPCVOID pParentData, LPCVOID *ppData, UINT *pBytes);
	/// close an include file
	STDMETHOD(Close)(LPCVOID pData);
private:
	CBGame* Game;
};

#endif // WME_D3D9