/*
 * @file
 * @brief Componente que gestiona una camara.
 */
#pragma once
#include <Component.h>
#include <Vector3.h>
#include <Color.h>

using cameraID = uint64_t;

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
	~Camera() ;

	bool init(const Properties& p) override;

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
};

