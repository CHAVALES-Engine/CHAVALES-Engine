/**
 * @file
 * @brief Componente que representa la posición, escala y rotación de una entidad.
 */
#pragma once
#include "EngineComponent.h"
#include <vector>
#include <Quaternion.h>
#include <Vector3.h>
#include <EngineAPI.h>
#include <CommonEnums.h>
#include <Entity.h>

using transformID = uint64_t;

/*
 * +-----------+
 * | TRANSFORM |
 * +-----------+
 *
 * --- Ejemplo de uso en lua ---
 * Transform = {
 *		position = Vector3.new(float...),
 *		rotation = Quaternion.new(float...),
 *		scale = Vector3.new(float...),
 *		children = {
 *			"entidad2", "entidad3"
 *			}
 * }
 *
 *
 * --- Ejemplo de inicializacion ---
 * En bool init(const Properties& p):
 *		# Ej1, asignacion:
 * component = getProperty<tipo>(properties, "atributo1");
 *		# Ej2, setter:
 * return setProperty(properties, "atributo1", component);
 *
*/
;
class ENGINE_API Transform : public EngineComponent
{
	transformID _transformID;
	core::Vector3<> _localPosition;
	core::Quaternion<> _localRotation;
	core::Vector3<> _localScale;
	Transform* _parent = nullptr;
	std::vector<std::string> _pendingChildren;
	std::vector<Transform*> _children;
	bool _lockRotX = false;
	bool _lockRotY = false;
	bool _lockRotZ = false;

	/**
	 * @brief Lista de nombres de entidades a anyadir como hijos (Solo se usa en la inicializacion de componentes).
	 */
public:
	Transform();

	bool init(const Properties& p) override;

	void ready() override;

	void setGlobalPosition(const core::Vector3<>& gp);
	void setLocalPosition(const core::Vector3<>& lp);

	void setGlobalRotation(const core::Quaternion<>& gr);
	void setLocalRotation(const core::Quaternion<>& lr);

	void setGlobalScale(const core::Vector3<>& gs);
	void setLocalScale(const core::Vector3<>& ls);

	void lockRotationX(bool v);
	void lockRotationY(bool v);
	void lockRotationZ(bool v);

	core::Vector3<> getGlobalPosition() const;
	core::Vector3<> getLocalPosition() const;

	core::Quaternion<> getGlobalRotation() const;
	core::Quaternion<> getLocalRotation() const;

	core::Vector3<> getGlobalScale() const;
	core::Vector3<> getLocalScale() const;

	Transform* getParent() const;

	/**
	 * @brief Establece el Transform padre a este
	 * @param t - Transform del padre
	 * @param keepWorldMeasures - Si mantener o no el estado del Transform al cambiar de parentesco, por defecto es true
	 */
	void setParent(Transform* t, bool keepWorldMeasures = false);
	/**
	 * @return Todos los hijos de primer grado de este transform
	 */
	std::vector<Transform*>& getChildren();
	/**
	 * @brief Deshereda a su hijo de Transform c
	 */
	void detachChild(Transform* c);
	/**
	 * @brief Deshereda a todos sus hijos
	 */
	void detachChildren();

	/**
	 * @brief Aplica una traslacion t en formato vector a la posicion local
	 */
	void translate(const core::Vector3<>& t);
	/**
	 * @brief Aplica una rotacion q en formato quaternion a la rotacion local
	 */
	void rotateLocal(const core::Quaternion<>& q);
	/**
	 * @brief Aplica una rotacion v en formato vector a la rotacion local
	 */
	void rotateLocal(const core::Vector3<>& v);
	/**
	 * @brief Aplica una rotacion q en formato quaternion a la rotacion global
	 */
	void rotateGlobal(const core::Quaternion<>& q);
	/**
	 * @brief Aplica una rotacion v en formato vector a la rotacion global
	 */
	void rotateGlobal(const core::Vector3<>& v);

	/**
	 * @returns Devuelve un vector normalizado representando el eje X en coordenadas globales
	 */
	core::Vector3<> right() const;
	/**
	 * @return Devuelve un vector normalizado representando el eje Y en coordenadas globales
	 */
	core::Vector3<> up() const;
	/**
	 * @return Devuelve un vector normalizado representando el eje Z en coordenadas globales
	 */
	core::Vector3<> forward() const;

	/**
	 * @return Angulo de yaw (rotacion sobre Y) en grados.
	 */
	float yaw() const;
	void yaw(float degrees);
	/**
	 * @return Angulo de pitch (rotacion sobre X) en grados.
	 */
	float pitch() const;
	void pitch(float degrees);
	/**
	 * @return Angulo de roll (rotacion sobre Z) en grados.
	 */
	float roll() const;
	void roll(float degrees);

	/**
	 * @brief Rota el transform para que su eje -forward apunte hacia una posicion global.
	 * @param target Posicion objetivo en coordenadas globales.
	 */
	void LookAt(const core::Vector3<>& target);

	/**
	 * @param name - nombre del compomente a buscar
	 * @return Devuelve el primer componente de nombre name en los padres del transform
	 */
	std::shared_ptr<Component> getComponentInParents(const std::string& name) const;
	/**
	 * @param name - nombre del compomente a buscar
	 * @return Devuelve todos los componentes de nombre name en los padres del transform
	 */
	std::vector<std::shared_ptr<Component>> getComponentsInParents(const std::string& name) const;

	/**
	 * @tparam T - tipo del compomente a buscar
	 * @return Devuelve el primer componente de tipo T en los padres del transform
	 */
	template <typename T>
	T* getComponentInParents() const
	{
		const Transform* parent = getParent();
		while (parent != nullptr)
		{
			core::Entity* e = parent->getEntity();
			if (e != nullptr)
			{
				auto* c = e->getComponent<T>();
				if (c != nullptr)
					return c;
			}
			parent = parent->getParent();
		}
		return nullptr;
	}
	/**
	 * @tparam T - tipo del compomente a buscar
	 * @return Devuelve todos los componentes de tipo T en los padres del transform
	 */
	template <typename T>
	std::vector<T*> getComponentsInParents() const
	{
		std::vector<T*> result;
		const Transform* parent = getParent();
		while (parent != nullptr)
		{
			core::Entity* e = parent->getEntity();
			if (e != nullptr)
			{
				auto* c = e->getComponent<T>();
				if (c != nullptr)
					result.push_back(c);
			}
			parent = parent->getParent();
		}
		return result;
	}

	/**
	 * @brief Ajusta posicion al nuevo contexto.
	 */
	void refreshPostion();
	/**
	 * @brief Ajusta rotacion al nuevo contexto.
	 */
	void refreshRotation();
	/**
	 * @brief Ajusta escala al nuevo contexto.
	 */
	void refreshScale();
};