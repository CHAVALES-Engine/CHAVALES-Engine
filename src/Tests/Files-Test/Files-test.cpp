#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <fstream>
#include "Entity.h"
#include "Scene.h"

#include "../../ExecutableProject/GameLoader.h"

int main(int argc, char* argv[])
{
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

	sol::state lua;
	lua.script_file("./scenes/scene_prueba.lua");
	sol::table scene = lua["scene"];
	core::Scene* s = new core::Scene("0");

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

			if (componenteName == "Transform")
			{
				// busca en el mapa de componentes y lo construye

				// mete el componente a la entidad creada
				//e->addComponent<>();
			}
			else if (componenteName == "ComponentTest")
			{
				
			}
			else
			{
				std::cout << "Componente " << componenteName << " no registrado" << '\n';
			}
		}

		// mete la entidad en la escena
		s->addEntity(e);
	}

	s->update(10);
	delete s;
}
