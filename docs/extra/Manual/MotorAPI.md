\page MotorAPI

El motor esta compuesto de varios Módulos, estos proveerán una interfaz para el desarrollador.

El propio motor es un ejecutable que generamos para ejecutar un .dll linkeada (El juego).
Los módulos funcionarán como instancias que se inicializaran en la instanciación del motor.
[[Módulo de Render]], [[Módulo de Físicas]], [[Módulo de Audio]], [[Módulo Plataforma]], [[Módulo de Recursos]], (Utilizamos un instancias de la api ya que queremos una destrucción e instanciación rápida, para asi conseguir un [hot-reload ](https://learn.microsoft.com/es-es/visualstudio/debugger/hot-reload?view=visualstudio&pivots=programming-language-dotnet)mas controlado)
## Métodos públicos:

| Método         | Descripción                  | Devuelve           |
| -------------- | ---------------------------- | ------------------ |
| int init()     | Init de todos los módulos    | int = 0 (Correcto) |
| int release()  | Release de todos los módulos | int = 0 (Correcto) |
| int gameLoop() | Crea el gameloop             | int = 0 (Correcto) |
|                |                              |                    |
