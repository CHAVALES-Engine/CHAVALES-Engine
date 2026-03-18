@echo off
SETLOCAL EnableDelayedExpansion

echo ------------------
echo Compilando SDL3
echo ------------------

:: ------------------------
:: RUTAS
:: ------------------------
set "ROOTDIR=%~dp0"
set "SRCDIR=%ROOTDIR%src\SDL3"
set "BUILDDIR=%ROOTDIR%build"
set "LIBSDIR=%ROOTDIR%libs"
set "DEBUGDIR=%LIBSDIR%\Debug"
set "RELEASEDIR=%LIBSDIR%\Release"
set "PLATFORM=x64"

if not exist "%BUILDDIR%" mkdir "%BUILDDIR%"
if not exist "%LIBSDIR%" mkdir "%LIBSDIR%"
if not exist "%DEBUGDIR%" mkdir "%DEBUGDIR%"
if not exist "%RELEASEDIR%" mkdir "%RELEASEDIR%"

pushd "%BUILDDIR%"

echo Configurando SDL3...

cmake -G "Visual Studio 17 2022" -A %PLATFORM% ^
 -DSDL_AUDIO=OFF ^
 -DSDL_CAMERA=OFF ^
 -DSDL_GPU=ON ^
 -DSDL_HAPTIC=ON ^
 -DSDL_HIDAPI=ON ^
 -DSDL_JOYSTICK=ON ^
 -DSDL_POWER=ON ^
 -DSDL_SENSOR=ON ^
 -DSDL_VIDEO=ON ^
 -DSDL_OPENGL=ON ^
 -DSDL_OPENGLES=ON ^
 -DSDL_VULKAN=OFF ^
 -DSDL_EXAMPLES=OFF ^
 -DSDL_SHARED=OFF ^
 -DSDL_STATIC=ON ^
 -DSDL_TESTS=OFF ^
 "%SRCDIR%"

if %errorlevel% neq 0 (
    echo ERROR: Fallo en configuracion SDL3
    pause
    exit /b %errorlevel%
)

echo Compilando DEBUG...
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Debug SDL3
    pause
    exit /b %errorlevel%
)

echo Compilando RELEASE...
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Release SDL3
    pause
    exit /b %errorlevel%
)

popd


del /q "%DEBUGDIR%\*.lib" 2>nul
del /q "%RELEASEDIR%\*.lib" 2>nul

for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\Debug\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%DEBUGDIR%\" >nul
    )
)

for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\Release\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%RELEASEDIR%\" >nul
    )
)

echo -----------------------------
echo SDL3 compilado correctamente
echo -----------------------------

exit /b 0
