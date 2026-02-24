// This file is part of the course TPV2@UCM - Samir Genaim

// DO NOT USE/MODIFY THIS FILE, IT IS JUST AN EXAMPLE OF HOW YOUR
// game/ecs_defs.h SHOULD LOOK LIKE
//
// Note that the backslash \ after each line below is very important
// when using #define, if you remove it then put all at the same
// line of #define
//

#pragma once

// Components list - must have at least one element
//
#define _CMPS_LIST_ \
	TRANSFORM, \
	IMAGE

// Groups list - must have at least one element
//
#define _GRPS_LIST_ \
	STAR, \
	ASTEROID

// Handlers list - must have at least one element
//
#define _HDLRS_LIST_ \
    BALL, \
	PADDLE

