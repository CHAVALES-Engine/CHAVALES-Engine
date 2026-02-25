---
title: PlatformModule
summary: Modulo de Plataforma. 

---

# PlatformModule



Modulo de Plataforma. 


`#include <PlatformModule.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| bool | **[Init](Classes/classPlatformModule.md#function-init)**()<br>Inicializador de [PlatformModule](Classes/classPlatformModule.md).  |
| | **[PlatformModule](Classes/classPlatformModule.md#function-platformmodule)**()<br>Constructora vacia de [PlatformModule](Classes/classPlatformModule.md).  |
| const HWND * | **[getWindowHandle](Classes/classPlatformModule.md#function-getwindowhandle)**() const<br>Devuelve el handle de la ventana de Windows.  |
| | **[~PlatformModule](Classes/classPlatformModule.md#function-~platformmodule)**()<br>Destructora de [PlatformModule](Classes/classPlatformModule.md).  |

## Public Functions Documentation

### function Init

```cpp
bool Init()
```

Inicializador de [PlatformModule](Classes/classPlatformModule.md). 

**Return**: bool - Si se ha creado correctamente 



```
Crea la ventana y el renderer
```


### function PlatformModule

```cpp
PlatformModule()
```

Constructora vacia de [PlatformModule](Classes/classPlatformModule.md). 

### function getWindowHandle

```cpp
const HWND * getWindowHandle() const
```

Devuelve el handle de la ventana de Windows. 

**Return**: HWND - Hanlde de la ventana (internamente un id) 

### function ~PlatformModule

```cpp
~PlatformModule()
```

Destructora de [PlatformModule](Classes/classPlatformModule.md). 

Elimina la ventana, el renderer y hace el Quit de SDL 


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100