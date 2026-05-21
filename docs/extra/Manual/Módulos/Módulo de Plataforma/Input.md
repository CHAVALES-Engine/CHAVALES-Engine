## Descripción:
Input se Usara para encapsular la gestión del input de SDL, utilizaremos dispositivos virtuales para gestionar varios usuarios de forma independiente a la plataforma.

## Métodos públicos:

| Método                             | Descripción                                   | Devuelve           |
| ---------------------------------- | --------------------------------------------- | ------------------ |
| static int init()                  | Inicializacion del módulo                     | int = 0 (Correcto) |
| static int release()               | Cierre del módulo liberando la memoria creada | int = 0 (Correcto) |
| static InputState getInput(id = 0) | Devuelve el input del dispositivo id          | InputState         |
|                                    |                                               |                    |
