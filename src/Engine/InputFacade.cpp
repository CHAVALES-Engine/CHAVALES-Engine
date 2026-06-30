#include "InputFacade.h"
#include <PlatformModule.h>
#include "checkMLNew.h"
#include "ScriptsManager.h"

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
std::pair<bool,input::DeviceID> InputFacade::isKeyPressedWithDevice(input::InputEvent inputAction) const
{
	return _platform->isKeyPressedWithDevice(inputAction);
}

bool InputFacade::isJustPressed(input::InputEvent inputEvent, input::DeviceID device) const
{
	return _platform->isJustPressed(inputEvent, device);
}
std::pair<bool, input::DeviceID> InputFacade::isJustPressedWithDevice(input::InputEvent inputEvent) const
{
	return _platform->isJustPressedWithDevice(inputEvent);
}

bool InputFacade::isKeyReleased(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->isKeyReleased(inputAction, device);
}
std::pair<bool, input::DeviceID> InputFacade::isKeyReleasedWithDevice(input::InputEvent inputAction) const
{
	return _platform->isKeyReleasedWithDevice(inputAction);
}

float InputFacade::getAxis(input::InputEvent inputAction, input::DeviceID device) const
{
	return _platform->getAxis(inputAction, device);
}
std::pair<float, input::DeviceID> InputFacade::getAxisWithDevice(input::InputEvent inputAction) const
{
	return _platform->getAxisWithDevice(inputAction);
}

bool InputFacade::isActionPressed(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionPressed(actionName, device);
}
std::pair<bool, input::DeviceID> InputFacade::isActionPressedWithDevice(const std::string& actionName) const
{
	return _platform->isActionPressedWithDevice(actionName);
}

bool InputFacade::isActionJustPressed(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionJustPressed(actionName, device);
}
std::pair<bool, input::DeviceID> InputFacade::isActionJustPressedWithDevice(const std::string& actionName) const
{
	return _platform->isActionJustPressedWithDevice(actionName);
}

bool InputFacade::isActionReleased(const std::string& actionName, input::DeviceID device) const
{
	return _platform->isActionReleased(actionName, device);
}
std::pair<bool, input::DeviceID> InputFacade::isActionReleasedWithDevice(const std::string& actionName) const
{
	return _platform->isActionReleasedWithDevice(actionName);
}

float InputFacade::getActionAxis(const std::string& actionName, input::DeviceID device) const
{
	return _platform->getActionAxis(actionName, device);
}
std::pair<float, input::DeviceID> InputFacade::getActionAxisWithDevice(const std::string& actionName) const
{
	return _platform->getActionAxisWithDevice(actionName);
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

void InputFacade::backspaceTextInput(input::DeviceID device) const
{
	_platform->backspaceTextInput();
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

void InputFacade::setGamepadColor(input::DeviceID id, const core::Color& color)
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

void InputFacade::_registerScriptBindings()
{
	auto& sm = ScriptsManager::instance();

	// Helper local: convierte un Property a input::InputEvent.
	// InputEvent es un variant, no un enum, asi que no se puede castear directo.
	auto getEventArg = [](const Property& p) -> input::InputEvent {
		return std::visit(input::overloaded{
			[](input::Key k)            -> input::InputEvent { return k; },
			[](input::MouseButton mb)   -> input::InputEvent { return mb; },
			[](input::MouseAxis ma)     -> input::InputEvent { return ma; },
			[](input::GamepadButton gb) -> input::InputEvent { return gb; },
			[](input::GamepadAxis ga)   -> input::InputEvent { return ga; },
			[](int i) -> input::InputEvent {
				// Decodificar segun rango
				if (i < 1000) return static_cast<input::Key>(i);
				if (i < 2000) return static_cast<input::MouseButton>(i - 1000);
				if (i < 3000) return static_cast<input::MouseAxis>(i - 2000);
				if (i < 4000) return static_cast<input::GamepadButton>(i - 3000);
				return static_cast<input::GamepadAxis>(i - 4000);
			},
			[](auto&&) -> input::InputEvent {
				Debug::error("Tipo invalido pasado a InputEvent");
				return input::KEY_NONE;
			}
			}, p);
		};

	sm.bindMethodImpl("InputFacade", "setRelativeMouseMode",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->setRelativeMouseMode(ScriptsManager::instance().getArg<bool>(a[0]));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "isDeviceConnected",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<InputFacade*>(o)->isDeviceConnected(ScriptsManager::instance().getArg<int>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isKeyPressed",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isKeyPressed(getEventArg(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isKeyPressedWithDevice",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isKeyPressedWithDevice(getEventArg(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isJustPressed",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isJustPressed(getEventArg(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isJustPressedWithDevice",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isJustPressedWithDevice(getEventArg(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isKeyReleased",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isKeyReleased(getEventArg(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isKeyReleasedWithDevice",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isKeyReleasedWithDevice(getEventArg(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "getAxis",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->getAxis(getEventArg(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "getAxisWithDevice",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->getAxisWithDevice(getEventArg(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isActionPressed",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionPressed(std::get<std::string>(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isActionPressedWithDevice",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionPressedWithDevice(std::get<std::string>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isActionJustPressed",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionJustPressed(std::get<std::string>(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isActionJustPressedWithDevice",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionJustPressedWithDevice(std::get<std::string>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "isActionReleased",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionReleased(std::get<std::string>(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "isActionReleasedWithDevice",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->isActionReleasedWithDevice(std::get<std::string>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "getActionAxis",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->getActionAxis(std::get<std::string>(a[0]), dev));
		});
	sm.bindMethodImpl("InputFacade", "getActionAxisWithDevice",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->getActionAxisWithDevice(std::get<std::string>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "startTextInput",
		[](void* o, const std::vector<Property>& a) -> Property {
			bool block = (a.size() > 0) ? std::get<bool>(a[0]) : true;
			static_cast<InputFacade*>(o)->startTextInput(block);
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "stopTextInput",
		[](void* o, const std::vector<Property>&) -> Property {
			static_cast<InputFacade*>(o)->stopTextInput();
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "getTextInput",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 0) ? std::get<int>(a[0]) : input::ANY_DEVICE;
			return Property(static_cast<InputFacade*>(o)->getTextInput(dev));
		});

	sm.bindMethodImpl("InputFacade", "clearTextInput",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 0) ? std::get<int>(a[0]) : input::ANY_DEVICE;
			static_cast<InputFacade*>(o)->clearTextInput(dev);
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "getWindowWidth",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<InputFacade*>(o)->getWindowWidth());
		});

	sm.bindMethodImpl("InputFacade", "getWindowHeight",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<InputFacade*>(o)->getWindowHeight());
		});

	sm.bindMethodImpl("InputFacade", "setWindowSize",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->setWindowSize(ScriptsManager::instance().getArg<int>(a[0]), ScriptsManager::instance().getArg<int>(a[1]));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "setIcon",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<InputFacade*>(o)->setIcon(ScriptsManager::instance().getArg<std::string>(a[0])));
		});

	sm.bindMethodImpl("InputFacade", "setWindowName",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->setWindowName(ScriptsManager::instance().getArg<std::string>(a[0]));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "setGamepadVibration",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->setGamepadVibration(
				ScriptsManager::instance().getArg<int>(a[0]),
				ScriptsManager::instance().getArg<float>(a[1]),
				ScriptsManager::instance().getArg<float>(a[2]),
				static_cast<uint32_t>(ScriptsManager::instance().getArg<int>(a[3])));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "setGamepadColor",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->setGamepadColor(
				ScriptsManager::instance().getArg<int>(a[0]), ScriptsManager::instance().getArg<core::Color>(a[1]));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "addEventToAction",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 2) ? std::get<int>(a[2]) : input::ANY_DEVICE;
			static_cast<InputFacade*>(o)->addEventToAction(
				std::get<std::string>(a[0]), getEventArg(a[1]), dev);
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "removeEvent",
		[getEventArg](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 2) ? std::get<int>(a[2]) : input::ANY_DEVICE;
			static_cast<InputFacade*>(o)->removeEvent(
				std::get<std::string>(a[0]), getEventArg(a[1]), dev);
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "removeEvents",
		[](void* o, const std::vector<Property>& a) -> Property {
			static_cast<InputFacade*>(o)->removeEvents(ScriptsManager::instance().getArg<std::string>(a[0]));
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "removeEventsFromID",
		[](void* o, const std::vector<Property>& a) -> Property {
			input::DeviceID dev = (a.size() > 1) ? std::get<int>(a[1]) : input::ANY_DEVICE;
			static_cast<InputFacade*>(o)->removeEventsFromID(std::get<std::string>(a[0]), dev);
			return Property(0);
		});

	sm.bindMethodImpl("InputFacade", "getActions",
		[](void* o, const std::vector<Property>&) -> Property {
			return Property(static_cast<InputFacade*>(o)->getActions());
		});

	sm.bindMethodImpl("InputFacade", "hasAction",
		[](void* o, const std::vector<Property>& a) -> Property {
			return Property(static_cast<InputFacade*>(o)->hasAction(ScriptsManager::instance().getArg<std::string>(a[0])));
		});

	sm.bindGlobalImpl("Input", "InputFacade", this);
}

