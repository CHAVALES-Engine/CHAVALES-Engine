#include "Camera.h"

#include "RenderModule.h"
#include "Entity.h"
#include "Engine.h"

#include <Debug.h>
#include <PluginSDK.h>

#include "GameConfigurator.h"
#include "Transform.h"
#include "ScriptsManager.h"
#include "checkMLNew.h"

REGISTER_COMPONENT(Camera);

Camera::Camera()
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

	registerMethod("setAsActiveCamera", [this](const std::vector<std::any>& args) {
		setAsActiveCamera();
		});

	registerMethod("screenToWorld", [this](const std::vector<std::any>& args) -> std::any {
		if (args.size() >= 3) {

			core::Vector2<> viewPos = std::any_cast<core::Vector2<>>(args[0]);
			float viewWidth = std::any_cast<float>(args[1]);
			float viewHeight = std::any_cast<float>(args[2]);

			core::Vector3<> outRayDir;
			core::Vector3<> result =
				screenToWorld(viewPos, viewWidth, viewHeight, outRayDir);

			// devolver ambos resultados
			return std::make_any<std::vector<std::any>>(
				std::vector<std::any>{
				result,
					outRayDir
			}
			);
		}

		return std::any{};
		});

	registerMethod("worldToScreen", [this](const std::vector<std::any>& args) -> std::any {
		if (args.size() >= 3) {

			core::Vector3<> worldPos =
				std::any_cast<core::Vector3<>>(args[0]);

			float viewWidth =
				std::any_cast<float>(args[1]);

			float viewHeight =
				std::any_cast<float>(args[2]);

			return std::make_any<core::Vector2<>>(
				worldToScreen(worldPos, viewWidth, viewHeight)
			);
		}

		return std::any{};
		});
}

Camera::~Camera()
{
}

bool Camera::init(const Properties& p)
{
	if (!Component::init(p)) return false;
	if (!setProperty(p, "background color", backgroundColor, false))
		backgroundColor = core::GameConfigurator::instance()._clearColor;

	return true;
}

void Camera::ready()
{
	_cameraID = render()->addCamera(getEntity()->getEntityID(), FOVy, nearClipDistance, farClipDistance, focalLength, backgroundColor);
}

void Camera::setAsActiveCamera()
{
	render()->setAsActiveCamera(_cameraID);
	Engine::instance()->setViewportBGColor(backgroundColor);
}

void Camera::setFOVy(const float& _FOVy)
{
	FOVy = _FOVy;
	render()->setCameraFOVy(_cameraID, FOVy);
}

void Camera::setNearClipDistance(const float& _nearClipDistance)
{
	nearClipDistance = _nearClipDistance;
	render()->setCameraNearClipDistance(_cameraID, nearClipDistance);
}

void Camera::setFarClipDistance(const float& _farClipDistance)
{
	farClipDistance = _farClipDistance;
	render()->setCameraFarClipDistance(_cameraID, farClipDistance);
}

void Camera::setFocalLength(const float& _focalLength)
{
	focalLength = _focalLength;
	render()->setCameraFocalLength(_cameraID, focalLength);
}

void Camera::setBgColor(const core::Color& _bgColor)
{
	backgroundColor = _bgColor;
	render()->setViewportBGColor(backgroundColor);
}

float Camera::getFOVy() const { return FOVy; }

float Camera::getNearClipDistance() const { return nearClipDistance; }

float Camera::getFarClipDistance() const { return farClipDistance; }

float Camera::getFocalLength() const { return focalLength; }

core::Color Camera::getBgColor() const { return backgroundColor; }

core::Vector3<> Camera::screenToWorld(const core::Vector2<>& viewPos, float viewWidth, float viewHeight,
	core::Vector3<>& outRayDir) const
{
	std::shared_ptr<Transform> transform = getEntity()->getComponent<Transform>();
	if (!transform) return {};

	int vpX = 0, vpY = 0, vpW = viewWidth, vpH = viewHeight;
	core::Vector2<> ref = { static_cast<float>(vpW), static_cast<float>(vpH) };
	if (Engine::instance() != nullptr)
	{
		Engine::instance()->getViewportRect(vpX, vpY, vpW, vpH);
		ref = Engine::instance()->getLogicResolution();
	}
	if (vpW <= 0 || vpH <= 0) return {};

	float pixelX = viewPos.getX(), pixelY = viewPos.getY();
	if (ref.getX() > 0 && ref.getY() > 0
		&& pixelX >= 0.0f && pixelY >= 0.0f
		&& pixelX <= ref.getX() && pixelY <= ref.getY())
	{
		pixelX = vpX + pixelX / ref.getX() * vpW;
		pixelY = vpY + pixelY / ref.getY() * vpH;
	}

	const float localX = pixelX - vpX;
	const float localY = pixelY - vpY;

	// Pantalla -> NDC [-1, 1]
	float ndc_x = (2.0f * localX) / vpW - 1.0f;
	float ndc_y = 1.0f - (2.0f * localY) / vpH;

	// NDC -> direccion en espacio de vista
	const float aspect = static_cast<float>(vpW) / static_cast<float>(vpH);
	float halfH = std::tan(FOVy / 2.0f);
	float halfW = halfH * aspect;

	outRayDir = core::Vector3<>(ndc_x * halfW, ndc_y * halfH, -1.0f);

	// Normalizar
	const float length = std::sqrt(outRayDir.getX() * outRayDir.getX() +
		outRayDir.getY() * outRayDir.getY() +
		outRayDir.getZ() * outRayDir.getZ());
	if (length <= 0.0f) return transform->getGlobalPosition();
	outRayDir = core::Vector3<>(outRayDir.getX() / length, outRayDir.getY() / length, outRayDir.getZ() / length);

	// Vista -> mundo
	outRayDir = transform->getGlobalRotation() * outRayDir;

	// Retorna el origen (posicion de la camara)
	return transform->getGlobalPosition();
}

core::Vector2<> Camera::worldToScreen(const core::Vector3<>& worldPos, float viewWidth, float viewHeight) const
{
	std::shared_ptr<Transform> transform = getEntity()->getComponent<Transform>();
	if (!transform)	return { -1,-1 };

	int vpX = 0, vpY = 0, vpW = viewWidth, vpH = viewHeight;
	core::Vector2<> ref = { static_cast<float>(vpW), static_cast<float>(vpH) };
	if (Engine::instance() != nullptr)
	{
		Engine::instance()->getViewportRect(vpX, vpY, vpW, vpH);
		ref = Engine::instance()->getLogicResolution();
	}
	if (vpW <= 0 || vpH <= 0) return { -1,-1 };

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
	const float aspect = static_cast<float>(vpW) / static_cast<float>(vpH);

	float height = 2.0f * std::tan(FOVy / 2.0f);
	float width = height * aspect;

	// Proyectar al plano de vista
	float distance = -cameraPosSpace.getZ();
	float planeHeight = 2.0f * distance * std::tan(FOVy / 2.0f);
	float planeWidth = planeHeight * aspect;

	// Coordenadas normalizadas en el plano de vista [-1, 1]
	float ndcX = cameraPosSpace.getX() / (planeWidth / 2.0f);
	float ndcY = cameraPosSpace.getY() / (planeHeight / 2.0f);

	// Convertir a coordenadas de pantalla
	const float pixelX = vpX + (ndcX + 1.0f) * 0.5f * vpW;
	const float pixelY = vpY + (1.0f - ndcY) * 0.5f * vpH;

	if (ref.getX() <= 0 || ref.getY() <= 0 || vpW <= 0 || vpH <= 0) return { pixelX, pixelY };
	return {
		(pixelX - vpX) / vpW * ref.getX(),
		(pixelY - vpY) / vpH * ref.getY()
	};
}

void Camera::destroy()
{
	render()->deleteCamera(_cameraID);
}