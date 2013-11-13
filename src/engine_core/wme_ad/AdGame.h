// This file is part of Wintermute Engine
// For conditions of distribution and use, see copyright notice in license.txt
// http://dead-code.org/redir.php?target=wme


#if !defined(AFX_ADGAME_H__DCAD4311_4A3B_4D76_BA18_09734988BBB1__INCLUDED_)
#define AFX_ADGAME_H__DCAD4311_4A3B_4D76_BA18_09734988BBB1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BGame.h"

class CAdGame : public CBGame  
{
public:
	virtual HRESULT OnScriptShutdown(CScScript* Script);

	virtual HRESULT OnMouseLeftDown();
	virtual HRESULT OnMouseLeftUp();
	virtual HRESULT OnMouseLeftDblClick();
	virtual HRESULT OnMouseRightDown();
	virtual HRESULT OnMouseRightUp();

	virtual HRESULT DisplayDebugInfo();

	virtual CBObject* GetNextAccessObject(CBObject* CurrObject);
	virtual CBObject* GetPrevAccessObject(CBObject* CurrObject);

	virtual HRESULT GetLayerSize(int* LayerWidth, int* LayerHeight, RECT* Viewport, bool* CustomViewport);
	virtual DWORD GetAmbientLightColor();
	virtual HRESULT GetFogParams(bool* FogEnabled, DWORD* FogColor, float* Start, float* End);

	virtual HRESULT RenderShadowGeometry();
	virtual HRESULT InitAfterLoad();
	static void AfterLoadScene(void* Scene, void* Data);
	virtual TShadowType GetMaxShadowType(CBObject* Object=NULL);

	bool m_SmartItemCursor;

	CBArray<char*, char*> m_SpeechDirs;
	HRESULT AddSpeechDir(char* Dir);
	HRESULT RemoveSpeechDir(char* Dir);
	char* FindSpeechFile(char* StringID);

	HRESULT DeleteItem(CAdItem* Item);
	char* m_ItemsFile;
	bool m_TempDisableSaveState;
	virtual HRESULT ResetContent();
	HRESULT AddItem(CAdItem *Item);
	CAdItem* GetItemByName(char *Name);
	CBArray<CAdItem*, CAdItem*> m_Items;
	CAdObject* m_InventoryOwner;
	bool IsItemTaken(char* ItemName);
	HRESULT RegisterInventory(CAdInventory* Inv);
	HRESULT UnregisterInventory(CAdInventory* Inv);

	CAdObject* m_InvObject;
	CBArray<CAdInventory*, CAdInventory*> m_Inventories;
	virtual HRESULT DisplayContent(bool Update=true, bool DisplayAll=false);
	char* m_DebugStartupScene;
	char* m_StartupScene;
	bool m_InitialScene;
	bool GameResponseUsed(int ID);
	HRESULT AddGameResponse(int ID);
	HRESULT ResetResponse(int ID);

	bool BranchResponseUsed(int ID);
	HRESULT AddBranchResponse(int ID);
	HRESULT ClearBranchResponses(char* Name);
	HRESULT StartDlgBranch(char* BranchName, char* ScriptName, char* EventName);
	HRESULT EndDlgBranch(char* BranchName, char* ScriptName, char* EventName);
	virtual HRESULT WindowLoadHook(CUIWindow *Win, char **Buf, char **Params);
	virtual HRESULT WindowScriptMethodHook(CUIWindow* Win, CScScript* Script, CScStack* Stack, char* Name);

	CAdSceneState* GetSceneState(char* Filename, bool Saving);
	CBViewport* m_SceneViewport;
	int m_TexItemLifeTime;
	int m_TexWalkLifeTime;
	int m_TexStandLifeTime;
	int m_TexTalkLifeTime;

	TTalkSkipButton m_TalkSkipButton;
	TVideoSkipButton m_VideoSkipButton;

	virtual HRESULT GetVersion(BYTE* VerMajor, BYTE* VerMinor, BYTE* ExtMajor, BYTE* ExtMinor);
	HRESULT ScheduleChangeScene(char *Filename, bool FadeIn);
	char* m_ScheduledScene;
	bool m_ScheduledFadeIn;
	void SetPrevSceneName(char* Name);
	void SetPrevSceneFilename(char* Name);
	char* m_PrevSceneName;
	char* m_PrevSceneFilename;
	virtual HRESULT LoadGame(char* Filename);
	CAdItem* m_SelectedItem;
	HRESULT Cleanup();
	DECLARE_PERSISTENT(CAdGame, CBGame);

	void FinishSentences();
	HRESULT ShowCursor();
	TGameStateEx m_StateEx;
	CAdResponseBox* m_ResponseBox;
	CAdInventoryBox* m_InventoryBox;
	HRESULT DisplaySentences(bool Frozen);
	void AddSentence(CAdSentence* Sentence);
	HRESULT ChangeScene(char* Filename, bool FadeIn);
	HRESULT RemoveObject(CAdObject* Object);
	HRESULT AddObject(CAdObject* Object);
	CAdScene* m_Scene;
	HRESULT InitLoop();
	CAdGame();
	virtual ~CAdGame();
	CBArray<CAdObject*, CAdObject*> m_Objects;
	CBArray<CAdSentence*, CAdSentence*> m_Sentences;
	
	CBArray<CAdSceneState*, CAdSceneState*> m_SceneStates;
	CBArray<char*, char*> m_DlgPendingBranches;

	CBArray<CAdResponseContext*, CAdResponseContext*> m_ResponsesBranch;
	CBArray<CAdResponseContext*, CAdResponseContext*> m_ResponsesGame;

	virtual HRESULT LoadFile(char * Filename);
	virtual HRESULT LoadBuffer(BYTE * Buffer, bool Complete=true);

	HRESULT LoadItemsFile(char * Filename, bool Merge=false);
	HRESULT LoadItemsBuffer(BYTE * Buffer, bool Merge=false);


	virtual void PublishNatives();
	virtual HRESULT ExternalCall(CScScript* Script, CScStack* Stack, CScStack* ThisStack, char* Name);

	// scripting interface
	virtual CScValue* ScGetProperty(char* Name);
	virtual HRESULT ScSetProperty(char *Name, CScValue *Value);
	virtual HRESULT ScCallMethod(CScScript* Script, CScStack *Stack, CScStack *ThisStack, char *Name);	
	bool ValidMouse();
};

#endif // !defined(AFX_ADGAME_H__DCAD4311_4A3B_4D76_BA18_09734988BBB1__INCLUDED_)
