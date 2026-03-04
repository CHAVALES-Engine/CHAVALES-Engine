@echo off
SETLOCAL EnableDelayedExpansion
echo ------------------
echo Compilando PhysX
echo ------------------

set "ROOTDIR=%~dp0"
set "PHYSXDIR=%ROOTDIR%physx"
set "SLN_DIR=%PHYSXDIR%\compiler\vc17win64-cpu-only"
set "SOLUTION_FILE=%SLN_DIR%\PhysXSDK.sln"
set "BUILDDIR=%SLN_DIR%\bin\win.x86_64.vc143.mt"
set "LIBSDIR=%ROOTDIR%\libs"
set "DEBUGDIR=%LIBSDIR%\Debug"
set "RELEASEDIR=%LIBSDIR%\Release"
set "PLATFORM=x64"



if not exist "%LIBSDIR%" mkdir "%LIBSDIR%"
if not exist "%DEBUGDIR%" mkdir "%DEBUGDIR%"
if not exist "%RELEASEDIR%" mkdir "%RELEASEDIR%"


pushd "%PHYSXDIR%"
call generate_projects.bat vc17win64-cpu-only
popd


msbuild "%SOLUTION_FILE%" /p:Configuration=Debug /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo compilando Debug
    pause
    exit /b %errorlevel%
)


msbuild "%SOLUTION_FILE%" /p:Configuration=Release /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo compilando Release
    pause
    exit /b %errorlevel%
)


del /q "%DEBUGDIR%\*.lib" 2>nul
del /q "%RELEASEDIR%\*.lib" 2>nul


for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\debug\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%DEBUGDIR%\" >nul
    )
)

:: Copiar Release
for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\release\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%RELEASEDIR%\" >nul
    )
)

echo -----------------------------
echo PHYSX compilado correctamente
echo -----------------------------
exit /b 0
