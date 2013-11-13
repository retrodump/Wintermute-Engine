// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdActor3DX.h"


IMPLEMENT_PERSISTENT(CAdActor3DX, false);

//////////////////////////////////////////////////////////////////////////
CAdActor3DX::CAdActor3DX(CBGame* inGame):CAdObject3D(inGame)
{
	m_TargetPoint3D = D3DXVECTOR3(0,0,0);
	m_TargetPoint2D = new CBPoint;

	m_TargetAngle = 0;
	m_AfterWalkAngle = -1.0f;

	m_Path3D = new CAdPath3D(Game);
	m_Path2D = new CAdPath(Game);

	m_TalkAnimName = NULL;
	CBUtils::SetString(&m_TalkAnimName, "talk");

	m_IdleAnimName = NULL;
	CBUtils::SetString(&m_IdleAnimName, "idle");

	m_WalkAnimName = NULL;
	CBUtils::SetString(&m_WalkAnimName, "walk");

	m_TurnLeftAnimName = NULL;
	CBUtils::SetString(&m_TurnLeftAnimName, "turnleft");

	m_TurnRightAnimName = NULL;
	CBUtils::SetString(&m_TurnRightAnimName, "turnright");

	m_TalkAnimChannel = 0;

	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	m_Renderer->EnableShadows();

	// direct controls
	m_DirectWalkMode = DIRECT_WALK_NONE;
	m_DirectTurnMode = DIRECT_TURN_NONE;
	m_DirectWalkAnim = NULL;
	m_DirectTurnAnim = NULL;
	m_DirectWalkVelocity = 0.0f;
	m_DirectTurnVelocity = 0.0f;

	m_DefaultTransTime = 200;
	m_DefaultStopTransTime = 200;
	m_StateAnimChannel = -1;

	m_GoToTolerance = 2;

	m_PartBone = NULL;
	m_PartOffset = D3DXVECTOR3(0, 0, 0);
}


//////////////////////////////////////////////////////////////////////////
CAdActor3DX::~CAdActor3DX()
{
	// delete attachments
	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		delete m_Attachments[i];
	}
	m_Attachments.RemoveAll();


	// delete transition times
	for(int i=0; i<m_TransitionTimes.GetSize(); i++)
	{
		delete m_TransitionTimes[i];
	}
	m_TransitionTimes.RemoveAll();


	SAFE_DELETE_ARRAY(m_TalkAnimName);
	SAFE_DELETE_ARRAY(m_IdleAnimName);
	SAFE_DELETE_ARRAY(m_WalkAnimName);
	SAFE_DELETE_ARRAY(m_TurnLeftAnimName);
	SAFE_DELETE_ARRAY(m_TurnRightAnimName);

	SAFE_DELETE_ARRAY(m_DirectWalkAnim);
	SAFE_DELETE_ARRAY(m_DirectTurnAnim);

	SAFE_DELETE(m_Path3D);
	SAFE_DELETE(m_Path2D);

	SAFE_DELETE(m_TargetPoint2D);

	SAFE_DELETE_ARRAY(m_PartBone);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::Update()
{
	if(!m_ModelX) return S_OK;
	if(Game->m_State==GAME_FROZEN) return S_OK;	

	CAdGame* AdGame = (CAdGame*)Game;

	if(m_State==STATE_READY && m_StateAnimChannel >= 0 && m_ModelX)
	{
		//if(!m_ModelX->IsAnimPending(m_StateAnimChannel)) m_ModelX->StopAnim(m_StateAnimChannel);
		m_StateAnimChannel = -1;
	}

	if(m_Sentence && m_State!=STATE_TALKING)
	{
		m_Sentence->Finish();
		SAFE_DELETE(m_Sentence);

		// kill talking anim
		if(m_TalkAnimChannel > 0) m_ModelX->StopAnim(m_TalkAnimChannel, m_DefaultStopTransTime);
	}

	// update state
	switch(m_State)
	{
		//////////////////////////////////////////////////////////////////////////
		case STATE_DIRECT_CONTROL:
			if(m_DirectWalkMode==DIRECT_WALK_NONE && m_DirectTurnMode==DIRECT_TURN_NONE)
			{
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
			else
			{
				// set animation
				if(m_DirectWalkMode!=DIRECT_WALK_NONE)
				{
					//if(m_DirectWalkAnim!=NULL) m_ModelX->PlayAnim(0, m_DirectWalkAnim, true, m_DefaultTransTime);
					//else m_ModelX->PlayAnim(0, m_WalkAnimName, true, m_DefaultTransTime);
				}
				else if(m_DirectTurnMode!=DIRECT_TURN_NONE)
				{
					if(m_DirectTurnAnim!=NULL) m_ModelX->PlayAnim(0, m_DirectTurnAnim, m_DefaultTransTime, false, m_DefaultStopTransTime);
					else m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
				}
				
				// move and/or turn
				float TurnVel = m_DirectTurnVelocity==0.0f?m_AngVelocity:m_DirectTurnVelocity;
				if(m_DirectTurnMode==DIRECT_TURN_CW)
				{
					m_Angle += TurnVel * (float)Game->m_TimerDelta/1000.f;
					m_Angle = CBUtils::NormalizeAngle(m_Angle);
				}

				if(m_DirectTurnMode==DIRECT_TURN_CCW)
				{
					m_Angle -= TurnVel * (float)Game->m_TimerDelta/1000.f;
					m_Angle = CBUtils::NormalizeAngle(m_Angle);
				}

				float WalkVel = m_DirectWalkVelocity==0.0f?m_Velocity:m_DirectWalkVelocity;
				D3DXVECTOR3 NewPos = m_PosVector;
				if(m_DirectWalkMode==DIRECT_WALK_FW)
				{
					NewPos.x += -sinf(DegToRad(m_Angle)) * WalkVel * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
					NewPos.z += -cosf(DegToRad(m_Angle)) * WalkVel * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
				}
				if(m_DirectWalkMode==DIRECT_WALK_BK)
				{
					NewPos.x -= -sinf(DegToRad(m_Angle)) * WalkVel * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
					NewPos.z -= -cosf(DegToRad(m_Angle)) * WalkVel * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
				}
				CAdScene* Scene = ((CAdGame*)Game)->m_Scene;
				if(Scene && Scene->m_Geom)
				{
					bool CanWalk = false;

					if(Scene->m_2DPathfinding)
					{
						D3DXMATRIX NewWorldMat;
						GetMatrix(&NewWorldMat, &NewPos);

						int NewX, NewY;
						Convert3DTo2D(&NewWorldMat, &NewX, &NewY);
						CanWalk = !Scene->IsBlockedAt(NewX, NewY, false, this);
					}
					else
					{
						CanWalk = Scene->m_Geom->DirectPathExists(&m_PosVector, &NewPos);
					}


					if(CanWalk)
					{
						if(m_DirectWalkAnim!=NULL) m_ModelX->PlayAnim(0, m_DirectWalkAnim, m_DefaultTransTime, false, m_DefaultStopTransTime);
						else m_ModelX->PlayAnim(0, m_WalkAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);

						m_PosVector = NewPos;
					}
					else
					{
						m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
					}
				}
			}
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_TURNING:
			if(m_TurningLeft)
				m_ModelX->PlayAnim(0, m_TurnLeftAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
			else
				m_ModelX->PlayAnim(0, m_TurnRightAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);

			if(TurnToStep(m_AngVelocity))
			{
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
		break;

		//////////////////////////////////////////////////////////////////////////
		case STATE_SEARCHING_PATH:
			// keep asking scene for the path
			m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
			
			if(AdGame->m_Scene->m_2DPathfinding)
			{
				if(AdGame->m_Scene->GetPath(CBPoint(m_PosX, m_PosY), *m_TargetPoint2D, m_Path2D, this))
					m_State = STATE_WAITING_PATH;
			}
			else
			{
				if(AdGame->m_Scene->m_Geom->GetPath(m_PosVector, m_TargetPoint3D, m_Path3D))
					m_State = STATE_WAITING_PATH;
			}
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_WAITING_PATH:
			// wait until the scene finished the path
			m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
			if(AdGame->m_Scene->m_2DPathfinding)
			{
				if(m_Path2D->m_Ready) FollowPath2D();
			}
			else
			{
				if(m_Path3D->m_Ready) FollowPath3D();
			}
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_FOLLOWING_PATH:
			if(AdGame->m_Scene->m_2DPathfinding) GetNextStep2D();
			else GetNextStep3D();

			m_ModelX->PlayAnim(0, m_WalkAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
		break;

		//////////////////////////////////////////////////////////////////////////
		case STATE_TALKING:
		{
			m_Sentence->Update();
			if(m_Sentence->m_CurrentSkelAnim) m_TempSkelAnim = m_Sentence->m_CurrentSkelAnim;

			bool TimeIsUp = (m_Sentence->m_Sound && m_Sentence->m_SoundStarted && (!m_Sentence->m_Sound->IsPlaying()&&!m_Sentence->m_Sound->IsPaused())) || (!m_Sentence->m_Sound && m_Sentence->m_Duration <= Game->m_Timer - m_Sentence->m_StartTime);
			if(m_TempSkelAnim==NULL || !m_ModelX->IsAnimPending(0, m_TempSkelAnim) || TimeIsUp)
			{
				if(TimeIsUp)
				{
					m_Sentence->Finish();
					m_TempSkelAnim = NULL;
					m_State = m_NextState;
					m_NextState = STATE_READY;

					if(m_TalkAnimChannel > 0) m_ModelX->StopAnim(m_TalkAnimChannel, m_DefaultStopTransTime);
				}
				else
				{
					m_TempSkelAnim = m_Sentence->GetNextStance();
					if(m_TempSkelAnim) m_ModelX->PlayAnim(0, m_TempSkelAnim, m_DefaultTransTime, true, m_DefaultStopTransTime);
					else
					{
						if(m_ModelX->GetAnimationSetByName(m_TalkAnimName))
							m_ModelX->PlayAnim(m_TalkAnimChannel, m_TalkAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
						else
							m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
					}

					((CAdGame*)Game)->AddSentence(m_Sentence);
				}
			}
			else
			{
				if(m_TempSkelAnim) m_ModelX->PlayAnim(0, m_TempSkelAnim, m_DefaultTransTime, false, m_DefaultStopTransTime);
				((CAdGame*)Game)->AddSentence(m_Sentence);
			}
		}
		break;

		case STATE_PLAYING_ANIM:
			if(m_StateAnimChannel!=0) m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_READY:
			m_ModelX->PlayAnim(0, m_IdleAnimName, m_DefaultTransTime, false, m_DefaultStopTransTime);
		break;
	} // switch(m_State)

	// finished playing animation?
	if(m_State==STATE_PLAYING_ANIM && !m_ModelX->IsAnimPending(m_StateAnimChannel))
	{
		m_State = m_NextState;
		m_NextState = STATE_READY;
	}

	UpdateBlockRegion();
	m_Ready = (m_State == STATE_READY);


	// setup 2D position
	int OrigX = m_PosX;
	int OrigY = m_PosY;
	
	HRESULT Ret = CAdObject3D::Update();
	if(OrigX!=m_PosX || OrigY!=m_PosY) AfterMove();

	if(m_ModelX)
	{
		m_ModelX->Update();
		if(m_ShadowModel) m_ShadowModel->Update();
	}
	UpdateAttachments();
	UpdatePartEmitter();	

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::Display()
{
	if(!m_ModelX) return S_OK;
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	UpdateSounds();
	SetupLights();

	m_Renderer->SetSpriteBlendMode(m_BlendMode);

	if(m_HasAmbientLightColor)
	{
		m_Renderer->SetAmbientLightColor(m_AmbientLightColor);
	}
	
	TShadowType ShadowType = Game->GetMaxShadowType(this);
	if(ShadowType==SHADOW_STENCIL) DisplayShadowVolume();
	else if(ShadowType > SHADOW_NONE) m_Renderer->DisplayShadow(this, &D3DXVECTOR3(m_ShadowLightPos.x * m_Scale3D, m_ShadowLightPos.y * m_Scale3D, m_ShadowLightPos.z * m_Scale3D), NULL, true);
	
	//if(m_ShadowEnabled) m_Renderer->DisplayShadow(this, &D3DXVECTOR3(m_ShadowLightPos.x * m_Scale3D, m_ShadowLightPos.y * m_Scale3D, m_ShadowLightPos.z * m_Scale3D), NULL, true);	
	
		
	//if(m_ShadowEnabled) m_Renderer->DisplayShadow(this, &D3DXVECTOR3(162, 225, -314), &D3DXVECTOR3(162, 1, -313), false);
	
	m_Renderer->SetSpriteBlendMode(m_BlendMode, true);
	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);	
	HRESULT res = m_ModelX->Render();


	if(m_Registrable) Game->m_Renderer->m_RectList.Add(new CBActiveRect(Game, this, m_ModelX, m_ModelX->m_BoundingRect.left, m_ModelX->m_BoundingRect.top, m_ModelX->m_BoundingRect.right - m_ModelX->m_BoundingRect.left, m_ModelX->m_BoundingRect.bottom-m_ModelX->m_BoundingRect.top, true));	
	m_Renderer->m_LastTexture = NULL;

	DisplayAttachments(true);

	if(m_HasAmbientLightColor) m_Renderer->SetDefaultAmbientLightColor();


	if(m_Active && m_PartEmitter)
	{
		m_Renderer->Setup2D();
		m_PartEmitter->Display();
	}


	// accessibility
	if(Game->m_AccessMgr->GetActiveObject()==this)
	{
		Game->m_AccessMgr->SetHintRect(&m_ModelX->m_BoundingRect);
	}


	return res;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::RenderModel()
{
	if(!m_ModelX) return S_OK;
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	HRESULT res;
	if(m_ShadowModel) res = m_ShadowModel->Render();
	else res = m_ModelX->Render();

	DisplayAttachments(false);
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::DisplayShadowVolume()
{
	D3DXVECTOR3 pos;
	D3DXVECTOR3 target;
	D3DXVECTOR3 LightVector;
	float ExtrusionDepth;

	if(!m_ModelX) return E_FAIL;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	Rend->m_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	
	// for future use
	bool LightPosRelative = true;
	D3DXVECTOR3 LightPos = D3DXVECTOR3(m_ShadowLightPos.x * m_Scale3D, m_ShadowLightPos.y * m_Scale3D, m_ShadowLightPos.z * m_Scale3D);
	D3DXVECTOR3* LightTarget = NULL;

	if(LightPosRelative) pos = m_PosVector + LightPos;
	else pos = LightPos;

	if(LightTarget) target = *LightTarget;
	else target	 = m_PosVector;


	LightVector = pos - target;
	ExtrusionDepth = D3DXVec3Length(&LightVector) * 1.5f;
	D3DXVec3Normalize(&LightVector, &LightVector);
	
	GetShadowVolume()->SetColor(m_ShadowColor);

	bool Update = true;
	/*
	m_ShadowUpdateFrames++;
	if(m_ShadowUpdateFrames > 1)
	{
		Update = true;
		m_ShadowUpdateFrames = 0;
	}
	*/
	
	if(Update)
	{
		GetShadowVolume()->Reset();
		CXModel* ShadowModel;
		if(m_ShadowModel) ShadowModel = m_ShadowModel;
		else ShadowModel = m_ModelX;

		ShadowModel->UpdateShadowVol(GetShadowVolume(), &m_WorldMatrix, &LightVector, ExtrusionDepth);
	}

	D3DXMATRIX OrigWorld;
	Rend->m_Device->GetTransform(D3DTS_WORLD, &OrigWorld);

	// handle the attachments
	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		CAdAttach3DX* At = m_Attachments[i];
		if(!At->m_Active) continue;

		D3DXMATRIX* BoneMat = m_ModelX->GetBoneMatrix(At->GetParentBone());
		if(!BoneMat) continue;

		D3DXMATRIX ViewMat;
		D3DXMatrixMultiply(&ViewMat, BoneMat, &m_WorldMatrix);

		At->DisplayShadowVol(&ViewMat, &LightVector, ExtrusionDepth, Update);
	}

	// restore model's world matrix and render the shadow volume
	Rend->m_Device->SetTransform(D3DTS_WORLD, &OrigWorld);
	
	GetShadowVolume()->RenderToStencilBuffer();

	// finally display all the shadows rendered into stencil buffer
	GetShadowVolume()->RenderToScene();
		

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::UpdateAttachments()
{
	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		if(m_Attachments[i]->m_Active) m_Attachments[i]->Update();
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::DisplayAttachments(bool RegisterObjects)
{
	if(!m_ModelX) return E_FAIL;
	if(m_Attachments.GetSize()==0) return S_OK;

	CBRenderD3D* Rend = (CBRenderD3D*)Game->m_Renderer;
	D3DXMATRIX OrigView;
	Rend->m_Device->GetTransform(D3DTS_WORLD, &OrigView);

	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		CAdAttach3DX* At = m_Attachments[i];
		if(!At->m_Active) continue;
		
		D3DXMATRIX* BoneMat = m_ModelX->GetBoneMatrix(At->GetParentBone());
		if(!BoneMat) continue;

		D3DXMATRIX ViewMat;
		D3DXMatrixMultiply(&ViewMat, BoneMat, &OrigView);

		At->Display(&ViewMat, RegisterObjects);
	}
	Rend->m_Device->SetTransform(D3DTS_WORLD, &OrigView);

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::TurnTo(float Angle)
{
	m_TurningLeft = PrepareTurn(Angle);

	if(m_TargetAngle==m_Angle)
	{ // no need to turn
		m_State = m_NextState;
		m_NextState = STATE_READY;
	}
	else m_State = STATE_TURNING;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::GoTo3D(D3DXVECTOR3 TargetPos, float TargetAngle)
{
	m_AfterWalkAngle = TargetAngle;

	if(m_TargetPoint3D == TargetPos && m_State==STATE_FOLLOWING_PATH) return;

	m_Path3D->Reset();
	m_Path3D->SetReady(false);

	m_TargetPoint3D = TargetPos;

	m_State = STATE_SEARCHING_PATH;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::GoTo2D(int X, int Y, float TargetAngle)
{
	m_AfterWalkAngle = TargetAngle;

	if(X == m_TargetPoint2D->x && Y == m_TargetPoint2D->y && m_State==STATE_FOLLOWING_PATH) return;


	m_Path2D->Reset();
	m_Path2D->SetReady(false);

	m_TargetPoint2D->x = X;
	m_TargetPoint2D->y = Y;

	((CAdGame*)Game)->m_Scene->CorrectTargetPoint(m_PosX, m_PosY, &m_TargetPoint2D->x, &m_TargetPoint2D->y, true, this);
	//Game->QuickMessageForm("Orig: %d, %d", X, Y);
	//Game->QuickMessageForm("Corr: %d, %d", m_TargetPoint2D->x, m_TargetPoint2D->y);

	m_State = STATE_SEARCHING_PATH;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::FollowPath3D()
{
	m_Path3D->GetFirst();

	// are there points to follow?
	if(m_Path3D->GetCurrent() != NULL)
	{
		m_State = STATE_FOLLOWING_PATH;;
		InitLine3D(m_PosVector, *m_Path3D->GetCurrent(), true);
	}
	else
	{
		if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
		else m_State = STATE_READY;
		//Game->QuickMessageForm("No path from %d,%d to %d,%d", (int)m_PosVector.x, (int)m_PosVector.z, (int)m_TargetPoint.x, (int)m_TargetPoint.z);
	}
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::GetNextStep3D()
{
	if(m_Angle != m_TargetAngle) TurnToStep(m_AngVelocity);

	D3DXVECTOR3 NewPos = m_PosVector;
	NewPos.x += -sinf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
	NewPos.z += -cosf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;

	D3DXVECTOR3 OrigVec, NewVec;
	D3DXVECTOR3* CurrentPos = m_Path3D->GetCurrent();

	if(CurrentPos != NULL)
	{
		OrigVec = *CurrentPos - m_PosVector;
		NewVec = *CurrentPos - NewPos;
	}

	if(CurrentPos==NULL || D3DXVec3Length(&OrigVec) < D3DXVec3Length(&NewVec))
	{
		if(CurrentPos != NULL) m_PosVector = *CurrentPos;

		if(m_Path3D->GetNext()==NULL)
		{
			m_Path3D->Reset();
			if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
			else
			{
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
		}
		else InitLine3D(m_PosVector, *m_Path3D->GetCurrent(), false);
	}
	else m_PosVector = NewPos;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::InitLine3D(D3DXVECTOR3 StartPt, D3DXVECTOR3 EndPt, bool FirstStep)
{	
	if(FirstStep)
	{
		m_NextState = STATE_FOLLOWING_PATH;
		TurnTo(RadToDeg(-atan2(EndPt.z - StartPt.z, EndPt.x - StartPt.x))-90);
	}
	else
		m_TurningLeft = PrepareTurn(RadToDeg(-atan2(EndPt.z - StartPt.z, EndPt.x - StartPt.x))-90);
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::GetNextStep2D()
{
	CAdGame* AdGame = (CAdGame*)Game;
	if(!AdGame || !AdGame->m_Scene || !AdGame->m_Scene->m_Geom || !m_Path2D || !m_Path2D->GetCurrent())
	{
		m_State = m_NextState;
		m_NextState = STATE_READY;
		return;
	}

	if(m_Angle != m_TargetAngle) TurnToStep(m_AngVelocity);

	D3DXVECTOR3 NewPos = m_PosVector;
	NewPos.x += -sinf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
	NewPos.z += -cosf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;

	D3DXVECTOR3 CurrentPoint;
	AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(m_Path2D->GetCurrent()->x, m_Path2D->GetCurrent()->y, &CurrentPoint);
	
	D3DXVECTOR3 OrigVec, NewVec;

	OrigVec = CurrentPoint - m_PosVector;
	NewVec = CurrentPoint - NewPos;

	if(D3DXVec3Length(&OrigVec) < D3DXVec3Length(&NewVec))
	{
		m_PosVector = CurrentPoint;

		if(m_Path2D->GetNext()==NULL)
		{
			m_Path2D->Reset();
			if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
			else
			{
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
		}
		else
		{
			AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(m_Path2D->GetCurrent()->x, m_Path2D->GetCurrent()->y, &CurrentPoint);
			InitLine3D(m_PosVector, CurrentPoint, false);
		}
	}
	else m_PosVector = NewPos;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::FollowPath2D()
{
	CAdGame* AdGame = (CAdGame*)Game;

	// skip current position
	m_Path2D->GetFirst();
	while(m_Path2D->GetCurrent()!=NULL)
	{
		if(m_Path2D->GetCurrent()->x != m_PosX || m_Path2D->GetCurrent()->y != m_PosY) break;
		m_Path2D->GetNext();
	}


	// are there points to follow?
	if(m_Path2D->GetCurrent() != NULL)
	{
		m_State = STATE_FOLLOWING_PATH;


		D3DXVECTOR3 CurrentPoint;
		AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(m_Path2D->GetCurrent()->x, m_Path2D->GetCurrent()->y, &CurrentPoint);


		InitLine3D(m_PosVector, CurrentPoint, true);
	}
	else
	{
		if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
		else m_State = STATE_READY;
	}
}


//////////////////////////////////////////////////////////////////////////
bool CAdActor3DX::PrepareTurn(float TargetAngle)
{
	bool TurnLeft;

	m_Angle = CBUtils::NormalizeAngle(m_Angle);
	TargetAngle = CBUtils::NormalizeAngle(TargetAngle);

	if(m_Angle == TargetAngle)
	{
		m_TargetAngle = m_Angle;
		return true;
	}

	float delta1, delta2, delta3, delta;

	delta1 = TargetAngle - m_Angle;
	delta2 = TargetAngle + 360 - m_Angle;
	delta3 = TargetAngle - 360 - m_Angle;

	delta1 = (fabs(delta1) <= fabs(delta2))?delta1:delta2;
	delta = (fabs(delta1) <= fabs(delta3))?delta1:delta3;

	m_TargetAngle = m_Angle + delta;
	TurnLeft = (delta<0);

	return TurnLeft;
}


//////////////////////////////////////////////////////////////////////////
bool CAdActor3DX::TurnToStep(float Velocity)
{

	if(m_TurningLeft)
	{
		m_Angle -= Velocity * (float)Game->m_TimerDelta/1000.f;
		if(m_Angle < m_TargetAngle) m_Angle = m_TargetAngle;
	}
	else
	{
		m_Angle += Velocity * (float)Game->m_TimerDelta/1000.f;
		if(m_Angle > m_TargetAngle) m_Angle = m_TargetAngle;
	}

	// done turning?
	if(m_Angle == m_TargetAngle)
	{
		m_Angle = CBUtils::NormalizeAngle(m_Angle);
		m_TargetAngle = m_Angle;
		return true;
	}
	else return false;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL)
	{
		Game->LOG(0, "CAdActor3DX::LoadFile failed for file '%s'", Filename);
		return E_FAIL;
	}

	HRESULT ret;

	m_Filename = new char [strlen(Filename)+1];
	strcpy(m_Filename, Filename);

	if(FAILED(ret = LoadBuffer(Buffer, true))) Game->LOG(0, "Error parsing ACTOR3D file '%s'", Filename);


	delete [] Buffer;

	return ret;
}


TOKEN_DEF_START
	TOKEN_DEF (ACTOR3DX)
	TOKEN_DEF (X)
	TOKEN_DEF (Y)
	TOKEN_DEF (Z)
	TOKEN_DEF (ANGLE)
	TOKEN_DEF (VELOCITY)
	TOKEN_DEF (ANGULAR_VELOCITY)
	TOKEN_DEF (TEMPLATE)
	TOKEN_DEF (NAME)
	TOKEN_DEF (REGISTRABLE)
	TOKEN_DEF (INTERACTIVE)
	TOKEN_DEF (ACTIVE)
	TOKEN_DEF (MODEL)
	TOKEN_DEF (EVENTS)
	TOKEN_DEF (FONT)
	TOKEN_DEF (CURSOR)
	TOKEN_DEF (DROP_TO_FLOOR)
	TOKEN_DEF (SCRIPT)
	TOKEN_DEF (CAPTION)
	TOKEN_DEF (PROPERTY)
	TOKEN_DEF (ANIMATION)
	TOKEN_DEF (EDITOR_PROPERTY)
	TOKEN_DEF (SHADOW_IMAGE)
	TOKEN_DEF (SHADOW_SIZE)
	TOKEN_DEF (SIMPLE_SHADOW)
	TOKEN_DEF (SHADOW_COLOR)
	TOKEN_DEF (SHADOW_MODEL)
	TOKEN_DEF (SHADOW_TYPE)
	TOKEN_DEF (LIGHT_POSITION)
	TOKEN_DEF (SHADOW)
	TOKEN_DEF (SCALE)
	TOKEN_DEF (DRAW_BACKFACES)
	TOKEN_DEF (BLOCKED_REGION)
	TOKEN_DEF (WAYPOINTS)
	TOKEN_DEF (EFFECT_FILE)
	TOKEN_DEF (EFFECT)
	TOKEN_DEF (MATERIAL)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (ACTOR3DX)
		TOKEN_TABLE (X)
		TOKEN_TABLE (Y)
		TOKEN_TABLE (Z)
		TOKEN_TABLE (ANGLE)
		TOKEN_TABLE (VELOCITY)
		TOKEN_TABLE (ANGULAR_VELOCITY)
		TOKEN_TABLE (TEMPLATE)
		TOKEN_TABLE (NAME)
		TOKEN_TABLE (REGISTRABLE)
		TOKEN_TABLE (INTERACTIVE)
		TOKEN_TABLE (ACTIVE)
		TOKEN_TABLE (MODEL)
		TOKEN_TABLE (EVENTS)
		TOKEN_TABLE (FONT)
		TOKEN_TABLE (CURSOR)
		TOKEN_TABLE (DROP_TO_FLOOR)
		TOKEN_TABLE (SCRIPT)
		TOKEN_TABLE (CAPTION)
		TOKEN_TABLE (PROPERTY)
		TOKEN_TABLE (ANIMATION)
		TOKEN_TABLE (EDITOR_PROPERTY)
		TOKEN_TABLE (SHADOW_IMAGE)
		TOKEN_TABLE (SHADOW_SIZE)
		TOKEN_TABLE (SIMPLE_SHADOW)
		TOKEN_TABLE (SHADOW_COLOR)
		TOKEN_TABLE (SHADOW_MODEL)
		TOKEN_TABLE (SHADOW_TYPE)
		TOKEN_TABLE (LIGHT_POSITION)
		TOKEN_TABLE (SHADOW)
		TOKEN_TABLE (SCALE)
		TOKEN_TABLE (DRAW_BACKFACES)
		TOKEN_TABLE (BLOCKED_REGION)
		TOKEN_TABLE (WAYPOINTS)
		TOKEN_TABLE (EFFECT)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete)
	{
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_ACTOR3DX)
		{
			Game->LOG(0, "'ACTOR3DX' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

	SAFE_DELETE(m_ModelX);
	SAFE_DELETE(m_ShadowModel);

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_TEMPLATE:
				if(FAILED(LoadFile((char*)params))) cmd = PARSERR_GENERIC;
			break;

			case TOKEN_X:
				parser.ScanStr((char*)params, "%f", &m_PosVector.x);
			break;

			case TOKEN_Y:
				parser.ScanStr((char*)params, "%f", &m_PosVector.y);
			break;

			case TOKEN_Z:
				parser.ScanStr((char*)params, "%f", &m_PosVector.z);
			break;

			case TOKEN_ANGLE:
				parser.ScanStr((char*)params, "%f", &m_Angle);
				CBUtils::NormalizeAngle(m_Angle);
			break;

			case TOKEN_SHADOW_SIZE:
				parser.ScanStr((char*)params, "%f", &m_ShadowSize);
				m_ShadowSize = max(m_ShadowSize, 0.0f);
			break;
			
			case TOKEN_SIMPLE_SHADOW:
			{
				bool SimpleShadow;
				parser.ScanStr((char*)params, "%b", &SimpleShadow);
				if(SimpleShadow) m_ShadowType = SHADOW_SIMPLE;
			}
			break;

			case TOKEN_SHADOW_COLOR:
			{
				int r, g, b, a;
				parser.ScanStr((char*)params, "%d,%d,%d,%d", &r, &g, &b, &a);
				m_ShadowColor = DRGBA(r, g, b, a);
			}
			break;

			case TOKEN_LIGHT_POSITION:
				parser.ScanStr((char*)params, "%f,%f,%f", &m_ShadowLightPos.x, &m_ShadowLightPos.y, &m_ShadowLightPos.z);
			break;

			case TOKEN_SHADOW:
			{
				bool ShadowEnabled;
				parser.ScanStr((char*)params, "%b", &ShadowEnabled);
				if(!ShadowEnabled) m_ShadowType = SHADOW_NONE;
			}
			break;

			case TOKEN_DRAW_BACKFACES:
				parser.ScanStr((char*)params, "%b", &m_DrawBackfaces);
			break;

			case TOKEN_VELOCITY:
				parser.ScanStr((char*)params, "%f", &m_Velocity);
			break;

			case TOKEN_ANGULAR_VELOCITY:
				parser.ScanStr((char*)params, "%f", &m_AngVelocity);
			break;

			case TOKEN_SCALE:
				parser.ScanStr((char*)params, "%f", &m_Scale3D);
				m_Scale3D /= 100.0f;
			break;

			case TOKEN_NAME:
				SetName((char*)params);
			break;

			case TOKEN_CAPTION:
				SetCaption((char*)params);
			break;

			case TOKEN_FONT:
				SetFont((char*)params);
			break;

			case TOKEN_REGISTRABLE:
			case TOKEN_INTERACTIVE:
				parser.ScanStr((char*)params, "%b", &m_Registrable);
			break;

			case TOKEN_ACTIVE:
				parser.ScanStr((char*)params, "%b", &m_Active);
			break;

			case TOKEN_DROP_TO_FLOOR:
				parser.ScanStr((char*)params, "%b", &m_DropToFloor);
			break;

			case TOKEN_SHADOW_TYPE:
			{			
				char* TypeName = (char*)params;
				if(stricmp(TypeName, "none")==0) m_ShadowType = SHADOW_NONE;
				else if(stricmp(TypeName, "simple")==0) m_ShadowType = SHADOW_SIMPLE;
				else if(stricmp(TypeName, "flat")==0) m_ShadowType = SHADOW_FLAT;
				else if(stricmp(TypeName, "stencil")==0) m_ShadowType = SHADOW_STENCIL;
				else
				{
					m_ShadowType = (TShadowType)atoi(TypeName);
					if(m_ShadowType < 0) m_ShadowType = SHADOW_NONE;
					if(m_ShadowType > SHADOW_STENCIL) m_ShadowType = SHADOW_STENCIL;
				}
			}
			break;

			case TOKEN_MODEL:
				if(!m_ModelX)
				{
					m_ModelX = new CXModel(Game, this);

					if(!m_ModelX || FAILED(m_ModelX->LoadFromFile((char*)params)))
					{
						SAFE_DELETE(m_ModelX);
						cmd = PARSERR_GENERIC;
					}
				}
				else
				{
					if(FAILED(m_ModelX->MergeFromFile((char*)params)))
					{
						cmd = PARSERR_GENERIC;
					}
				}
			break;

			case TOKEN_SHADOW_MODEL:
				if(m_ModelX)
				{
					SAFE_DELETE(m_ShadowModel);
					m_ShadowModel = new CXModel(Game, this);
	
					if(!m_ShadowModel || FAILED(m_ShadowModel->LoadFromFile((char*)params, m_ModelX)))
					{
						SAFE_DELETE(m_ShadowModel);
						cmd = PARSERR_GENERIC;
					}
				}
				else Game->LOG(0, "Error: a MODEL= line must precede shadow model assignment (file: %s)", m_Filename);
			break;

			case TOKEN_CURSOR:
				SAFE_DELETE(m_Cursor);
				m_Cursor = new CBSprite(Game);
				if(!m_Cursor || FAILED(m_Cursor->LoadFile((char*)params)))
				{
					SAFE_DELETE(m_Cursor);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_SCRIPT:
				AddScript((char*)params);
			break;

			case TOKEN_PROPERTY:
				ParseProperty(params, false);
			break;

			case TOKEN_EDITOR_PROPERTY:
				ParseEditorProperty(params, false);
			break;

			case TOKEN_ANIMATION:
				if(m_ModelX) m_ModelX->ParseAnim(params);
				else Game->LOG(0, "Error: a MODEL= line must precede any animation definitions (file: %s)", m_Filename);
			break;

			case TOKEN_EFFECT:
				if(m_ModelX) ParseEffect(params);
				else Game->LOG(0, "Error: a MODEL= line must precede any effect definitions (file: %s)", m_Filename);
			break;

			case TOKEN_SHADOW_IMAGE:
				if(m_ShadowImage) Game->m_SurfaceStorage->RemoveSurface(m_ShadowImage);
				m_ShadowImage = NULL;

				m_ShadowImage = Game->m_SurfaceStorage->AddSurface((char*)params);
			break;

			case TOKEN_BLOCKED_REGION:
			{
				SAFE_DELETE(m_BlockRegion);
				SAFE_DELETE(m_CurrentBlockRegion);
				CBRegion* rgn = new CBRegion(Game);
				CBRegion* crgn = new CBRegion(Game);
				if(!rgn || !crgn || FAILED(rgn->LoadBuffer(params, false)))
				{
					SAFE_DELETE(m_BlockRegion);
					SAFE_DELETE(m_CurrentBlockRegion);
					cmd = PARSERR_GENERIC;
				}
				else
				{
					m_BlockRegion = rgn;
					m_CurrentBlockRegion = crgn;
					m_CurrentBlockRegion->Mimic(m_BlockRegion);
				}
			}
			break;

			case TOKEN_WAYPOINTS:
			{
				SAFE_DELETE(m_WptGroup);
				SAFE_DELETE(m_CurrentWptGroup);
				CAdWaypointGroup* wpt = new CAdWaypointGroup(Game);
				CAdWaypointGroup* cwpt = new CAdWaypointGroup(Game);
				if(!wpt || !cwpt || FAILED(wpt->LoadBuffer(params, false)))
				{
					SAFE_DELETE(m_WptGroup);
					SAFE_DELETE(m_CurrentWptGroup);
					cmd = PARSERR_GENERIC;
				}
				else
				{
					m_WptGroup = wpt;
					m_CurrentWptGroup = cwpt;
					m_CurrentWptGroup->Mimic(m_WptGroup);
				}
			}
			break;
		}
	}
	
	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in ACTOR3DX definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading ACTOR3DX definition");
		return E_FAIL;
	}

	if(!m_ModelX)
	{
		Game->LOG(0, "Error: No model has been loaded for 3D actor");
		return E_FAIL;
	}

	m_State = m_NextState = STATE_READY;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
float CAdActor3DX::DirToAngle(TDirection Dir)
{
	switch(Dir)
	{
		case DI_UP:        return 180.0f;
		case DI_UPRIGHT:   return 225.0f;
		case DI_RIGHT:     return 270.0f;
		case DI_DOWNRIGHT: return 315.0f;
		case DI_DOWN:      return 0.0f;
		case DI_DOWNLEFT:  return 45.0f;
		case DI_LEFT:      return 90.0f;
		case DI_UPLEFT:    return 135.0f;
		case DI_NONE:      return -1.0f;
		default:           return 0.0f;
	}
}

//////////////////////////////////////////////////////////////////////////
TDirection CAdActor3DX::AngleToDir(float Angle)
{
	if(Angle>=337.0f || Angle < 22.0f)  return DI_DOWN;
	if(Angle>=22.0f  && Angle < 67.0f)  return DI_DOWNLEFT;
	if(Angle>=67.0f  && Angle < 112.0f) return DI_LEFT;
	if(Angle>=112.0f && Angle < 157.0f) return DI_UPLEFT;
	if(Angle>=157.0f && Angle < 202.0f) return DI_UP;
	if(Angle>=202.0f && Angle < 247.0f) return DI_UPRIGHT;
	if(Angle>=247.0f && Angle < 292.0f) return DI_RIGHT;
	if(Angle>=292.0f && Angle < 337.0f) return DI_DOWNRIGHT;

	return DI_NONE;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::PlayAnim(char* Name, bool SetState)
{
	return PlayAnim(0, Name, SetState);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::PlayAnim(int Channel, char* Name, bool SetState)
{
	if(!m_ModelX) return E_FAIL;

	HRESULT res = m_ModelX->PlayAnim(Channel, Name, m_DefaultTransTime, true, m_DefaultStopTransTime);
	if(SUCCEEDED(res) && SetState)
	{
		m_State = STATE_PLAYING_ANIM;
		m_StateAnimChannel = Channel;
	}

	return res;
}

//////////////////////////////////////////////////////////////////////////
void CAdActor3DX::Talk(char *Text, char* Sound, DWORD Duration, char *Stances, TTextAlign Align)
{
	//if(m_ModelX) m_ModelX->StopAnim(); // need to set animation to NULL
	CAdObject::Talk(Text, Sound, Duration, Stances, Align);
}

//////////////////////////////////////////////////////////////////////////
int CAdActor3DX::GetHeight()
{
	if(!m_ModelX) return 0;
	else return  m_PosY - m_ModelX->m_BoundingRect.top - 5;
}

//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// PlayAnim / PlayAnimAsync
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "PlayAnim")==0 || strcmp(Name, "PlayAnimAsync")==0)
	{
		bool Async = strcmp(Name, "PlayAnimAsync")==0;
		Stack->CorrectParams(1);
		if(FAILED(PlayAnim(Stack->Pop()->GetString(), true /*!Async*/))) Stack->PushBool(false);
		else
		{
			if(!Async) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnim
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnim")==0)
	{
		Stack->CorrectParams(1);
		int TransTime = Stack->Pop()->GetInt(m_DefaultStopTransTime);
		HRESULT Ret = E_FAIL;
		if(m_ModelX) Ret = m_ModelX->StopAnim(0, TransTime);
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnimChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnimChannel")==0)
	{
		Stack->CorrectParams(2);
		int Channel = Stack->Pop()->GetInt();
		int TransTime = Stack->Pop()->GetInt();
		HRESULT Ret = E_FAIL;
		if(m_ModelX) Ret = m_ModelX->StopAnim(Channel, TransTime);
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayAnimChannel / PlayAnimChannelAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayAnimChannel")==0 || strcmp(Name, "PlayAnimChannelAsync")==0)
	{
		bool Async = strcmp(Name, "PlayAnimChannelAsync")==0;

		Stack->CorrectParams(2);
		int Channel = Stack->Pop()->GetInt();
		char* Name = Stack->Pop()->GetString();
		if(FAILED(PlayAnim(Channel, Name, !Async))) Stack->PushBool(false);
		else
		{
			if(!Async) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsAnimPlaying
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsAnimPlaying")==0)
	{
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();
		char* AnimName;
		if(Val->IsNULL()) AnimName = NULL;
		else AnimName = Val->GetString();

		if(m_ModelX) Stack->PushBool(m_ModelX->IsAnimPending(0, AnimName));
		else Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsAnimChannelPlaying
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsAnimChannelPlaying")==0)
	{
		Stack->CorrectParams(2);
		int Channel = Stack->Pop()->GetInt(0);
		CScValue* Val = Stack->Pop();
		char* AnimName;
		if(Val->IsNULL()) AnimName = NULL;
		else AnimName = Val->GetString();

		if(m_ModelX) Stack->PushBool(m_ModelX->IsAnimPending(Channel, AnimName));
		else Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AddAttachment / AddMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AddAttachment")==0 || strcmp(Name, "AddMesh")==0)
	{
		if(strcmp(Name, "AddMesh")==0) Game->LOG(0, "Warning: AddMesh is now obsolete, use AddAttachment");

		Stack->CorrectParams(3);
		char* Filename = Stack->Pop()->GetString();
		char* AttachName = Stack->Pop()->GetString();
		char* BoneName = Stack->Pop()->GetString();

		if(!m_ModelX) Stack->PushBool(false);
		else
		{
			if(!m_ModelX->GetBoneMatrix(BoneName))
			{
				Script->RuntimeError("Bone '%s' cannot be found", BoneName);
				Stack->PushBool(false);
			}
			else
			{
				CAdAttach3DX* At = new CAdAttach3DX(Game, this);
				if(!At || FAILED(At->Init(Filename, AttachName, BoneName)))
				{
					Script->RuntimeError("Error adding attachment");
					SAFE_DELETE(At);
					Stack->PushBool(false);
				}
				else
				{
					bool IsSet = false;
					for(int i=0; i<m_Attachments.GetSize(); i++)
					{
						if(stricmp(m_Attachments[i]->m_Name, AttachName)==0)
						{
							SAFE_DELETE(m_Attachments[i]);
							m_Attachments[i] = At;
							IsSet = true;
							break;
						}
					}
					if(!IsSet) m_Attachments.Add(At);

					Stack->PushBool(true);
				}
			}
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RemoveAttachment / RemoveMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveAttachment")==0 || strcmp(Name, "RemoveMesh")==0)
	{
		if(strcmp(Name, "RemoveMesh")==0) Game->LOG(0, "Warning: RemoveMesh is now obsolete, use RemoveAttachment");

		Stack->CorrectParams(1);
		char* Name = Stack->Pop()->GetString();
		if(!m_ModelX) Stack->PushBool(false);
		else
		{
			bool IsFound = false;
			for(int i=0; i<m_Attachments.GetSize(); i++)
			{
				if(stricmp(m_Attachments[i]->m_Name, Name)==0)
				{
					SAFE_DELETE(m_Attachments[i]);
					m_Attachments.RemoveAt(i);
					IsFound = true;
					break;
				}
			}
			Stack->PushBool(IsFound);
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetAttachment
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetAttachment")==0)
	{
		Stack->CorrectParams(1);
		char* Name = Stack->Pop()->GetString();

		if(!m_ModelX) Stack->PushNULL();
		else
		{
			bool IsFound = false;
			for(int i=0; i<m_Attachments.GetSize(); i++)
			{
				if(stricmp(m_Attachments[i]->m_Name, Name)==0)
				{
					Stack->PushNative(m_Attachments[i], true);
					IsFound = true;
					break;
				}
			}
			if(!IsFound) Stack->PushNULL();
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoTo3D / GoTo3DAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoTo3D")==0 || strcmp(Name, "GoTo3DAsync")==0)
	{
		Stack->CorrectParams(3);
		D3DXVECTOR3 pos;
		pos.x = Stack->Pop()->GetFloat();
		pos.y = Stack->Pop()->GetFloat();
		pos.z = Stack->Pop()->GetFloat();
		GoTo3D(pos);
		if(strcmp(Name, "GoTo3DAsync")!=0) Script->WaitForExclusive(this);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoTo / GoToAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoTo")==0 || strcmp(Name, "GoToAsync")==0)
	{
		Stack->CorrectParams(2);
		int x = Stack->Pop()->GetInt();
		int y = Stack->Pop()->GetInt();
		CAdGame* AdGame = (CAdGame*)Game;

		
		if(IsGoToNeeded(x, y))
		{
			if(AdGame->m_Scene->m_2DPathfinding)
			{
				GoTo2D(x, y);
				if(strcmp(Name, "GoToAsync")!=0) Script->WaitForExclusive(this);
			}
			else
			{
				if(AdGame->m_Scene && AdGame->m_Scene->m_Geom)
				{
					D3DXVECTOR3 pos;
					if(AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(x, y, &pos))
					{		
						//Game->QuickMessageForm("%d, %d -> %f, %f, %f", x, y, pos.x, pos.y, pos.z);
						GoTo3D(pos);
						if(strcmp(Name, "GoToAsync")!=0) Script->WaitForExclusive(this);
					}
				}
			}
		}
		else
		{
			if(m_Path2D) m_Path2D->Reset();
			if(m_Path3D) m_Path3D->Reset();
		}

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoToObject / GoToObjectAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoToObject")==0 || strcmp(Name, "GoToObjectAsync")==0)
	{
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();
		if(!Val->IsNative())
		{
			Script->RuntimeError("actor.%s method accepts an entity refrence only", Name);
			Stack->PushNULL();
			return S_OK;
		}
		CAdObject* Obj = (CAdObject*)Val->GetNative();
		if(!Obj || Obj->m_Type != OBJECT_ENTITY)
		{
			Script->RuntimeError("actor.%s method accepts an entity refrence only", Name);
			Stack->PushNULL();
			return S_OK;
		}
		CAdEntity* Ent = (CAdEntity*)Obj;

		CAdGame* AdGame = (CAdGame*)Game;

		bool GoToNeeded = true;
		if(Ent->m_WalkToX==0 && Ent->m_WalkToY==0) GoToNeeded = IsGoToNeeded(Ent->m_PosX, Ent->m_PosY);
		else GoToNeeded = IsGoToNeeded(Ent->m_WalkToX, Ent->m_WalkToY);

		if(!GoToNeeded)
		{
			// no goto needed, but we still want to turn
			if(Ent->m_WalkToX!=0 || Ent->m_WalkToY!=0)
			{
				TurnTo(DirToAngle(Ent->m_WalkToDir));
				if(strcmp(Name, "GoToObjectAsync")!=0) Script->WaitForExclusive(this);
			}

			if(m_Path2D) m_Path2D->Reset();
			if(m_Path3D) m_Path3D->Reset();

			Stack->PushNULL();
			return S_OK;
		}


		
		if(AdGame->m_Scene->m_2DPathfinding)
		{
			if(Ent->m_WalkToX==0 && Ent->m_WalkToY==0) GoTo2D(Ent->m_PosX, Ent->m_PosY);
			else GoTo2D(Ent->m_WalkToX, Ent->m_WalkToY, DirToAngle(Ent->m_WalkToDir));
			if(strcmp(Name, "GoToObjectAsync")!=0) Script->WaitForExclusive(this);
		}
		else
		{
			if(AdGame->m_Scene->m_Geom)
			{
				D3DXVECTOR3 pos;
				if(AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(Ent->m_WalkToX, Ent->m_WalkToY, &pos))
				{
					if(Ent->m_WalkToX==0 && Ent->m_WalkToY==0) GoTo3D(pos);
					else GoTo3D(pos, DirToAngle(Ent->m_WalkToDir));
					if(strcmp(Name, "GoToObjectAsync")!=0) Script->WaitForExclusive(this);
				}
			}
		}
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnTo / TurnToAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnTo")==0 || strcmp(Name, "TurnToAsync")==0)
	{
		Stack->CorrectParams(1);
		int dir;
		CScValue* val = Stack->Pop();

		float angle = 0.0;

		// turn to object?
		if(val->IsNative() && Game->ValidObject((CBObject*)val->GetNative()))
		{
			CBObject* obj = (CBObject*)val->GetNative();
			//angle = (atan2((double)(obj->m_PosY - m_PosY), (double)(obj->m_PosX - m_PosX)) * (180/3.14));
			D3DXVECTOR3 ObjPos;
			((CAdGame*)Game)->m_Scene->m_Geom->Convert2Dto3D(obj->m_PosX, obj->m_PosY, &ObjPos);			
			angle = RadToDeg(-atan2(ObjPos.z - m_PosVector.z, ObjPos.x - m_PosVector.x))-90;
		}
		// otherwise turn to direction
		else
		{
			dir = val->GetInt();
			angle = DirToAngle((TDirection)dir);
		}
		if(m_Path2D) m_Path2D->Reset();
		if(m_Path3D) m_Path3D->Reset();

		TurnTo(angle);
		if(strcmp(Name, "TurnToAsync")!=0) Script->WaitForExclusive(this);
		
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnToAngle / TurnToAngleAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnToAngle")==0 || strcmp(Name, "TurnToAngleAsync")==0)
	{
		Stack->CorrectParams(1);
		float angle = Stack->Pop()->GetFloat();		

		if(m_Path2D) m_Path2D->Reset();
		if(m_Path3D) m_Path3D->Reset();

		TurnTo(angle);
		if(strcmp(Name, "TurnToAngleAsync")!=0) Script->WaitForExclusive(this);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsWalking
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsWalking")==0)
	{
		Stack->CorrectParams(0);
		Stack->PushBool(m_State==STATE_FOLLOWING_PATH);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectWalk / DirectWalkBack
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectWalk")==0 || strcmp(Name, "DirectWalkBack")==0)
	{
		Stack->CorrectParams(2);
		float Velocity = 0.0f;
		char* AnimName = NULL;

		CScValue* ValVelocity = Stack->Pop();
		CScValue* ValAnim = Stack->Pop();

		SAFE_DELETE_ARRAY(m_DirectWalkAnim);
		
		if(!ValVelocity->IsNULL()) m_DirectWalkVelocity = ValVelocity->GetFloat();
		else m_DirectWalkVelocity = 0.0f;

		if(!ValAnim->IsNULL()) CBUtils::SetString(&m_DirectWalkAnim, ValAnim->GetString());
		
		m_State = STATE_DIRECT_CONTROL;

		if(strcmp(Name, "DirectWalk")==0)
			m_DirectWalkMode = DIRECT_WALK_FW;
		else
			m_DirectWalkMode = DIRECT_WALK_BK;

		Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectWalkStop
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectWalkStop")==0)
	{
		Stack->CorrectParams(0);
		m_DirectWalkMode = DIRECT_WALK_NONE;
		Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectTurnLeft / DirectTurnRight
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectTurnLeft")==0 || strcmp(Name, "DirectTurnRight")==0)
	{
		Stack->CorrectParams(2);
		float Velocity = 0.0f;
		char* AnimName = NULL;

		CScValue* ValVelocity = Stack->Pop();
		CScValue* ValAnim = Stack->Pop();

		SAFE_DELETE_ARRAY(m_DirectTurnAnim);

		if(!ValVelocity->IsNULL()) m_DirectTurnVelocity = ValVelocity->GetFloat();
		else m_DirectTurnVelocity = 0.0f;

		if(!ValAnim->IsNULL()) CBUtils::SetString(&m_DirectTurnAnim, ValAnim->GetString());

		m_State = STATE_DIRECT_CONTROL;

		if(strcmp(Name, "DirectTurnLeft")==0)
			m_DirectTurnMode = DIRECT_TURN_CCW;
		else
			m_DirectTurnMode = DIRECT_TURN_CW;

		Stack->PushBool(true);

		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectTurnStop
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectTurnStop")==0)
	{
		Stack->CorrectParams(0);
		m_DirectTurnMode = DIRECT_TURN_NONE;
		Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetTexture
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetTexture")==0)
	{
		Stack->CorrectParams(2);
		char* MaterialName = Stack->Pop()->GetString();
		char* TextureFilename = Stack->Pop()->GetString();

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialSprite(MaterialName, TextureFilename)))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetTheoraTexture
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetTheoraTexture")==0)
	{
		Stack->CorrectParams(2);
		char* MaterialName = Stack->Pop()->GetString();
		char* TheoraFilename = Stack->Pop()->GetString();

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialTheora(MaterialName, TheoraFilename)))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetEffect
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetEffect")==0)
	{
		Stack->CorrectParams(2);
		char* MaterialName = Stack->Pop()->GetString();
		char* EffectFilename = Stack->Pop()->GetString();

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialEffect(MaterialName, EffectFilename)))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RemoveEffect
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveEffect")==0)
	{
		Stack->CorrectParams(1);
		char* MaterialName = Stack->Pop()->GetString();

		if(m_ModelX && SUCCEEDED(m_ModelX->RemoveMaterialEffect(MaterialName)))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetEffectParam
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetEffectParam")==0)
	{
		Stack->CorrectParams(3);
		char* MaterialName = Stack->Pop()->GetString();
		char* ParamName = Stack->Pop()->GetString();
		CScValue* val = Stack->Pop();

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialEffectParam(MaterialName, ParamName, val)))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetEffectParamVector
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetEffectParamVector")==0)
	{
		Stack->CorrectParams(6);
		char* MaterialName = Stack->Pop()->GetString();
		char* ParamName = Stack->Pop()->GetString();
		float x = Stack->Pop()->GetFloat();
		float y = Stack->Pop()->GetFloat();
		float z = Stack->Pop()->GetFloat();
		float w = Stack->Pop()->GetFloat();

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialEffectParam(MaterialName, ParamName, D3DXVECTOR4(x, y, z, w))))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetEffectParamColor
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetEffectParamColor")==0)
	{
		Stack->CorrectParams(3);
		char* MaterialName = Stack->Pop()->GetString();
		char* ParamName = Stack->Pop()->GetString();
		DWORD Color = Stack->Pop()->GetInt();

		float r = D3DCOLGetR(Color) / 255.0f;
		float g = D3DCOLGetG(Color) / 255.0f;
		float b = D3DCOLGetB(Color) / 255.0f;
		float a = D3DCOLGetA(Color) / 255.0f;

		if(m_ModelX && SUCCEEDED(m_ModelX->SetMaterialEffectParam(MaterialName, ParamName, D3DXVECTOR4(r, g, b, a))))
			Stack->PushBool(true);
		else
			Stack->PushBool(false);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// MergeAnims
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "MergeAnims")==0)
	{
		Stack->CorrectParams(1);
		char* Filename = Stack->Pop()->GetString();
		
		Stack->PushBool(SUCCEEDED(MergeAnimations(Filename)));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// UnloadAnim
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "UnloadAnim")==0)
	{
		Stack->CorrectParams(1);
		char* AnimName = Stack->Pop()->GetString();

		Stack->PushBool(SUCCEEDED(UnloadAnimation(AnimName)));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// SetAnimTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "SetAnimTransitionTime")==0)
	{
		Stack->CorrectParams(3);
		char* AnimFrom = Stack->Pop()->GetString();
		char* AnimTo = Stack->Pop()->GetString();
		int Time = Stack->Pop()->GetInt();

		bool Found = false;
		for(int i=0; i<m_TransitionTimes.GetSize(); i++)
		{
			CBAnimTransitionTime* Trans = m_TransitionTimes[i];
			if(Trans->m_AnimFrom && Trans->m_AnimTo && stricmp(Trans->m_AnimFrom, AnimFrom)==0 && stricmp(Trans->m_AnimTo, AnimTo)==0)
			{
				Found = true;
				if(Time < 0)
				{
					delete Trans;
					m_TransitionTimes.RemoveAt(i);
				}
				else Trans->m_Time = (DWORD)Time;
				break;
			}
		}
		if(!Found && Time >= 0)
		{
			CBAnimTransitionTime* Trans = new CBAnimTransitionTime(AnimFrom, AnimTo, (DWORD)Time);
			m_TransitionTimes.Add(Trans);
		}
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GetAnimTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GetAnimTransitionTime")==0)
	{
		Stack->CorrectParams(2);
		char* AnimFrom = Stack->Pop()->GetString();
		char* AnimTo = Stack->Pop()->GetString();

		int Time = -1;
		for(int i=0; i<m_TransitionTimes.GetSize(); i++)
		{
			CBAnimTransitionTime* Trans = m_TransitionTimes[i];
			if(Trans->m_AnimFrom && Trans->m_AnimTo && stricmp(Trans->m_AnimFrom, AnimFrom)==0 && stricmp(Trans->m_AnimTo, AnimTo)==0)
			{
				Time = Trans->m_Time;
				break;
			}			
		}
		Stack->PushInt(Time);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// CreateParticleEmitterBone
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "CreateParticleEmitterBone")==0)
	{
		Stack->CorrectParams(4);
		char* BoneName = Stack->Pop()->GetString();
		float OffsetX = Stack->Pop()->GetFloat();
		float OffsetY = Stack->Pop()->GetFloat();
		float OffsetZ = Stack->Pop()->GetFloat();

		CPartEmitter* Emitter = CreateParticleEmitter(BoneName, D3DXVECTOR3(OffsetX, OffsetY, OffsetZ));
		if(Emitter) Stack->PushNative(m_PartEmitter, true);
		else Stack->PushNULL();

		return S_OK;
	}

	else return CAdObject3D::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CAdActor3DX::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0)
	{
		m_ScValue->SetString("actor3dx");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TalkAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TalkAnimName")==0)
	{
		m_ScValue->SetString(m_TalkAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TalkAnimChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TalkAnimChannel")==0)
	{
		m_ScValue->SetInt(m_TalkAnimChannel);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// WalkAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WalkAnimName")==0)
	{
		m_ScValue->SetString(m_WalkAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// IdleAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IdleAnimName")==0)
	{
		m_ScValue->SetString(m_IdleAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnLeftAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnLeftAnimName")==0)
	{
		m_ScValue->SetString(m_TurnLeftAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnRightAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnRightAnimName")==0)
	{
		m_ScValue->SetString(m_TurnRightAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectionAngle / DirAngle
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectionAngle")==0 || strcmp(Name, "DirAngle")==0)
	{
		m_ScValue->SetFloat(m_Angle);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Direction
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Direction")==0)
	{
		m_ScValue->SetInt(AngleToDir(m_Angle));
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AnimTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AnimTransitionTime")==0)
	{
		m_ScValue->SetInt(m_DefaultTransTime);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// AnimStopTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AnimStopTransitionTime")==0)
	{
		m_ScValue->SetInt(m_DefaultStopTransTime);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoToTolerance
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoToTolerance")==0)
	{
		m_ScValue->SetInt(m_GoToTolerance);
		return m_ScValue;
	}

	else return CAdObject3D::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::ScSetProperty(char *Name, CScValue *Value)
{
	//////////////////////////////////////////////////////////////////////////
	// TalkAnimName
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "TalkAnimName")==0)
	{
		if(Value->IsNULL()) CBUtils::SetString(&m_TalkAnimName, "talk");
		else CBUtils::SetString(&m_TalkAnimName, Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TalkAnimChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TalkAnimChannel")==0)
	{
		m_TalkAnimChannel = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// WalkAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WalkAnimName")==0)
	{
		if(Value->IsNULL()) CBUtils::SetString(&m_WalkAnimName, "walk");
		else CBUtils::SetString(&m_WalkAnimName, Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IdleAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IdleAnimName")==0)
	{
		if(Value->IsNULL()) CBUtils::SetString(&m_IdleAnimName, "idle");
		else CBUtils::SetString(&m_IdleAnimName, Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnLeftAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnLeftAnimName")==0)
	{
		if(Value->IsNULL()) CBUtils::SetString(&m_TurnLeftAnimName, "turnleft");
		else CBUtils::SetString(&m_TurnLeftAnimName, Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnRightAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnRightAnimName")==0)
	{
		if(Value->IsNULL()) CBUtils::SetString(&m_TurnRightAnimName, "turnright");
		else CBUtils::SetString(&m_TurnRightAnimName, Value->GetString());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectionAngle / DirAngle
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectionAngle")==0 || strcmp(Name, "DirAngle")==0)
	{
		m_Angle = CBUtils::NormalizeAngle(Value->GetFloat());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// Direction
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Direction")==0)
	{
		m_Angle = DirToAngle((TDirection)Value->GetInt());
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AnimTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AnimTransitionTime")==0)
	{
		m_DefaultTransTime = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AnimStopTransitionTime
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AnimStopTransitionTime")==0)
	{
		m_DefaultStopTransTime = Value->GetInt();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoToTolerance
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoToTolerance")==0)
	{
		m_GoToTolerance = Value->GetInt();
		return S_OK;
	}

	else return CAdObject3D::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CAdActor3DX::ScToString()
{
	return "[actor3dx object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::Persist(CBPersistMgr* PersistMgr)
{
	CAdObject3D::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Path3D));
	PersistMgr->Transfer(TMEMBER(m_Path2D));
	PersistMgr->Transfer(TMEMBER(m_TargetAngle));
	PersistMgr->Transfer(TMEMBER(m_TargetPoint3D));
	PersistMgr->Transfer(TMEMBER(m_TargetPoint2D));
	PersistMgr->Transfer(TMEMBER(m_TurningLeft));
	PersistMgr->Transfer(TMEMBER(m_AfterWalkAngle));
	
	PersistMgr->Transfer(TMEMBER(m_TalkAnimName));
	PersistMgr->Transfer(TMEMBER(m_IdleAnimName));
	PersistMgr->Transfer(TMEMBER(m_WalkAnimName));
	PersistMgr->Transfer(TMEMBER(m_TurnLeftAnimName));
	PersistMgr->Transfer(TMEMBER(m_TurnRightAnimName));

	// direct controls
	PersistMgr->Transfer(TMEMBER_INT(m_DirectWalkMode));
	PersistMgr->Transfer(TMEMBER_INT(m_DirectTurnMode));
	PersistMgr->Transfer(TMEMBER(m_DirectWalkAnim));
	PersistMgr->Transfer(TMEMBER(m_DirectTurnAnim));
	PersistMgr->Transfer(TMEMBER(m_DirectWalkVelocity));
	PersistMgr->Transfer(TMEMBER(m_DirectTurnVelocity));

	// new for X
	PersistMgr->Transfer(TMEMBER(m_DefaultTransTime));
	m_Attachments.Persist(PersistMgr);
	PersistMgr->Transfer(TMEMBER(m_StateAnimChannel));

	PersistMgr->Transfer(TMEMBER(m_GoToTolerance));

	if(PersistMgr->CheckVersion(1, 7, 94))
	{
		PersistMgr->Transfer(TMEMBER(m_DefaultStopTransTime));
	}
	else m_DefaultStopTransTime = 0;

	// animation transition times
	if(PersistMgr->CheckVersion(1, 7, 92))
	{
		if(PersistMgr->m_Saving)
		{
			int NumItems = m_TransitionTimes.GetSize();
			PersistMgr->Transfer(TMEMBER(NumItems));
			for(int i=0; i<m_TransitionTimes.GetSize(); i++)
			{
				m_TransitionTimes[i]->Persist(PersistMgr);
			}
		}
		else
		{
			int NumItems = m_TransitionTimes.GetSize();
			PersistMgr->Transfer(TMEMBER(NumItems));
			for(int i=0; i<NumItems; i++)
			{
				CBAnimTransitionTime* Trans = new CBAnimTransitionTime();
				Trans->Persist(PersistMgr);
				m_TransitionTimes.Add(Trans);
			}
		}
	}

	if(PersistMgr->CheckVersion(1, 7, 92))
	{
		PersistMgr->Transfer(TMEMBER(m_TalkAnimChannel));
	}
	else
	{
		m_TalkAnimChannel = 0;
	}

	if(PersistMgr->CheckVersion(1, 8, 4))
	{
		PersistMgr->Transfer(TMEMBER(m_PartBone));
		PersistMgr->Transfer(TMEMBER(m_PartOffset));
	}
	else
	{
		m_PartBone = NULL;
		m_PartOffset = D3DXVECTOR3(0, 0, 0);
	}

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::InvalidateDeviceObjects()
{
	if(m_ModelX) m_ModelX->InvalidateDeviceObjects();
	if(m_ShadowModel) m_ShadowModel->InvalidateDeviceObjects();

	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		m_Attachments[i]->InvalidateDeviceObjects();
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::RestoreDeviceObjects()
{
	if(m_ModelX) m_ModelX->RestoreDeviceObjects();
	if(m_ShadowModel) m_ShadowModel->RestoreDeviceObjects();

	for(int i=0; i<m_Attachments.GetSize(); i++)
	{
		m_Attachments[i]->RestoreDeviceObjects();
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::MergeAnimations(char* Filename)
{
	if(!m_ModelX) return E_FAIL;

	HRESULT Res = m_ModelX->MergeFromFile(Filename);
	if(FAILED(Res))
	{
		Game->LOG(Res, "Error: MergeAnims failed for file '%s'", Filename);
		return Res;
	}

	// does anim description file exist?
	char AnimExtFile[MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];

	_splitpath(Filename, drive, dir, fname, NULL);
	_makepath(AnimExtFile, drive, dir, fname, ".anim");

	CBFile* TestFile = Game->m_FileManager->OpenFile(AnimExtFile);
	if(TestFile)
	{
		Game->m_FileManager->CloseFile(TestFile);
		return MergeAnimations2(AnimExtFile);
	}
	else return S_OK;

}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::MergeAnimations2(char* Filename)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (ANIMATION)
	TOKEN_TABLE_END

	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL) return E_FAIL;

	BYTE* BufferOrig = Buffer;
	

	BYTE* params;
	int cmd;
	CBParser parser(Game);

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
			case TOKEN_ANIMATION:
			{
				if(FAILED(m_ModelX->ParseAnim(params)))
				{
					cmd = PARSERR_GENERIC;
				}
			}
			break;
		}
	}
	delete [] BufferOrig;

	if (cmd == PARSERR_TOKENNOTFOUND)
	{
		Game->LOG(0, "Syntax error in animation definition file");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC)
	{
		Game->LOG(0, "Error loading animation definition file");
		return E_FAIL;
	}

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::UnloadAnimation(char* AnimName)
{
	if(m_ModelX) return m_ModelX->UnloadAnimation(AnimName);
	else return E_FAIL;
}

//////////////////////////////////////////////////////////////////////////
bool CAdActor3DX::IsGoToNeeded(int X, int Y)
{
	if(abs(X - m_PosX) <= m_GoToTolerance && abs(Y - m_PosY) <= m_GoToTolerance) return false;
	else return true;
}

//////////////////////////////////////////////////////////////////////////
DWORD CAdActor3DX::GetAnimTransitionTime(char* From, char* To)
{
	for(int i=0; i<m_TransitionTimes.GetSize(); i++)
	{
		CBAnimTransitionTime* Trans = m_TransitionTimes[i];
		if(Trans->m_AnimFrom && Trans->m_AnimTo && stricmp(Trans->m_AnimFrom, From)==0 && stricmp(Trans->m_AnimTo, To)==0)
			return Trans->m_Time;
	}
	return m_DefaultTransTime;
}


//////////////////////////////////////////////////////////////////////////
CPartEmitter* CAdActor3DX::CreateParticleEmitter(bool FollowParent, int OffsetX, int OffsetY)
{
	SAFE_DELETE_ARRAY(m_PartBone);
	return CAdObject::CreateParticleEmitter(FollowParent, OffsetX, OffsetY);
}

//////////////////////////////////////////////////////////////////////////
CPartEmitter* CAdActor3DX::CreateParticleEmitter(char* BoneName, D3DXVECTOR3 Offset)
{
	CBUtils::SetString(&m_PartBone, BoneName);
	m_PartOffset = Offset;
	return CAdObject::CreateParticleEmitter(true);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::UpdatePartEmitter()
{
	if(!m_PartEmitter) return E_FAIL;
	if(!m_PartBone) return CAdObject::UpdatePartEmitter();

	CAdGame* AdGame = (CAdGame*)Game;
	if(!AdGame->m_Scene || ! AdGame->m_Scene->m_Geom) return E_FAIL;
	

	D3DXVECTOR3 BonePos;
	GetBonePosition3D(m_PartBone, &BonePos, &m_PartOffset);
	int X = 0, Y = 0;
	AdGame->m_Scene->m_Geom->Convert3Dto2D(&BonePos, &X, &Y);

	m_PartEmitter->m_PosX = X - Game->m_Renderer->m_DrawOffsetX;
	m_PartEmitter->m_PosY = Y - Game->m_Renderer->m_DrawOffsetY;

	return m_PartEmitter->Update();
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3DX::ParseEffect(BYTE* Buffer)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (MATERIAL)
		TOKEN_TABLE (EFFECT_FILE)
	TOKEN_TABLE_END

		BYTE* params;
	int cmd;
	CBParser parser(Game);

	char* EffectFile = NULL;
	char* Material = NULL;

	while ((cmd = parser.GetCommand ((char**)&Buffer, commands, (char**)&params)) > 0)
	{
		switch (cmd)
		{
		case TOKEN_EFFECT_FILE:
			CBUtils::SetString(&EffectFile, (char*)params);
			break;

		case TOKEN_MATERIAL:
			CBUtils::SetString(&Material, (char*)params);
			break;

		}
	}
	if (cmd != PARSERR_EOF) return E_FAIL;

	if (EffectFile && Material)
	{
		C3DEffect* effect = new C3DEffect(Game);
		if (FAILED(effect->CreateFromFile(EffectFile)))
		{
			Game->LOG(0, "Error creating effect from file '%s'", EffectFile);
			SAFE_DELETE(effect);
		}
		else
		{
			if (FAILED(m_ModelX->SetMaterialEffect(Material, EffectFile)))
			{
				Game->LOG(0, "Error assigning effect to material '%s'", Material);
				SAFE_DELETE(effect);
			}
		}
	}

	SAFE_DELETE_ARRAY(EffectFile);
	SAFE_DELETE_ARRAY(Material);

	return S_OK;

}
