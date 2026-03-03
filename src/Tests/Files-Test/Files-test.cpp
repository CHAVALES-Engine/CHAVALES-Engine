#include <iostream>
#include <string>
#include <filesystem>

#include "Debug.h"
namespace fs = std::filesystem;
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include "Entity.h"
#include "Component.h"
#include "Scene.h"
#include "ComponentRegister.h"
#include "DLLLoader.h"

int main(int argc, char* argv[])
{
	sol::state lua;
	lua.script_file("./game/scenes/scene_prueba.lua");
	sol::table scene = lua["scene"];
	core::Scene* s = new core::Scene("0");

	DLLLoader dllLoader;

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
				core::Debug::warning("Componente ", componenteName, " no registrado\n");
			}
		}

		// mete la entidad en la escena
		s->addEntity(e);
	}

	s->fixedUpdate();
}

//  std::string path = "./hola";
//  std::string file;
//  for (const auto& entry : fs::directory_iterator(path))
//  {
	  //file = entry.path().generic_string();

	  //std::ifstream entrada(file);

	  //if (!entrada.is_open())
	  //{
	  //	return -1;
	  //}

	  //auto cinbuf = std::cin.rdbuf(entrada.rdbuf());

	  //std::string entidadName;
	  //std::cin >> entidadName;

	  //std::cout << entidadName << '\n';

	  //std::cin.rdbuf(cinbuf);
//  }