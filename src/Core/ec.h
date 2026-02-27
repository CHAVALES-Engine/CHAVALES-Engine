/*
 * @file
 * @brief
 * This file was adapted from a part of the course TPV2@UCM - Samir Genaim
 */
#pragma once

#include <cstdint>

namespace core 
{
	// forward declaration of some classes, to be used when we
	// just need to know that they exist
	class Manager;
	class Entity;
	class Component;

	// we hide the actual type of the entity
	//using entity_t = Entity*;

	// We define type for the identifiers so we can change them easily.
	// For example, if we have less than 256 components we can use one
	// byte, i.e. uint8_t, if we have up to 512 we can use uint16_t,
	// and so on ...
	//
	using cmpId_t = uint8_t;
	using grpId_t = uint8_t;
}
