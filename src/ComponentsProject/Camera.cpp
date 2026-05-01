#include "Camera.h"

#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

#include "GameConfigurator.h"
#include "checkMLNew.h"
#include "Transform.h"


REGISTER_COMPONENT(Camera);

Camera::Camera() : _FOVy(45.0f), _nearClipDistance(0.1f), _farClipDistance(1000.0f), _focalLength(1.0f), _bgColor(0.0f, 0.0f, 0.0f, 1.0f)
{
	registerMethod("setFOVy", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setFOVy(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setNearClipDistance", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setNearClipDistance(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setFarClipDistance", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setFarClipDistance(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setFocalLength", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setFocalLength(std::any_cast<float>(args[0]));
		}
		});

	registerMethod("setBgColor", [this](const std::vector<std::any>& args) {
		if (args.size() >= 1) {
			setBgColor(std::any_cast<core::Color>(args[0]));
		}
		});

	registerMethod("getFOVy", [this](const std::vector<std::any>& args) {
		return getFOVy();
		});

	registerMethod("getNearClipDistance", [this](const std::vector<std::any>& args) {
		return getNearClipDistance();
		});

	registerMethod("getFarClipDistance", [this](const std::vector<std::any>& args) {
		return getFarClipDistance();
		});

	registerMethod("getFocalLength", [this](const std::vector<std::any>& args) {
		return getFocalLength();
		});

	registerMethod("getBgColor", [this](const std::vector<std::any>& args) {
		return getBgColor();
		});
}

Camera::~Camera()
{
	Engine::instance()->deleteCamera(_cameraID);
}

bool Camera::init(const Properties& p)
{
	_FOVy = getProperty<float>(p, "FOVy");
	_nearClipDistance = getProperty<float>(p, "nearPlane");
	_farClipDistance = getProperty<float>(p, "farPlane");
	_focalLength = getProperty<float>(p, "focal length");
	if (!setProperty(p, "background color", _bgColor))
		_bgColor = core::GameConfigurator::instance()._clearColor;

	return true;
}

void Camera::ready()
{
	_cameraID = Engine::instance()->addCamera(getEntity()->getEntityID(), _FOVy, _nearClipDistance, _farClipDistance, _focalLength, _bgColor);
}

void Camera::setAsActiveCamera()
{
	Engine::instance()->setAsActiveCamera(_cameraID);
	Engine::instance()->setViewportBGColor(_bgColor);
}

void Camera::setFOVy(const float& FOVy)
{
	_FOVy = FOVy;
	Engine::instance()->setCameraFOVy(_cameraID, _FOVy);
}

void Camera::setNearClipDistance(const float& nearClipDistance)
{
	_nearClipDistance = nearClipDistance;
	Engine::instance()->setCameraNearClipDistance(_cameraID, _nearClipDistance);
}

void Camera::setFarClipDistance(const float& farClipDistance)
{
	_farClipDistance = farClipDistance;
	Engine::instance()->setCameraFarClipDistance(_cameraID, _farClipDistance);
}

void Camera::setFocalLength(const float& focalLength)
{
	_focalLength = focalLength;
	Engine::instance()->setCameraFocalLength(_cameraID, _focalLength);
}

void Camera::setBgColor(const core::Color& bgColor)
{
	_bgColor = bgColor;
	Engine::instance()->setViewportBGColor(_bgColor);
}

float Camera::getFOVy() const { return _FOVy; }

float Camera::getNearClipDistance() const { return _nearClipDistance; }

float Camera::getFarClipDistance() const { return _farClipDistance; }

float Camera::getFocalLength() const { return _focalLength; }

core::Color Camera::getBgColor() const { return _bgColor; }

core::Vector3<> Camera::screenToWorld(core::Vector2<>& screenPos, float screenWidth, float screenHeight,
	core::Vector3<>& outRayDir) const
{
	std::shared_ptr<Transform> transform = getEntity()->getComponent<Transform>();
	if (!transform) return {};

	// Pantalla -> NDC [-1, 1]
	float ndc_x = (2.0f * screenPos.getX()) / screenWidth - 1.0f;
	float ndc_y = 1.0f - (2.0f * screenPos.getY()) / screenHeight;

	// NDC -> direccion en espacio de vista
	float aspect = screenWidth / screenHeight;
	float halfH = std::tan(_FOVy / 2.0f);
	float halfW = halfH * aspect;

	outRayDir = core::Vector3<>(ndc_x * halfW, ndc_y * halfH, -1.0f);

	// Normalizar
	float length = std::sqrt(outRayDir.getX() * outRayDir.getX() +
		outRayDir.getY() * outRayDir.getY() +
		outRayDir.getZ() * outRayDir.getZ());
	outRayDir = core::Vector3<>(outRayDir.getX() / length, outRayDir.getY() / length, outRayDir.getZ() / length);

	// Vista -> mundo
	outRayDir = transform->getGlobalRotation() * outRayDir;

	// Retorna el origen (posicion de la camara)
	return transform->getGlobalPosition();
}

core::Vector2<> Camera::worldToScreen(const core::Vector3<>& worldPos, float screenWidth, float screenHeight) const
{
	std::shared_ptr<Transform> transform = getEntity()->getComponent<Transform>();
	if (!transform)	return { -1,-1 };

	// Obtener posicion de la camara
	core::Vector3<> cameraPos = transform->getGlobalPosition();

	// Posicion relativa a la camara
	core::Vector3<> relativePos(
		worldPos.getX() - cameraPos.getX(),
		worldPos.getY() - cameraPos.getY(),
		worldPos.getZ() - cameraPos.getZ()
	);

	core::Vector3<> cameraPosSpace = transform->getGlobalRotation().inversed() * relativePos;

	// Calcular la proyeccion perspectiva
	float aspect = screenWidth / screenHeight;

	float height = 2.0f * std::tan(_FOVy / 2.0f);
	float width = height * aspect;

	// Proyectar al plano de vista
	float distance = -cameraPosSpace.getZ();
	float planeHeight = 2.0f * distance * std::tan(_FOVy / 2.0f);
	float planeWidth = planeHeight * aspect;

	// Coordenadas normalizadas en el plano de vista [-1, 1]
	float ndcX = cameraPosSpace.getX() / (planeWidth / 2.0f);
	float ndcY = cameraPosSpace.getY() / (planeHeight / 2.0f);

	// Convertir a coordenadas de pantalla
	return core::Vector2<>(
		(ndcX + 1.0f) / 2.0f * screenWidth,
		(1.0f - ndcY) / 2.0f * screenHeight);
}

void Camera::destroy()
{
	Engine::instance()->deleteCamera(_cameraID);
}
