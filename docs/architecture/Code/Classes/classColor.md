---
title: Color
summary: Color que representa un color en RGBA. 

---

# Color



[Color]() que representa un color en RGBA.  [More...](#detailed-description)


`#include <Color.h>`

Inherits from [Vector4< float >](Classes/classVector4.md)

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Color](Classes/classColor.md#function-color)**(float r, float g, float b, float a) |
| float | **[getAlpha](Classes/classColor.md#function-getalpha)**() const<br>Getter de A (alpha).  |
| float | **[getBlue](Classes/classColor.md#function-getblue)**() const<br>Getter de B (blue).  |
| float | **[getGreen](Classes/classColor.md#function-getgreen)**() const<br>Getter de G (green).  |
| float | **[getRed](Classes/classColor.md#function-getred)**() const<br>Getter de R (red).  |

## Additional inherited members

**Public Functions inherited from [Vector4< float >](Classes/classVector4.md)**

|                | Name           |
| -------------- | -------------- |
| | **[Vector4](Classes/classVector4.md#function-vector4)**()<br>Constructor vacio.  |
| | **[Vector4](Classes/classVector4.md#function-vector4)**(T a)<br>Constructor de un solo parametro.  |
| | **[Vector4](Classes/classVector4.md#function-vector4)**(T x, T y, T z, T w)<br>Constructor de dos parametros.  |
| | **[Vector4](Classes/classVector4.md#function-vector4)**(const [Vector4](Classes/classVector4.md) & v)<br>Constructor por copia.  |
| | **[Vector4](Classes/classVector4.md#function-vector4)**([Vector4](Classes/classVector4.md) && v)<br>Constructor de movimiento.  |
| T | **[getW](Classes/classVector4.md#function-getw)**() const<br>Getter de W.  |
| T | **[getX](Classes/classVector4.md#function-getx)**() const<br>Getter de X.  |
| T | **[getY](Classes/classVector4.md#function-gety)**() const<br>Getter de Y.  |
| T | **[getZ](Classes/classVector4.md#function-getz)**() const<br>Getter de Z.  |
| T | **[magnitude](Classes/classVector4.md#function-magnitude)**()<br>Devuelve el modulo del vector.  |
| [Vector4](Classes/classVector4.md) | **[normalized](Classes/classVector4.md#function-normalized)**()<br>Devuelve el vector normalizado.  |
| bool | **[operator!=](Classes/classVector4.md#function-operator!=)**(const [Vector4](Classes/classVector4.md) & v)<br>Devuelve true si los vectores son diferentes.  |
| T | **[operator*](Classes/classVector4.md#function-operator*)**(const [Vector4](Classes/classVector4.md) & v)<br>Producto escalar de dos vectores.  |
| [Vector4](Classes/classVector4.md) & | **[operator*](Classes/classVector4.md#function-operator*)**(T a)<br>Multiplica un vector por un escalar.  |
| [Vector4](Classes/classVector4.md) & | **[operator+](Classes/classVector4.md#function-operator+)**(const [Vector4](Classes/classVector4.md) & v)<br>Suma dos vectores.  |
| [Vector4](Classes/classVector4.md) & | **[operator-](Classes/classVector4.md#function-operator-)**(const [Vector4](Classes/classVector4.md) & v)<br>Resta dos vectores.  |
| [Vector4](Classes/classVector4.md) & | **[operator/](Classes/classVector4.md#function-operator/)**(T a)<br>Divide un vector por un escalar.  |
| [Vector4](Classes/classVector4.md) & | **[operator=](Classes/classVector4.md#function-operator=)**(const [Vector4](Classes/classVector4.md) & v)<br>Asignacion.  |
| bool | **[operator==](Classes/classVector4.md#function-operator==)**(const [Vector4](Classes/classVector4.md) & v)<br>Devuelve true si los vectores son iguales.  |
| void | **[set](Classes/classVector4.md#function-set)**(T x, T y, T z)<br>Setter de X Y Z W de un vector ya existente.  |
| void | **[set](Classes/classVector4.md#function-set)**(const [Vector4](Classes/classVector4.md) & v)<br>Setter de X Y Z W de un vector ya existente.  |
| void | **[setW](Classes/classVector4.md#function-setw)**(T w)<br>Setter de W.  |
| void | **[setX](Classes/classVector4.md#function-setx)**(T x)<br>Setter de X.  |
| void | **[setY](Classes/classVector4.md#function-sety)**(T y)<br>Setter de Y.  |
| void | **[setZ](Classes/classVector4.md#function-setz)**(T z)<br>Setter de Z.  |
| | **[~Vector4](Classes/classVector4.md#function-~vector4)**() |


## Detailed Description

```cpp
class Color;
```

[Color]() que representa un color en RGBA. 



```
Clase para representar un color en formato RGBA
rojo (r), verde (g), azul (b), and alfa (a).
Cada atributo es un float entre 0.0 y 1.0.
```

## Public Functions Documentation

### function Color

```cpp
inline Color(
    float r,
    float g,
    float b,
    float a
)
```


### function getAlpha

```cpp
inline float getAlpha() const
```

Getter de A (alpha). 

### function getBlue

```cpp
inline float getBlue() const
```

Getter de B (blue). 

### function getGreen

```cpp
inline float getGreen() const
```

Getter de G (green). 

### function getRed

```cpp
inline float getRed() const
```

Getter de R (red). 

-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100