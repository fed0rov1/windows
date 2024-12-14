@echo off

setlocal enabledelayedexpansion

:: ESC key
prompt $E_F
for /F "tokens=1 delims=_" %%i in ('"for %%v in (any) do rem"') do (
	set ESC=%%i
	prompt
)


:: colors
set RED=%ESC%[31m
set BLUE=%ESC%[34m
set GREEN=%ESC%[32m
set YELLOW=%ESC%[33m
SET MAGENTA=%ESC%[35m

set "SPACE_CONST=           "


set /A column2_len=20

:show	

	cls
	echo.
	echo %MAGENTA%TCP PORTS(CONNECTED)
	echo.

	set "space= "
	
	for /F "tokens=2,3,4,5" %%b in ('"netstat -o -n -p TCP | findstr /I /C:"ESTABLISHED""') do (

		set /A num=%%e
		
		if !num! GTR 0 if !num! LSS 10 (

			set /A space_num=!column2_len!-2

			for /L %%i in (1,1,!space_num!) do (
			
				set "space=!space! "
			)

		)
		
		
		
		if !num! GEQ 10 if !num! LSS 100 (

			set /A space_num=!column2_len!-3

			for /L %%i in (1,1,!space_num!) do (
				set "space=!space! "
			)

		)
		
		if !num! GEQ 100 if !num! LSS 1000 (
			
			set /A space_num=!column2_len!-4

			for /L %%i in (1,1,!space_num!) do (
				set "space=!space! "
			)
			
		)
		
		
		if !num! GEQ 1000 if !num! LSS 10000 (
			
			set /A space_num=!column2_len!-5

			for /L %%i in (1,1,!space_num!) do (
				set "space=!space! "
			)
			
		)
		
		if !num! GEQ 10000 if !num! LSS 100000 (
			
			set /A space_num=!column2_len!-6

			for /L %%i in (1,1,!space_num!) do (
				set "space=!space! "
			)
			
		)

		echo %SPACE_CONST%%BLUE%IP: %%b%SPACE_CONST%%YELLOW%PID: %%e!space!%RED%SERVER: %%c
		set "space= "
	
	)
		
	timeout /T 2 > nul
	cls

goto show

	










