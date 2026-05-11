/*
 * @file
 * @brief Componente que gestiona una camara.
 */
#pragma once
#include <Component.h>
#include <Color.h>
#include <EngineAPI.h>

#include "PhysicsTypes.h"

using cameraID = uint64_t;

/*
 * +--------+
 * | CAMERA |
 * +--------+
 *
 * --- Ejemplo de uso en lua ---
 * Camera = {
 *		FOVy = float,
 *		near clip distance = float,
 *		far clip distance = float,
 *		focal length = float,
 *		background color = Color.new(float...)
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/
class ENGINE_API Camera : public core::Component
{
	cameraID _cameraID;

	float _FOVy;
	float _nearClipDistance;
	float _farClipDistance;
	float _focalLength;
	core::Color _bgColor;

public:
	Camera();
	~Camera();

	bool init(const Properties& p) override;
	void ready() override;

	void setAsActiveCamera();

	void setFOVy(const float& FOVy);
	void setNearClipDistance(const float& nearClipDistance);
	void setFarClipDistance(const float& farClipDistance);
	void setFocalLength(const float& focalLength);
	void setBgColor(const core::Color& bgColor);

	float getFOVy() const;
	float getNearClipDistance() const;
	float getFarClipDistance() const;
	float getFocalLength() const;
	core::Color getBgColor() const;
	
	/**
	 * @brief Convierte coordenadas de pantalla a coordenadas mundiales con referencia de la camara.
	 * Pasamos alto y ancho de la ventana por si hay 2 viewports.
	 * @param screenPos - Posicion del punto de pantalla.
	 * @param screenWidth - Ancho de la ventana.
	 * @param screenHeight - Alto de la ventana.
	 * @param outRayDir - Direccion a la que apuntar si se quiere lanzar un rayo a este punto.
	 * @return core::Vector3<> - Coordenadas en el mundo.
	 */
	core::Vector3<> screenToWorld(
		core::Vector2<>& screenPos,
		float screenWidth,
		float screenHeight,
		core::Vector3<>&outRayDir
	) const;

	/**
	 * @brief Convierte un punto del mundo a coordenadas de pantalla.
	 * Pasamos alto y ancho de la ventana por si hay 2 viewports.
	 * @param worldPos - Posicion en coordenadas de mundo.
	 * @param screenWidth - Ancho de la ventana.
	 * @param screenHeight - Alto de la ventana.
	 * @return core::Vector2<> - Posicion en pantalla (0,0 en esquina superior izquierda)
	 *                   o Vector2(-1,-1) si el punto esta detras de la camara.
	 */
	core::Vector2<> worldToScreen(
		const core::Vector3<>& worldPos,
		float screenWidth,
		float screenHeight
	) const;
	void destroy() override;
};

