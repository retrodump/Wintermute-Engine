// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#pragma once

#ifdef WME_D3D9

class C3DEffectParams
{
public:

	//////////////////////////////////////////////////////////////////////////
	class C3DEffectParam
	{
	public:
		enum ParamType
		{
			EP_UNKNOWN,
			EP_STRING,
			EP_FLOAT,
			EP_INT,
			EP_BOOL,
			EP_VECTOR
		};

		C3DEffectParam();
		C3DEffectParam(char* paramName);
		~C3DEffectParam();

		void SetValue(char* val);
		void SetValue(int val);
		void SetValue(float val);
		void SetValue(bool val);
		void SetValue(D3DXVECTOR4 val);

		const char* GetParamName() const { return m_ParamName; }

		HRESULT Persist(CBPersistMgr* PersistMgr);
		void SetEffectParam(ID3DXEffect* effect);

	private:
		void SetDefaultValues();
		ParamType m_Type;
		char* m_ParamName;
		bool m_Initialized;
		D3DXHANDLE m_Handle;

		char* m_ValString;
		int m_ValInt;
		float m_ValFloat;
		D3DXVECTOR4 m_ValVector;
		bool m_ValBool;
	};


	//////////////////////////////////////////////////////////////////////////
	C3DEffectParams();
	~C3DEffectParams();

	HRESULT Persist(CBPersistMgr* PersistMgr);
	void SetEffectParams(ID3DXEffect* effect);
	void Clear();
	void SetParam(char* ParamName, CScValue* Val);
	void SetParam(char* ParamName, D3DXVECTOR4 Val);

private:
	C3DEffectParam* GetParamByName(char* paramName);
	CBArray<C3DEffectParam*, C3DEffectParam*> m_Params;
};

#endif // WME_D3D9