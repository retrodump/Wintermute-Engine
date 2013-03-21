@echo off

echo Cleanup...
pause


del /Q .\Install\Output\*.*

attrib -R *.* /S


del /Q /S *.ncb
del /Q /S *.pch
del /Q /S *.dcp
del /Q /S *.obj
del /Q /S *.pdb
del /Q /S *.ilk
del /Q /S *.idb
del /Q /S *.svn-base
del /Q /S *.svn


del lib\wme_ad.lib
del lib\wme_ad_d.lib
del lib\wme_base.lib
del lib\wme_base_d.lib 
del doc\html\wme.chm 
