set VSCMD_START_DIR=%CD%
call "%VS140COMNTOOLS%VsDevCmd.bat"

cl tx2al.c
del *.obj
