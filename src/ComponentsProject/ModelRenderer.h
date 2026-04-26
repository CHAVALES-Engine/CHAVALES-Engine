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
 *		file = string,
 *		number of textures = int,
 *		textureN = string
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
	std::string _modelName;
	std::vector<std::vector<std::string>> _textures;

public:
	ModelRenderer();
	~ModelRenderer();

	bool init(const Properties& p) override;
	void ready() override;

	void setDiffuse(const std::string& textureName, const int& submesh = 0);
	void setTint(const core::Color& tint, const int& submesh = 0);
	void setVisible(const bool& visible);
	void enable() override;
	void disable() override;
	void destroy() override;
};

