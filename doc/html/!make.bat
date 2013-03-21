@echo off

"%ProgramFiles(x86)%\HTML Help Workshop\hhc.exe" wme.hhp
hhc2html wme.hhc web\contents.html ../