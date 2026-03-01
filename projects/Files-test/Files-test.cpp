// Files-test.cpp : Este archivo contiene la función "main". La ejecución del programa comienza y termina ahí.
//

#include <iostream>
#include <string>
#include <filesystem>
namespace fs = std::filesystem;
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

#include <fstream>

int main()
{
    std::string path = "./hola";
    std::string file;
    for (const auto& entry : fs::directory_iterator(path))
    {
		file = entry.path().generic_string();

		std::ifstream entrada(file);

		if (!entrada.is_open())
		{
			return -1;
		}

		auto cinbuf = std::cin.rdbuf(entrada.rdbuf());

		std::string entidadName;
		std::cin >> entidadName;

		std::cout << entidadName << '\n';

		std::cin.rdbuf(cinbuf);
    }
}