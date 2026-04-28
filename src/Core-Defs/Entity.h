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
		//void setGroupId(grpId_t id); // deberiamos usar esto para matrices de colisiones fisicas en algun momento
		void setName(const std::string& n);

		// --- GETTERS
		bool isAlive() const;
		bool isVisible() const;
		bool isEnabled() const;
		bool getDontDestroyOnLoad() const;
		Scene* getScene() const;
		entityID getEntityID() const;
		//grpId_t getGroupId() const; // deberiamos usar esto para matrices de colisiones fisicas en algun momento
		const std::string& getName() const;
		const std::vector<std::shared_ptr<Component>>& getComponents() const; 
		size_t getComponentCount() const;

#pragma region LIFECYCLE
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
		* @brief Ejecuta la actualizacion por frame despues de update
		*/
		void lateUpdate(uint64_t dT);
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
#pragma endregion

#pragma region COMPONENTS
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

			c->setEntity(this);
			c->enable();
			//c->ready();
			T* c_ref = c.get();
			components.push_back(std::move(c));
			return c_ref;
		}

		/**
		* @brief Elimina todos los componentes
		* O(n)
		*/
		void removeComponents();

		/**
		* @brief Elimina todos los componentes del tipo indicado
		* O(n)
		*/
		template <typename T>
		void removeComponent()
		{
			for (auto it = components.begin(); it != components.end(); )
			{
				auto* c = it->get();

				if (c != nullptr && dynamic_cast<T*>(c))
				{
					c->disable();
					c->destroy();
					it = components.erase(it);
				}
				else
				{
					++it;
				}
			}
		}

		/**
		* @brief Elimina todos los componentes del nombre
		* O(n)
		*/
		void removeComponent(const std::string& name);

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
		* @brief Obtiene el primer componente con el nombre indicado
		* O(n)
		*/
		std::shared_ptr<Component> getComponent(const std::string& name) const;

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
#pragma endregion

	protected:
		std::vector<std::shared_ptr<Component>> components;
		bool alive;
		bool visible;
		bool enabled;
		Scene* scene;
		entityID entityID;
		//grpId_t groupId; // para colisiones
		std::string name;
		bool dontDestroyOnLoad;
	};
} // end of name space
