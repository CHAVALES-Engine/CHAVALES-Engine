---
title: ecs::Manager

---

# ecs::Manager





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Manager](Classes/classecs_1_1Manager.md#function-manager)**() |
| template <typename T ,typename ... Ts\> <br>T * | **[addComponent](Classes/classecs_1_1Manager.md#function-addcomponent)**(entity_t e, Ts &&... args) |
| entity_t | **[addEntity](Classes/classecs_1_1Manager.md#function-addentity)**(ecs::grpId_t gId =ecs::grp::DEFAULT) |
| template <typename T \> <br>T * | **[getComponent](Classes/classecs_1_1Manager.md#function-getcomponent)**(entity_t e) |
| const auto & | **[getEntities](Classes/classecs_1_1Manager.md#function-getentities)**(grpId_t gId =ecs::grp::DEFAULT) |
| entity_t | **[getHandler](Classes/classecs_1_1Manager.md#function-gethandler)**(hdlrId_t hId) |
| ecs::grpId_t | **[groupId](Classes/classecs_1_1Manager.md#function-groupid)**(entity_t e) |
| template <typename T \> <br>bool | **[hasComponent](Classes/classecs_1_1Manager.md#function-hascomponent)**(entity_t e) |
| bool | **[isAlive](Classes/classecs_1_1Manager.md#function-isalive)**(entity_t e) |
| void | **[refresh](Classes/classecs_1_1Manager.md#function-refresh)**() |
| template <typename T \> <br>void | **[removeComponent](Classes/classecs_1_1Manager.md#function-removecomponent)**(entity_t e) |
| void | **[render](Classes/classecs_1_1Manager.md#function-render)**(entity_t e) |
| void | **[render](Classes/classecs_1_1Manager.md#function-render)**() |
| void | **[setAlive](Classes/classecs_1_1Manager.md#function-setalive)**(entity_t e, bool alive) |
| void | **[setHandler](Classes/classecs_1_1Manager.md#function-sethandler)**(hdlrId_t hId, entity_t e) |
| void | **[update](Classes/classecs_1_1Manager.md#function-update)**(entity_t e) |
| void | **[update](Classes/classecs_1_1Manager.md#function-update)**() |
| virtual | **[~Manager](Classes/classecs_1_1Manager.md#function-~manager)**() |

## Public Functions Documentation

### function Manager

```cpp
Manager()
```


### function addComponent

```cpp
template <typename T ,
typename ... Ts>
inline T * addComponent(
    entity_t e,
    Ts &&... args
)
```


### function addEntity

```cpp
inline entity_t addEntity(
    ecs::grpId_t gId =ecs::grp::DEFAULT
)
```


### function getComponent

```cpp
template <typename T >
inline T * getComponent(
    entity_t e
)
```


### function getEntities

```cpp
inline const auto & getEntities(
    grpId_t gId =ecs::grp::DEFAULT
)
```


### function getHandler

```cpp
inline entity_t getHandler(
    hdlrId_t hId
)
```


### function groupId

```cpp
inline ecs::grpId_t groupId(
    entity_t e
)
```


### function hasComponent

```cpp
template <typename T >
inline bool hasComponent(
    entity_t e
)
```


### function isAlive

```cpp
inline bool isAlive(
    entity_t e
)
```


### function refresh

```cpp
void refresh()
```


### function removeComponent

```cpp
template <typename T >
inline void removeComponent(
    entity_t e
)
```


### function render

```cpp
inline void render(
    entity_t e
)
```


### function render

```cpp
inline void render()
```


### function setAlive

```cpp
inline void setAlive(
    entity_t e,
    bool alive
)
```


### function setHandler

```cpp
inline void setHandler(
    hdlrId_t hId,
    entity_t e
)
```


### function update

```cpp
inline void update(
    entity_t e
)
```


### function update

```cpp
inline void update()
```


### function ~Manager

```cpp
virtual ~Manager()
```


-------------------------------

Updated on 2026-02-25 at 10:42:50 +0100