/**
 * @file Entity
 * @brief Clase contenedora de componentes que representa una entidad en escena
 */
#pragma once
#include <memory>
#include <vector>
#include <string>
#include "ec.h"
#include "guid.h"
#include "Component.h"

class Scene;

namespace core
{
	using entityID = ChavalesGUID;

	class Entity
	{
	public:
		Entity();
		Entity(std::string);
		virtual ~Entity(); // Destroys the entity

		// prohibimos copia
		Entity(const Entity&) = delete;
		Entity& operator=(const Entity&) = delete;

		// --- SETTERS
		void setAlive(bool a);
		void setVisible(bool v);
		void setEnabled(bool e);
		void setDontDestroyOnLoad(bool ddol);
		void setScene(Scene* s);
		void setEntityID(entityID id);
		void setGroupId(grpId_t id);
		void setName(const std::string& n);

		// --- GETTERS
		bool isAlive() const;
		bool isVisible() const;
		bool isEnabled() const;
		bool getDontDestoroyOnLoad() const;
		Scene* getScene() const;
		entityID getEntityID() const;
		grpId_t getGroupId() const;
		//bool inGroup(grpId_t id) const;
		const std::string& getName() const;
		const std::vector<std::shared_ptr<Component>>& getComponents() const;

		// --- LIFECYLE
		/**
		* @brief Inicializa la entidad
		*/
		//void init(const Properties& p);
		/**
		* @brief Ejecuta la fase de arranque de la entidad y sus componentes
		*/
		void ready();
		/**
		* @brief Activa la entidad y sus componentes
		*/
		void enable();
		/**
		* @brief Ejecuta la actualizacion en tiempo fijo
		*/
		void fixedUpdate();
		/**
		* @brief Ejecuta la actualizacion por frame
		*/
		void update(uint64_t dT);
		/**
		* @brief Renderiza la entidad
		*/
		//void render() const;
		/**
		* @brief Desactiva la entidad y sus componentes
		*/
		void disable();
		/**
		* @brief Destruye la entidad y sus componentes
		*/
		void destroy();

		// --- EC
		/**
		* @brief Agrega un componente ya creado a la entidad
		* O(n) 
		*/
		Component* addComponent(std::shared_ptr<Component> comp);

		/**
		* @brief Crea y agrega un componente del tipo indicado
		* O(n)
		*/
		template <typename T, typename... Ts>
		T* addComponent(Ts&... args)
		{
			// evitando duplicados
			// esto lo hace O(n)
			//if (hasComponent<T>())
			//	return getComponent<T>();

			std::shared_ptr<Component> c = std::make_shared<T>(std::forward<Ts>(args)...);
			//T* c = new T(std::forward<Ts>(args)...);

			c->setEntity(this);
			c->enable();
			c->ready();
			T* c_ref = c.get();
			components.push_back(std::move(c));
			return c_ref;
		}


		/**
		* @brief Elimina el primer componente del tipo indicado
		* O(n)
		*/
		template <typename T>
		void removeComponent()
		{
			for (auto it = components.begin(); it != components.end(); ++it)
			{
				if (it->get() != nullptr)
				{
					(*it)->disable();
					(*it)->destroy();
					components.erase(it);
					return;
				}
			}
		}

		/**
		* @brief Obtiene el primer componente del tipo indicado
		* O(n)
		*/
		template <typename T>
		T* getComponent()
		{
			for (std::shared_ptr<Component>& c : components)
			{
				if (T* ptr = dynamic_cast<T*>(c.get()))
					return ptr;
			}
			return nullptr;
		}

		/**
		* @brief Elimina todos los componentes del mismo tipo indicado
		* O(n)
		*/
		template <typename T>
		void removeComponents()
		{
			for (auto it = components.begin(); it != components.end(); ++it)
			{
				if (it->get() != nullptr)
				{
					(*it)->disable();
					(*it)->destroy();
					components.erase(it);
				}
			}
		}

		/**
		* @brief Obtiene todos los componentes del mismo tipo indicado
		* O(n)
		*/
		template <typename T>
		std::vector<T*> getComponents()
		{
			std::vector<T*> cmp; 
			for (std::shared_ptr<Component>& c : components)
			{
				if (T* ptr = dynamic_cast<T*>(c.get()))
					cmp.push_back(ptr);
			}
			if (cmp.empty())
				return nullptr;
			return cmp;
		}

		// O(n)
		/**
		* @brief Comprueba si la entidad contiene un componente del tipo indicado
		* O(n)
		*/
		template <typename T>
		bool hasComponent() const
		{
			for (const auto& c : components)
			{
				if (dynamic_cast<T*>(c.get()) != nullptr)
					return true;
			}
			return false;
		}

	protected:
		std::vector<std::shared_ptr<Component>> components;
		bool alive;
		bool visible;
		bool enabled;
		Scene* scene;
		entityID entityID;
		grpId_t groupId;
		std::string name;
		bool dontDestroyOnLoad;
	};
} // end of name space
