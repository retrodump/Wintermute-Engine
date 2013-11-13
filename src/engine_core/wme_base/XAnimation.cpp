// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#include "dcgf.h"
#include ".\XAnimation.h"
#include "XModel.h"
#include "XFrameNode.h"
#include "XAnimationSet.h"
#include "3DUtils.h"


//////////////////////////////////////////////////////////////////////////
CXAnimation::CXAnimation(CBGame* inGame) : CBBase(inGame)
{
	m_TargetName = NULL;
	m_TargetFrame = NULL;
}


//////////////////////////////////////////////////////////////////////////
CXAnimation::~CXAnimation()
{
	SAFE_DELETE_ARRAY(m_TargetName);


	for(int i=0; i<m_PosKeys.GetSize(); i++)
	{
		delete m_PosKeys[i];
	}
	m_PosKeys.RemoveAll();


	for(int i=0; i<m_RotKeys.GetSize(); i++)
	{
		delete m_RotKeys[i];
	}
	m_RotKeys.RemoveAll();


	for(int i=0; i<m_ScaleKeys.GetSize(); i++)
	{
		delete m_ScaleKeys[i];
	}
	m_ScaleKeys.RemoveAll();
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::FindBone(CXFrameNode* RootFrame)
{
	if(m_TargetName && strcmp(m_TargetName, "") != 0)
	{
		m_TargetFrame = RootFrame->FindFrame(m_TargetName);
	}
	return S_OK;
}

#ifdef WME_D3D9
//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::Load(LPDIRECTXFILEDATA XObj, CXAnimationSet* ParentAnimSet)
{
	HRESULT res;
	GUID ObjectType;

	// Query the child for it's FileDataReference
	if(XObj->IsReference())
	{
		// The original data is found
		res = XObj->GetType(&ObjectType);
		if(FAILED(res))
		{
			Game->LOG(res, "Couldn't retrieve object type while loading animation");
			return res;
		}

		// The object must be a frame
		if(ObjectType == TID_D3DRMFrame)
		{
			// The frame is found, get its name
			// The name will be used later by the FindBone function to get
			// a pointer to the target frame
			if(m_TargetFrame != NULL)
			{
				Game->LOG(0, "Animation frame name reference duplicated");
				return E_FAIL;
			}

			// get name
			res = CXModel::LoadName(&m_TargetName, XObj);
			if(FAILED(res))
			{
				Game->LOG(res, "Error retrieving frame name while loading animation");
				return res;
			}
		}
	}
	else
	{
		// a data object is found, get its type
		res = XObj->GetType(&ObjectType);
		if(FAILED(res))
		{
			Game->LOG(res, "Couldn't retrieve object type");
			return res;
		}

		if(ObjectType == TID_D3DRMAnimationKey)
		{				
			// an animation key is found, load the data
			DWORD Size;
			BYTE* Buffer;
			res = XObj->Lock(&Size, (LPCVOID*)&Buffer);
			if(FAILED(res))
			{
				Game->LOG(res, "Error retrieving data");
				return res;
			}

			if(FAILED(res = LoadAnimationKeyData(Buffer)))
			{
				XObj->Unlock();
				return res;
			}
			XObj->Unlock();
		}
		else if(ObjectType == TID_D3DRMAnimationOptions)
		{
			DWORD Size;
			BYTE* Buffer;
			res = XObj->Lock(&Size, (LPCVOID*)&Buffer);
			if(FAILED(res))
			{
				Game->LOG(res, "Error retrieving data");
				return res;
			}
			LoadAnimationOptionData(Buffer, ParentAnimSet);
			XObj->Unlock();
		}
	}
	return S_OK;
}

#else

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::Load(LPDIRECTXFILEOBJECT XObj, CXAnimationSet* ParentAnimSet)
{
	HRESULT res;

	LPDIRECTXFILEDATA XDataObj = NULL;
	LPDIRECTXFILEDATAREFERENCE XRefObj = NULL;
	const GUID* ObjectType;

	// Query the child for it's FileDataReference
	res = XObj->QueryInterface(IID_IDirectXFileDataReference, (void**)&XRefObj);
	if(SUCCEEDED(res))
	{
		// The object is of type reference. Use it to get a reference to the target
		// frame
		res = XRefObj->Resolve(&XDataObj);
		if(SUCCEEDED(res))
		{
			// The original data is found
			res = XDataObj->GetType(&ObjectType);
			if(FAILED(res))
			{
				RELEASE(XRefObj);
				RELEASE(XDataObj);
				Game->LOG(res, "Couldn't retrieve object type while loading animation");
				return res;
			}

			// The object must be a frame
			if(*ObjectType == TID_D3DRMFrame)
			{
				// The frame is found, get its name
				// The name will be used later by the FindBone function to get
				// a pointer to the target frame
				if(m_TargetFrame != NULL)
				{
					RELEASE(XRefObj);
					RELEASE(XDataObj);
					Game->LOG(0, "Animation frame name reference duplicated");
					return E_FAIL;
				}

				// get name
				res = CXModel::LoadName(&m_TargetName, XDataObj);
				if(FAILED(res))
				{
					RELEASE(XRefObj);
					RELEASE(XDataObj);
					Game->LOG(res, "Error retrieving frame name while loading animation");
					return res;
				}
			}
			RELEASE(XDataObj);
		}
		RELEASE(XRefObj);
	}
	else
	{
		// Query the child for it's FileData
		res = XObj->QueryInterface(IID_IDirectXFileData, (void**)&XDataObj);
		if(SUCCEEDED(res))
		{
			// a data object is found, get its type
			res = XDataObj->GetType(&ObjectType);
			if(FAILED(res))
			{
				RELEASE(XDataObj);
				Game->LOG(res, "Couldn't retrieve object type");
				return res;
			}

			if(*ObjectType == TID_D3DRMAnimationKey)
			{				
				// an animation key is found, load the data
				DWORD Size;
				BYTE* Buffer;
				res = XDataObj->GetData(NULL, &Size, (void**)&Buffer);
				if(FAILED(res))
				{
					RELEASE(XDataObj);
					Game->LOG(res, "Error retrieving data");
					return res;
				}

				if(FAILED(res = LoadAnimationKeyData(Buffer)))
				{
					RELEASE(XDataObj);
					return res;
				}				
			}

			else if(*ObjectType == TID_D3DRMAnimationOptions)
			{
				DWORD Size;
				BYTE* Buffer;
				res = XDataObj->GetData(NULL, &Size, (void**)&Buffer);
				if(FAILED(res))
				{
					RELEASE(XDataObj);
					Game->LOG(res, "Error retrieving data");
					return res;
				}
				LoadAnimationOptionData(Buffer, ParentAnimSet);
			}

			RELEASE(XDataObj);
		}
	}
	return S_OK;
}
#endif


//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::LoadAnimationOptionData(BYTE* Buffer, CXAnimationSet* ParentAnimSet)
{
	// get the type and count of the key
	DWORD OpenClosed = ((DWORD*)Buffer)[0];
	DWORD PositionQuality = ((DWORD*)Buffer)[1];

	if(OpenClosed && ParentAnimSet) ParentAnimSet->m_Looping = true;

	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::LoadAnimationKeyData(BYTE* Buffer)
{
	// define datatypes needed by the load function
	struct tmpPOSITIONKEY
	{
		DWORD dwTime;
		DWORD dwFloatsCount;
		D3DXVECTOR3 Pos;
	} *FilePosKey;

	struct tmpSCALEKEY
	{
		DWORD dwTime;
		DWORD dwFloatsCount;
		D3DXVECTOR3 Scale;
	} *FileScaleKey;

	struct tmpROTATIONKEY
	{
		DWORD dwTime;
		DWORD dwFloatsCount;
		//NOTE x files are w x y z and QUATERNIONS are x y z w
		float w;
		float x;
		float y;
		float z;
	} *FileRotKey;

	struct tmpMATRIXKEY
	{
		DWORD dwTime;
		DWORD dwFloatsCount;
		D3DXMATRIX Mat;
	} *FileMatrixKey;


	// get the type and count of the key
	DWORD KeyType = ((DWORD*)Buffer)[0];
	DWORD NumKeys = ((DWORD*)Buffer)[1];

	if(KeyType == 0 /*rotation key*/)
	{
		if(m_RotKeys.GetSize() != 0)
		{
			Game->LOG(0, "Rotation key duplicated");
			return E_FAIL;
		}

		//NOTE x files are w x y z and QUATERNIONS are x y z w

		FileRotKey = (tmpROTATIONKEY*)(Buffer + (sizeof(DWORD) * 2));
		for(DWORD Key = 0; Key < NumKeys; Key++)
		{
			BONEROTATIONKEY* RotKey = new BONEROTATIONKEY;
			RotKey->Time       = FileRotKey->dwTime;
			RotKey->Rotation.x = FileRotKey->x;
			RotKey->Rotation.y = FileRotKey->y;
			RotKey->Rotation.z = FileRotKey->z;
			RotKey->Rotation.w = FileRotKey->w;

			m_RotKeys.Add(RotKey);

			FileRotKey++;
		}
	}

	else if(KeyType == 1 /*scale key*/ )
	{
		if(m_ScaleKeys.GetSize() != 0)
		{
			Game->LOG(0, "Scale key duplicated");
			return E_FAIL;
		}

		FileScaleKey = (tmpSCALEKEY*)(Buffer + (sizeof(DWORD) * 2));
		for(DWORD Key = 0; Key < NumKeys; Key++)
		{
			BONESCALEKEY* ScaleKey = new BONESCALEKEY;
			ScaleKey->Time  = FileScaleKey->dwTime;
			ScaleKey->Scale = FileScaleKey->Scale;

			m_ScaleKeys.Add(ScaleKey);

			FileScaleKey++;
		}
	}

	else if(KeyType == 2 /*position key*/ )
	{
		if(m_PosKeys.GetSize() != 0)
		{
			Game->LOG(0, "Position key duplicated");
			return E_FAIL;
		}

		FilePosKey = (tmpPOSITIONKEY*)(Buffer + (sizeof(DWORD) * 2));
		for(DWORD Key = 0; Key < NumKeys; Key++)
		{
			BONEPOSITIONKEY* PosKey = new BONEPOSITIONKEY;
			PosKey->Time = FilePosKey->dwTime;
			PosKey->Pos  = FilePosKey->Pos;

			m_PosKeys.Add(PosKey);

			FilePosKey++;
		}
	}

	else if(KeyType == 4 /*matrix key*/ )
	{
		if(m_RotKeys.GetSize() != 0 || m_ScaleKeys.GetSize() != 0 || m_PosKeys.GetSize() != 0)
		{
			Game->LOG(0, "Matrix key duplicated");
			return E_FAIL;
		}

		D3DXQUATERNION QRot;
		D3DXVECTOR3 TransVec;
		D3DXVECTOR3 ScaleVec;

		FileMatrixKey = (tmpMATRIXKEY*)(Buffer + (sizeof(DWORD) * 2));
		for(DWORD Key = 0; Key < NumKeys; Key++ )
		{
			// we always convert matrix keys to T-R-S
			C3DUtils::DecomposeMatrixSimple(&FileMatrixKey->Mat, &TransVec, &ScaleVec, &QRot);

			BONEPOSITIONKEY* PosKey = new BONEPOSITIONKEY;
			PosKey->Time = FileMatrixKey->dwTime;
			PosKey->Pos  = TransVec;
			m_PosKeys.Add(PosKey);

			BONESCALEKEY* ScaleKey = new BONESCALEKEY;
			ScaleKey->Time  = FileMatrixKey->dwTime;
			ScaleKey->Scale = ScaleVec;
			m_ScaleKeys.Add(ScaleKey);

			BONEROTATIONKEY* RotKey = new BONEROTATIONKEY;
			RotKey->Time     = FileMatrixKey->dwTime;
			RotKey->Rotation = QRot;

			RotKey->Rotation.x = -RotKey->Rotation.x;
			RotKey->Rotation.y = -RotKey->Rotation.y;
			RotKey->Rotation.z = -RotKey->Rotation.z;
			m_RotKeys.Add(RotKey);

			FileMatrixKey++;
		}
	}
	else
	{
		// the type is unknown, report the error
		Game->LOG(0, "Unexpected animation key type (%d)", KeyType);
	}
	return S_OK;
}

//////////////////////////////////////////////////////////////////////////
HRESULT CXAnimation::Update(int Slot, DWORD LocalTime, float AnimLerpValue)
{
	// no target frame = no animation keys
	if(!m_TargetFrame) return S_OK;


	D3DXVECTOR3 ResultPos(0.0f, 0.0f, 0.0f);
	D3DXVECTOR3 ResultScale(1.0f, 1.0f, 1.0f);
	D3DXQUATERNION ResultRot(0.0f, 0.0f, 0.0f, 1.0f);


	// initialize a matrix to hold the result of the transformation
	/*
	D3DXMATRIX ResultMat;
	D3DXMatrixIdentity(&ResultMat);
	*/

	int KeyIndex1, KeyIndex2;
	DWORD Time1, Time2;
	float LerpValue;

	bool Animate = false;

	// scale keys
	if(m_ScaleKeys.GetSize() > 0)
	{
		KeyIndex1 = KeyIndex2 = 0;
		//LocalTime = Time % m_ScaleKeys[m_ScaleKeys.GetSize()-1].Time;

		// get the two keys between which the time is currently in
		for(int Key = 0; Key < m_ScaleKeys.GetSize(); Key++)
		{
			if(m_ScaleKeys[Key]->Time > LocalTime)
			{
				KeyIndex2 = Key;

				if(Key > 0)
					KeyIndex1 = Key - 1;
				else  // when ikey == 0, then dwp2 == 0
					KeyIndex1 = Key;
				break;
			}
		}
		Time1 = m_ScaleKeys[KeyIndex1]->Time;
		Time2 = m_ScaleKeys[KeyIndex2]->Time;

		// get the lerp value
		if((Time2 - Time1) == 0) LerpValue = 0;
		else LerpValue = float(LocalTime - Time1) / float(Time2 - Time1);

		// apply the lerp function on the scale vector
		D3DXVec3Lerp(&ResultScale, &m_ScaleKeys[KeyIndex1]->Scale, &m_ScaleKeys[KeyIndex2]->Scale, LerpValue);

		// create the matrix
		/*
		D3DXMATRIX ScaleMat;
		D3DXMatrixScaling(&ScaleMat, ResultScale.x, ResultScale.y, ResultScale.z);
		D3DXMatrixMultiply(&ResultMat, &ResultMat, &ScaleMat);
		*/

		Animate = true;
	}

	// rotation keys
	if(m_RotKeys.GetSize() > 0)
	{
		KeyIndex1 = KeyIndex2 = 0;
		//LocalTime = Time % m_RotKeys[m_RotKeys.GetSize()-1].Time;

		// get the two keys surrounding the current time value
		for(int Key = 0; Key < m_RotKeys.GetSize(); Key++)
		{
			if(m_RotKeys[Key]->Time > LocalTime)
			{
				KeyIndex2 = Key;
				if(Key > 0)
					KeyIndex1 = Key - 1;
				else  // when ikey == 0, then dwp2 == 0
					KeyIndex1 = Key;
				break;
			}
		}
		Time1 = m_RotKeys[KeyIndex1]->Time;
		Time2 = m_RotKeys[KeyIndex2]->Time;

		// get the lerp value
		if((Time2 - Time1) == 0) LerpValue = 0;
		else LerpValue = float(LocalTime - Time1) / float(Time2 - Time1);

		//apply spherical lerp function
		D3DXQUATERNION q1,q2;

		q1.x = -m_RotKeys[KeyIndex1]->Rotation.x;
		q1.y = -m_RotKeys[KeyIndex1]->Rotation.y;
		q1.z = -m_RotKeys[KeyIndex1]->Rotation.z;
		q1.w =  m_RotKeys[KeyIndex1]->Rotation.w;

		q2.x = -m_RotKeys[KeyIndex2]->Rotation.x;
		q2.y = -m_RotKeys[KeyIndex2]->Rotation.y;
		q2.z = -m_RotKeys[KeyIndex2]->Rotation.z;
		q2.w =  m_RotKeys[KeyIndex2]->Rotation.w;

		D3DXQuaternionSlerp(&ResultRot, &q1, &q2, LerpValue);

		// create the matrix
		/*
		D3DXMATRIX RotMat;
		D3DXMatrixRotationQuaternion(&RotMat, &ResultRot);
		D3DXMatrixMultiply(&ResultMat, &ResultMat, &RotMat);
		*/
		
		Animate = true;
	}

	// position keys
	if(m_PosKeys.GetSize() > 0)
	{
		KeyIndex1 = KeyIndex2 = 0;
		//LocalTime = Time % m_PosKeys[m_PosKeys.GetSize()-1].Time;

		// get the two keys surrounding the time value
		for(int Key = 0; Key < m_PosKeys.GetSize(); Key++)
		{
			if(m_PosKeys[Key]->Time > LocalTime)
			{
				KeyIndex2 = Key;
				if(Key > 0)
					KeyIndex1 = Key - 1;
				else  // when ikey == 0, then dwp2 == 0
					KeyIndex1 = Key;
				break;
			}
		}
		Time1 = m_PosKeys[KeyIndex1]->Time;
		Time2 = m_PosKeys[KeyIndex2]->Time;

		// get the lerp value
		if(Time2 - Time1 == 0)
			LerpValue = 0;
		else
			LerpValue = float(LocalTime - Time1) / float(Time2 - Time1);

		// apply the lerp function
		D3DXVec3Lerp(&ResultPos, &m_PosKeys[KeyIndex1]->Pos, &m_PosKeys[KeyIndex2]->Pos, LerpValue);

		// prepare the matrix
		/*
		D3DXMATRIX PosMat;
		D3DXMatrixTranslation(&PosMat, ResultPos.x, ResultPos.y, ResultPos.z);
		D3DXMatrixMultiply(&ResultMat, &ResultMat, &PosMat);
		*/
		
		Animate = true;
	}
	/*
	else
	{
		// if no position key is found, use the original position of the frame
		D3DXMATRIX* OrigMatrix = m_TargetFrame->GetOriginalMatrix();

		D3DXMATRIX PosMat;
		D3DXMatrixTranslation(&PosMat, OrigMatrix->_41, OrigMatrix->_42, OrigMatrix->_43);
		D3DXMatrixMultiply(&ResultMat, &ResultMat, &PosMat);
	}
	*/

	if(Animate)
	{
		//m_TargetFrame->SetTransformationMatrix(&ResultMat);
		m_TargetFrame->SetTransformation(Slot, ResultPos, ResultScale, ResultRot, AnimLerpValue);
	}


	return S_OK;
}


//////////////////////////////////////////////////////////////////////////
int CXAnimation::GetFrameTime()
{
	int FrameTime = 0;
	int PrevTime;
	
	// get the shortest frame time
	PrevTime = 0;
	for(int i=0; i<m_RotKeys.GetSize(); i++)
	{
		if(FrameTime==0 || m_RotKeys[i]->Time - PrevTime < FrameTime)
			FrameTime = m_RotKeys[i]->Time - PrevTime;

		PrevTime = m_RotKeys[i]->Time;
	}

	PrevTime = 0;
	for(int i=0; i<m_PosKeys.GetSize(); i++)
	{
		if(FrameTime==0 || m_PosKeys[i]->Time - PrevTime < FrameTime)
			FrameTime = m_PosKeys[i]->Time - PrevTime;

		PrevTime = m_PosKeys[i]->Time;
	}

	PrevTime = 0;
	for(int i=0; i<m_ScaleKeys.GetSize(); i++)
	{
		if(FrameTime==0 || m_ScaleKeys[i]->Time - PrevTime < FrameTime)
			FrameTime = m_ScaleKeys[i]->Time - PrevTime;

		PrevTime = m_ScaleKeys[i]->Time;
	}

	return FrameTime;
}


//////////////////////////////////////////////////////////////////////////
DWORD CXAnimation::GetTotalTime()
{
	DWORD TotalTime = 0;
	if(m_RotKeys.GetSize() > 0)   TotalTime = max(TotalTime, m_RotKeys[m_RotKeys.GetSize()-1]->Time);
	if(m_PosKeys.GetSize() > 0)   TotalTime = max(TotalTime, m_PosKeys[m_PosKeys.GetSize()-1]->Time);
	if(m_ScaleKeys.GetSize() > 0) TotalTime = max(TotalTime, m_ScaleKeys[m_ScaleKeys.GetSize()-1]->Time);

	return TotalTime;
}
