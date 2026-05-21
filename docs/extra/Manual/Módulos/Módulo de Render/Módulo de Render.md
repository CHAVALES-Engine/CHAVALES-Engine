## Descripción:
El módulo de render es una encapsulación de [Ogre](https://www.ogre3d.org/) y otras librerías, se ejecutará en un hilo a parte y para ello crearemos una cola de renderizado atómica alterable desde el ciclo de juego.

El módulo de render cuenta con tres submódulos que implementan las diferentes partes de un render completo:

- [[Low Level]] Encargado de renderizar mallas, shaders y animaciones.
- [[FrontEnd]] Encargado del renderizado de interfaz de usuario para el jugador.
- [[Optimización]] Encargado de técnicas de optimización de geometría.

---
## Métodos públicos:

| Método               | Descripción                                   | Devuelve           |
| -------------------- | --------------------------------------------- | ------------------ |
| static int init()    | Inicialización del audio                      | int = 0 (Correcto) |
| static int release() | Cierra fmod y libera la memoria creada        | int = 0 (Correcto) |
| static void render() | Renderiza las instrucciones de la renderqueue | void               |
|                      |                                               |                    |

---