---
title: ec::Component

---

# ec::Component





## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Component](Classes/classec_1_1Component.md#function-component)**() |
| virtual void | **[init](Classes/classec_1_1Component.md#function-init)**() |
| virtual void | **[onCreate](Classes/classec_1_1Component.md#function-oncreate)**() |
| virtual void | **[onDestroy](Classes/classec_1_1Component.md#function-ondestroy)**() |
| virtual void | **[onDisable](Classes/classec_1_1Component.md#function-ondisable)**() |
| virtual void | **[onEnable](Classes/classec_1_1Component.md#function-onenable)**() |
| virtual void | **[render](Classes/classec_1_1Component.md#function-render)**() |
| void | **[setEntity](Classes/classec_1_1Component.md#function-setentity)**(Entity * e) |
| virtual void | **[update](Classes/classec_1_1Component.md#function-update)**() |
| virtual | **[~Component](Classes/classec_1_1Component.md#function-~component)**() |

## Protected Attributes

|                | Name           |
| -------------- | -------------- |
| bool | **[enabled](Classes/classec_1_1Component.md#variable-enabled)**  |
| Entity * | **[ent](Classes/classec_1_1Component.md#variable-ent)**  |

## Public Functions Documentation

### function Component

```cpp
inline Component()
```


### function init

```cpp
inline virtual void init()
```


### function onCreate

```cpp
inline virtual void onCreate()
```


### function onDestroy

```cpp
inline virtual void onDestroy()
```


### function onDisable

```cpp
inline virtual void onDisable()
```


### function onEnable

```cpp
inline virtual void onEnable()
```


### function render

```cpp
inline virtual void render()
```


### function setEntity

```cpp
inline void setEntity(
    Entity * e
)
```


### function update

```cpp
inline virtual void update()
```


### function ~Component

```cpp
inline virtual ~Component()
```


## Protected Attributes Documentation

### variable enabled

```cpp
bool enabled;
```


### variable ent

```cpp
Entity * ent;
```


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100