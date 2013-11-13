//////////////////////////////////////////////////////////////////////////
// Wintermute Engine plugin interface header
// (c) Dead:Code 2010
//
// !!! DO NOT MODIFY THIS FILE !!!
//
// See the WME documentation for a detailed description
// of the interface classes
//////////////////////////////////////////////////////////////////////////

#ifndef WME_PLUGIN_H
#define WME_PLUGIN_H


//////////////////////////////////////////////////////////////////////////
// the WME version this plugin is compiled for
#define WME_VERSION_MAJOR 1
#define WME_VERSION_MINOR 5
#define WME_VERSION_BUILD 2


class IWmeParamSet;
class IWmeValue;
class IWmeFile;
class IWmeSubFrame;

//////////////////////////////////////////////////////////////////////////
// WME events
typedef enum
{
	WME_EVENT_UPDATE=0,
	WME_EVENT_SCENE_DRAW_BEGIN,
	WME_EVENT_SCENE_DRAW_END,
	WME_EVENT_SCENE_INIT,
	WME_EVENT_SCENE_SHUTDOWN,
	WME_EVENT_GAME_BEFORE_SAVE,
	WME_EVENT_GAME_AFTER_LOAD,
	WME_EVENT_MAX
}
EWmeEvent;


//////////////////////////////////////////////////////////////////////////
// interface class for all WME objects
class IWmeObject
{
public:
	virtual bool SendEvent(const char* EventName) = 0;
	
	virtual IWmeValue* CallMethod(const char* MethodName, IWmeParamSet* Params=NULL) = 0;
	
	virtual bool SetProperty(const char* PropName, IWmeValue* Value) = 0;
	virtual bool SetProperty(const char* PropName, int Value) = 0;
	virtual bool SetProperty(const char* PropName, const char* Value) = 0;
	virtual bool SetProperty(const char* PropName, double Value) = 0;
	virtual bool SetProperty(const char* PropName, bool Value) = 0;
	virtual bool SetProperty(const char* PropName, IWmeObject* Value) = 0;
	virtual bool SetProperty(const char* PropName) = 0;
	
	virtual IWmeValue* GetProperty(const char* PropName) = 0;

	virtual void* GetInterface(const char* ClassName) = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class for acessing WME game object
class IWmeGame : public IWmeObject
{
public:
	virtual IWmeValue* CreateValue() = 0;
	virtual bool DeleteValue(IWmeValue* Value) = 0;

	virtual IWmeParamSet* CreateParamSet() = 0;
	virtual bool DeleteParamSet(IWmeParamSet* ParamSet) = 0;

	virtual bool SubscribeEvent(IWmeObject* Object, EWmeEvent Event) = 0;
	virtual bool UnsubscribeEvent(IWmeObject* Object, EWmeEvent Event) = 0;

	virtual IWmeFile* OpenFile(const char* Filename) = 0;
	virtual bool CloseFile(IWmeFile* File) = 0;

	virtual IWmeObject* CreateObject(const char* ClassName, IWmeParamSet* Params=NULL) = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class for WME variant script values
class IWmeValue
{
public:
	// getters
	virtual int GetValInt() = 0;
	virtual const char* GetValString() = 0;
	virtual double GetValFloat() = 0;
	virtual bool GetValBool() = 0;
	virtual IWmeObject* GetValNative() = 0;

	// setters
	virtual bool SetVal(int Value) = 0;
	virtual bool SetVal(const char* Value) = 0;
	virtual bool SetVal(double Value) = 0;
	virtual bool SetVal(bool Value) = 0;
	virtual bool SetVal(IWmeObject* Value) = 0;
	virtual bool SetVal(IWmeValue* Value) = 0;
	virtual bool SetVal() = 0;

	// type
	virtual bool IsValInt() = 0;
	virtual bool IsValString() = 0;
	virtual bool IsValFloat() = 0;
	virtual bool IsValBool() = 0;
	virtual bool IsValNative() = 0;
	virtual bool IsValObject() = 0;
	virtual bool IsValNull() = 0;

	// properties
	virtual IWmeValue* GetProperty(const char* PropName) = 0;
	virtual bool SetProperty(const char* PropName, int Value) = 0;
	virtual bool SetProperty(const char* PropName, const char* Value) = 0;
	virtual bool SetProperty(const char* PropName, double Value) = 0;
	virtual bool SetProperty(const char* PropName, bool Value) = 0;
	virtual bool SetProperty(const char* PropName, IWmeObject* Value) = 0;
	virtual bool SetProperty(const char* PropName, IWmeValue* Value) = 0;
	virtual bool SetProperty(const char* PropName) = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class to simplify passing parameters to WME script methods
class IWmeParamSet
{
public:
	virtual bool Clean() = 0;

	virtual bool AddParam(int Value) = 0;
	virtual bool AddParam(const char* Value) = 0;
	virtual bool AddParam(double Value) = 0;
	virtual bool AddParam(bool Value) = 0;
	virtual bool AddParam(IWmeObject* Value) = 0;
	virtual bool AddParam(IWmeValue* Value) = 0;
	virtual bool AddParam() = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class for accessing WME stacks
class IWmeStack
{
public:
	virtual IWmeValue* PopValue() = 0;
	
	virtual bool PushValue(int Value) = 0;
	virtual bool PushValue(const char* Value) = 0;
	virtual bool PushValue(double Value) = 0;
	virtual bool PushValue(bool Value) = 0;
	virtual bool PushValue(IWmeObject* Value) = 0;
	virtual bool PushValue(IWmeValue* Value) = 0;
	virtual bool PushValue() = 0;

	virtual bool ExpectParams(int NumParams) = 0;
};


//////////////////////////////////////////////////////////////////////////
// interface class for WME serialization manager
class IWmeSerialMgr
{
public:
	virtual bool IsSaving() = 0;

	virtual bool GetBuffer(unsigned char* Buffer, int Size) = 0;
	virtual bool PutBuffer(unsigned char* Buffer, int Size) = 0;

	virtual bool TransferValue(IWmeGame** Value) = 0;
	virtual bool TransferValue(IWmeObject** Value) = 0;
	virtual bool TransferValue(IWmeSubFrame** Value) = 0;
	virtual bool TransferValue(int* Value) = 0;
	virtual bool TransferValue(unsigned char* Value) = 0;
	virtual bool TransferValue(float* Value) = 0;
	virtual bool TransferValue(double* Value) = 0;
	virtual bool TransferValue(bool* Value) = 0;	
};

//////////////////////////////////////////////////////////////////////////
// interface class for WME script objects
class IWmeScript
{
public:
	virtual const char* GetFilename() = 0;
	virtual int GetCurrentLine() = 0;
	virtual void ReportRuntimeError(const char* Description) = 0;
	
	virtual IWmeValue* GetVariable(const char* VarName) = 0;
	virtual bool SetVariable(const char* VarName, int Value) = 0;
	virtual bool SetVariable(const char* VarName, const char* Value) = 0;
	virtual bool SetVariable(const char* VarName, double Value) = 0;
	virtual bool SetVariable(const char* VarName, bool Value) = 0;
	virtual bool SetVariable(const char* VarName, IWmeObject* Value) = 0;
	virtual bool SetVariable(const char* VarName, IWmeValue* Value) = 0;
	virtual bool SetVariable(const char* VarName) = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class for WME file-system file objects
class IWmeFile
{
public:
	virtual unsigned long ReadData(unsigned char* Buffer, unsigned long NumBytes) = 0;
	virtual unsigned long GetFileSize() = 0;
	virtual unsigned long GetFilePosition() = 0;
	virtual bool SeekToPosition(unsigned long Position) = 0;
};

//////////////////////////////////////////////////////////////////////////
// interface class for sub-frame objects, allowing operations on pixel level
class IWmeSubFrame : public IWmeObject
{
public:
	virtual bool StartPixelOperations() = 0;
	virtual bool EndPixelOperations() = 0;
	virtual unsigned long GetPixel(int X, int Y) = 0;
	virtual bool PutPixel(int X, int Y, unsigned long Pixel) = 0;

	virtual int GetWidth() = 0;
	virtual int GetHeight() = 0;
};

//////////////////////////////////////////////////////////////////////////
// exported function prototypes
typedef unsigned long (*PLUGIN_OBJ_CONSTRUCT) (IWmeObject* Owner, IWmeGame* Game, IWmeScript* Script, IWmeStack* Stack);
typedef unsigned long (*PLUGIN_OBJ_CONSTRUCT_EMPTY) (IWmeObject* Owner, IWmeGame* Game);
typedef void (*PLUGIN_OBJ_DESTRUCT) (unsigned long ID);
typedef bool (*PLUGIN_OBJ_CALL_METHOD) (unsigned long ID, const char* MethodName, IWmeScript* Script, IWmeStack* Stack);
typedef bool (*PLUGIN_OBJ_SET_PROPERTY) (unsigned long ID, const char* PropName, IWmeValue* Value);
typedef bool (*PLUGIN_OBJ_GET_PROPERTY) (unsigned long ID, const char* PropName, IWmeValue* Value);
typedef bool (*PLUGIN_OBJ_SERIALIZE) (unsigned long ID, IWmeSerialMgr* SerialMgr);
typedef bool (*PLUGIN_OBJ_RECEIVE_EVENT) (unsigned long ID, EWmeEvent EventID, void* EventData1, void* EventData2);



//////////////////////////////////////////////////////////////////////////
// plugin information struct
struct SWmePluginInfo
{
	char Description[128];
	char WmeVersion[20];
	char PluginVersion[20];
};

//////////////////////////////////////////////////////////////////////////
// class definition struct
struct SWmeClassDef
{
	PLUGIN_OBJ_CONSTRUCT Construct;
	PLUGIN_OBJ_CONSTRUCT_EMPTY ConstructEmpty;
	PLUGIN_OBJ_DESTRUCT Destruct;
	PLUGIN_OBJ_CALL_METHOD CallMethod;
	PLUGIN_OBJ_SET_PROPERTY SetProperty;
	PLUGIN_OBJ_GET_PROPERTY GetProperty;
	PLUGIN_OBJ_SERIALIZE Serialize;
	PLUGIN_OBJ_RECEIVE_EVENT ReceiveEvent;
};

//////////////////////////////////////////////////////////////////////////
// mandatory DLL exports
typedef void (*PLUGIN_GETPLUGININFO) (SWmePluginInfo* ClassDef);
typedef const char* (*PLUGIN_GETSUPPORTEDCLASSES) ();
typedef void (*PLUGIN_GETCLASSDEFINITION) (const char* ClassName, SWmeClassDef* ClassDef);



#endif // WME_PLUGIN_H
