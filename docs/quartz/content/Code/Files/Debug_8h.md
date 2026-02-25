---
title: C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/src/Core/Debug.h

---

# C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/src/Core/Debug.h



## Namespaces

| Name           |
| -------------- |
| **[chavalescore](Namespaces/namespacechavalescore.md)**  |

## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[chavalescore::Debug](Classes/classchavalescore_1_1Debug.md)**  |

## Defines

|                | Name           |
| -------------- | -------------- |
|  | **[_Error](Files/Debug_8h.md#define--error)**(...)  |
|  | **[_File_Out](Files/Debug_8h.md#define--file-out)**  |
|  | **[_Out](Files/Debug_8h.md#define--out)**(...)  |
|  | **[_Warnig](Files/Debug_8h.md#define--warnig)**(...)  |




## Macros Documentation

### define _Error

```cpp
#define _Error(
    ...
)
std::cout << "\033[1;31m[E] " << __VA_ARGS__ << "\033[0m" << std::endl;\
```


### define _File_Out

```cpp
#define _File_Out = true
```


### define _Out

```cpp
#define _Out(
    ...
)
std::cout << "[M] " << __VA_ARGS__ << std::endl;\
```


### define _Warnig

```cpp
#define _Warnig(
    ...
)
std::cout << "\033[1;33m[W] " << __VA_ARGS__ << "\033[0m" << std::endl;\
```




-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100
