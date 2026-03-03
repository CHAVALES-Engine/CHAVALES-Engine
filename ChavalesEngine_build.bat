@echo off
SETLOCAL

echo --------------------
echo Compilando ChavalesEngine
echo --------------------


set "ROOT_DIR=%~dp0"
set "SOLUTION_FILE=%ROOT_DIR%ChavalesEngine.sln"
set "PLATFORM=x64"

msbuild "%SOLUTION_FILE%" /p:Configuration=Debug /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo compilando Debug
    pause
    exit /b %errorlevel%
)

echo ------------------------
echo Compilando Release...
msbuild "%SOLUTION_FILE%" /p:Configuration=Release /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo compilando Release
    pause
    exit /b %errorlevel%
)


echo --------------------
echo Motor Compilado
echo --------------------


pause
ENDLOCAL
exit /b 0