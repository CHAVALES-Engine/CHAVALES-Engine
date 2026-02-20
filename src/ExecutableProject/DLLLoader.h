#pragma once
/**
* @file DLLLoader
* @brief DLL loader, this DLLLoader is only usable on windows.
*/

#include <string>
#include <vector>
struct HMODULE;
class DLLLoader
{
public:
	/**
	* @brief Empty constructor.
	*
	*		
	*/
	DLLLoader();
	/**
	* @brief Destructor.
	*
	*		Unload all dynamic libraries.
	*/
	~DLLLoader();
	/**
	* @brief Load library method.
	*
	*		It loads the library on the path.
	* @param path - Path to loaded dll.
	* @return bool - It loaded correctly.
	*/
	bool load(const std::string& path);
	/**
	* @brief unload all libaries
	*
	*			Unloads all dynamic libraries, It will be replaced by other more hot-reload freandly.
	*/
	void unLoadAll();
	/**
	* @brief Unload library method.
	*		
	*		TODO: It unloads the library on the path.
	* @param path - Path to unloaded dll.
	* @return bool - It unloaded correctly.
	*/
	bool unload(const std::string& path);
private:
	struct LoadedLibrary {
		HMODULE handle;
		std::string path;
	};
	/**
	* @brief
	*
	*		Vector that contains the information of a dll.
	*/
	std::vector<LoadedLibrary> _libraries;

};

