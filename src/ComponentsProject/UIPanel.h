/**
 * @file
 * @brief Componente que gestiona un panel donde se anadiran los diferentes elementos de UI
 */

#pragma once
#include "EngineComponent.h"
#include <EngineAPI.h>

using panelID = uint64_t;
/**
 * +---------+
 * | UIPANEL |
 * +---------+
 *
 * --- Ejemplo de uso en lua ---
 * UiPanel = {
 *		name = string
 * }
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/
class ENGINE_API UIPanel : public EngineComponent
{
private:
	panelID _panelID;
	PROPERTY(std::string, name);

public:
	UIPanel();
	~UIPanel();
	bool init(const Properties& p) override;
	panelID getPanelID() const;
	void setVisible (bool visible);
	void destroy() override;
};