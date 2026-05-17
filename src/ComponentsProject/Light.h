/**
 * @file
 * @brief Componente que gestiona una luz.
 */
#pragma once
#include "EngineComponent.h"
#include <Vector3.h>
#include <Color.h>
#include <EngineAPI.h>

using lightID = uint64_t;

/*
 * +-------+
 * | LIGHT |
 * +-------+
 *
 * --- Ejemplo de uso en lua ---
 * Light = {
 *		type = int
 *		color = Color.new(float...),
 *		intensity = float,
 *		inner = float,
 *		outer = float,
 *		fallof = float
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
class ENGINE_API Light : public EngineComponent
{
public:
	enum class Type { POINT = 0, DIRECTIONAL = 1, SPOT = 2 , RECTLIGHT = 3  };

private:
	lightID _lightID;
	int _typeInt;
	Type _type;
	core::Color _color;
	float _intensity;

	float _innerAngle;
	float _outerAngle;
	float _falloff;

public:
	Light();
	~Light();

	bool init(const Properties& p) override;
	void ready() override;
	void setType(const Type& type);
	void setColor(const core::Color& color);
	void setIntensity(const float& intensity);
	void setDirection(const core::Vector3<float>& dir);
	void setSpotRange(const float& inner, const float& outer, const float& falloff);

	Type getType() const;
	core::Color getColor() const;
	float getIntensity() const;
	
	float getInnerAngle() const;
	float getOuterAngle() const;
	float getFallOff() const;
	
	void destroy() override;
};