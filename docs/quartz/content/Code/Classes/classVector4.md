---
title: Vector4
summary: Vector cuatridimensional. 

---

# Vector4



Vector cuatridimensional.  [More...](#detailed-description)


`#include <Vector4.h>`

## Public Functions

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
template <typename T  =float>
class Vector4;
```

Vector cuatridimensional. 



```
Clase que implementa un vector cuatridimensional.
```

## Public Functions Documentation

### function Vector4

```cpp
inline Vector4()
```

Constructor vacio. 



```
Constructor vacio, se inicializan x e y a 0.
```


### function Vector4

```cpp
inline Vector4(
    T a
)
```

Constructor de un solo parametro. 

**Parameters**: 

  * **a** - Escalar con el que inicializar a todas las componentes. 




```
Constructor de un solo parametro, se inicializan todas las componentes con el.
```


### function Vector4

```cpp
inline Vector4(
    T x,
    T y,
    T z,
    T w
)
```

Constructor de dos parametros. 

**Parameters**: 

  * **x** - Escalar con el que inicializar la componente X. 
  * **y** - Escalar con el que inicializar la componente Y. 
  * **z** - Escalar con el que inicializar la componente Z. 
  * **w** - Escalar con el que inicializar la componente W. 


### function Vector4

```cpp
inline Vector4(
    const Vector4 & v
)
```

Constructor por copia. 

**Parameters**: 

  * **v** - Vector a copiar. 


### function Vector4

```cpp
inline Vector4(
    Vector4 && v
)
```

Constructor de movimiento. 

**Parameters**: 

  * **v** - Vector del que transferir los datos. 


### function getW

```cpp
inline T getW() const
```

Getter de W. 

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
inline Vector4 normalized()
```

Devuelve el vector normalizado. 

### function operator!=

```cpp
inline bool operator!=(
    const Vector4 & v
)
```

Devuelve true si los vectores son diferentes. 

**Parameters**: 

  * **v** - vector a comparar. 


### function operator*

```cpp
inline T operator*(
    const Vector4 & v
)
```

Producto escalar de dos vectores. 

**Parameters**: 

  * **v** - vector a multiplicar. 


### function operator*

```cpp
inline Vector4 & operator*(
    T a
)
```

Multiplica un vector por un escalar. 

**Parameters**: 

  * **a** - escalar a multiplicar. 


### function operator+

```cpp
inline Vector4 & operator+(
    const Vector4 & v
)
```

Suma dos vectores. 

**Parameters**: 

  * **v** - vector a sumar. 


### function operator-

```cpp
inline Vector4 & operator-(
    const Vector4 & v
)
```

Resta dos vectores. 

**Parameters**: 

  * **v** - vector a restar. 


### function operator/

```cpp
inline Vector4 & operator/(
    T a
)
```

Divide un vector por un escalar. 

**Parameters**: 

  * **a** - escalar por el que dividir. 


### function operator=

```cpp
inline Vector4 & operator=(
    const Vector4 & v
)
```

Asignacion. 

**Parameters**: 

  * **v** - vector a asignar. 


### function operator==

```cpp
inline bool operator==(
    const Vector4 & v
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

Setter de X Y Z W de un vector ya existente. 

**Parameters**: 

  * **x** - Valor a settear en X. 
  * **y** - Valor a settear en Y. 
  * **z** - Valor a settear en Z. 
  * **w** - Valor a settear en W. 


### function set

```cpp
inline void set(
    const Vector4 & v
)
```

Setter de X Y Z W de un vector ya existente. 

**Parameters**: 

  * **v** - vector del que sacar X Y Z W para settear. 


### function setW

```cpp
inline void setW(
    T w
)
```

Setter de W. 

**Parameters**: 

  * **w** - Valor a settear en W. 


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


### function ~Vector4

```cpp
inline ~Vector4()
```


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100