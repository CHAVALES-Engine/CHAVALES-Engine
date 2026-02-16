# CHAVALES ENGINE

## Planificación

Andrés García, Ismael Ortega, Cynthia Tristán, Pablo Iglesias, Iván Palomino, Sergio Naranjo, Jule Page, Nieves Alonso, Daniel Ramos y Javier Gómez

## 0. Motivación

Documento que recaba la planificación diseñada para el desarrollo del motor, buscando reducir incertidumbre y avanzar de manera estructural a través de cada hito.

## 1. Planificación general

El concepto principal sobre el que se planea es a través de **“capas”** y toma de decisiones: se prioriza la **validación temprana** del diseño arquitectónico básico del motor y a partir de ahí se continúa a la siguiente unidad de decisión estructural básica o **“capa”**. El **desarrollo general** es más importante que el de los detalles.

Lo que se busca con esto es equivocarse lo antes posible si es caso para poder alertar de problemas y **rectificar** en errores de diseño o implementación cuanto antes, minimizando así trabajo potencialmente perdido.

Previo a la finalización de cada hito se planea un **análisis** y eliminación global de **líneas rojas**, e inmediatamente posterior a cada hito se hará una retrospectiva rápida de lo que ha ido bien, lo que ha ido mal y **reevaluación** de las prioridades de cara al siguiente hito.

Durante todo el desarrollo del proyecto se llevarán a cabo **weeklies** a principio de semana, principalmente los martes, para poner en conjunto el progreso de cada equipo, planificar las siguientes tareas y reorganizar lo que se necesite. Además, por el **servidor de Discord** tendremos a disposición diversos canales para el intercambio de ideas, información y recursos, especialmente un canal de **changelog** en el que se publicará cada avance o cambio importante que tenga lugar durante el desarrollo.

En el caso de necesitar ayuda o una reorganización, ya sea porque un equipo ha finalizado sus tareas o algo no está funcionando, se comunicará al resto del equipo de inmediato para poner solución lo antes posible. El objetivo es una **comunicación fluida** que permita el correcto desarrollo del proyecto, adaptándose a las situaciones que se den y con plena **transparencia**.

## 2. Software y plataformas

Para todo este proyecto el software que se utilizará será:

* Visual Studio para toda la creación de código y del proyecto
* GitHub para los repositorios de los juegos y el motor
* GitHub Projects para la gestión de tareas
* Doxygen para la automatización de la documentación
* Mermaid y Figma para la elaboración de gráficos
* Discord para la comunicación del equipo y reuniones a distancia

Adicionalmente:

* Carpeta Drive para creación de documentos rápidos o intercambio de recursos
* Grupo de WhatsApp para avisos más inmediatos

## 3. Planificación por hitos

### **3.1 Hito 1**

#### 3.1.1 **Fecha**

06/03/2026

#### 3.1.2 **Características esperadas**

* Separación motor / juegos
* Repositorios de motor y juegos
* Compilar Ogre 3D
* Puesta en marcha y bucle principal
* Estructurar proyecto
* Definir estructura EC
* Módulos y juegos como librerías compiladas
* Compilación automática
* Integración con Lua
* Componentes vacíos
* Carga de mapas con esos componentes vacíos
* Carga de esqueletos de los juegos con sus esqueletos de componentes desde mapas

#### 3.1.3 **Desarrollo planeado**

Se organizará el equipo inicialmente en el HITO 1 a través de pequeños grupos y tareas para asentar las bases arquitectónicas:

* Estructuración de la solución y los proyectos
* Núcleo y bucle principal
* Compilación y su automatización
  * Ogre
  * Physx
  * Librerías
    * Motor
    * Juegos
* Arquitectura EC
* Carga de escenas e integración con Lua

### **3.2 Hito 2**

#### 3.2.1 **Fecha**

14/04/2026

#### 3.2.2 **Características esperadas**

* Gran desarrollo de módulos
* Módulos funcionales y bien definidos
* API estabilizada
* Componentes funcionales
* Juegos con componentes reales
* Avances, si viables, en los juegos

#### 3.2.3 **Desarrollo planeado**

Una vez esté la estructura inicializada y el proyecto puesto en marcha, el equipo se dividirá en el HITO 2 en grupos según los módulos principales del motor:

* Render
* Física
* Audio
* Plataforma
* Recursos

Se trabajará en la siguiente capa de cada módulo según el estado de desarrollo hasta acabarlo con un módulo funcional resultante.

### **3.3 Hito 3**

#### 3.3.1 **Fecha**

08/05/2026

#### 3.3.2 **Características esperadas**

* Fin del desarrollo de MATABICHOS
* Fin del desarrollo de TIRABOLAS
* Testing

#### 3.3.3 **Desarrollo planeado**

El equipo se dividirá en dos grupos de cinco personas cada uno para cada juego. Se centrará el desarrollo en acabar los juegos y, probablemente, en arreglar los problemas del motor que idealmente serán localizables y dependientes de un módulo concreto.
