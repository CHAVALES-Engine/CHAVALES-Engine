#include "Camera.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

#include "GameConfigurator.h"


REGISTER_COMPONENT(Camera);

Camera::Camera() : _FOVy(45.0f), _nearClipDistance(0.1f), _farClipDistance(1000.0f), _focalLength(1.0f), _bgColor(0.0f, 0.0f, 0.0f, 1.0f) {}

Camera::~Camera()
{
	//Engine::deleteCamera(_cameraID);
}

bool Camera::init(const Properties& p)
{
	_FOVy = getProperty<float>(p, "FOVy");
	_nearClipDistance = getProperty<float>(p, "nearPlane");
	_farClipDistance = getProperty<float>(p, "farPlane");
	_focalLength = getProperty<float>(p, "focal length");
	if (!setProperty(p, "background color", _bgColor))
		_bgColor = core::GameConfigurator::instance()._clearColor;



	_cameraID = Engine::instance()->addCamera(getEntity()->getEntityID(), _FOVy, _nearClipDistance, _farClipDistance, _focalLength, _bgColor);
	return true;
}

void Camera::setAsActiveCamera()
{
	//Engine::setAsActiveCamera(_cameraID);
	//Engine::setCameraBgColor(_cameraID, _bgColor);
}

void Camera::setFOVy(const float& FOVy)
{
	_FOVy = FOVy;
	//Engine::setCameraFOVy(_cameraID, _FOVy);
}

void Camera::setNearClipDistance(const float& nearClipDistance)
{
	_nearClipDistance = nearClipDistance;
	//Engine::setCameraNearClipDistance(_cameraID, _nearClipDistance);
}

void Camera::setFarClipDistance(const float& farClipDistance)
{
	_farClipDistance = farClipDistance;
	//Engine::setCameraFarClipDistance(_cameraID, _farClipDistance);
}

void Camera::setFocalLength(const float& focalLength)
{
	_focalLength = focalLength;
	//Engine::setCameraFocalLength(_cameraID, _focalLength);
}

void Camera::setBgColor(const core::Color& bgColor)
{
	_bgColor = bgColor;
	//Engine::setCameraBgColor(_cameraID, _bgColor);
}

float Camera::getFOVy() const { return _FOVy; }

float Camera::getNearClipDistance() const { return _nearClipDistance; }

float Camera::getFarClipDistance() const { return _farClipDistance; }

float Camera::getFocalLength() const { return _focalLength; }

core::Color Camera::getBgColor() const { return _bgColor; }
