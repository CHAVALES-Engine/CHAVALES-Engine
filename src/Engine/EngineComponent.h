#pragma once
#include "Component.h"
#include "EngineAPI.h"

class PlatformModule;
class RenderModule;
class AudioModule;
class PhysicsModule;

/**
 * @brief Clase componente para componentes internos,
 * Permite el acceso a los metodos privados de los que no debe disponer un componente de usuario final
 */
class ENGINE_API EngineComponent : public core::Component
{
protected:
	PlatformModule* platform()const;
	RenderModule* render()const;
	AudioModule* audio()const;
	PhysicsModule* physics() const;
};

