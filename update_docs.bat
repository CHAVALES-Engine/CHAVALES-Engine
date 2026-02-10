@echo off
echo Limpiando carpetas antiguas...
if exist xml rmdir /s /q xml
if exist docs_output rmdir /s /q docs_output

echo Generando XML con Doxygen...
doxygen Doxyfile

echo Convirtiendo a Markdown...
:: Creamos la carpeta por si acaso
mkdir ".\docs\ArquitecturaMotorDeVideojuegos\Code"
doxybook2.exe --input .\doxygen\xml --output ".\docs\ArquitecturaMotorDeVideojuegos\Code" --config doxybook_config.json

echo Pasando contenido a Quartz...
rd /s /q ".\docs\quartz\content"
mkdir ".\docs\quartz\content"
xcopy ".\docs\ArquitecturaMotorDeVideojuegos" ".\docs\quartz\content" /E /I /Y

pause