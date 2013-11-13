// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_3DSKELANIM_H__97D517E5_BC2E_42A3_9D16_71D7F32DD68F__INCLUDED_)
#define AFX_3DSKELANIM_H__97D517E5_BC2E_42A3_9D16_71D7F32DD68F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class C3DSkelAnim : public CBNamedObject
{
public:
	//////////////////////////////////////////////////////////////////////////
	class C3DAnimEvent
	{
	public:
		char* m_EventName;
		int m_Frame;

		C3DAnimEvent()
		{
			m_EventName = NULL;
			m_Frame = -1;
		}

		C3DAnimEvent(char* Name, int Frame)
		{
			m_EventName = NULL;
			CBUtils::SetString(&m_EventName, Name);
			m_Frame = Frame;
		}

		virtual ~C3DAnimEvent()
		{
			if(m_EventName) delete [] m_EventName;
		}

		HRESULT Persist(CBPersistMgr *PersistMgr)
		{
			PersistMgr->Transfer(TMEMBER(m_EventName));
			PersistMgr->Transfer(TMEMBER(m_Frame));
			return S_OK;
		}
	};


public:
	DECLARE_PERSISTENT(C3DSkelAnim, CBNamedObject);

	bool m_Finished;
	
	class C3DJointAnim{
	public:
		C3DJointAnim();
		virtual ~C3DJointAnim();
		int m_NumRotationKeyframes;
		int m_NumTranslationKeyframes;
	
		C3DKeyframe* m_TranslationKeyframes;
		C3DKeyframe* m_RotationKeyframes;

		HRESULT Persist(CBPersistMgr* PersistMgr);
	};

	C3DSkelAnim(CBGame* inGame);
	virtual ~C3DSkelAnim();
	double m_TotalTime;
	double m_FrameTime;
	C3DJointAnim* m_JointAnims;
	int m_NumJointAnims;
	bool m_Looping;
	int m_CurrentFrame;

	CBArray<C3DAnimEvent*, C3DAnimEvent*> m_Events;
};

#endif // !defined(AFX_3DSKELANIM_H__97D517E5_BC2E_42A3_9D16_71D7F32DD68F__INCLUDED_)
