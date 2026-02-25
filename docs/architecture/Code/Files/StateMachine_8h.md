---
title: C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/src/ExecutableProject/StateMachine.h

---

# C:/Users/andres/Documents/GitHub/2526-Grupo03-ChavalesEngine/src/ExecutableProject/StateMachine.h



## Classes

|                | Name           |
| -------------- | -------------- |
| class | **[StateMachine](Classes/classStateMachine.md)**  |

## Types

|                | Name           |
| -------------- | -------------- |
| using std::pair< sceneID, scenePtr > | **[scene](Files/StateMachine_8h.md#using-scene)**  |
| using uint32_t | **[sceneID](Files/StateMachine_8h.md#using-sceneid)**  |
| using std::shared_ptr< Scene > | **[scenePtr](Files/StateMachine_8h.md#using-sceneptr)**  |
| using std::unordered_map< sceneID, scenePtr > | **[stateMachine](Files/StateMachine_8h.md#using-statemachine)**  |

## Types Documentation

### using scene

```cpp
using scene = std::pair<sceneID, scenePtr>;
```


### using sceneID

```cpp
using sceneID = uint32_t;
```


### using scenePtr

```cpp
using scenePtr = std::shared_ptr<Scene>;
```


### using stateMachine

```cpp
using stateMachine = std::unordered_map<sceneID, scenePtr>;
```







-------------------------------

Updated on 2026-02-25 at 10:42:50 +0100
