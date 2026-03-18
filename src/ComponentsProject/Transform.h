/*
 * @file
 * @brief Componente que representa la posición, escala y rotación de una entidad.
 * TODO:
 *		- eventos? (e.g. onPositionChanged, onRotationChanged...?)s
 *		- lookAt()
 *		- getChild() con algo mas que el transform, e.g. por nombre o posicion del hijo en el vector
 */
#pragma once
#include <vector>
#include <Component.h>
#include <Quaternion.h>
#include <Vector3.h>

using transformID = uint64_t;

class Transform : public core::Component
{
	transformID _transformID;

	core::Vector3<> _localPosition;
	core::Quaternion<> _localRotation;
	core::Vector3<> _localScale;
	Transform* _parent;
	std::vector<Transform*> _children;

public:
	//Transform();
	//~Transform() ;

	bool init(const Properties& p) override;

	void setGlobalPosition(core::Vector3<> gp);
	void setLocalPosition(const core::Vector3<>& lp);

	void setGlobalRotation(const core::Quaternion<>& gr);
	void setLocalRotation(const core::Quaternion<>& lr);

	void setGlobalScale(const core::Vector3<>& gs);
	void setLocalScale(const core::Vector3<>& ls);

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
	void setParent(Transform* t, bool keepWorldMeasures = true);
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
	void rotate(const core::Quaternion<>& q);
	/**
	 * @brief Aplica una rotacion v en formato vector a la rotacion local
	 */
	void rotate(core::Vector3<> v);

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

	// TODO?
	//Transform* getChildByName(std::string);
	//Transform* getChildByID(size_t);
	//void lookAt();
};