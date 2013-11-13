// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include "3DSkelAnim.h"


IMPLEMENT_PERSISTENT(C3DSkelAnim, false);

//////////////////////////////////////////////////////////////////////////
C3DSkelAnim::C3DSkelAnim(CBGame* inGame):CBNamedObject(inGame)
{
	m_TotalTime = 0;
	m_FrameTime = 0;
	m_JointAnims = NULL;
	m_NumJointAnims = 0;
	m_Looping = true;
	m_Finished = false;
	m_CurrentFrame = -1;
}


//////////////////////////////////////////////////////////////////////////
C3DSkelAnim::~C3DSkelAnim()
{
	SAFE_DELETE_ARRAY(m_JointAnims);

	for(int i=0; i<m_Events.GetSize(); i++) delete m_Events[i];
	m_Events.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////
C3DSkelAnim::C3DJointAnim::C3DJointAnim()
{
	m_NumRotationKeyframes = m_NumTranslationKeyframes = 0;
	m_RotationKeyframes = NULL;
	m_TranslationKeyframes = NULL;		
}


//////////////////////////////////////////////////////////////////////////
C3DSkelAnim::C3DJointAnim::~C3DJointAnim()
{
	SAFE_DELETE_ARRAY(m_RotationKeyframes);
	SAFE_DELETE_ARRAY(m_TranslationKeyframes);
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DSkelAnim::Persist(CBPersistMgr* PersistMgr)
{
	if(!PersistMgr->m_Saving)
	{
		m_TotalTime = 0;
		m_FrameTime = 0;
		m_JointAnims = NULL;
		m_NumJointAnims = 0;
		m_Looping = true;
		m_Finished = false;
		m_Name = NULL;
		m_CurrentFrame = -1;
	}

	PersistMgr->Transfer(TMEMBER(m_CurrentFrame));
	PersistMgr->Transfer(TMEMBER(m_Finished));
	PersistMgr->Transfer(TMEMBER(m_FrameTime));
	PersistMgr->Transfer(TMEMBER(m_Looping));
	PersistMgr->Transfer(TMEMBER(m_Name));
	PersistMgr->Transfer(TMEMBER(m_TotalTime));

	int i;

	// persist events
	int NumEvents;
	if(PersistMgr->m_Saving) NumEvents = m_Events.GetSize();
	PersistMgr->Transfer(TMEMBER(NumEvents));

	for(i=0; i<NumEvents; i++)
	{
		if(PersistMgr->m_Saving) m_Events[i]->Persist(PersistMgr);
		else
		{
			C3DAnimEvent* Event = new C3DAnimEvent();
			Event->Persist(PersistMgr);
			m_Events.Add(Event);
		}
	}

	// persist joint anims
	PersistMgr->Transfer(TMEMBER(m_NumJointAnims));
	if(!PersistMgr->m_Saving) m_JointAnims = new C3DJointAnim[m_NumJointAnims];

	for(i=0; i<m_NumJointAnims; i++)
	{
		m_JointAnims[i].Persist(PersistMgr);
	}


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
HRESULT C3DSkelAnim::C3DJointAnim::Persist(CBPersistMgr* PersistMgr)
{
	PersistMgr->Transfer(TMEMBER(m_NumRotationKeyframes));
	PersistMgr->Transfer(TMEMBER(m_NumTranslationKeyframes));

	if(!PersistMgr->m_Saving)
	{
		m_RotationKeyframes = new C3DKeyframe[m_NumRotationKeyframes];
		m_TranslationKeyframes = new C3DKeyframe[m_NumTranslationKeyframes];
	}

	int i;

	for(i=0; i<m_NumRotationKeyframes; i++)
	{
		PersistMgr->Transfer(TMEMBER(m_RotationKeyframes[i].m_JointIndex));
		PersistMgr->Transfer(TMEMBER(m_RotationKeyframes[i].m_Parameter));
		PersistMgr->Transfer(TMEMBER(m_RotationKeyframes[i].m_Time));
	}

	for(i=0; i<m_NumTranslationKeyframes; i++)
	{
		PersistMgr->Transfer(TMEMBER(m_TranslationKeyframes[i].m_JointIndex));
		PersistMgr->Transfer(TMEMBER(m_TranslationKeyframes[i].m_Parameter));
		PersistMgr->Transfer(TMEMBER(m_TranslationKeyframes[i].m_Time));
	}

	return S_OK;
}