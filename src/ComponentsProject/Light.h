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

/**
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
	PROPERTY(int, type);
	Type _type;
	PROPERTY(core::Color, color);
	PROPERTY(float, intensity);

	PROPERTY(float, innerAngle);
	PROPERTY(float, outerAngle);
	PROPERTY(float, falloff);

public:
	Light();
	~Light();

	bool init(const Properties& p) override;
	void ready() override;
	void setType(const Type& type);
	void setColor(const core::Color& _color);
	void setIntensity(const float& _intensity);
	//void setDirection(const core::Vector3<float>& _dir);
	void setSpotRange(const float& _inner, const float& _outer, const float& _falloff);

	Type getType() const;
	core::Color getColor() const;
	float getIntensity() const;
	
	float getInnerAngle() const;
	float getOuterAngle() const;
	float getFallOff() const;
	
	void destroy() override;
};