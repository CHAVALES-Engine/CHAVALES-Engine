@echo off
SETLOCAL

echo ------------------
echo Compilando Dependencias
echo ------------------

:: Carpeta  dependencias
set "DEPS_ROOT=%~dp0"
if exist "%DEPS_ROOT%fmod\fmod_build.bat" (
    echo ------------------------
    echo Compilando FMOD
    call "%DEPS_ROOT%fmod\fmod_build.bat" 
)
if exist "%DEPS_ROOT%lua\lua_build.bat" (
    echo ------------------------
    echo Compilando LUA
    call "%DEPS_ROOT%lua\lua_build.bat" 
)
if exist "%DEPS_ROOT%ogre\ogre_build.bat" (
    echo ------------------------
    echo Compilando OGRE
    call "%DEPS_ROOT%ogre\ogre_build.bat" 
)
if exist "%DEPS_ROOT%PhysX\physx_build.bat" (
    echo ------------------------
    echo Compilando PHYSX
    call "%DEPS_ROOT%PhysX\physx_build.bat" 
)
if exist "%DEPS_ROOT%SDL\sdl_build.bat" (
    echo ------------------------
    echo Compilando SDL
    call "%DEPS_ROOT%SDL\sdl_build.bat" 
)

echo --------------------------------------
echo   DEPENDENCIAS COMPILADAS
echo ----------------------------------------
pause
ENDLOCAL
exit /b 0