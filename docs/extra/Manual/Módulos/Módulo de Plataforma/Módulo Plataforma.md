\page Modulo_Plataforma
## Descripción:
Interfaz de abstracción de la plataforma, Engloba dentro dos módulos importantes como son [[Input]] y [[Ventana]]

## Métodos públicos:

| Método                               | Descripción                                                               | Devuelve           |
| ------------------------------------ | ------------------------------------------------------------------------- | ------------------ |
| static void Init()                   | Inicialización del módulo, crea ventana e inicializa los módulos internos |                    |
| static int release()                 | Release de módulos y libera la memoria creada                             | int = 0 (Correcto) |
| static InputState getInput( int id ) | Devuelve el estado del input para un jugador con id                       |                    |
