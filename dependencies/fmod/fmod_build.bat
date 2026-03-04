@echo off
SETLOCAL EnableDelayedExpansion

echo ------------------
echo Copiando FMOD
echo ------------------


set "ROOTDIR=%~dp0"
set "DEPENDDIR=%ROOTDIR%.."
set "DLLDIR=%ROOTDIR%dll\x64"
set "BINDIR=%DEPENDDIR%\..\bin"

:: Crear carpetas si no existen
if not exist "%DLLDIR%" (
    echo ERROR: No existe la carpeta %DLLDIR%
    pause
    exit /b 1
)

if not exist "%BINDIR%" mkdir "%BINDIR%"

xcopy "%DLLDIR%\*.dll" "%BINDIR%\" /Y >nul

echo -----------------------------
echo FMOD copiado correctamente
echo -----------------------------

exit /b 0
