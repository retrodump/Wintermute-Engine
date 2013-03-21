@echo off
copy src\compiler\Release\dcscomp.dll install\source
rem copy src\ProjectMan\Release\ProjectMan.exe install\source
rem copy src\SceneEdit\Release\SceneEdit.exe install\source
copy src\SpriteEdit\Release\SpriteEdit.exe install\source
copy src\wme\Release\wme.exe install\source
copy src\wme_comp\Release\wme_comp.exe install\source
copy src\settings\Release\settings.exe install\source
copy src\plugin\wme_snow\Release\wme_snow.dll install\source
copy src\plugin\wme_sample_pixel\Release\wme_sample_pixel.dll install\source
rem copy src\external_lib\BugSlayer\release\BugslayerUtil.dll install\source
copy src\StringTableMgr\StringTableMgr\bin\Release\StringTableMgr.exe install\source
copy src\asm\Global\Global\bin\Release\DeadCode.WME.Global.dll install\source
copy src\asm\ControlsNew\ControlsNew\bin\Release\DeadCode.WME.Controls.dll install\source
copy src\asm\ScriptParser\ScriptParser\bin\Release\DeadCode.WME.ScriptParser.dll install\source
copy src\asm\WmeWrapper\release\DeadCode.WME.Core.dll install\source
copy src\asm\DocMaker\DocMaker\bin\Release\DeadCode.WME.DocMaker.dll install\source
copy src\Integrator\Integrator\bin\Release\Integrator.exe install\source
copy src\scite\Release\scite.exe install\source\SciTE
copy src\WindowEdit\WindowEdit\bin\Release\WindowEdit.exe install\source

xcopy src\ProjectMan\Debug\help\ProjectMan\*.* install\source\help\ProjectMan\*.* /E /Y

xcopy src\plugin\*.* install\source\plugin\ /E /Y
del /S /Q install\source\plugin\wme_sample\release
del /S /Q install\source\plugin\wme_sample\debug
del /S /Q install\source\plugin\wme_snow\release
del /S /Q install\source\plugin\wme_snow\debug
del /S /Q install\source\plugin\wme_sample_pixel\release
del /S /Q install\source\plugin\wme_sample_pixel\debug
del /S /Q install\source\plugin\*.ncb
del /S /Q "install\source\plugin\*.Jan Nedoma.user"

copy src\engine_core\wme_base\wme_plugin.h install\source\plugin\include

xcopy src\BuildAll\release\*.exe install\source /Y /D
xcopy src\BuildAll\release\*.dll install\source /Y /D
xcopy src\BuildAll\releaseD3D9\*.exe install\source /Y /D
xcopy src\BuildAll\releaseD3D9\*.dll install\source /Y /D


copy doc\html\wme.chm install\source
