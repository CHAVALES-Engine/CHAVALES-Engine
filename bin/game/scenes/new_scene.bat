@echo off
setlocal enabledelayedexpansion

set NAME=Scene
set OUTPUT_DIR= game\scenes

:: Permitir pasar el nombre de la escena como argumento: new_scene.bat MyScene
if not "%~1"=="" set NAME=%~1
:: Permitir pasar la ruta de salida como argumento: new_scene.bat MyScene src\scenes
if not "%~2"=="" set OUTPUT_DIR=%~2

echo  Creando nueva escena: %NAME% en %OUTPUT_DIR%

: Verificar que existe OUTPUT_DIR
if not exist "%OUTPUT_DIR%" (
    echo  Creando ruta de salida: %OUTPUT_DIR%
    mkdir "%OUTPUT_DIR%"
    if errorlevel 1 (
        echo.
        echo  [ERROR] No se pudo crear la ruta de salida: %OUTPUT_DIR%
    )
    echo.
)

(
    echo -- scene1.lua
    echo -- Escena principal de ejemplo
    echo scene = { -- entidades en escena
	echo    -- ENTIDAD 1
	echo    entidad1 = { -- nombre de la entidad
	echo    	components = { -- componentes de esa entidad
	echo    		Transform = { -- componente transform
	echo    			-- posicion local
	echo    			position = Vector3.new^(1,2,3^),
	echo    			-- rotacion local
	echo    			rotation = Quaternion.new^(^),
	echo    			-- escala local
	echo    			scale = Vector3.new^(1^),
	echo    			children = {"entidad2"}
	echo    		},
    echo    	},
    echo    },
    echo    -- ENTIDAD 2
    echo    entidad2 = {
    echo    	components = {
    echo    		Transform = {
    echo    			position = Vector3.new^(0,1,0^),
    echo    			rotation = Quaternion.new^(^),
    echo    			scale = Vector3.new^(0.5^),
    echo    		},
    echo    	ModelRenderer = {
    echo               ["model folder"] = "cube.obj",
    echo               materialPath = "material.tres",
    echo               castShadows = true,
    echo               receiveShadows = true        
    echo    	    },
    echo    	},
    echo    },
    echo }

) > "%OUTPUT_DIR%\%NAME%.lua"
