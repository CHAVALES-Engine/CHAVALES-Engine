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

	std::string _modelName;

public:
	ModelRenderer();
	~ModelRenderer();

	bool init(const Properties& p) override;

	void setDiffuse(std::string textureName, int submesh = 0);
	void setTint(core::Color tint, int submesh = 0);
	void setVisible(bool visible);
	void destroy() override;
};

