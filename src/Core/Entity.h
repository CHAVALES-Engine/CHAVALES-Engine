/*
 * @file
 * @brief
 * This file was adapted from a part of the course TPV2@UCM - Samir Genaim
 */
#pragma once
#include <vector>
#include <string>
#include "ec.h"

class Scene;

namespace ec
{
	class Entity
	{
	public:
		Entity();
		virtual ~Entity(); // Destroys the entity

		// prohibimos copia
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		// --- SETTERS
		void setAlive(bool a);
		void setVisible(bool v);
		void setEnabled(bool e);
		void setScene(Scene* s);
		void setGroupId(grpId_t id);
		void setName(const std::string& n);

		// --- GETTERS
		bool isAlive() const;
		bool isVisible() const;
		bool isEnabled() const;
		const Scene* getScene() const;
		grpId_t getGroupId() const;
		//bool inGroup(grpId_t id) const;
		const std::string& getName() const;
		const std::vector<Component*>& getComponents() const;

		// --- LIFECYLE
		void init();
		void update(double deltaTime);
		void render() const;
		void destroy();
		void enable();
		void disable();

		// --- EC
		// O(n) para todos los siguientes métodos, 
		template <typename T, typename... Ts>
		T* addComponent(Ts&&... args);

		template <typename T>
		void removeComponent();

		template <typename T>
		T* getComponent();

		template <typename T>
		bool hasComponent() const;

	protected:
		std::vector<Component*> components;
		bool alive;
		bool visible;
		bool enabled;
		Scene* scene;
		grpId_t groupId;
		std::string name;
	};
} // end of name space
