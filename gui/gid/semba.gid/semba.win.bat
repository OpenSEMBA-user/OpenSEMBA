rem OutputFile: %1.log
rem Errorfile: %1.err

set SEMBA = %3\bin\semba.exe
echo Path to command: %SEMBA% > %1.log
echo Project name: %1 >> %1.log
echo Project path: %2 >> %1.log
echo Problem type path: %3 >> %1.log

set SEMBA_ARGS1 = -i
set SEMBA_ARGS2 = %2\%1.dat
echo %SEMBA% %SEMBA_ARGS1% %SEMBA_ARGS2% >> %1.log
call %SEMBA% %SEMBA_ARGS1% %SEMBA_ARGS2% >> %1.log 2> %1.err
