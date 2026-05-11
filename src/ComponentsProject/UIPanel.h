/*
 * @file
 * @brief Componente que gestiona un panel donde se añadiran los diferentes elementos de UI
 */

#pragma once
#include <Component.h>
#include <EngineAPI.h>

using panelID = uint64_t;
/*
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
class ENGINE_API UIPanel : public core::Component
{
	

private:
	panelID _panelID;
	std::string _title;
public:

	UIPanel();
	~UIPanel();
	void ready() override;
	bool init(const Properties& p) override;
	panelID getPanelID() const;
	void setVisible (bool visible);
	void destroy() override;
};

