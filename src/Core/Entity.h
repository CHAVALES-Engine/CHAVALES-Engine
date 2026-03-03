/**
 * @file Entity
 * @brief Clase contenedora de componentes que representa una entidad en escena
 */
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "ec.h"

class Scene;

namespace core
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
		const std::vector<std::shared_ptr<Component>>& getComponents() const;

		// --- LIFECYLE
		/*
		* @brief
		*/
		void init();
		/*
		* @brief
		*/
		void fixedUpdate();
		/*
		* @brief
		*/
		void update(uint64_t dT);
		/*
		* @brief
		*/
		void render() const;
		/*
		* @brief
		*/
		void destroy();
		/*
		* @brief
		*/
		void enable();
		/*
		* @brief
		*/
		void disable();
		/*
		* @brief
		*/

		// --- EC
		/*
		* @brief
		* O(n) 
		*/
		template <typename T, typename... Ts>
		T* addComponent(Ts&&... args);
		Component* addComponent(std::shared_ptr<Component> comp);

		/*
		* @brief
		* O(n)
		*/
		template <typename T>
		void removeComponent();

		/*
		* @brief
		* O(n)
		*/
		template <typename T>
		T* getComponent();

		// O(n)
		/*
		* @brief
		* O(n)
		*/
		template <typename T>
		bool hasComponent() const;

	protected:
		std::vector<std::shared_ptr<Component>> components;
		bool alive;
		bool visible;
		bool enabled;
		Scene* scene;
		grpId_t groupId;
		std::string name;
	};
} // end of name space
