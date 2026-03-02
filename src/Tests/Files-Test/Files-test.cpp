#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <fstream>

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

	for (auto& entidad : scene)
	{
		auto nombre = entidad.first;
		std::cout << "ENTIDAD: " << nombre.as<std::string>() << std::endl;
		
		sol::table partes = entidad.second;
		sol::table componentes = partes["components"];
		for (auto& componente : componentes)
		{
			auto nombre2 = componente.first;
			std::cout << "COMPONENTE: " << nombre2.as<std::string>() << std::endl;
		}
	}
}