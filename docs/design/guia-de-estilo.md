# CHAVALES ENGINE

## Guía de Estilo

Andrés García, Ismael Ortega, Cynthia Tristán, Pablo Iglesias, Iván Palomino, Sergio Naranjo, Jule Page, Nieves Alonso, Daniel Ramos y Javier Gómez

## 0. Motivación

Documento normativo y conciso para mantener las convenciones de estilo a lo largo de todo el motor de manera fácilmente consultable en pro de la **legibilidad, mantenibilidad y usabilidad del código en C++**.

## Índice

1. Idioma
2. Comentarios
3. Nombres
   * 3.1 Clases, estructuras y enumerados
   * 3.2 Funciones
   * 3.3 Variables
   * 3.4 Constantes
   * 3.5 Claves de recursos
   * 3.6 Espacios de nombres
4. Indentaciones
5. Directivas de compilación
6. General
7. Referencias

## 1. Idioma

La lengua usada para los comentarios será el español para mejor entendimiento, para todo lo demás será el inglés para mejor estándar.

* **Comentarios:** español
* **Resto:** inglés

## 2. Comentarios

### 2.1 Formato

Estándar **C++ / Doxygen**

### 2.2 Ejemplo

```cpp
/**
 * @file 
 * @brief Frase corta descriptiva del método (opcional)
 *
 * Descripción detallada
 *
 * @param argumento - Descripción del parámetro
 * @note Nota aclaratoria (opcional)
 * @warning Aviso (opcional)
 * @return descripción del retorno
 */
void* funcion(int argumento) { … }
```

## 3. Nombres

### 3.1 Clases, estructuras y enumerados

* **Formato:** PascalCase
* **Tipo de palabra(s):** sustantivos

**Ejemplo:**

```cpp
class ExampleComponent;
struct DataPackage;
enum RenderFlags;
```

### 3.2 Funciones

* **Formato:** camelCase, barra baja (`_`) como prefijo si son privadas
* **Tipo de palabra(s):** verbos o frases verbales

**Ejemplo:**

```cpp
void update(float deltaTime) { … }
int _sumar(int a, int b) { … }
```

---

### 3.3 Variables

* **Formato:** camelCase, barra baja (`_`) como prefijo si son privadas
* **Tipo de palabra(s):** nombres descriptivos

  * sin abreviaturas

**Ejemplo:**

```cpp
public:
Transform transform;

private:
bool _isRed;
```

### 3.4 Constantes

* **Formato:** SCREAMING_SNAKE_CASE
* **Tipo de palabra(s):** sustantivos

**Ejemplo:**

```cpp
const int MAX_ENTITIES = 10000;
constexpr float MAX_DELTA = 0.33f;
```

### 3.5 Claves de recursos

* **Formato:** SCREAMING_SNAKE_CASE
* **Tipo de palabra(s):** sustantivos descritos

**Ejemplo:**

```cpp
loadTexture("RICE");
```

### 3.6 Espacios de nombres

* **Formato:** lowercase

  * una única palabra
  * posible abreviación
* **Tipo de palabra(s):** sustantivos

**Ejemplo:**

```cpp
namespace engine {
namespace ecs {

class Test;

}
}

engine::ecs::Test;
```

## 4. Indentaciones

* **Formato:** Allman / K&R

**Ejemplo:**

```cpp
int foo(int a, int b) // indentación Allman
{
    a += b;
    return a;
}
```

```cpp
int foo(int a, int b) { // indentación K&R
    a += b;
    return a;
}
```

## 5. Directivas de compilación

`#pragma once` por definición general, exceptuando casos en los que un mismo archivo pueda ser incluido por múltiples rutas o sea dependiente de reinclusión, en los que se usará:

```cpp
#ifndef
#define
#endif
```

## 6. General

* Indentaciones siempre por **tabulaciones**.
* Separar operadores del resto de la sentencia por un espacio.
* Uso explícito de los *namespaces* sin declaración `using`.

**Ejemplo:**

```cpp
int main()
{
    std::cout << "Hello World!\n";
}
```

## 7. Referencias

* [cppreference — Identifiers](https://en.cppreference.com/w/cpp/language/identifiers.html)
* [cpp-coding-guidelines](https://github.com/kmhofmann/cpp-coding-guidelines)
