---
title: DLLLoader

---

# DLLLoader





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[DLLLoader](Classes/classDLLLoader.md#function-dllloader)**()<br>Empty constructor.  |
| bool | **[load](Classes/classDLLLoader.md#function-load)**(const std::string & path)<br>Load library method.  |
| void | **[unLoadAll](Classes/classDLLLoader.md#function-unloadall)**()<br>unload all libaries  |
| bool | **[unload](Classes/classDLLLoader.md#function-unload)**(const std::string & path)<br>Unload library method.  |
| | **[~DLLLoader](Classes/classDLLLoader.md#function-~dllloader)**()<br>Destructor.  |

## Public Functions Documentation

### function DLLLoader

```cpp
DLLLoader()
```

Empty constructor. 

### function load

```cpp
bool load(
    const std::string & path
)
```

Load library method. 

**Parameters**: 

  * **path** - Path to loaded dll. 


**Return**: bool - It loaded correctly. 



```
It loads the library on the path.
```


### function unLoadAll

```cpp
void unLoadAll()
```

unload all libaries 



```
    Unloads all dynamic libraries, It will be replaced by other more hot-reload freandly.
```


### function unload

```cpp
bool unload(
    const std::string & path
)
```

Unload library method. 

**Parameters**: 

  * **path** - Path to unloaded dll. 


**Return**: bool - It unloaded correctly. 



```
TODO: It unloads the library on the path.
```


### function ~DLLLoader

```cpp
~DLLLoader()
```

Destructor. 



```
Unload all dynamic libraries.
```


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100