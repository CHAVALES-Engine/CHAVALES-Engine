/*
 * @file
 * @brief Componente que gestiona una camara.
 */
#pragma once
#include <Component.h>
#include <Color.h>

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
class Camera : public core::Component
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
	void destroy() override;
};

