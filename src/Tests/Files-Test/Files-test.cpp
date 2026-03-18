//#include "Debug.h"
//namespace fs = std::filesystem;
//#define SOL_ALL_SAFETIES_ON 1
//#include <sol/sol.hpp>

//#include "Entity.h"
//#include "Component.h"
//#include "Scene.h"
//#include <ComponentRegister.h>
//#include "ComponentDLLLoader.h"

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>

#include <chrono>
#include <ctime>

int main(int argc, char* argv[])
{
	/*
	sol::state lua;
	lua.script_file("./game/scenes/scene_prueba.lua");
	sol::table scene = lua["scene"];
	core::Scene* s = new core::Scene("0");

	ComponentDLLLoader dllLoader;

#if DEBUG_PROCESS
	dllLoader.load("./ComponentsProject_d.dll");
#else 
	dllLoader.load("./ComponentsProject_r.dll");
#endif
	dllLoader.load("./DLL-Test.dll");

	// recorre las entidades
	for (auto& entidad : scene)
	{
		// nombre de la entidad
		auto entidadName = entidad.first;
		std::cout << "ENTIDAD: " << entidadName.as<std::string>() << std::endl;

		// crea la entidad
		core::Entity* e = new core::Entity();
		
		// 
		sol::table partes = entidad.second;
		sol::table componentes = partes["components"];
		for (auto& componente : componentes)
		{
			auto componenteObj = componente.first;
			std::string componenteName = componenteObj.as<std::string>();
			std::cout << "COMPONENTE: " << componenteName << std::endl;
			std::shared_ptr<core::Component> component = ComponentRegister::instance().create(componenteName);

			if (component != nullptr)
			{
				// mete el componente a la entidad creada
				e->addComponent(std::move(component));
				
			}
			else
			{
				Debug::warning("Componente ", componenteName, " no registrado\n");
			}
		}

		// mete la entidad en la escena
		s->addEntity(e);
	}

	s->fixedUpdate();
	*/

	std::string path = "./game/scenes/scene1.lua";

	std::filesystem::file_time_type lastTime;
	auto lastSize = std::filesystem::file_size(path);

	lastTime = std::filesystem::last_write_time(path);

	while (true)
	{
		try
		{
			std::filesystem::file_time_type ftime = std::filesystem::last_write_time(path);
			auto fsize = std::filesystem::file_size(path);

			if (ftime > lastTime && // para saber la ultima modificacion en tiempo
				lastSize < fsize) // si se ha modificado el archivo de verdad
			{
				std::cout << "modificado" << std::endl;
				lastTime = ftime;
				lastSize = fsize;
			}
		}
		// si lo borras a mitad que limpie la memoria de esa escena y que vuelva a 
		// preguntar que escena quieres cargar a continuacion, dando margen de recuperar la escena
		// por si la has borrado sin querer
		catch (...)
		{
			return 1;
		}
	}
	return 0;
}