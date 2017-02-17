rmdir /S /Q %~dp0..\NDBPool\release
rmdir /S /Q %~dp0..\NDBPool\debug
rmdir /S /Q %~dp0..\NDBPool\build_
rmdir /S /Q %~dp0..\example\build_
rmdir /S /Q %~dp0..\example\release
rmdir /S /Q %~dp0..\example\debug
rmdir /S /Q %~dp0..\bin
del /Q %~dp0..\Makefile*
del /Q %~dp0..\NDBPool\Makefile*
del /Q %~dp0..\example\Makefile*
