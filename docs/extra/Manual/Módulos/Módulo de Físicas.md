## Descripción:
Encapsulación de [PhysiX](https://www.nvidia.com/es-es/drivers/physx/9_19_0218/physx-9-19-0218-driver-es/), Este módulo gestiona escenas de PhysiS internas y entidades que se asocian a entidades de juego, es un espacio paralelo que simula las físicas.

Utilizaremos un unorderer "map<int, handler>" donde mappearemos las id de entidades dentro del juego con su equivalente en PhysiX (handler)
## Métodos públicos:

| Método                     | Descripción                                                                 | Devuelve           |
| -------------------------- | --------------------------------------------------------------------------- | ------------------ |
| static int init()          | Inicialización del módulo                                                   | int = 0 (Correcto) |
| static int release()       | Libera la memoria creada                                                    | int = 0 (Correcto) |
| static int syncToPhysicX() | Sincroniza las entidades del juego con su mappeo en las entidades de PhysiX | int = 0 (Correcto) |
| static int syncToEngine()  | Sincroniza las entidades simuladas en PhysiX con las entidades de juego     | int = 0 (Correcto) |
