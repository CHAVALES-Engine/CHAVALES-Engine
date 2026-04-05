/*
 * @file
 * @brief Componente que gestiona un modelo.
 */
#pragma once

#include <Component.h>

#include <utility>

using modelID = uint64_t;

/*
 * +----------------+
 * | MODEL RENDERER |
 * +----------------+
 *
 * --- Ejemplo de uso en lua ---
 * ModelRenderer = {
 *		...
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * ...
 *		# Ej2, setter:
 * ...
 *
*/
class ModelRenderer : public core::Component
{
	modelID _modelID;

	std::string _modelFolder;
	std::string _modelFile;

	int _nTextures;

	/*
	* @brief Clave: Nombre textura		Valor: par(carpeta archivo, nombre archivo)
	* */
	std::unordered_map<std::string, std::pair<std::string, std::string>> _textures;

public:
	ModelRenderer();
	~ModelRenderer();

	bool init(const Properties& p) override;

	void setDiffuse(std::string textureFolder, std::string textureName, int submesh = 0);
	void setTint(core::Color tint, int submesh = 0);
	void setVisible(bool visible);
};

