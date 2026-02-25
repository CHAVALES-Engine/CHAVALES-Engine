---
title: Vector2
summary: Vector bidimensional. 

---

# Vector2



Vector bidimensional.  [More...](#detailed-description)


`#include <Vector2.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Vector2](Classes/classVector2.md#function-vector2)**()<br>Constructor vacio.  |
| | **[Vector2](Classes/classVector2.md#function-vector2)**(T a)<br>Constructor de un solo parametro.  |
| | **[Vector2](Classes/classVector2.md#function-vector2)**(T x, T y)<br>Constructor de dos parametros.  |
| | **[Vector2](Classes/classVector2.md#function-vector2)**(const [Vector2](Classes/classVector2.md) & v)<br>Constructor por copia.  |
| | **[Vector2](Classes/classVector2.md#function-vector2)**([Vector2](Classes/classVector2.md) && v)<br>Constructor de movimiento.  |
| T | **[getX](Classes/classVector2.md#function-getx)**() const<br>Getter de X.  |
| T | **[getY](Classes/classVector2.md#function-gety)**() const<br>Getter de Y.  |
| T | **[magnitude](Classes/classVector2.md#function-magnitude)**()<br>Devuelve el modulo del vector.  |
| [Vector2](Classes/classVector2.md) | **[normalized](Classes/classVector2.md#function-normalized)**()<br>Devuelve el vector normalizado.  |
| bool | **[operator!=](Classes/classVector2.md#function-operator!=)**(const [Vector2](Classes/classVector2.md) & v)<br>Devuelve true si los vectores son diferentes.  |
| T | **[operator*](Classes/classVector2.md#function-operator*)**(const [Vector2](Classes/classVector2.md) & v)<br>Producto escalar de dos vectores.  |
| [Vector2](Classes/classVector2.md) & | **[operator*](Classes/classVector2.md#function-operator*)**(T a)<br>Multiplica un vector por un escalar.  |
| [Vector2](Classes/classVector2.md) & | **[operator+](Classes/classVector2.md#function-operator+)**(const [Vector2](Classes/classVector2.md) & v)<br>Suma dos vectores.  |
| [Vector2](Classes/classVector2.md) & | **[operator-](Classes/classVector2.md#function-operator-)**(const [Vector2](Classes/classVector2.md) & v)<br>Resta dos vectores.  |
| [Vector2](Classes/classVector2.md) & | **[operator/](Classes/classVector2.md#function-operator/)**(T a)<br>Divide un vector por un escalar.  |
| [Vector2](Classes/classVector2.md) & | **[operator=](Classes/classVector2.md#function-operator=)**(const [Vector2](Classes/classVector2.md) & v)<br>Asignacion.  |
| bool | **[operator==](Classes/classVector2.md#function-operator==)**(const [Vector2](Classes/classVector2.md) & v)<br>Devuelve true si los vectores son iguales.  |
| void | **[set](Classes/classVector2.md#function-set)**(T x, T y)<br>Setter de X e Y de un vector ya existente.  |
| void | **[set](Classes/classVector2.md#function-set)**(const [Vector2](Classes/classVector2.md) & v)<br>Setter de X e Y de un vector ya existente.  |
| void | **[setX](Classes/classVector2.md#function-setx)**(T x)<br>Setter de X.  |
| void | **[setY](Classes/classVector2.md#function-sety)**(T y)<br>Setter de Y.  |
| | **[~Vector2](Classes/classVector2.md#function-~vector2)**() |

## Detailed Description

```cpp
template <typename T  =float>
class Vector2;
```

Vector bidimensional. 



```
Clase que implementa un vector bidimensional.
```

## Public Functions Documentation

### function Vector2

```cpp
inline Vector2()
```

Constructor vacio. 



```
Constructor vacio, se inicializan x e y a 0.
```


### function Vector2

```cpp
inline Vector2(
    T a
)
```

Constructor de un solo parametro. 

**Parameters**: 

  * **a** - Escalar con el que inicializar a todas las componentes. 




```
Constructor de un solo parametro, se inicializan todas las componentes con el.
```


### function Vector2

```cpp
inline Vector2(
    T x,
    T y
)
```

Constructor de dos parametros. 

**Parameters**: 

  * **x** - Escalar con el que inicializar la componente X. 
  * **y** - Escalar con el que inicializar la componente Y. 


### function Vector2

```cpp
inline Vector2(
    const Vector2 & v
)
```

Constructor por copia. 

**Parameters**: 

  * **v** - Vector a copiar. 


### function Vector2

```cpp
inline Vector2(
    Vector2 && v
)
```

Constructor de movimiento. 

**Parameters**: 

  * **v** - Vector del que transferir los datos. 


### function getX

```cpp
inline T getX() const
```

Getter de X. 

### function getY

```cpp
inline T getY() const
```

Getter de Y. 

### function magnitude

```cpp
inline T magnitude()
```

Devuelve el modulo del vector. 

### function normalized

```cpp
inline Vector2 normalized()
```

Devuelve el vector normalizado. 

### function operator!=

```cpp
inline bool operator!=(
    const Vector2 & v
)
```

Devuelve true si los vectores son diferentes. 

**Parameters**: 

  * **v** - vector a comparar. 


### function operator*

```cpp
inline T operator*(
    const Vector2 & v
)
```

Producto escalar de dos vectores. 

**Parameters**: 

  * **v** - vector a multiplicar. 


### function operator*

```cpp
inline Vector2 & operator*(
    T a
)
```

Multiplica un vector por un escalar. 

**Parameters**: 

  * **a** - escalar a multiplicar. 


### function operator+

```cpp
inline Vector2 & operator+(
    const Vector2 & v
)
```

Suma dos vectores. 

**Parameters**: 

  * **v** - vector a sumar. 


### function operator-

```cpp
inline Vector2 & operator-(
    const Vector2 & v
)
```

Resta dos vectores. 

**Parameters**: 

  * **v** - vector a restar. 


### function operator/

```cpp
inline Vector2 & operator/(
    T a
)
```

Divide un vector por un escalar. 

**Parameters**: 

  * **a** - escalar por el que dividir. 


### function operator=

```cpp
inline Vector2 & operator=(
    const Vector2 & v
)
```

Asignacion. 

**Parameters**: 

  * **v** - vector a asignar. 


### function operator==

```cpp
inline bool operator==(
    const Vector2 & v
)
```

Devuelve true si los vectores son iguales. 

**Parameters**: 

  * **v** - vector a comparar. 


### function set

```cpp
inline void set(
    T x,
    T y
)
```

Setter de X e Y de un vector ya existente. 

**Parameters**: 

  * **x** - Valor a settear en X. 
  * **y** - Valor a settear en Y. 


### function set

```cpp
inline void set(
    const Vector2 & v
)
```

Setter de X e Y de un vector ya existente. 

**Parameters**: 

  * **v** - vector del que sacar X e Y para settear. 


### function setX

```cpp
inline void setX(
    T x
)
```

Setter de X. 

**Parameters**: 

  * **x** - Valor a settear en X. 


### function setY

```cpp
inline void setY(
    T y
)
```

Setter de Y. 

**Parameters**: 

  * **y** - Valor a settear en Y. 


### function ~Vector2

```cpp
inline ~Vector2()
```


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100