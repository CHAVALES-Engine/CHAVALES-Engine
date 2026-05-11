#include "EngineComponent.h"
#include "Engine.h"

PlatformModule* EngineComponent::platform() const
{
	return Engine::_instance->_platformModule;
}

RenderModule* EngineComponent::render() const
{
	return Engine::_instance->_renderModule;
}

AudioModule* EngineComponent::audio() const
{
	return Engine::_instance->_audioModule;
}

PhysicsModule* EngineComponent::physics() const
{
	return Engine::_instance->_physicsModule;
}
