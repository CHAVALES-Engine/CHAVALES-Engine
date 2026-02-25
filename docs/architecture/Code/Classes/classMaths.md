---
title: Maths
summary: Funciones matematicas. 

---

# Maths



Funciones matematicas.  [More...](#detailed-description)


`#include <Maths.h>`

## Public Functions

|                | Name           |
| -------------- | -------------- |
| template <class T \> <br>T | **[Abs](Classes/classMaths.md#function-abs)**(T a)<br>Devuelve el valor absoluto de a.  |
| float | **[Ceil](Classes/classMaths.md#function-ceil)**(float a)<br>Devuelve a redondeado hacia arriba.  |
| template <class T \> <br>T | **[Clamp](Classes/classMaths.md#function-clamp)**(T a, T min, T max)<br>Devuelve el valor saturado entre minimo y maximo.  |
| float | **[Cos](Classes/classMaths.md#function-cos)**(float a)<br>Devuelve el coseno del angulo a en radianes.  |
| float | **[Exp](Classes/classMaths.md#function-exp)**(float p)<br>Devuelve e elevado a p.  |
| float | **[Floor](Classes/classMaths.md#function-floor)**(float a)<br>Devuelve a redondeado hacia abajo.  |
| float | **[Lerp](Classes/classMaths.md#function-lerp)**(float a, float b, float t)<br>Interpola linealmente entre a y b en funcion de t.  |
| float | **[Log](Classes/classMaths.md#function-log)**(float a, float b)<br>Devuelve el logaritmo de a en base b.  |
| float | **[Log10](Classes/classMaths.md#function-log10)**(float a)<br>Devuelve el logaritmo de a en base 10.  |
| | **[Maths](Classes/classMaths.md#function-maths)**() =default |
| template <class T \> <br>T | **[Max](Classes/classMaths.md#function-max)**(T a, T b)<br>Devuelve el maximo entre a y b.  |
| template <class T \> <br>T | **[Min](Classes/classMaths.md#function-min)**(T a, T b)<br>Devuelve el minimo entre a y b.  |
| float | **[Pow](Classes/classMaths.md#function-pow)**(float a, float p)<br>Devuelve a elevado a p.  |
| float | **[Sin](Classes/classMaths.md#function-sin)**(float a)<br>Devuelve el seno del angulo a en radianes.  |
| template <class T \> <br>float | **[Sqrt](Classes/classMaths.md#function-sqrt)**(T a)<br>Devuelve la raiz cuadrada de a.  |
| float | **[Tan](Classes/classMaths.md#function-tan)**(float a)<br>Devuelve la tangente del angulo a en radianes.  |
| | **[~Maths](Classes/classMaths.md#function-~maths)**() =default |

## Detailed Description

```cpp
class Maths;
```

Funciones matematicas. 



```
Clase que implementa diferentes funciones y utilidades matematicas.
```

## Public Functions Documentation

### function Abs

```cpp
template <class T >
static inline T Abs(
    T a
)
```

Devuelve el valor absoluto de a. 

**Parameters**: 

  * **a** - numero con el que operar. 


### function Ceil

```cpp
static inline float Ceil(
    float a
)
```

Devuelve a redondeado hacia arriba. 

**Parameters**: 

  * **a** - numero con el que operar. 


### function Clamp

```cpp
template <class T >
static inline T Clamp(
    T a,
    T min,
    T max
)
```

Devuelve el valor saturado entre minimo y maximo. 

**Parameters**: 

  * **a** - numero con el que operar. 
  * **min** - cota minima. 
  * **max** - cota maxima. 


### function Cos

```cpp
static inline float Cos(
    float a
)
```

Devuelve el coseno del angulo a en radianes. 

**Parameters**: 

  * **a** - angulo en radianes. 


### function Exp

```cpp
static inline float Exp(
    float p
)
```

Devuelve e elevado a p. 

**Parameters**: 

  * **p** - potencia. 


### function Floor

```cpp
static inline float Floor(
    float a
)
```

Devuelve a redondeado hacia abajo. 

**Parameters**: 

  * **a** - numero con el que operar. 


### function Lerp

```cpp
static inline float Lerp(
    float a,
    float b,
    float t
)
```

Interpola linealmente entre a y b en funcion de t. 

**Parameters**: 

  * **a** - inicio de la interpolacion. 
  * **b** - final de la interpolacion. 
  * **t** - factor de interpolacion en el rango [0,1]. 




```
El parametro t esta en el rango [0,1] 
Cuando t = 0 devuelve a.
Cuando t = 1 devuelve b.
Cuando t = 0.5 devuelve el valor intermedio entre a y b.
```


### function Log

```cpp
static inline float Log(
    float a,
    float b
)
```

Devuelve el logaritmo de a en base b. 

**Parameters**: 

  * **a** - numero con el que operar. 
  * **b** - base del logaritmo. 


### function Log10

```cpp
static inline float Log10(
    float a
)
```

Devuelve el logaritmo de a en base 10. 

**Parameters**: 

  * **a** - numero con el que operar. 


### function Maths

```cpp
Maths() =default
```


### function Max

```cpp
template <class T >
static inline T Max(
    T a,
    T b
)
```

Devuelve el maximo entre a y b. 

**Parameters**: 

  * **a** - numero a comparar. 
  * **b** - numero a comparar. 


### function Min

```cpp
template <class T >
static inline T Min(
    T a,
    T b
)
```

Devuelve el minimo entre a y b. 

**Parameters**: 

  * **a** - numero a comparar. 
  * **b** - numero a comparar. 


### function Pow

```cpp
static inline float Pow(
    float a,
    float p
)
```

Devuelve a elevado a p. 

**Parameters**: 

  * **a** - numero que elevar. 
  * **p** - potencia. 


### function Sin

```cpp
static inline float Sin(
    float a
)
```

Devuelve el seno del angulo a en radianes. 

**Parameters**: 

  * **a** - angulo en radianes. 


### function Sqrt

```cpp
template <class T >
static inline float Sqrt(
    T a
)
```

Devuelve la raiz cuadrada de a. 

**Parameters**: 

  * **a** - numero con el que operar. 


### function Tan

```cpp
static inline float Tan(
    float a
)
```

Devuelve la tangente del angulo a en radianes. 

**Parameters**: 

  * **a** - angulo en radianes. 


### function ~Maths

```cpp
~Maths() =default
```


-------------------------------

Updated on 2026-02-25 at 11:59:32 +0100