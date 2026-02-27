//// This file is part of the course TPV2@UCM - Samir Genaim
//
//#pragma once
//#include <iostream>
//#include <vector>
//#include <array>
//#include <cassert>
//
//#include "Component.h"
//#include "ecs.h"
//#include "messages.h"
//#include "Entity.h"
//#include "System.h"
//
//namespace ecs
//{
//	/*
//	 * A class for managing the list of entities, groups, etc.
//	 *
//	 * In this version the manager is responsible on several things
//	 * that used to be done by the class Entity (like addComponent), just
//	 * to make it easier to develop the memory management later.
//	 *
//	 * Note that no outside the classes of ECS can access an Entity directly,
//	 * they just have a reference to it.
//	 *
//	 *  */
//	class Manager
//	{
//	public:
//		Manager();
//		virtual ~Manager();
//
//		// Adding an entity simply creates an instance of Entity, adds
//		// it to the list of the given group and returns it to the caller.
//		//
//		entity_t addEntity(grpId_t gId = grp::DEFAULT)
//		{
//			// create and initialise the entity
//			auto e = new Entity(gId);
//			e->alive_ = true;
//
//			// add the entity 'e' to list of entities of the given group
//			//
//			// IMPORTANT NOTE:
//			//
//			// Currently we immediately add the entity to the list of entities,
//			// so we will actually see them in this 'frame' if we traverse the list of
//			// entities afterwards!
//			//
//			// A better solution would be to add them to an auxiliary list, and
//			// define a method 'flush()' that moves them from the auxiliary list
//			// to the corresponding list of entities.
//			//
//			// We will have to call 'flush()' in each iteration of the
//			// main loop. This way we guarantee that entities that are added in one
//			// 'frame' they will appear only in the next 'frame' -- I leave it as an
//			// exercise for you ... it could be incorporated in 'refresh' as well.
//			//
//			entsByGroup_[gId].push_back(e);
//
//			// return it to the caller
//			//
//			return e;
//		}
//
//		// Setting the state of entity 'e' (alive or dead)
//		//
//		void setAlive(entity_t e, bool alive)
//		{
//			e->alive_ = alive;
//		}
//
//		// Returns the state of the entity 'e' (alive o dead)
//		//
//		bool isAlive(entity_t e)
//		{
//			return e->alive_;
//		}
//
//		// Adds a component to the entity 'e'. It receives the type
//		// T (to be created), and the list of arguments (if any) to
//		// be passed to the constructor. The component identifier
//		// 'cId' is taken from T::id.
//		//
//		template <typename T, typename... Ts>
//		T* addComponent(entity_t e, Ts&&... args)
//		{
//			constexpr cmpId_t cId = T::id;
//			assert(cId < maxComponentId);
//
//			// delete the current component, if any
//			//
//			removeComponent<T>(e);
//
//			// create, initialise and install the new component
//			//
//			Component* c = new T(std::forward<Ts>(args)...);
//			c->setContext(e, this);
//			c->initComponent();
//			e->cmps_[cId] = c;
//			e->currCmps_.push_back(c);
//
//			// return it to the user so i can be initialised if needed
//			return static_cast<T*>(c);
//		}
//
//		// Removes the component of Entity 'e' at position T::id.
//		//
//		template <typename T>
//		void removeComponent(entity_t e)
//		{
//			constexpr cmpId_t cId = T::id;
//			assert(cId < maxComponentId);
//
//			if (e->cmps_[cId] != nullptr)
//			{
//				// find the element that is equal to e->cmps_[cId] (returns an iterator)
//				//
//				auto iter = std::find(e->currCmps_.begin(), e->currCmps_.end(),
//				                      e->cmps_[cId]);
//
//				// must have such a component
//				assert(iter != e->currCmps_.end());
//
//				// and then remove it
//				e->currCmps_.erase(iter);
//				// destroy it
//				//
//				delete e->cmps_[cId];
//
//				// remove the pointer
//				//
//				e->cmps_[cId] = nullptr;
//			}
//		}
//
//		// Returns the component that corresponds to position T::id, casting it
//		// to T*. The casting is done just for ease of use, to avoid casting
//		// outside.
//		//
//		template <typename T>
//		T* getComponent(entity_t e)
//		{
//			constexpr cmpId_t cId = T::id;
//			assert(cId < maxComponentId);
//
//			return static_cast<T*>(e->cmps_[cId]);
//		}
//
//		// returns true if there is a component with identifier T::id
//		// in the entity 'e'
//		//
//		template <typename T>
//		bool hasComponent(entity_t e)
//		{
//			constexpr cmpId_t cId = T::id;
//			assert(cId < maxComponentId);
//
//			return e->cmps_[cId] != nullptr;
//		}
//
//		// returns the group 'gId' of entity 'e'
//		//
//		grpId_t groupId(entity_t e)
//		{
//			return e->gId_;
//		}
//
//		// returns the vector of all entities of a given group
//		//
//		const auto& getEntities(grpId_t gId = grp::DEFAULT)
//		{
//			return entsByGroup_[gId];
//		}
//
//		// associates the entity 'e' to the handler 'hId'
//		//
//		void setHandler(hdlrId_t hId, entity_t e)
//		{
//			assert(hId < maxHandlerId);
//			hdlrs_[hId] = e;
//		}
//
//		// returns the entity associated to the handler 'hId'
//		//
//		entity_t getHandler(hdlrId_t hId)
//		{
//			assert(hId < maxHandlerId);
//			return hdlrs_[hId];
//		}
//
//		// Adds a System to the manager. It receives the type
//		// T of the system (to be created), and the list of
//		// arguments (if any) to be passed to the system. The
//		// system's identifier 'cId' is taken from T::id.
//		//
//		template <typename T, typename... Ts>
//		T* addSystem(Ts&&... args)
//		{
//			constexpr sysId_t sId = T::id;
//			assert(sId < maxSystemId);
//
//			removeSystem<T>();
//
//			// create, initialise and install the new component
//			//
//			System* s = new T(std::forward<Ts>(args)...);
//			s->setContext(this);
//			s->initSystem();
//			sys_[sId] = s;
//
//			// return it to the user so it can be initialised if needed
//			return static_cast<T*>(s);
//		}
//
//		// Removes the system at position T::id.
//		//
//		template <typename T>
//		void removeSystem()
//		{
//			constexpr sysId_t sId = T::id;
//			assert(sId < maxSystemId);
//
//			if (sys_[sId] != nullptr)
//			{
//				// destroy it
//				//
//				delete sys_[sId];
//
//				// remove the pointer
//				//
//				sys_[sId] = nullptr;
//			}
//		}
//
//		// Returns the system that corresponds to position T::id, casting it
//		// to T*. The casting is done just for ease of use, to avoid casting
//		// outside.
//		//
//		template <typename T>
//		T* getSystem()
//		{
//			constexpr sysId_t sId = T::id;
//			assert(sId < maxSystemId);
//
//			return static_cast<T*>(sys_[sId]);
//		}
//
//		void send(const Message& m, bool delay = false)
//		{
//			if (!delay)
//			{
//				for (System* s : sys_)
//				{
//					if (s != nullptr)
//						s->recieve(m);
//				}
//			}
//			else
//			{
//				// will make a copy of m, we could use std::move to move it
//				// but then we will have to remove const from above
//				msgs_.emplace_back(m);
//			}
//		}
//
//		// this method should be called in the main loop to send queued
//		// messages, i.e., those were sent using send(m,true)
//		//
//		void flushMessages()
//		{
//			// we traverse until msgs_.size(), so if new message
//			// we added we don't send them now. If you wish to send
//			// them as will you should write this loop in a different way
//			// and maybe using std::list would be better.
//			//
//			auto size = msgs_.size();
//			for (auto i = 0u; i < size; i++)
//			{
//				auto& m = msgs_[i];
//				for (System* s : sys_)
//				{
//					if (s != nullptr)
//						s->recieve(m);
//				}
//			}
//
//			// delete all message that we have sent. This might be expensive
//			// since it has to shift all remaining elements to the left. A better
//			// solution would be to keep two vectors 'v1' and 'v2', when sending a
//			// message we always add it to 'v1' and in flush we swap them and send
//			// all messages in v2. After flush we simply clear v2
//			//
//			msgs_.erase(msgs_.begin(), msgs_.begin() + size);
//		}
//
//		// THIS IS THE VERSION THAT SWAP QUEUES, IF YOU WANT TO USE IT
//		//
//		// this method should be called in the main loop to send queued
//		// messages, i.e., those were sent using send(m,true)
//		//
//		void flushMessagesWithSwap()
//		{
//			std::swap(msgs_, msgs_aux_);
//			for (auto& m : msgs_aux_)
//			{
//				for (System* s : sys_)
//				{
//					if (s != nullptr)
//						s->recieve(m);
//				}
//			}
//
//			msgs_aux_.clear();
//		}
//
//		// Updating  an entity simply calls the update of all
//		// components
//		//
//		void update(entity_t e)
//		{
//			auto n = e->currCmps_.size();
//			for (auto i = 0u; i < n; i++)
//				e->currCmps_[i]->update();
//		}
//
//		// Rendering an entity simply calls the render of all
//		// components
//		//
//		void render(entity_t e)
//		{
//			auto n = e->currCmps_.size();
//			for (auto i = 0u; i < n; i++)
//				e->currCmps_[i]->render();
//		}
//
//		// call update of all entities
//		//
//		void update()
//		{
//			for (auto& ents : entsByGroup_)
//			{
//				auto n = ents.size();
//				for (auto i = 0u; i < n; i++)
//					update(ents[i]);
//			}
//		}
//
//		// call render of all entities
//		//
//		void render()
//		{
//			for (auto& ents : entsByGroup_)
//			{
//				auto n = ents.size();
//				for (auto i = 0u; i < n; i++)
//					render(ents[i]);
//			}
//		}
//
//
//		void refresh();
//
//	private:
//		std::array<entity_t, maxHandlerId> hdlrs_;
//		std::array<std::vector<entity_t>, maxGroupId> entsByGroup_;
//		std::array<System*, maxSystemId> sys_;
//
//		std::vector<Message> msgs_;
//		std::vector<Message> msgs_aux_;
//	};
//} // end of namespace
