#include "GameConfigurator.h"
#include "checkMLNew.h"

core::GameConfigurator& core::GameConfigurator::instance()
{
	static core::GameConfigurator instance; // Se crea la primera vez, destruye al cerrar.
	return instance;
}

toml::table core::GameConfigurator::Serialize()
{
	return toml::table{
		{ "root", _root },
		{

			"game", toml::table{
				{ "first_scene", _firstScene },
				{ "game_dll", _gameDLL },
				{ "scenes_root", _scenesRoot }
			}
		},
		{
			"window", toml::table{
				{ "window_name", _windowName },
				{ "icon_root", _iconRoot },
				{ "width", _windowWidth },
				{ "height", _windowHeight },
				{
					"clear_color", toml::table{
						{ "r", _clearColor.getRed() },
						{ "g", _clearColor.getGreen() },
						{ "b", _clearColor.getBlue() },
						{ "a", _clearColor.getAlpha() }
					}
				}
			}
		},
		{
			"assets", toml::table{
				{ "assets_root", _assetsRoot },
			}
		} };
}

void core::GameConfigurator::Deserialize(const toml::table& data)
{
	_root = GetValue<std::string>(data, "root", _root);

	// Game settings
	if (data.contains("game"))
	{
		auto& gameTable = *data["game"].as_table();
		_firstScene = GetValue<std::string>(gameTable, "first_scene", _firstScene);
		_gameDLL = GetValue<std::string>(gameTable, "game_dll", _gameDLL);
		_scenesRoot = GetValue<std::string>(gameTable, "scenes_root", _scenesRoot);
	}

	// Window settings
	if (data.contains("window"))
	{
		auto& windowTable = *data["window"].as_table();
		_windowName = GetValue<std::string>(windowTable, "window_name", _windowName);
		_iconRoot = GetValue<std::string>(windowTable, "icon_root", _iconRoot);
		_windowWidth = GetValue<int>(windowTable, "width", _windowWidth);
		_windowHeight = GetValue<int>(windowTable, "height", _windowHeight);

		// Clear color
		if (windowTable.contains("clear_color"))
		{
			auto& colorTable = *windowTable["clear_color"].as_table();
			float r = GetValue<float>(colorTable, "r", _clearColor.getRed());
			float g = GetValue<float>(colorTable, "g", _clearColor.getGreen());
			float b = GetValue<float>(colorTable, "b", _clearColor.getBlue());
			float a = GetValue<float>(colorTable, "a", _clearColor.getAlpha());
			_clearColor = core::Color(r, g, b, a);
		}
	}

	// Assets
	if (data.contains("assets"))
	{
		auto& assetsTable = *data["assets"].as_table();
		_assetsRoot = GetValue<std::string>(assetsTable, "assets_root", _assetsRoot);
	}

	Debug::out("GAME CONFIGURATOR: Configuración cargada correctamente.");
}

void core::GameConfigurator::SaveToFile(const std::string& path)
{
	std::ofstream file(path);
	if (!file.is_open())
	{
		Debug::error("GAME CONFIGURATOR: No se pudo abrir archivo para guardar: ", path);
		return;
	}

	file << Serialize();
	file.close();
	Debug::out("GAME CONFIGURATOR: Configuración guardada en ", path);
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
	catch (...)
	{
		Debug::error("GAME CONFIGURATOR: Error desconocido leyendo configuracion.");
		return false;
	}
}
