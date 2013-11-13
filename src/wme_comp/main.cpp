#include <io.h>
#include "dcgf_ad.h"

#if _DEBUG
	#pragma comment(lib, "wme_ad_d.lib")
#else
	#pragma comment(lib, "wme_ad.lib")
#endif


void __stdcall ErrorCallback(int Line, char *Text, void *Data);
void FindProject(char* ScriptFile, char* ProjectFile);
void FindWprFile(char* TestPath, char* ProjectFile);

typedef enum FormatType
{
	FORMAT_DEFAULT, FORMAT_SCITE
};

FormatType g_OutputFormat;

////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[ ])
{
	
	char project_filename[MAX_PATH] = "";
	char project_path[MAX_PATH] = "";
	char script_filename[MAX_PATH] = "";
	char script_short[MAX_PATH] = "";
	char output_path[MAX_PATH] = "";
	char format[MAX_PATH] = "";
	int i;

	// parse params
	for(i=1; i<argc-1; i++){
		if(stricmp(argv[i], "-project")==0 && argv[i+1][0]!='-')
			strcpy(project_filename, argv[i+1]);
		
		if(stricmp(argv[i], "-script")==0 && argv[i+1][0]!='-')
			strcpy(script_filename, argv[i+1]);

		if(stricmp(argv[i], "-output")==0 && argv[i+1][0]!='-')
			strcpy(output_path, argv[i+1]);

		if(stricmp(argv[i], "-format")==0 && argv[i+1][0]!='-')
			strcpy(format, argv[i+1]);
	}

	// write help
	if(script_filename[0]=='\0')
	{
		printf("Wintermute Engine Script Compiler %d.%d.%03d, (c) 2013 Dead:Code\n\n", DCGF_VER_MAJOR, DCGF_VER_MINOR, DCGF_VER_BUILD);
		printf("Usage:\n\n  wme_comp.exe -script script.dcs [-project project.wpr] [-output output_path] [-format default|scite]\n\n");
		return 0;
	}
	
	if(project_filename[0]=='\0')
	{
		FindProject(script_filename, project_filename);
		if(project_filename[0]=='\0')
		{
			printf("** Error ** No project found.\n\n");
			return 0;
		}
	}

	if(stricmp(format, "scite")==0) g_OutputFormat = FORMAT_SCITE;
	else g_OutputFormat = FORMAT_DEFAULT;


	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];
	_splitpath(project_filename, drive, path, NULL, NULL);
	sprintf(project_path, "%s%s", drive, path);

	_splitpath(script_filename, NULL, NULL, fname, ext);
	sprintf(script_short, "%s%s", fname, ext);


	CAdGame* Game = new CAdGame;
	if(!Game || FAILED(Game->Initialize1())){
		printf("** Error **  initializing the engine\n");
		SAFE_DELETE(Game);
		return 1;
	}

	if(!Game->m_ScEngine->m_CompilerAvailable){
		printf("** Error **  script compiler DLL was not found\n");
		SAFE_DELETE(Game);
		return 1;
	}
	
	Game->m_Registry->SetIniName(project_filename);
	Game->m_FileManager->SetBasePath(project_path);
	Game->m_ScEngine->SetCompileErrorCallback(ErrorCallback, script_short);
	
	DWORD CompSize = 0;
	BYTE* CompBuffer = Game->m_ScEngine->GetCompiledScript(script_filename, &CompSize, true);

	if(output_path[0]!=0 && CompBuffer!=NULL)
	{
		if(output_path[strlen(output_path)-1]!='\\') strcat(output_path, "\\");
		strcat(output_path, script_short);
		FILE* f = fopen(output_path, "wb");
		if(!f)
		{
			printf("Error writing to file '%s'\n", output_path);
		}
		else
		{
			fwrite(CompBuffer, CompSize, 1, f);
			fclose(f);
		}
	}
	
	Game->m_ScEngine->EmptyScriptCache();
	SAFE_DELETE(Game);

	return 0;
}


//////////////////////////////////////////////////////////////////////////
void __stdcall ErrorCallback(int Line, char *Text, void *Data)
{
	switch(g_OutputFormat)
	{
		case FORMAT_DEFAULT:
			printf("%s,%d: %s\n", (char*)Data, Line, Text);
			break;

		case FORMAT_SCITE:
			printf("%s(%d) : %s\n", (char*)Data, Line, Text);
			break;
	}	
}

//////////////////////////////////////////////////////////////////////////
void FindProject(char* ScriptFile, char* ProjectFile)
{
	char TestPath[MAX_PATH];
	
	char drive[_MAX_DRIVE];
	char path[_MAX_PATH];
	_splitpath(ScriptFile, drive, path, NULL, NULL);
	sprintf(TestPath, "%s%s", drive, path);

	for(int i = strlen(TestPath) - 1; i >= 0; i--)
	{
		if(TestPath[i]=='/' || TestPath[i]=='\\')
		{
			TestPath[i+1] = '\0';
			ProjectFile[0] = '\0';
			FindWprFile(TestPath, ProjectFile);
			if(ProjectFile[0] != '\0') return;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
void FindWprFile(char* TestPath, char* ProjectFile)
{
	char Mask[MAX_PATH];
	sprintf(Mask, "%s*.wpr", TestPath);

	// find exe
	struct _finddata_t find_file;
	long hFile;

	if((hFile = _findfirst(Mask, &find_file)) != -1L)
	{
		if(!(find_file.attrib & _A_SUBDIR))
		{
			sprintf(ProjectFile, "%s%s", TestPath, find_file.name);
			_findclose(hFile);
			return;
		}

		while(_findnext(hFile, &find_file) == 0)
		{
			if(!(find_file.attrib & _A_SUBDIR))
			{
				sprintf(ProjectFile, "%s%s", TestPath, find_file.name);
				_findclose(hFile);
				return;
			}
		}
		_findclose(hFile);
	}
}
