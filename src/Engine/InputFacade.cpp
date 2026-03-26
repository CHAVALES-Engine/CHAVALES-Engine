#include "InputFacade.h"
#include <PlatformModule.h>

InputFacade::InputFacade(PlatformModule* platform): _platform(platform)
{}

//------ Raw InputFacade
bool InputFacade::isKeyPressed(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->isKeyPressed(inputAction, device);
}

bool InputFacade::isKeyReleased(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->isKeyReleased(inputAction, device);
}

float InputFacade::getAxis(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->getAxis(inputAction, device);
}

bool InputFacade::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionPressed(actionName, device);
}

bool InputFacade::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionReleased(actionName, device);
}

void InputFacade::startTextInput() const
{
	_platform->startTextInput();
}

void InputFacade::stopTextInput() const
{
	_platform->stopTextInput();
}

std::string InputFacade::getTextInput(input::DeviceID device) const
{
	return _platform->getTextInput(device);
}

//------Metodos de InputMapper:
void InputFacade::addEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id)
{
	_platform->getInputMapper()->addEvent(actionName, inputEvent, id);
}

void InputFacade::removeEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id)
{
	_platform->getInputMapper()->removeEvent(actionName, inputEvent, id);
}

void InputFacade::removeEvents(const std::string& actionName)
{
	_platform->getInputMapper()->removeEvents(actionName);
}

void InputFacade::removeEventsFromID(const std::string& actionName, input::DeviceID id)
{
	_platform->getInputMapper()->removeEventsFromID(actionName, id);
}

std::vector<input::InputEvent> InputFacade::getInputEvents(const std::string& actionName, input::DeviceID id)
{
	return _platform->getInputMapper()->getInputEvents(actionName, id);
}

std::vector<std::string> InputFacade::getActions()
{
	return _platform->getInputMapper()->getActions();
}

bool InputFacade::hasAction(const std::string& actionName) const
{
	return _platform->getInputMapper()->hasAction(actionName);
}