setlocal EnableDelayedExpansion
set x=0
set pre=BM_WORSTMISTAKE
set combined=%pre%!x!

echo %str%

echo  > test.txt
for /r %%i in (*.bmp) do (
	set word=\\
	set "pathto=%%i"
	call set pathto=%%pathto:\=!word!%%
	call set /A x=!x!+1
	set combined=%pre%!x! 
	echo !combined! BITMAP "!pathto!" >> test.txt
)
set /A x=0
set /A y=104
for /r %%i in (*.bmp) do (
	call set /A x=!x!+1
	call set /A y=!y!+1
	set combined=%pre%!x! 
	echo #define !combined!!y! >> test.txt
)
set x=0
echo | set /p dummyName={ >> test.txt
for /r %%i in (*.bmp) do (
	call set /A x=!x!+1
	set combined=%pre%!x! 
	echo | set /p dummyName=!combined!, >> test.txt
)
echo | set /p dummyName=} >> test.txt