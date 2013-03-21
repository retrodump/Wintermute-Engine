@echo off

rem *** Make sure Bison can find these files
copy bin\bison.simple . >nul
copy bin\bison.hairy  . >nul

rem *** Run Bison to generate the parser
rem ..\bison --defines --verbose -o parse.cpp lang.y
bin\bison --defines --verbose -o parse.c lang.y

rem *** Put the files back
del bison.simple  >nul
del bison.hairy  >nul

rem *** Rename parse.cpp.h to lexsymb.h
if exist lexsymb.h del lexsymb.h
rem ren parse.c.h lexsymb.h

:End
