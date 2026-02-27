// This file is part of the course TPV2@UCM - Samir Genaim

#pragma once

/*
 * We put the definition of Message in a separated file just to
 * avoid circular dependencies with ecs.h (maybe there is a better
 * solution!).
 *
 * We do not put them in the ecs namspace to avoid the boredom
 * of writing the prefix ecs:: in the user classes.
 *
 * By default the struct Message is
 *
 *  struct Message {
 *	   uint8_t id;
 *  }
 *
 * and the user can redefine it in ../gemae/messages_defs.h (using
 * the same name Message).
 *
 * There are many ways how to define several messages, but keep it
 * simple! You only one struct called Message that has a field
 *
 *    uint8_t id
 *
 * to indicated the type of the message, and other fields to carry all
 * necessary information. If space matters for you, you can use
 * union types to save some space, but if you use union types then use
 * fields of primite types only, otherwise you will endup defining
 * constructors everywhere!
 *
 * For an example, see messages_defs_example.h
 *
 *
 * Never include your messages_defs.h directly in your program, always
 * use ecs/messages.h
 *
 */

#if __has_include("../game/messages_defs.h")
#include "../game/messages_defs.h"
#else
#include <cstdint>
struct Message {
	uint8_t id;
};
#endif
