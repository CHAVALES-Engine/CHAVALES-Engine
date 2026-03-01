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

namespace core
{
	class Transform : public Component
	{
		Vector3<> _localPosition;
		Quaternion<> _localRotation;
		Vector3<> _localScale;
		Transform* _parent;
		std::vector<Transform*> _children;

	public:
		Transform();
		~Transform() override;

		void setGlobalPosition(Vector3<> gp);
		void setLocalPosition(Vector3<> lp);

		void setGlobalRotation(Quaternion<> gr);
		void setLocalRotation(Quaternion<> lr);

		void setGlobalScale(Vector3<> gs);
		void setLocalScale(Vector3<> ls);

		Vector3<> getGlobalPosition() const;
		Vector3<> getLocalPosition() const;

		Quaternion<> getGlobalRotation() const;
		Quaternion<> getLocalRotation() const;

		Vector3<> getGlobalScale() const;
		Vector3<> getLocalScale() const;

		Transform* getParent() const;

		/*
		 * @brief Establece el Transform padre a este
		 * @param t - Transform del padre
		 * @param keepWorldMeasures - Si mantener o no el estado del Transform al cambiar de parentesco, por defecto es true
		 */
		void setParent(Transform* t, bool keepWorldMeasures = true);
		/*
		 * @return Todos los hijos de primer grado de este transform
		 */
		std::vector<Transform*>& getChildren();
		/*
		 * @brief Deshereda a su hijo de Transform c
		 */
		void detachChild(Transform* c);
		/*
		 * @brief Deshereda a todos sus hijos
		 */
		void detachChildren();

		/*
		 * @brief Aplica una traslacion t en formato vector a la posicion local
		 */
		void translate(Vector3<> t);
		/*
		 * @brief Aplica una rotacion q en formato quaternion a la rotacion local
		 */
		void rotate(Quaternion<> q);
		/*
		 * @brief Aplica una rotacion v en formato vector a la rotacion local
		 */
		void rotate(Vector3<> v);

		/*
		 * @returns Devuelve un vector normalizado representando el eje X en coordenadas globales
		 */
		Vector3<> right() const;
		/*
		 * @return Devuelve un vector normalizado representando el eje Y en coordenadas globales
		 */
		Vector3<> up() const;
		/*
		 * @return Devuelve un vector normalizado representando el eje Z en coordenadas globales
		 */
		Vector3<> forward() const;

		// TODO?
		//Transform* getChildByName(std::string);
		//Transform* getChildByID(size_t);
		//void lookAt();
	};
}
