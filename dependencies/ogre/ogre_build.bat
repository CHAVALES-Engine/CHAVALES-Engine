@echo off
SETLOCAL EnableDelayedExpansion
echo ------------------
echo Compilando OGRE
echo ------------------

:: ------------------------
:: RUTAS
:: ------------------------
set "ROOTDIR=%~dp0"
set "SRCDIR=%ROOTDIR%src\ogre"
set "BUILDDIR=%ROOTDIR%build"
set "LIBSDIR=%ROOTDIR%libs"
set "DEBUGDIR=%LIBSDIR%\Debug"
set "RELEASESDIR=%LIBSDIR%\Release"
set "PLATFORM=x64"
set "ASSIMPDIR=%BUILDDIR%\assimp-6.0.3"
set "FREETYPEDIR=%BUILDDIR%\freetype-2.14.1\objs"
:: Crear carpetas 
if not exist "%BUILDDIR%" mkdir "%BUILDDIR%"
if not exist "%LIBSDIR%" mkdir "%LIBSDIR%"
if not exist "%DEBUGDIR%" mkdir "%DEBUGDIR%"
if not exist "%RELEASESDIR%" mkdir "%RELEASESDIR%"

pushd "%BUILDDIR%"

echo Configurando OGRE

cmake -G "Visual Studio 17 2022" -A %PLATFORM% ^
 -DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL ^
 -DOGRE_BITES_NATIVE_DIALOG=OFF ^
 -DOGRE_BUILD_COMPONENT_BITES=OFF ^
 -DOGRE_BUILD_COMPONENT_CSHARP=OFF ^
 -DOGRE_BUILD_COMPONENT_JAVA=OFF ^
 -DOGRE_BUILD_COMPONENT_MESHLODGENERATOR=OFF ^
 -DOGRE_BUILD_COMPONENT_OVERLAY=ON ^
 -DOGRE_BUILD_COMPONENT_OVERLAY_IMGUI=ON ^
 -DOGRE_BUILD_COMPONENT_PROPERTY=OFF ^
 -DOGRE_BUILD_COMPONENT_PYTHON=OFF ^
 -DOGRE_BUILD_COMPONENT_RTSHADERSYSTEM=ON ^
 -DOGRE_BUILD_COMPONENT_TERRAIN=OFF ^
 -DOGRE_BUILD_COMPONENT_VOLUME=OFF ^
 -DOGRE_BUILD_COMPONENT_BULLET=OFF ^
 -DOGRE_BUILD_DEPENDENCIES=ON ^
 -DOGRE_BUILD_MSVC_MP=ON ^
 -DOGRE_BUILD_MSVC_ZM=ON ^
 -DOGRE_BUILD_PLUGIN_BSP=OFF ^
 -DOGRE_BUILD_PLUGIN_GLSLANG=OFF ^
 -DOGRE_BUILD_PLUGIN_OCTREE=OFF ^
 -DOGRE_BUILD_PLUGIN_PCZ=OFF ^
 -DOGRE_BUILD_PLUGIN_PFX=ON ^
 -DOGRE_BUILD_PLUGIN_RSIMAGE=OFF ^
 -DOGRE_BUILD_PLUGIN_STBI=ON ^
 -DOGRE_BUILD_RENDERSYSTEM_D3D9=OFF ^
 -DOGRE_BUILD_RENDERSYSTEM_D3D11=OFF ^
 -DOGRE_BUILD_RENDERSYSTEM_GL=OFF ^
 -DOGRE_BUILD_RENDERSYSTEM_GL3PLUS=ON ^
 -DOGRE_BUILD_RENDERSYSTEM_GLES2=ON ^
 -DOGRE_BUILD_RENDERSYSTEM_TINY=OFF ^
 -DOGRE_BUILD_RENDERSYSTEM_VULKAN=OFF ^
 -DOGRE_BUILD_TOOLS=OFF ^
 -DOGRE_CONFIG_ENABLE_QUAD_BUFFER_STEREO=OFF ^
 -DOGRE_CONFIG_ENABLE_ASSIMP=ON ^
 -DOGRE_CONFIG_ENABLE_FREETYPE=ON ^
 -DOGRE_CONFIG_FILESYSTEM_UNICODE=ON ^
 -DOGRE_COPY_DEPENDENCIES=ON ^
 -DOGRE_ENABLE_PRECOMPILED_HEADERS=OFF ^
 -DOGRE_INSTALL_DEPENDENCIES=OFF ^
 -DOGRE_INSTALL_DOCS=OFF ^
 -DOGRE_INSTALL_PDB=OFF ^
 -DOGRE_INSTALL_TOOLS=OFF ^
 -DOGRE_INSTALL_VSPROPS=ON ^
 -DOGRE_NODEDEFAULT_POSITIONING=OFF ^
 -DOGRE_PROJECT_FOLDERS=ON ^
 -DOGRE_STATIC=ON ^
 -DOGRE_BUILD_SHARED_LIBS=OFF ^
 "%SRCDIR%"


if %errorlevel% neq 0 (
    echo ERROR: Fallo en configuracion Ogre
    pause
    exit /b %errorlevel%
)

echo Compilando DEBUG
cmake --build . --config Debug
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Debug
    pause
    exit /b %errorlevel%
)


echo Compilando RELEASE
cmake --build . --config Release
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Release
    pause
    exit /b %errorlevel%
)

pushd "%ASSIMPDIR%"
echo Compilando ASSIMP
echo Compilando DEBUG
msbuild Assimp.sln /p:Configuration=Debug /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Debug
    pause
    exit /b %errorlevel%
)
msbuild Assimp.sln /p:Configuration=Release /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Release
    pause
    exit /b %errorlevel%
)

popd
popd

pushd "%FREETYPEDIR%"
echo Compilando FRETYPE
echo Compilando DEBUG
msbuild freetype.sln /p:Configuration=Debug /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Debug
    pause
    exit /b %errorlevel%
)
msbuild freetype.sln /p:Configuration=Release /p:Platform=%PLATFORM%
if %errorlevel% neq 0 (
    echo ERROR: Fallo en build Release
    pause
    exit /b %errorlevel%
)
popd

del /q "%DEBUGDIR%\*.lib" 2>nul
del /q "%RELEASESDIR%\*.lib" 2>nul

echo Copiando librerías Ogre
for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\Debug\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%DEBUGDIR%\" >nul
    )
)


for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\Release\\" >nul
    if !errorlevel! equ 0 (
        copy "%%f" "%RELEASESDIR%\" >nul
    )
)


for /r "%BUILDDIR%" %%f in (*.lib) do (
    echo "%%f" | findstr /i "\\Debug\\" >nul
    if !errorlevel! neq 0 (
        echo "%%f" | findstr /i "\\Release\\" >nul
        if !errorlevel! neq 0 (
            copy "%%f" "%DEBUGDIR%\" >nul
            copy "%%f" "%RELEASESDIR%\" >nul
        )
    )
)


echo -----------------------------
echo OGRE compilado correctamente
echo ---------------------------------
exit /b 0