## Descripción:
Es un manager que gestiona cuando se cargan o se quitan DLLs. este los añade al sistema para que el usuario desarrollador pueda utilizarlos en el desarrollo de sus juegos. Actúa como puente entre el código del desarrollador y el motor sin necesidad de recompilar el núcleo.

---
## Posible implementación
Una de las posibles implementaciones es implementar un patron observer, utilizando la funcion de [ReadDirectoryChangesW (winbase.h)](https://learn.microsoft.com/es-es/windows/win32/api/winbase/nf-winbase-readdirectorychangesw?redirectedfrom=MSDN) mantenemos el loader en espera, en un hilo a parte, cuando una DLL haya sido modificada, hacemos una copia del estado actual y recargamos la librería actualizada plasmando el estado guardado, asi conseguimos un hot-reloading en ejecución del motor.

Por otro lado podría darse el caso que tras la actualización lo nuevo añadido no sea compatible con el estado anterior, en ese caso habría que volver a lanzar todo el juego, reinstanciando entidades y escenas