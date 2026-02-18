@echo off
echo Limpiando carpetas antiguas...
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

echo Generando XML con Doxygen...
doxygen Doxyfile

echo Convirtiendo a Markdown...
:: Creamos la carpeta por si acaso
mkdir ".\architecture\Code"
doxybook2.exe --input .\doxygen\xml --output ".\architecture\Code" --config doxybook_config.json

echo Pasando contenido a Quartz...
rd /s /q ".\quartz\content"
mkdir ".\quartz\content"
xcopy ".\architecture" ".\quartz\content" /E /I /Y

pause