@echo off
SETLOCAL EnableDelayedExpansion

echo ------------------
echo Compilando LUA
echo ------------------


set "ROOTDIR=%~dp0"
set "SLNDIR=%ROOTDIR%sln"
set "LIBSDIR=%ROOTDIR%libs"
set "DEBUGDIR=%LIBSDIR%\Debug"
set "RELEASEDIR=%LIBSDIR%\Release"
set "PLATFORM=x64"

if not exist "%LIBSDIR%" mkdir "%LIBSDIR%"
if not exist "%DEBUGDIR%" mkdir "%DEBUGDIR%"
if not exist "%RELEASEDIR%" mkdir "%RELEASEDIR%"

pushd "%SLNDIR%"

echo Compilando DEBUG
msbuild LuaBuild.sln /p:Configuration=Debug /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Debug
    pause
    exit /b %errorlevel%
)

echo Compilando RELEASE
msbuild LuaBuild.sln /p:Configuration=Release /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Release
    pause
    exit /b %errorlevel%
)

popd


del /q "%DEBUGDIR%\*" 2>nul
del /q "%RELEASEDIR%\*" 2>nul


if exist "%SLNDIR%\x64\Debug" (
    xcopy "%SLNDIR%\x64\Debug\*" "%DEBUGDIR%\" /E /Y >nul
)

if exist "%SLNDIR%\x64\Release" (
    xcopy "%SLNDIR%\x64\Release\*" "%RELEASEDIR%\" /E /Y >nul
)

echo -----------------------------
echo LUA compilado correctamente
echo -----------------------------
exit /b 0
