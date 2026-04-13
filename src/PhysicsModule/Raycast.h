#pragma once

#include <PxPhysicsAPI.h>

using namespace physx;

/**
 * @class Raycast
 * @brief Encapsula operaciones de raycast dentro de la escena física.
 *
 * Permite lanzar rayos en la escena (PhysX) para detectar colisiones,
 * impactos y obtener información del objeto golpeado.
 */
class Raycast
{
private:

    /**
     * @brief Referencia a la escena de PhysX donde se realizan los raycasts
     */
    PxScene* scene;

public:

    /**
     * @brief Constructor que recibe la escena física
     */
    Raycast(PxScene* sceneRef);

    /**
     * @brief Lanza un rayo en la escena
     * @param origin Punto de origen del rayo
     * @param direction Dirección del rayo (debe estar normalizada)
     * @param maxDistance Distancia máxima del rayo
     * @param hitInfo Buffer donde se guarda la información del impacto
     * @return true si el rayo impacta, false si no
     */
    bool Cast(
        const PxVec3& origin,
        const PxVec3& direction,
        float maxDistance,
        PxRaycastBuffer& hitInfo
    );
};