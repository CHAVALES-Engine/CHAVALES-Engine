#include "GameConfigurator.h"

core::GameConfigurator& core::GameConfigurator::instance()
{
	static core::GameConfigurator instance; // Se crea la primera vez, destruye al cerrar.
	return instance;
}

toml::table core::GameConfigurator::Serialize()
{
	return toml::table{ {
		{ "game", toml::table{{
			{ "first_scene", _firstScene },
			{ "game_dll", _gameDLL }
		}}},
		{ "window", toml::table{{
			{ "window_name", _windowName },
			{ "icon_root", _iconRoot },

			{ "clear_color_r", _clearColor.getRed() },
			{ "clear_color_g", _clearColor.getGreen() },
			{ "clear_color_b", _clearColor.getBlue() },

			{ "window_width", _windowWidth },
			{ "window_height",_windowHeight }
		}}}
	} };
}

void core::GameConfigurator::Deserialize(const toml::table& data)
{
	_firstScene = data["game"]["first_scene"].value_or("");
	_gameDLL = data["game"]["game_dll"].value_or("");

	_windowName = data["window"]["window_name"].value_or("");
	_iconRoot = data["window"]["icon_root"].value_or("");

	float r = data["window"]["clear_color_r"].as_floating_point()->get();
	float g = data["window"]["clear_color_g"].as_floating_point()->get();
	float b = data["window"]["clear_color_b"].as_floating_point()->get();

	_clearColor = { r,g,b,1.0f };

	_windowWidth = data["window"]["window_width"].as_integer()->get();
	_windowHeight = data["window"]["window_height"].as_integer()->get();
}

void core::GameConfigurator::SaveToFile(const std::string& path)
{
	std::ofstream file(path);
	file << Serialize();
}

bool core::GameConfigurator::LoadFromFile(const std::string& path)
{
	try
	{
		toml::table data = toml::parse_file(path);
		Deserialize(data);
		return true;
	}
	catch (const toml::parse_error& e)
	{
		Debug::error("GAME CONFIGURATOR: ", e.description());
		return false;
	}
}