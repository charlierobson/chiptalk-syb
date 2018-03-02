@echo off
rem - construct the target filename using the name of this batch file itself.
rem - '%~n0' will yield the name of the batch file without its extension.
rem - this script assumes that zxsvr.exe is available in the path.
rem - change the com port to the one used by your computer
set PORT=COM4
set PNAME=%~n0.p
@zxsvr %PNAME% %PORT%