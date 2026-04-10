#pragma once
#include <Component.h>
#include "Vector3.h"

class Transform;

/*
 * +----------------+
 * | AUDIO LISTENER |
 * +----------------+
 *
 * --- Ejemplo de uso en lua ---
 * AudioListener = {
 *		
 * }
 * (Este componente no requiere de ningún parámetro)
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/
class AudioListener : public core::Component
{
private:
	Transform* _transform;
	core::Vector3<> _lastPos;

public:
	AudioListener();
	~AudioListener();

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;
};

