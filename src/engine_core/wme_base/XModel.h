// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once
#include "BBase.h"
#include "XAnimationSet.h"
#include "XFrameNode.h"
#include "XActiveAnimation.h"
#include "XAnimationChannel.h"
#include "3DEffect.h"
#include "3DEffectParams.h"
#include "3DEffect_D3D8.h"

#define X_NUM_ANIMATION_CHANNELS 10

class CXModel :	public CBObject
{
private:
	class CXModelMatSprite
	{
	public:
		char* m_MatName;
		char* m_EffectFile;
		CBSprite* m_Sprite;
		CVidTheoraPlayer* m_Theora;
		C3DEffect* m_Effect;
		C3DEffectParams* m_EffectParams;

		CXModelMatSprite()
		{
			m_MatName = NULL;
			m_Sprite = NULL;
			m_Theora = NULL;
			m_Effect = NULL;
			m_EffectFile = NULL;
			m_EffectParams = NULL;
		}

		CXModelMatSprite(char* MatName, CBSprite* Sprite)
		{
			m_Theora = NULL;
			m_MatName = NULL;
			m_Effect = NULL;
			CBUtils::SetString(&m_MatName, MatName);
			m_Sprite = Sprite;
			m_EffectFile = NULL;
			m_EffectParams = NULL;
		}

		CXModelMatSprite(char* MatName, CVidTheoraPlayer* Theora)
		{
			m_Sprite = NULL;
			m_MatName = NULL;
			m_Effect = NULL;
			CBUtils::SetString(&m_MatName, MatName);
			m_Theora = Theora;
			m_EffectFile = NULL;
			m_EffectParams = NULL;
		}

		CXModelMatSprite(char* MatName, C3DEffect* Effect)
		{
			m_Sprite = NULL;
			m_MatName = NULL;			
			m_Theora = NULL;
			CBUtils::SetString(&m_MatName, MatName);
			m_Effect = Effect;
			m_EffectFile = NULL;

			m_EffectParams = new C3DEffectParams();
		}

		~CXModelMatSprite()
		{
			SAFE_DELETE_ARRAY(m_MatName);
			SAFE_DELETE_ARRAY(m_EffectFile);
			SAFE_DELETE(m_Sprite);
			SAFE_DELETE(m_Theora);
			SAFE_DELETE(m_Effect);
			SAFE_DELETE(m_EffectParams);
		}

		HRESULT SetSprite(CBSprite* Sprite)
		{
			SAFE_DELETE(m_Theora);
			SAFE_DELETE(m_Sprite);
			m_Sprite = Sprite;

			return S_OK;
		}

		HRESULT SetTheora(CVidTheoraPlayer* Theora)
		{
			SAFE_DELETE(m_Theora);
			SAFE_DELETE(m_Sprite);
			m_Theora = Theora;

			return S_OK;
		}

		HRESULT SetEffect(C3DEffect* effect)
		{
			SAFE_DELETE(m_Effect);

			m_Effect = effect;
			if (!m_EffectParams) m_EffectParams = new C3DEffectParams();
			else m_EffectParams->Clear();

			return S_OK;
		}

		HRESULT Persist(CBPersistMgr* PersistMgr)
		{
			PersistMgr->Transfer(TMEMBER(m_MatName));
			PersistMgr->Transfer(TMEMBER(m_Sprite));

			if(PersistMgr->CheckVersion(1, 8, 5))
			{
				PersistMgr->Transfer(TMEMBER(m_Theora));
			}
			else
			{
				m_Theora = NULL;
			}

			if(PersistMgr->CheckVersion(1, 9, 0))
			{				
				if (PersistMgr->m_Saving)
				{
					char* effectFileName = NULL;
					if (m_Effect)
						CBUtils::SetString(&effectFileName, m_Effect->GetFileName());
					else
						effectFileName = NULL;

					PersistMgr->Transfer(TMEMBER(effectFileName));
					SAFE_DELETE_ARRAY(effectFileName);
				}
				else
				{
					PersistMgr->Transfer(TMEMBER(m_EffectFile));
				}				
			}
			else
			{
				m_Effect = NULL;
			}

			
			if(PersistMgr->CheckVersion(1, 9, 1))
			{
				if (PersistMgr->m_Saving)
				{
					bool hasParams = (m_EffectParams != NULL);
					PersistMgr->Transfer(TMEMBER(hasParams));

					if (hasParams) m_EffectParams->Persist(PersistMgr);
				}
				else
				{
					bool hasParams;
					PersistMgr->Transfer(TMEMBER(hasParams));

					if (hasParams)
					{
						m_EffectParams = new C3DEffectParams();
						m_EffectParams->Persist(PersistMgr);
					}
					else m_EffectParams = NULL;
				}
			}
			else m_EffectParams = NULL;
			

			return S_OK;
		}
	};

public:
	DECLARE_PERSISTENT(CXModel, CBObject);

	CXModel(CBGame* inGame, CBObject* Owner);
	virtual ~CXModel();

	CXModel* m_ParentModel;

	HRESULT LoadFromFile(char* Filename, CXModel* ParentModel=NULL);
	HRESULT MergeFromFile(char* Filename);

	HRESULT LoadAnimationSet(char* Filename, LPDIRECTXFILEDATA XObj);
	HRESULT LoadAnimation(char* Filename, LPDIRECTXFILEDATA XObj, CXAnimationSet* ParentAnimSet = NULL);

	HRESULT Update();
	HRESULT Render();
	HRESULT Reset();

	HRESULT UpdateShadowVol(C3DShadowVolume* Shadow, D3DXMATRIX* ModelMat, D3DXVECTOR3* Light, float ExtrusionDepth);

	HRESULT PlayAnim(int Channel, char* Name, DWORD TransitionTime=0, bool ForceReset = false, DWORD StopTransitionTime=0);
	bool IsAnimPending(char* AnimName=NULL);
	bool IsAnimPending(int Channel, char* AnimName=NULL);

	bool IsTransparentAt(int X, int Y);

	static HRESULT LoadName(CBNamedObject* Obj, LPDIRECTXFILEDATA Data);
	static HRESULT LoadName(char** TargetStr, LPDIRECTXFILEDATA Data);

	RECT m_BoundingRect;
	CBObject* m_Owner;

	HRESULT ParseAnim(BYTE* Buffer);
	HRESULT ParseEvent(CXAnimationSet* Anim, BYTE* Buffer);
	CXAnimationSet* GetAnimationSetByName(char* Name);

	HRESULT StopAnim(int Channel, DWORD TransitionTime);
	HRESULT StopAnim(DWORD TransitionTime);

	D3DXMATRIX* GetBoneMatrix(char* BoneName);
	CXFrameNode* GetRootFrame();

	HRESULT SetMaterialSprite(char* MaterialName, char* SpriteFilename);
	HRESULT SetMaterialTheora(char* MaterialName, char* TheoraFilename);
	HRESULT SetMaterialEffect(char* MaterialName, char* EffectFilename);
	HRESULT RemoveMaterialEffect(char* MaterialName);
	HRESULT SetMaterialEffectParam(char* MaterialName, char* ParamName, CScValue* Val);
	HRESULT SetMaterialEffectParam(char* MaterialName, char* ParamName, D3DXVECTOR4 Val);
	HRESULT InitializeSimple();

	virtual HRESULT InvalidateDeviceObjects();
	virtual HRESULT RestoreDeviceObjects();

	HRESULT UnloadAnimation(char* AnimName);

	DWORD m_TicksPerSecond;

	CBArray<CXAnimationSet*, CXAnimationSet*> m_AnimationSets;

private:
	void Cleanup(bool Complete = true);
	HRESULT FindBones(bool AnimOnly = false, CXModel* ParentModel=NULL);

	void UpdateBoundingRect();
	void static inline UpdateRect(RECT* rc, D3DXVECTOR3* vec);	
	D3DVIEWPORT m_DrawingViewport;
	D3DXMATRIX m_LastWorldMat;
	D3DXMATRIX m_LastViewMat;
	D3DXMATRIX m_LastProjMat;
	int m_LastOffsetX;
	int m_LastOffsetY;
	
	D3DXVECTOR3 m_BBoxStart;
	D3DXVECTOR3 m_BBoxEnd;
	
protected:
	CBArray<char*, char*> m_MergedModels;
	CXAnimationChannel* m_Channels[X_NUM_ANIMATION_CHANNELS];
	
	CXFrameNode* m_RootFrame;

	CBArray<CXModelMatSprite*, CXModelMatSprite*> m_MatSprites;
};
