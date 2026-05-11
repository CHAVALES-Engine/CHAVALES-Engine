/*
 * @file
 * @brief Componente que gestiona una animacion de transform.
 */
#pragma once
#include <Component.h>
#include <vector>
#include <utility>
#include <EngineAPI.h>

#include <Vector3.h>
#include <Quaternion.h>

/*
 * +----------+
 * | ANIMATOR |
 * +----------+
 *
 * --- Ejemplo de uso en lua ---
 * Animator = {
 *		...
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

using modelID = uint64_t;
using animationID = uint64_t;

enum Axis
{
	X,
	Y,
	Z
};

enum AnimationType { skeletal, transform };

struct Animation
{
	animationID id;
	AnimationType type;
	bool active;

	Animation() {};
	Animation(const animationID& id, const AnimationType& type)
		: id(id), type(type), active(false) {};
	Animation(const animationID& id, const AnimationType& type, const bool& active)
		: id(id), type(type), active(active) {};
};

class ENGINE_API Animator : public core::Component
{
	modelID _modelID;
	//Cada animacion tiene su nombre y un interruptor que indica su actividad
	std::unordered_map<std::string, Animation> _animations;

public:
	Animator();
	~Animator();

	void ready() override;
	void update(uint64_t deltaTime) override;

	/*
	* @brief Registrar animator. Llamar en ready.
	*/
	void registerAnimator();
	/*
	* @brief Establecer loop de animacion de esqueleto. Devuelve true si se ha encontrado la animacion.
	*/
	bool registerSkeletonAnim(const std::string& animationName, const bool& loop);
	/*
	* @brief Crear animacion de transform.
	*/
	void createTransformAnimation(const std::string& animationName, const bool& loop, const float& totalDuration);
	/*
	* @brief Anadir keyframe a animacion de transform. Time pos en segundos.
	*/
	void addTransformKeyFrame(const std::string& animationName,
							  const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale);
	/*
	* @brief Anadir keyframe a animacion de transform con rotacion sencilla. Time pos en segundos.
	*/
	void addTransformKeyFrame(const std::string& animationName,
							  const float& timePos, const core::Vector3<float>& pos, const float& rot, const Axis& axis, const core::Vector3<float>& scale);
	/*
	* @brief Establecer animacion activa.
	*/
	void setAnimEnabled(const std::string& animationName, const bool& active);
	/*
	* @brief Reanudar animacíon a partir de cierto instante de tiempo.
	*/
	void setAnimTimePos(const std::string& animationName, const float& timePos);
	/*
	* @brief Ajustar velocidad de animacion.
	*/
	void setAnimSpeed(const std::string& animationName, const float& speed);
};