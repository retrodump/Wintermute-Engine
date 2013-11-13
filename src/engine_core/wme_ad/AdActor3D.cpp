// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf_ad.h"
#include "AdActor3D.h"


IMPLEMENT_PERSISTENT(CAdActor3D, false);

//////////////////////////////////////////////////////////////////////////
CAdActor3D::CAdActor3D(CBGame* inGame):CAdObject3D(inGame)
{
	m_TargetPoint = D3DXVECTOR3(0,0,0);
	m_TargetAngle = 0;
	m_AfterWalkAngle = -1.0f;

	m_Path = new CAdPath3D(Game);

	m_TalkAnimName = NULL;
	CBUtils::SetString(&m_TalkAnimName, "talk");

	m_IdleAnimName = NULL;
	CBUtils::SetString(&m_IdleAnimName, "idle");

	m_WalkAnimName = NULL;
	CBUtils::SetString(&m_WalkAnimName, "walk");

	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;
	m_Renderer->EnableShadows();

	// direct controls
	m_DirectWalkMode = DIRECT_WALK_NONE;
	m_DirectTurnMode = DIRECT_TURN_NONE;
	m_DirectWalkAnim = NULL;
	m_DirectTurnAnim = NULL;
	m_DirectWalkVelocity = 0.0f;
	m_DirectTurnVelocity = 0.0f;
}


//////////////////////////////////////////////////////////////////////////
CAdActor3D::~CAdActor3D()
{
	SAFE_DELETE_ARRAY(m_TalkAnimName);
	SAFE_DELETE_ARRAY(m_IdleAnimName);
	SAFE_DELETE_ARRAY(m_WalkAnimName);

	SAFE_DELETE_ARRAY(m_DirectWalkAnim);
	SAFE_DELETE_ARRAY(m_DirectTurnAnim);

	SAFE_DELETE(m_Path);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::Update()
{
	if(Game->m_State==GAME_FROZEN) return S_OK;

	CAdGame* AdGame = (CAdGame*)Game;

	/*
	bool handled = false;

	// keyboard input
	if(GetAsyncKeyState(VK_LEFT) & 0x8000){
		m_Angle -= m_AngVelocity * (float)Game->m_TimerDelta/1000.f;
		handled = true;
	}
	if(GetAsyncKeyState(VK_RIGHT) & 0x8000){
		m_Angle += m_AngVelocity * (float)Game->m_TimerDelta/1000.f;
		handled = true;
	}


	D3DXVECTOR3 NewPos = m_PosVector;

	if(GetAsyncKeyState(VK_UP) & 0x8000){
		NewPos.x += -sinf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
		NewPos.z += -cosf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
		m_Model->SetAnimation(0, m_WalkAnimName);
		handled = true;
	}

	if(GetAsyncKeyState(VK_DOWN) & 0x8000){
	    NewPos.x -= -sinf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
		NewPos.z -= -cosf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
		m_Model->SetAnimation(0, m_WalkAnimName);
		handled = true;
	}


	//bool InBounds;
	//NewPos.y = AdGame->m_Scene->m_Geom->GetHeightAt(NewPos, 5.0f, &InBounds);
	//if(InBounds && AdGame->m_Scene->m_Geom->DirectPathExists(&m_PosVector, &NewPos)) m_PosVector = NewPos;

	m_PosVector = NewPos;
	*/


	/*
	if(GetAsyncKeyState(VK_RETURN) & 0x8000){
		m_Model->ShowMesh("Box01", !m_Model->IsMeshVisible("Box01"));
		handled = true;
	}
	*/
	
	
	if(m_Sentence && m_State!=STATE_TALKING) m_Sentence->Finish();

	// update state
	switch(m_State){

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
					if(m_DirectWalkAnim!=NULL) m_Model->SetAnimation(0, m_DirectWalkAnim);
					else m_Model->SetAnimation(0, m_WalkAnimName);
				}
				else if(m_DirectTurnMode!=DIRECT_TURN_NONE)
				{
					if(m_DirectTurnAnim!=NULL) m_Model->SetAnimation(0, m_DirectTurnAnim);
					else m_Model->SetAnimation(0, m_IdleAnimName);
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
					if(Scene->m_Geom->DirectPathExists(&m_PosVector, &NewPos))
					{
						m_PosVector = NewPos;
					}
					else m_Model->SetAnimation(0, m_IdleAnimName);
				}
			}
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_TURNING:
			m_Model->SetAnimation(0, m_IdleAnimName);
			if(TurnToStep(m_AngVelocity)){
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
		break;

		//////////////////////////////////////////////////////////////////////////
		case STATE_SEARCHING_PATH:
			// keep asking scene for the path
			m_Model->SetAnimation(0, m_IdleAnimName);
			if(((CAdGame*)Game)->m_Scene->m_Geom->GetPath(m_PosVector, m_TargetPoint, m_Path))
				m_State = STATE_WAITING_PATH;
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_WAITING_PATH:
			// wait until the scene finished the path
			m_Model->SetAnimation(0, m_IdleAnimName);
			if(m_Path->m_Ready) FollowPath();
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_FOLLOWING_PATH:
			GetNextStep();
			m_Model->SetAnimation(0, m_WalkAnimName);
		break;

		//////////////////////////////////////////////////////////////////////////
		case STATE_TALKING:
		{
			m_Sentence->Update();
			if(m_Sentence->m_CurrentSkelAnim) m_TempSkelAnim = m_Sentence->m_CurrentSkelAnim;

			bool TimeIsUp = (m_Sentence->m_Sound && m_Sentence->m_SoundStarted && (!m_Sentence->m_Sound->IsPlaying()&&!m_Sentence->m_Sound->IsPaused())) || (!m_Sentence->m_Sound && m_Sentence->m_Duration <= Game->m_Timer - m_Sentence->m_StartTime);
			if(m_TempSkelAnim==NULL || !m_Model->IsAnimPending(0, m_TempSkelAnim) || TimeIsUp)
			{
				if(TimeIsUp)
				{
					m_Sentence->Finish();
					m_TempSkelAnim = NULL;
					m_State = m_NextState;
					m_NextState = STATE_READY;
				}
				else
				{
					m_TempSkelAnim = m_Sentence->GetNextStance();
					if(m_TempSkelAnim) m_Model->SetAnimation(0, m_TempSkelAnim, true);
					else m_Model->SetAnimation(0, m_TalkAnimName, false);

					((CAdGame*)Game)->AddSentence(m_Sentence);
				}
			}
			else
			{
				if(m_TempSkelAnim) m_Model->SetAnimation(0, m_TempSkelAnim);
				((CAdGame*)Game)->AddSentence(m_Sentence);
			}
		}
		break;


		//////////////////////////////////////////////////////////////////////////
		case STATE_READY:
			m_Model->SetAnimation(0, m_IdleAnimName);
		break;
	} // switch(m_State)

	// finished playing animation?
	if(m_State==STATE_PLAYING_ANIM && !m_Model->IsAnimPending(0)){
		m_State = m_NextState;
		m_NextState = STATE_READY;
	}


	m_Ready = (m_State == STATE_READY);


	// setup 2D position
	int OrigX = m_PosX;
	int OrigY = m_PosY;
	
	HRESULT Ret = CAdObject3D::Update();
	if(OrigX!=m_PosX || OrigY!=m_PosY) AfterMove();

	UpdatePartEmitter();

	return Ret;
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::Display()
{
	if(!m_Model) return S_OK;
	CBRenderD3D* m_Renderer = (CBRenderD3D*)Game->m_Renderer;

	UpdateSounds();
	
	SetupLights();

	if(m_ShadowType > SHADOW_NONE) m_Renderer->DisplayShadow(this, &D3DXVECTOR3(m_ShadowLightPos.x, m_ShadowLightPos.y * m_Scale3D, m_ShadowLightPos.z), NULL, true);
	
	D3DVIEWPORT vp;
	m_Renderer->m_Device->GetViewport(&vp);


	m_Renderer->m_Device->SetTransform(D3DTS_WORLD, &m_WorldMatrix);
	m_Model->Update();
	HRESULT res = m_Model->Render();

	if(m_Registrable) Game->m_Renderer->m_RectList.Add(new CBActiveRect(Game, this, m_Model, m_Model->m_BoundingRect.left, m_Model->m_BoundingRect.top, m_Model->m_BoundingRect.right - m_Model->m_BoundingRect.left, m_Model->m_BoundingRect.bottom-m_Model->m_BoundingRect.top, true));

	m_Renderer->m_LastTexture = NULL;

	if(m_Active && m_PartEmitter) m_PartEmitter->Display();

	// accessibility
	if(Game->m_AccessMgr->GetActiveObject()==this)
	{
		Game->m_AccessMgr->SetHintRect(&m_Model->m_BoundingRect);
	}


	return res;
}

/*
//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::LoadModel(char *Filename)
{
	SAFE_DELETE(m_Model);
	m_Model = new C3DModel(Game);
	if(!m_Model) return E_FAIL;

	if(FAILED(m_Model->LoadFile(Filename))){
		SAFE_DELETE(m_Model);
		return E_FAIL;
	}
	else return S_OK;
}
*/

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::TurnTo(float Angle)
{
	m_TurningLeft = PrepareTurn(Angle);

	if(m_TargetAngle==m_Angle){ // no need to turn
		m_State = m_NextState;
		m_NextState = STATE_READY;
	}
	else m_State = STATE_TURNING;

	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3D::GoTo(D3DXVECTOR3 TargetPos, float TargetAngle)
{
	m_AfterWalkAngle = TargetAngle;
	if(m_TargetPoint == TargetPos && m_State==STATE_FOLLOWING_PATH) return;

	m_Path->Reset();
	m_Path->SetReady(false);

	m_TargetPoint = TargetPos;
	/*
	CAdGame* AdGame = ((CAdGame*)Game);
	if(AdGame->m_Scene->m_Geom) AdGame->m_Scene->m_Geom->CorrectTargetPoint(m_PosVector, &m_TargetPoint);
	*/

	m_State = STATE_SEARCHING_PATH;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3D::FollowPath()
{

	m_Path->GetFirst();

	// are there points to follow?
	if(m_Path->GetCurrent() != NULL){
		m_State = STATE_FOLLOWING_PATH;;
		InitLine(m_PosVector, *m_Path->GetCurrent(), true);
	}
	else{
		if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
		else m_State = STATE_READY;
		//Game->QuickMessageForm("No path from %d,%d to %d,%d", (int)m_PosVector.x, (int)m_PosVector.z, (int)m_TargetPoint.x, (int)m_TargetPoint.z);
	}
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3D::GetNextStep()
{
	if(m_Angle != m_TargetAngle) TurnToStep(m_AngVelocity);

	D3DXVECTOR3 NewPos = m_PosVector;
	NewPos.x += -sinf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
	NewPos.z += -cosf(DegToRad(m_TargetAngle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;

	D3DXVECTOR3 OrigVec, NewVec;

	OrigVec = *m_Path->GetCurrent() - m_PosVector;
	NewVec = *m_Path->GetCurrent() - NewPos;

	if(D3DXVec3Length(&OrigVec) < D3DXVec3Length(&NewVec)){
		m_PosVector = *m_Path->GetCurrent();

		if(m_Path->GetNext()==NULL){
			m_Path->Reset();
			if(m_AfterWalkAngle!=-1.0f) TurnTo(m_AfterWalkAngle);
			else
			{
				m_State = m_NextState;
				m_NextState = STATE_READY;
			}
		}
		else InitLine(m_PosVector, *m_Path->GetCurrent(), false);
	}
	else m_PosVector = NewPos;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3D::InitLine(D3DXVECTOR3 StartPt, D3DXVECTOR3 EndPt, bool FirstStep)
{	
	if(FirstStep){
		m_NextState = STATE_FOLLOWING_PATH;
		TurnTo(RadToDeg(-atan2(EndPt.z - StartPt.z, EndPt.x - StartPt.x))-90);
	}
	else
		m_TurningLeft = PrepareTurn(RadToDeg(-atan2(EndPt.z - StartPt.z, EndPt.x - StartPt.x))-90);
}


//////////////////////////////////////////////////////////////////////////
bool CAdActor3D::PrepareTurn(float TargetAngle)
{
	bool TurnLeft;

	m_Angle = CBUtils::NormalizeAngle(m_Angle);
	TargetAngle = CBUtils::NormalizeAngle(TargetAngle);

	if(m_Angle == TargetAngle){
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
bool CAdActor3D::TurnToStep(float Velocity)
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
HRESULT CAdActor3D::LoadFile(char * Filename)
{
	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL){
		Game->LOG(0, "CAdActor3D::LoadFile failed for file '%s'", Filename);
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
	TOKEN_DEF (ACTOR3D)	
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
	TOKEN_DEF (LIGHT_POSITION)
	TOKEN_DEF (SHADOW)
	TOKEN_DEF (SCALE)
	TOKEN_DEF (DRAW_BACKFACES)
TOKEN_DEF_END
//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::LoadBuffer(BYTE * Buffer, bool Complete)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (ACTOR3D)
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
		TOKEN_TABLE (LIGHT_POSITION)
		TOKEN_TABLE (SHADOW)
		TOKEN_TABLE (SCALE)
		TOKEN_TABLE (DRAW_BACKFACES)
	TOKEN_TABLE_END
	
	BYTE* params;
	int cmd;
	CBParser parser(Game);

	if(Complete){
		if(parser.GetCommand ((char**)&Buffer, commands, (char**)&params)!=TOKEN_ACTOR3D){
			Game->LOG(0, "'ACTOR3D' keyword expected.");
			return E_FAIL;
		}
		Buffer = params;
	}

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

			case TOKEN_MODEL:
				SAFE_DELETE(m_Model);
				m_Model = new C3DModel(Game, this);

				if(!m_Model || FAILED(m_Model->LoadFile((char*)params))){
					SAFE_DELETE(m_Model);
					cmd = PARSERR_GENERIC;
				}
			break;

			case TOKEN_CURSOR:
				SAFE_DELETE(m_Cursor);
				m_Cursor = new CBSprite(Game);
				if(!m_Cursor || FAILED(m_Cursor->LoadFile((char*)params))){
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
				if(m_Model) m_Model->ParseAnim(params);
				else Game->LOG(0, "Error: a MODEL= line must precede any animation definitions (file: %s)", m_Filename);
			break;

			case TOKEN_SHADOW_IMAGE:
				if(m_ShadowImage) Game->m_SurfaceStorage->RemoveSurface(m_ShadowImage);
				m_ShadowImage = NULL;

				m_ShadowImage = Game->m_SurfaceStorage->AddSurface((char*)params);
			break;
		}
	}
	
	if(m_Model) m_Model->LoadFinalize();

	if (cmd == PARSERR_TOKENNOTFOUND){
		Game->LOG(0, "Syntax error in ACTOR3D definition");
		return E_FAIL;
	}
	if (cmd == PARSERR_GENERIC){
		Game->LOG(0, "Error loading ACTOR3D definition");
		return E_FAIL;
	}

	m_State = m_NextState = STATE_READY;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
float CAdActor3D::DirToAngle(TDirection Dir)
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
TDirection CAdActor3D::AngleToDir(float Angle)
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
HRESULT CAdActor3D::PlayAnim(char *Name)
{
	return PlayAnim(0, Name);
}

//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::PlayAnim(int Channel, char *Name)
{
	HRESULT res = m_Model->SetAnimation(Channel, Name, true);
	if(SUCCEEDED(res) && Channel==0) m_State = STATE_PLAYING_ANIM;

	return res;
}


//////////////////////////////////////////////////////////////////////////
void CAdActor3D::Talk(char *Text, char* Sound, DWORD Duration, char *Stances, TTextAlign Align)
{
	if(m_Model) m_Model->AnimStop(); // need to set animation to NULL
	CAdObject::Talk(Text, Sound, Duration, Stances, Align);
}


//////////////////////////////////////////////////////////////////////////
int CAdActor3D::GetHeight()
{
	return  m_PosY - m_Model->m_BoundingRect.top - 5;
}


//////////////////////////////////////////////////////////////////////////
// high level scripting interface
//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name)
{
	//////////////////////////////////////////////////////////////////////////
	// PlayAnim / PlayAnimAsync
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "PlayAnim")==0 || strcmp(Name, "PlayAnimAsync")==0){
		Stack->CorrectParams(1);
		if(FAILED(PlayAnim(Stack->Pop()->GetString()))) Stack->PushBool(false);
		else{
			if(strcmp(Name, "PlayAnimAsync")!=0) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnim
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnim")==0){
		Stack->CorrectParams(0);
		HRESULT Ret = E_FAIL;
		if(m_Model) Ret = m_Model->AnimStop();
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// StopAnimChannel
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "StopAnimChannel")==0){
		Stack->CorrectParams(1);
		int Channel = Stack->Pop()->GetInt();
		HRESULT Ret = E_FAIL;
		if(m_Model) Ret = m_Model->AnimStop(Channel);
		Stack->PushBool(SUCCEEDED(Ret));
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// PlayAnimChannel / PlayAnimChannelAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "PlayAnimChannel")==0 || strcmp(Name, "PlayAnimChannelAsync")==0){
		Stack->CorrectParams(2);
		int Channel = Stack->Pop()->GetInt();
		char* Name = Stack->Pop()->GetString();
		if(FAILED(PlayAnim(Channel, Name))) Stack->PushBool(false);
		else{
			if(strcmp(Name, "PlayAnimChannelAsync")!=0) Script->WaitFor(this);
			Stack->PushBool(true);
		}		
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// HideMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "HideMesh")==0){
		Stack->CorrectParams(1);
		char* Name = Stack->Pop()->GetString();
		if(!m_Model) Stack->PushBool(false);
		else
		{
			HRESULT Ret = m_Model->ShowMesh(Name, false);
			Stack->PushBool(SUCCEEDED(Ret));
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// ShowMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "ShowMesh")==0){
		Stack->CorrectParams(1);
		char* Name = Stack->Pop()->GetString();
		if(!m_Model) Stack->PushBool(false);
		else
		{
			HRESULT Ret = m_Model->ShowMesh(Name, true);
			Stack->PushBool(SUCCEEDED(Ret));
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// AddMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "AddMesh")==0){
		Stack->CorrectParams(3);
		char* Filename = Stack->Pop()->GetString();
		char* MeshName = Stack->Pop()->GetString();
		char* JointName = Stack->Pop()->GetString();

		if(!m_Model) Stack->PushBool(false);
		else
		{
			HRESULT Ret = m_Model->AddMesh(Filename, MeshName, JointName);
			Stack->PushBool(SUCCEEDED(Ret));
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// RemoveMesh
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "RemoveMesh")==0){
		Stack->CorrectParams(1);
		char* Name = Stack->Pop()->GetString();
		if(!m_Model) Stack->PushBool(false);
		else
		{
			HRESULT Ret = m_Model->DeleteMesh(Name);
			Stack->PushBool(SUCCEEDED(Ret));
		}
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoTo3D / GoTo3DAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoTo3D")==0 || strcmp(Name, "GoTo3DAsync")==0){
		Stack->CorrectParams(3);
		D3DXVECTOR3 pos;
		pos.x = Stack->Pop()->GetFloat();
		pos.y = Stack->Pop()->GetFloat();
		pos.z = Stack->Pop()->GetFloat();
		GoTo(pos);
		if(strcmp(Name, "GoTo3DAsync")!=0) Script->WaitForExclusive(this);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoTo / GoToAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoTo")==0 || strcmp(Name, "GoToAsync")==0){
		Stack->CorrectParams(2);
		int x = Stack->Pop()->GetInt();
		int y = Stack->Pop()->GetInt();
		CAdGame* AdGame = (CAdGame*)Game;
		if(AdGame->m_Scene->m_Geom){
			D3DXVECTOR3 pos;
			if(AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(x, y, &pos)){			
				GoTo(pos);
				if(strcmp(Name, "GoToAsync")!=0) Script->WaitForExclusive(this);
			}
		}

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// GoToObject / GoToObjectAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "GoToObject")==0 || strcmp(Name, "GoToObjectAsync")==0){
		Stack->CorrectParams(1);
		CScValue* Val = Stack->Pop();
		if(!Val->IsNative()){
			Script->RuntimeError("actor.%s method accepts an entity refrence only", Name);
			Stack->PushNULL();
			return S_OK;
		}
		CAdObject* Obj = (CAdObject*)Val->GetNative();
		if(!Obj || Obj->m_Type != OBJECT_ENTITY){
			Script->RuntimeError("actor.%s method accepts an entity refrence only", Name);
			Stack->PushNULL();
			return S_OK;
		}
		CAdEntity* Ent = (CAdEntity*)Obj;

		CAdGame* AdGame = (CAdGame*)Game;
		if(AdGame->m_Scene->m_Geom){
			D3DXVECTOR3 pos;
			if(AdGame->m_Scene->m_Geom->Convert2Dto3DTolerant(Ent->m_WalkToX, Ent->m_WalkToY, &pos))
			{
				if(Ent->m_WalkToX==0 && Ent->m_WalkToY==0) GoTo(pos);
				else GoTo(pos, DirToAngle(Ent->m_WalkToDir));
				if(strcmp(Name, "GoToObjectAsync")!=0) Script->WaitForExclusive(this);
			}
		}
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnTo / TurnToAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnTo")==0 || strcmp(Name, "TurnToAsync")==0){
		Stack->CorrectParams(1);
		int dir;
		CScValue* val = Stack->Pop();

		float angle = 0.0;

		// turn to object?
		if(val->IsNative() && Game->ValidObject((CBObject*)val->GetNative())){
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

		TurnTo(angle);
		if(strcmp(Name, "TurnToAsync")!=0) Script->WaitForExclusive(this);
		
		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// TurnToAngle / TurnToAngleAsync
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TurnToAngle")==0 || strcmp(Name, "TurnToAngleAsync")==0){
		Stack->CorrectParams(1);
		float angle = Stack->Pop()->GetFloat();		
		TurnTo(angle);
		if(strcmp(Name, "TurnToAngleAsync")!=0) Script->WaitForExclusive(this);

		Stack->PushNULL();
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// IsWalking
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IsWalking")==0){
		Stack->CorrectParams(0);
		Stack->PushBool(m_State==STATE_FOLLOWING_PATH);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectWalk / DirectWalkBack
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectWalk")==0 || strcmp(Name, "DirectWalkBack")==0){
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
	else if(strcmp(Name, "DirectWalkStop")==0){
		Stack->CorrectParams(0);
		m_DirectWalkMode = DIRECT_WALK_NONE;
		Stack->PushBool(true);
		return S_OK;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectTurnLeft / DirectTurnRight
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectTurnLeft")==0 || strcmp(Name, "DirectTurnRight")==0){
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
	else if(strcmp(Name, "DirectTurnStop")==0){
		Stack->CorrectParams(0);
		m_DirectTurnMode = DIRECT_TURN_NONE;
		Stack->PushBool(true);
		return S_OK;
	}

	else return CAdObject3D::ScCallMethod(Script, Stack, ThisStack, Name);
}


//////////////////////////////////////////////////////////////////////////
CScValue* CAdActor3D::ScGetProperty(char *Name)
{
	m_ScValue->SetNULL();

	//////////////////////////////////////////////////////////////////////////
	// Type
	//////////////////////////////////////////////////////////////////////////
	if(strcmp(Name, "Type")==0){
		m_ScValue->SetString("actor3d");
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// TalkAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "TalkAnimName")==0){
		m_ScValue->SetString(m_TalkAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// WalkAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "WalkAnimName")==0){
		m_ScValue->SetString(m_WalkAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// IdleAnimName
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "IdleAnimName")==0){
		m_ScValue->SetString(m_IdleAnimName);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// DirectionAngle / DirAngle
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "DirectionAngle")==0 || strcmp(Name, "DirAngle")==0){
		m_ScValue->SetFloat(m_Angle);
		return m_ScValue;
	}

	//////////////////////////////////////////////////////////////////////////
	// Direction
	//////////////////////////////////////////////////////////////////////////
	else if(strcmp(Name, "Direction")==0){
		m_ScValue->SetInt(AngleToDir(m_Angle));
		return m_ScValue;
	}

	else return CAdObject3D::ScGetProperty(Name);
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::ScSetProperty(char *Name, CScValue *Value)
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

	else return CAdObject3D::ScSetProperty(Name, Value);
}


//////////////////////////////////////////////////////////////////////////
char* CAdActor3D::ScToString()
{
	return "[actor3d object]";
}


//////////////////////////////////////////////////////////////////////////
HRESULT CAdActor3D::Persist(CBPersistMgr* PersistMgr)
{
	CAdObject3D::Persist(PersistMgr);

	PersistMgr->Transfer(TMEMBER(m_Path));
	PersistMgr->Transfer(TMEMBER(m_TargetAngle));
	PersistMgr->Transfer(TMEMBER(m_TargetPoint));
	PersistMgr->Transfer(TMEMBER(m_TurningLeft));
	PersistMgr->Transfer(TMEMBER(m_AfterWalkAngle));
	
	PersistMgr->Transfer(TMEMBER(m_TalkAnimName));
	PersistMgr->Transfer(TMEMBER(m_IdleAnimName));
	PersistMgr->Transfer(TMEMBER(m_WalkAnimName));

	// direct controls
	PersistMgr->Transfer(TMEMBER_INT(m_DirectWalkMode));
	PersistMgr->Transfer(TMEMBER_INT(m_DirectTurnMode));
	PersistMgr->Transfer(TMEMBER(m_DirectWalkAnim));
	PersistMgr->Transfer(TMEMBER(m_DirectTurnAnim));
	PersistMgr->Transfer(TMEMBER(m_DirectWalkVelocity));
	PersistMgr->Transfer(TMEMBER(m_DirectTurnVelocity));

	return S_OK;
}

/*
NewPos.x += -sinf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
NewPos.z += -cosf(DegToRad(m_Angle)) * m_Velocity * m_Scale3D * (float)Game->m_TimerDelta/1000.f;
m_Model->SetAnimation(0, m_WalkAnimName);
*/



//////////////////////////////////////////////////////////////////////////
CAdObject3D* CAdActor3D::CreateFromFile(CBGame* Game, char* Filename)
{
	if(IsActorX(Game, Filename))
	{
		CAdActor3DX* Actor = new CAdActor3DX(Game);
		if(Actor)
		{
			if(FAILED(Actor->LoadFile(Filename)))
			{
				delete Actor;
				return NULL;
			}
		}
		return Actor;
	}
	else
	{
		Game->LOG(0, "Warning: 3D actors in MS3D format are now deprecated. Please convert your model to X format.");
		Game->LOG(0, "(see the documentation for migration tips)");
		
		CAdActor3D* Actor = new CAdActor3D(Game);
		if(Actor)
		{
			if(FAILED(Actor->LoadFile(Filename)))
			{
				delete Actor;
				return NULL;
			}
		}
		return Actor;
	}
}

//////////////////////////////////////////////////////////////////////////
bool CAdActor3D::IsActorX(CBGame* Game, char* Filename)
{
	TOKEN_TABLE_START(commands)
		TOKEN_TABLE (ACTOR3DX)
		TOKEN_TABLE (ACTOR3D)
	TOKEN_TABLE_END


	BYTE* Buffer = Game->m_FileManager->ReadWholeFile(Filename);
	if(Buffer==NULL) return false;

	BYTE* WorkBuffer = Buffer;

	char* params;
	CBParser parser(Game);

	bool Ret = false;
	if(parser.GetCommand ((char**)&WorkBuffer, commands, (char**)&params)==TOKEN_ACTOR3DX)
		Ret = true;

	delete [] Buffer;
	return Ret;
}
