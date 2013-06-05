@echo off & setLocal EnableDelayedExpansion
If %1a==a goto Usage
set starttime=%time%
call :ExtractHMSHn %starttime% StartTimeInNum
call %*
set endtime=%time%
call :ExtractHMSHn %endtime% EndTimeInNum
:find the Difference in time
set /a diff = %EndTimeInNum% - %StartTimeInNum%
Set /a Elapsedhrs= %diff% / (60 * 60 * 100)
Set /a Elapsedmin= (%diff% - (%Elapsedhrs% * 60*60*100)) / 6000
set /a elapsedsectmp= %diff% - (%Elapsedhrs% * 60*60*100 ) %% 6000
set /a elapsedsec= %elapsedsectmp% / 100
set /a elapsedhun= %elapsedsectmp% %% 100
Echo Elapsed time: %Elapsedhrs%:%Elapsedmin%:%elapsedsec%.%elapsedhun%
goto :EOF
:Usage
Echo Usage: %0 YourCommand [Your command's parameters]
Echo Example: %0 ping -n 2 127.0.0.1
echo - will execute "ping -n 2 127.0.0.1" and tells how long it for execution.
goto :EOF
:ExtractHMSHn
set t1=%1
set hun=%t1:~9,2%
set sec=%t1:~6,2%
set min=%t1:~3,2%
set hr=%t1:~0,2%
:Remove zero from the begining so that it does not crib about invalid octal
if %hun:~0,1%==0 set hun=%hun:~1,1%
if %sec:~0,1%==0 set sec=%sec:~1,1%
if %min:~0,1%==0 set min=%min:~1,1%
if %hr:~0,1%==0 set hr=%hr:~1,1%
set /a %2 = %hun% %% 100 + %sec% * 100 + %min% * 60*100 + %hr% * 60*60*100
exit /b
endlocal
