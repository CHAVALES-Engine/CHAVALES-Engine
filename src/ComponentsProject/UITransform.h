/**
 * @file
 * @brief Componente que representa la posición y dimensiones  de una entidad de UI.
 */
#pragma once
#include "EngineComponent.h"
#include <Vector2.h>
#include <Entity.h>
#include <EngineAPI.h>


using UItransformID = uint64_t;

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
  * --- Ejemplo de inicializacion ---
  * En bool init(const Properties& p):
  *		# Ej1, asignacion:
  * component = getProperty<tipo>(properties, "atributo1");
  *		# Ej2, setter:
  * return setProperty(properties, "atributo1", component);
  *
 */
class ENGINE_API UITransform : public EngineComponent
{
	UItransformID _UItransformID;
	core::Vector2<> _position;
	core::Vector2<> _dimension;
	int _dLayer;
	float _rotation;

	UITransform* _parent = nullptr;
	std::vector<UITransform*> _children;

public:
	UITransform();
	bool init(const Properties& p) override;

	void setPosition(const core::Vector2<>& pos);
	void setDimension(const core::Vector2<>& dim);
	void setRotation(float r);
	void setDepthLayer(int dLayer);

	core::Vector2<> getPosition() const;
	core::Vector2<> getGlobalPosition() const;
	core::Vector2<> getDimension() const;
	float getRotation() const;
	int getDepthLayer() const;

	void setParent(UITransform* t);
	UITransform* getParent() const;
	std::vector<UITransform*>& getChildren();
	void detachChild(UITransform* uT);
	void detachChildren();

	std::shared_ptr<Component> getComponentInParents(const std::string& name) const;
	std::vector<std::shared_ptr<Component>> getComponentsInParents(const std::string& name) const;

	template <typename T>
	std::shared_ptr<T> getComponentInParents() const
	{
		const UITransform* parent = getParent();
		while (parent != nullptr)
		{
			core::Entity* e = parent->getEntity();
			if (e != nullptr)
			{
				std::shared_ptr<T> c = e->getComponent<T>();
				if (c != nullptr)
					return c;
			}
			parent = parent->getParent();
		}
		return nullptr;
	}
};