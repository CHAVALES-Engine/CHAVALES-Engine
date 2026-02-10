### Descripción
GameLoader es la clase de C++ encargada de hacer un puente entre LUA y C++, lee metatablas de LUA que configuran escenas, objetos y componentes e instancia lo que va necesitando.

Idealmente, cuando lleguemos a la etapa de release, podríamos hacer un bakeado de los scripts de LUA a binario, para así acelerar procesos de lectura y ejecución.

### Ejemplo de implementación

LUA:
```LUA
Scene = { 
	entities = { 
		{
			Name = "Orc",
			Transform = { x = 0, y = 10 }, 
			Health = { max = 100 } 
		}
	}
}
```
(Posible implementación)

---
## LUA en la carga de escenas
### Prefabs
Usando LUA podemos buscar scripts que devuelvan información de prefabs, Los scripts definen una entidad con sus componentes y propiedades.

También se maneja una **caché de prefabs**, cargando aquellos que se usen una única vez, Cuando se lee una escena, si se encuentra una entrada con la palabra clave "prefab" se busca en la caché si existe una entrada correspondiente, si no se crea una entrada y se añade la configuración devuelta por LUA. 

```Lua
entities = {  
	{ prefab = "Orc", pos = {x=10, y=0} }, 
	{ prefab = "Orc", pos = {x=20, y=0} }, 
	{ prefab = "Chest", pos = {x=5, y=5}, loot = "Gold" } }
```
(Posible implementación)
```C++
void GameLoader::loadScene(std::string sceneFile) {
    sol::table sceneData = lua.script_file(sceneFile);
    sol::table entities = sceneData["entities"];

    for (auto& item : entities) {
        sol::table entry = item.second;
        
        Entity* newEntity = nullptr;

        // ¿Es un Prefab?
        if (entry["prefab"].valid()) {
            std::string prefabName = entry["prefab"];
            
            // Revisar Caché
            if (prefabCache.find(prefabName) == prefabCache.end()) {
                // CACHE MISS: Pedir a Lua la definición base y guardarla
                prefabCache[prefabName] = loadPrefabDefinitionFromLua(prefabName);
            }
            
            // Instanciar desde el molde de C++ (Muy rápido)
            newEntity = spawnFromBlueprint(prefabCache[prefabName]);
        } 
        else {
            // Es una entidad única definida ahí mismo
            newEntity = parseRawEntity(entry);
        }

        // Aplicar diferencias (Overrides)
        applyOverrides(newEntity, entry); // Ej: Poner la posición específica
    }
}
```
(Posible implementación)

Para controlar la creación de prefabs deberíamos elegir una de estas dos opciones para el desarrollo, o incluso soportar ambas:
1. Almacenar en LUA una **tabla maestra** de prefabs con nombres como "Orc" como entrada, así podemos acceder rápidamente a cada uno y coger su información.
2. En caso de que el proyecto crezca mucho, quizás sea conveniente tener cada prefab en un archivo, y utilizar el nombre para construir un path para llegar al archivo .lua.

---
## Usos de lua en runtime
### Eventos de escena
La idea es implementar eventos concretos a los que llamar cuando pasa algo, por ejemplo:

```Lua
-- Al iniciar el nivel 
function OnSceneStart() 
	print("El nivel ha comenzado") 
	MotorAPI.Audio:playMusic("battle_theme.mp3")
end

function onAllEnemiesDead():
	print("Se han muerto todos los enemigos")
	MotorAPI.Render:setSkyColor(Color.Blue)
end
```

Podemos implementar un sistema de suscripción en la escena, y desde componentes lanzar eventos que ejecuten funciones de escena, no se como de EC friendly es, pero es una idea de usos de LUA en ejecución.
### Consola de desarrollo para modificar parámetros
(Esto podría ayudarnos para, en caso de querer, hacer un editor mas adelante.)

La **Consola de Desarrollo** es una ventana interactiva que conecta el teclado del desarrollador directamente con la Máquina Virtual (VM) de LUA mientras el juego corre.

Técnicamente funciona como un bucle **REPL** (Read-Eval-Print Loop):
1. **Read:** Captura el texto que escribes.
2. **Eval:** Lo envía a LUA para que lo ejecute como código.
3. **Print:** Captura el resultado (o error) y lo muestra en pantalla.

La herramienta estándar de la industria es [Dear ImGui](https://www.dearimgui.com/). Es ligera, fácil de integrar y se renderiza sobre el juego.

Para implementarla, deberíamos implementar una clase **DeveloperConsole** que se inicialice al pulsar una tecla (por ejemplo "f1", "ctrl+c"...) capturar la entrada de teclado, y redirigir la salida de consola por esta nueva ventana.

```C++
class DeveloperConsole { 
private: 
	std::vector<std::string> logHistory; // Guarda lo que sale en pantalla char 
	inputBuffer[256]; // escrito
	bool isVisible = false; 
	sol::state* luaState; // Referencia a VM de Lua 
public:
	DeveloperConsole(sol::state* lua); 
	void toggle(); // Abrir/Cerrar
	void render(); // Dibujar la UI
	void executeCommand(std::string cmd); // Enviar a Lua 
	// Función para que C++ o Lua escriban en el log 
	void log(const std::string& message); 
};
```
(Posible implementación)

En el bucle principal habría que ejecutar la lógica de consola solo si esta visible, y renderizarla por encima de todo con el mismo criterio.

Ejemplos de comandos:
- `MotorAPI.Scene:load("Level2")`: Carga nivel instantánea.
- `MotorAPI.Physics:setDebugDraw(true)`: Ver colliders.
- `entity = Engine.create("Boss"); entity:setPosition(0, 50, 0)`: Spawnear jefe.