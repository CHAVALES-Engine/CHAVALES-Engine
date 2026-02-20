#pragma once
/**
* @file ComponentsLoader
* @brief ComponentsRegister contains methods to instantiate dll components.
*/
#include <unordered_map>
#include <string>

class ComponentsRegister
{
public:
	/**
	* @brief Empty Constructor
	*/
	ComponentsRegister();
	/**
	* @brief Destructor
	*/
	~ComponentsRegister();
	//bool regist(const std::string &name, );
	bool create(const std::string &component);
private:
	//std::unordered_map<std::string, []()> components;
};

