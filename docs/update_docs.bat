@echo off
echo ================================
echo LIMPIANDO CARPETAS ANTIGUAS
echo ================================

if exist ".\doxygen" (
    echo Borrando carpeta: doxygen...
    rmdir /s /q "doxygen"
)

if exist ".\architecture\Code" (
    echo Borrando carpeta: architecture\Code...
    rmdir /s /q ".\architecture\Code"
)

if exist xml rmdir /s /q xml
if exist docs_output rmdir /s /q docs_output

echo.
echo ================================
echo GENERANDO XML CON DOXYGEN
echo ================================
doxygen Doxyfile

echo.
echo ================================
echo CONVIRTIENDO A MARKDOWN
echo ================================
mkdir ".\architecture\Code"
doxybook2.exe --input .\doxygen\xml --output ".\architecture\Code" --config doxybook_config.json

echo.
echo ================================
echo COPIANDO CONTENIDO A QUARTZ
echo ================================
rd /s /q ".\quartz\content"
mkdir ".\quartz\content"
xcopy ".\architecture" ".\quartz\content" /E /I /Y

echo.
echo ================================
echo BUILD DE QUARTZ
echo ================================
cd quartz
call npx quartz build
cd ..

echo.
echo ================================
echo COPIANDO PUBLIC A PUBLIC
echo ================================

if exist ".\public" (
    rmdir /s /q ".\public"
)

mkdir ".\public"
xcopy ".\quartz\public" ".\public" /E /I /Y

echo Creando .nojekyll...
type nul > ".\public\.nojekyll"

echo.
echo ================================
echo BUILD COMPLETADO
echo ================================
pause