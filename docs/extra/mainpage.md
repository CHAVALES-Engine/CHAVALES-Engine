# ChavalesEngine {#mainpage}

<div align="center">
  <img src="CHAVALES.png" width="750px">
  <h3>Motor de videojuegos desarrollado para Proyectos 3</h3>
  <p><i>Grado en Desarrollo de Videojuegos - Universidad Complutense de Madrid (UCM)</i></p>
</div>

---

## Equipo de Desarrollo

<table style="width:100%; border:none;">
  <tr>
    <td>• Nieves Alonso Gilsanz</td>
    <td>• Andrés García Navarro</td>
  </tr>
  <tr>
    <td>• Javier Gómez Zúñiga</td>
    <td>• Pablo Iglesias Rodrigo</td>
  </tr>
  <tr>
    <td>• Sergio Naranjo Barroso</td>
    <td>• Ismael Ortega Sánchez</td>
  </tr>
  <tr>
    <td>• Iván Palomino Rodríguez</td>
    <td>• Jule Page Galocha</td>
  </tr>
  <tr>
    <td>• Daniel Ramos Sánchez-Manjavacas</td>
    <td>• Cynthia Tristán Álvarez</td>
  </tr>
</table>



---



## Arquitectura del Motor
El motor está diseñado siguiendo una arquitectura modular para facilitar la extensibilidad y el rendimiento.

### Módulos
La base del motor se divide en los siguientes bloques fundamentales:

| Módulo | Descripción |
| :--- | :--- |
| **Core** | Definición de clases básicas y utilidades |
| **Plataforma** | Abstracción de ventana, entrada (input) y eventos del sistema. |
| **Render** | Pipeline gráfico optimizado y gestión de materiales. |
| **Físicas** | Integración de colisiones y dinámicas de cuerpos. |
| **Recursos** | Carga y gestión de assets (texturas, modelos, scripts). |
| **Audio** | Sistema de sonido espacial y gestión de pistas. |

### Engine
* **GameLoader:** Sistema de carga dinámica de escenas.
* **StateMachine:** Control del flujo de estados del juego.
* **ComponentDLLoader:** Carga de lógica de juego y plugins mediante bibliotecas dinámicas (DLLs).
* **ComponentRegistry:** Registro de componentes dinamico, actualizado con cada carga de DLL.
* **Tools:** Conjunto de utilidades para el desarrollo y depuración.

\subpage MotorAPI
\subpage Modulo_Plataforma

> nota