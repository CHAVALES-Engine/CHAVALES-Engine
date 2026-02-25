---
title: Vector3
summary: Vector tridimensional. 

---

# Vector3



Vector tridimensional.  [More...](#detailed-description)


`#include <Vector3.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| | **[Vector3](Classes/classVector3.md#function-vector3)**()<br>Constructor vacio.  |
| | **[Vector3](Classes/classVector3.md#function-vector3)**(T a)<br>Constructor de un solo parametro.  |
| | **[Vector3](Classes/classVector3.md#function-vector3)**(T x, T y, T z)<br>Constructor de dos parametros.  |
| | **[Vector3](Classes/classVector3.md#function-vector3)**(const [Vector3](Classes/classVector3.md) & v)<br>Constructor por copia.  |
| | **[Vector3](Classes/classVector3.md#function-vector3)**([Vector3](Classes/classVector3.md) && v)<br>Constructor de movimiento.  |
| T | **[getX](Classes/classVector3.md#function-getx)**() const<br>Getter de X.  |
| T | **[getY](Classes/classVector3.md#function-gety)**() const<br>Getter de Y.  |
| T | **[getZ](Classes/classVector3.md#function-getz)**() const<br>Getter de Z.  |
| T | **[magnitude](Classes/classVector3.md#function-magnitude)**()<br>Devuelve el modulo del vector.  |
| [Vector3](Classes/classVector3.md) | **[normalized](Classes/classVector3.md#function-normalized)**()<br>Devuelve el vector normalizado.  |
| bool | **[operator!=](Classes/classVector3.md#function-operator!=)**(const [Vector3](Classes/classVector3.md) & v)<br>Devuelve true si los vectores son diferentes.  |
| T | **[operator*](Classes/classVector3.md#function-operator*)**(const [Vector3](Classes/classVector3.md) & v)<br>Producto escalar de dos vectores.  |
| [Vector3](Classes/classVector3.md) & | **[operator*](Classes/classVector3.md#function-operator*)**(T a)<br>Multiplica un vector por un escalar.  |
| [Vector3](Classes/classVector3.md) & | **[operator+](Classes/classVector3.md#function-operator+)**(const [Vector3](Classes/classVector3.md) & v)<br>Suma dos vectores.  |
| [Vector3](Classes/classVector3.md) & | **[operator-](Classes/classVector3.md#function-operator-)**(const [Vector3](Classes/classVector3.md) & v)<br>Resta dos vectores.  |
| [Vector3](Classes/classVector3.md) & | **[operator/](Classes/classVector3.md#function-operator/)**(T a)<br>Divide un vector por un escalar.  |
| [Vector3](Classes/classVector3.md) & | **[operator=](Classes/classVector3.md#function-operator=)**(const [Vector3](Classes/classVector3.md) & v)<br>Asignacion.  |
| bool | **[operator==](Classes/classVector3.md#function-operator==)**(const [Vector3](Classes/classVector3.md) & v)<br>Devuelve true si los vectores son iguales.  |
| void | **[set](Classes/classVector3.md#function-set)**(T x, T y, T z)<br>Setter de X Y Z de un vector ya existente.  |
| void | **[set](Classes/classVector3.md#function-set)**(const [Vector3](Classes/classVector3.md) & v)<br>Setter de X Y Z de un vector ya existente.  |
| void | **[setX](Classes/classVector3.md#function-setx)**(T x)<br>Setter de X.  |
| void | **[setY](Classes/classVector3.md#function-sety)**(T y)<br>Setter de Y.  |
| void | **[setZ](Classes/classVector3.md#function-setz)**(T z)<br>Setter de Z.  |
| | **[~Vector3](Classes/classVector3.md#function-~vector3)**() |

## Detailed Description

```cpp
template <typename T  =float>
class Vector3;
```

Vector tridimensional. 



```
Clase que implementa un vector tridimensional.
```

## Public Functions Documentation

### function Vector3

```cpp
inline Vector3()
```

Constructor vacio. 



```
Constructor vacio, se inicializan x e y a 0.
```


### function Vector3

```cpp
inline Vector3(
    T a
)
```

Constructor de un solo parametro. 

**Parameters**: 

  * **a** - Escalar con el que inicializar a todas las componentes. 




```
Constructor de un solo parametro, se inicializan todas las componentes con el.
```


### function Vector3

```cpp
inline Vector3(
    T x,
    T y,
    T z
)
```

Constructor de dos parametros. 

**Parameters**: 

  * **x** - Escalar con el que inicializar la componente X. 
  * **y** - Escalar con el que inicializar la componente Y. 
  * **z** - Escalar con el que inicializar la componente Z. 


### function Vector3

```cpp
inline Vector3(
    const Vector3 & v
)
```

Constructor por copia. 

**Parameters**: 

  * **v** - Vector a copiar. 


### function Vector3

```cpp
inline Vector3(
    Vector3 && v
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

### function getZ

```cpp
inline T getZ() const
```

Getter de Z. 

### function magnitude

```cpp
inline T magnitude()
```

Devuelve el modulo del vector. 

### function normalized

```cpp
inline Vector3 normalized()
```

Devuelve el vector normalizado. 

### function operator!=

```cpp
inline bool operator!=(
    const Vector3 & v
)
```

Devuelve true si los vectores son diferentes. 

**Parameters**: 

  * **v** - vector a comparar. 


### function operator*

```cpp
inline T operator*(
    const Vector3 & v
)
```

Producto escalar de dos vectores. 

**Parameters**: 

  * **v** - vector a multiplicar. 


### function operator*

```cpp
inline Vector3 & operator*(
    T a
)
```

Multiplica un vector por un escalar. 

**Parameters**: 

  * **a** - escalar a multiplicar. 


### function operator+

```cpp
inline Vector3 & operator+(
    const Vector3 & v
)
```

Suma dos vectores. 

**Parameters**: 

  * **v** - vector a sumar. 


### function operator-

```cpp
inline Vector3 & operator-(
    const Vector3 & v
)
```

Resta dos vectores. 

**Parameters**: 

  * **v** - vector a restar. 


### function operator/

```cpp
inline Vector3 & operator/(
    T a
)
```

Divide un vector por un escalar. 

**Parameters**: 

  * **a** - escalar por el que dividir. 


### function operator=

```cpp
inline Vector3 & operator=(
    const Vector3 & v
)
```

Asignacion. 

**Parameters**: 

  * **v** - vector a asignar. 


### function operator==

```cpp
inline bool operator==(
    const Vector3 & v
)
```

Devuelve true si los vectores son iguales. 

**Parameters**: 

  * **v** - vector a comparar. 


### function set

```cpp
inline void set(
    T x,
    T y,
    T z
)
```

Setter de X Y Z de un vector ya existente. 

**Parameters**: 

  * **x** - Valor a settear en X. 
  * **y** - Valor a settear en Y. 
  * **z** - Valor a settear en Z. 


### function set

```cpp
inline void set(
    const Vector3 & v
)
```

Setter de X Y Z de un vector ya existente. 

**Parameters**: 

  * **v** - vector del que sacar X Y Z para settear. 


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


### function setZ

```cpp
inline void setZ(
    T z
)
```

Setter de Z. 

**Parameters**: 

  * **z** - Valor a settear en Z. 


### function ~Vector3

```cpp
inline ~Vector3()
```


-------------------------------

Updated on 2026-02-25 at 10:42:50 +0100