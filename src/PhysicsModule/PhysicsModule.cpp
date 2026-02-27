// PhysicModule.cpp : Defines the functions for the static library.
//
//#include "../../dependencies/PhysX/physx/include/PxPhysicsAPI.h"
#include "PxPhysicsAPI.h"
#include "PhysicsModule.h"
#include <vector>
// TODO: This is an example of a library function
using namespace physx;

PxDefaultAllocator		gAllocator;
PxDefaultErrorCallback	gErrorCallback;

PxFoundation* gFoundation = NULL;
PxPhysics* gPhysics = NULL;
PxPvd* gPvd = NULL;

//void fnPhysicModule()
//{
//}

PhysicsModule::PhysicsModule()
{
}

bool PhysicsModule::Init()
{
    gFoundation = PxCreateFoundation(
        PX_PHYSICS_VERSION,
        gAllocator,
        gErrorCallback
    );

    if (!gFoundation)
        return false;

    gPvd = PxCreatePvd(*gFoundation);

    PxPvdTransport* transport =
        PxDefaultPvdSocketTransportCreate("127.0.0.1", 5425, 10);//debugger visual 

    if (gPvd)
        gPvd->connect(*transport, PxPvdInstrumentationFlag::eALL);

    PxTolerancesScale scale;

    gPhysics = PxCreatePhysics(
        PX_PHYSICS_VERSION,
        *gFoundation,
        scale,
        true,
        gPvd
    );

    return gPhysics != nullptr;
}

