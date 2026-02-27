#pragma once
#include <string>
#include <vector>

#include "Component.h"
#include "Quaternion.h"
#include "Vector3.h"

namespace core
{
	class Transform : public Component
	{
		Vector3<> _position;
		Quaternion _rotation;
		Vector3<> _scale;
		Transform* _parent;
		std::vector<Transform*> _children;

	public:
		Transform();
		~Transform();

		void setGlobalPosition(Vector3<>);
		void setLocalPosition(Vector3<>);
		void setPosition(Vector3<>);
		void setRotation(Quaternion);
		void setScale(Vector3<>);
		void setParent();

		Vector3<> getGlobalPosition();
		Vector3<> getLocalPosition();
		Vector3<> getPosition();
		Quaternion getRotation();
		Vector3<> getScale();
		Transform* getParent();
		std::vector<Transform*> getChildren();

		void addChild(Transform*);
		void removeChild(Transform*);
		void removeChildren();

		template <typename T = float>
		void translate(Vector3<T>);	
		template <typename T = float>
		void translate(T, T, T);
		
		// TODO?
		//Transform* getChildByName(std::string);
		//Transform* getChildByID(size_t);
		//void lookAt();
	};
}
