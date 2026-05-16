#include "InputFacade.h"
#include <PlatformModule.h>
#include "checkMLNew.h"

InputFacade::InputFacade(PlatformModule* platform) : _platform(platform)
{
}

void InputFacade::setRelativeMouseMode(bool enabled) const
{
	_platform->setRelativeMouseMode(enabled);
}

bool InputFacade::isDeviceConnected(input::DeviceID device) const
{
	return _platform->isDeviceConnected(device);
}

//------ Raw InputFacade
bool InputFacade::isKeyPressed(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->isKeyPressed(inputAction, device);
}

bool InputFacade::isJustPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _platform->isJustPressed(inputEvent, device);
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

bool InputFacade::isActionJustPressed(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionJustPressed(actionName, device);
}

bool InputFacade::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionReleased(actionName, device);
}

float InputFacade::getActionAxis(const std::string& actionName, input::DeviceID device) const
{
	return _platform->getActionAxis(actionName, device);
}

void InputFacade::startTextInput(bool blockKeyboard) const
{
	_platform->startTextInput(blockKeyboard);
}

void InputFacade::stopTextInput() const
{
	_platform->stopTextInput();
}

std::string InputFacade::getTextInput(input::DeviceID device) const
{
	return _platform->getTextInput(device);
}

void InputFacade::clearTextInput(input::DeviceID device) const
{
	_platform->clearTextInput(device);
}

int  InputFacade::getWindowWidth() const
{
	return _platform->getWindowWidth();
}

int InputFacade::getWindowHeight() const
{
	return _platform->getWindowHeight();
}

void InputFacade::setWindowSize(int w, int h)
{
	_platform->setWindowSize(w, h);
}

bool InputFacade::setIcon(const std::string& path)
{
	return _platform->setIcon(path);
}

void InputFacade::setWindowName(const std::string& name)
{
	_platform->setWindowName(name);
}

void InputFacade::setGamepadVibration(input::DeviceID id, float lowFreq, float highFreq, uint32_t dur)
{
	_platform->setGamepadVibration(id, lowFreq, highFreq, dur);
}

void InputFacade::setGamepadColor(input::DeviceID id, core::Color color)
{
	_platform->setGamepadColor(id, color);
}

void InputFacade::setGamepadColor(input::DeviceID id, uint8_t r, uint8_t g, uint8_t b)
{
	_platform->setGamepadColor(id, r, g, b);
}

//------Metodos de InputMapper:
void InputFacade::addEventToAction(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id) const
{
	_platform->getInputMapper()->addEventToAction(actionName, inputEvent, id);
}

void InputFacade::removeEvent(const std::string& actionName, input::InputEvent inputEvent, input::DeviceID id) const
{
	_platform->getInputMapper()->removeEvent(actionName, inputEvent, id);
}

void InputFacade::removeEvents(const std::string& actionName) const
{
	_platform->getInputMapper()->removeEvents(actionName);
}

void InputFacade::removeEventsFromID(const std::string& actionName, input::DeviceID id) const
{
	_platform->getInputMapper()->removeEventsFromID(actionName, id);
}

std::vector<input::InputEvent> InputFacade::getInputEvents(const std::string& actionName, input::DeviceID id) const
{
	return _platform->getInputMapper()->getInputEvents(actionName, id);
}

std::vector<std::string> InputFacade::getActions() const
{
	return _platform->getInputMapper()->getActions();
}

bool InputFacade::hasAction(const std::string& actionName) const
{
	return _platform->getInputMapper()->hasAction(actionName);
}
