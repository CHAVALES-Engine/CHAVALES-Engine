/*
 * @file
 * @brief Componente que gestiona una luz.
 */
#pragma once
#include <Component.h>
#include <Vector3.h>
#include <Color.h>


using lightID = uint64_t;

class Light : public core::Component
{
public:
	enum class Type { POINT = 0, DIRECTIONAL = 1, SPOT = 2 , RECTLIGHT = 3  };

private:
	lightID _lightID;
	Type _type;
	core::Color _color;
	float _intensity;

	core::Vector3<float> _direction;
	float _innerAngle;
	float _outerAngle;
	float _falloff;
	
	//attenuation
	//diffuseColor
	//specularCOlor
public:
	Light();
	~Light();

	bool init(const Properties& p) override;
	void setType(Type type);
	void setColor(const core::Color& color);
	void setIntensity(float intensity);
	void setDirection(const core::Vector3<float>& dir);
	void setSpotRange(float inner, float outer, float falloff);

	Type getType() const;
	core::Color getColor() const;
	float getIntensity() const;
	core::Vector3<float> getDirection() const;
	
	float getInnerAngle() const;
	float getOuterAngle() const;
	float getFallOff() const;
	
};

