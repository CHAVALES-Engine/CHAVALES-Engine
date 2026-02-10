## Descripción:
En esta capa se contienen definiciones y sistemas Core como pueden ser matemáticas, Colores, api global...

---
# EngineAPI
La EngineAPI es un struct que contiene las referencias a los diferentes módulos para ser usados por los desarrolladores.

```
struct EngineAPI {
	RenderModule* render;
	PlatformModule* platform;
	PhysicsModule* physics;
	AudioModule* render;
	ResourcesModule* render;
}
```

---
# Matemáticas
# Vector
Clase vector con coordenadas

# Color
Los colores funcionan como un int32 (RGBA) con un rango de 0-255. La clase Color proporciona métodos públicos que ayudan a la gestión de colores
## Métodos públicos:

| Método                                  | Descripción                                             | Devuelve                       |
| --------------------------------------- | ------------------------------------------------------- | ------------------------------ |
| static Color WHITE                      | Devuelve un color blanco                                | Color que representa el blanco |
| static Color RED                        | Devuelve un color rojo                                  | Color que representa el rojo   |
| ...                                     | ...                                                     | ...                            |
| static Color BLACK                      | Devuelve un color negro                                 | Color que representa el negro  |
| static Color Multiply(Color A, Color B) | Devuelve una instancia de Color multiplicando otros dos | Color A * B Multiplicado       |
