//#pragma once
//#include <cstdint>
//
//#include "Color.h"
//#include "guid.h"
//#include "Quaternion.h"
//#include "Vector3.h"
//
//using entityID = ChavalesGUID;
//using transformID = uint64_t;
//using cameraID = uint64_t;
//using modelID = uint64_t;
//using animationID = uint64_t;
//using lightID = uint64_t;
//using particleGenID = uint64_t;
//
//class Engine;
//class RenderModule;
//
///**
// * @brief API publica para render del motor.
// * Expone solo los metodos seguros para componentes.
// * Para metodos internos, ver RenderModule.
// */
//class RenderFacade
//{
//private:
//	/**
//	 * @brief Clases amigas.
//	 */
//	friend Engine;
//	friend core::EngineComponent;
//	/**
//	 * @brief Referencia al modulo que envuelve.
//	 */
//	RenderModule* _render;
//	RenderFacade(RenderModule* render);
//
//	//Metodos transform
//#pragma region transform
//	/*
//	* @brief Anadir nodo.
//	*/
//	transformID addTransform(const entityID& entityID, const core::Vector3<float>& pos = core::Vector3<float>(0.0f, 0.0f, 0.0f), const core::Quaternion<float>& rot = core::Quaternion<float>(0.0f, 0.0f, 0.0f, 1.0f), const core::Vector3<float>& scale = core::Vector3<float>(1.0f, 1.0f, 1.0f));
//	/*
//	* @brief Establecer posicion del nodo.
//	*/
//	void setTransformPosition(const transformID& id, const core::Vector3<float>& pos);
//	/*
//	* @brief Establecer orientacion del nodo. Relativo a world space.
//	*/
//	void setTransformRotation(const transformID& id, const core::Quaternion<float>& rot);
//	/*
//	* @brief Establecer escala del nodo. Relativo a world space.
//	*/
//	void setTransformScale(const transformID& id, const core::Vector3<float>& scale);
//#pragma endregion
//
//	//Metodos camaras
//#pragma region camera
//	/*
//	* @brief Camara nueva. Se asigna un id por orden de creacion. Main Camera id 0 y a adidas manualmente 1 en adelante.
//	*/
//	cameraID addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor);
//	/*
//	* @brief Borrar camara por id. A las camaras creadas posteriormente se les resta el id en 1.
//	*/
//	void deleteCamera(const cameraID& id);
//	/*
//	* @brief El viewport mostrara la vista de esta camara.
//	*/
//	void setAsActiveCamera(const cameraID& id);
//	/*
//	* @brief Establecer FOVy.
//	*/
//	void setCameraFOVy(const cameraID& id, const float& FOVy);
//	/*
//	* @brief Establecer distancia del plano cercano.
//	*/
//	void setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance);
//	/*
//	* @brief Establecer distancia del plano lejano.
//	*/
//	void setCameraFarClipDistance(const cameraID& id, const float& farClipDistance);
//	/*
//	* @brief Establecer distancia focal.
//	*/
//	void setCameraFocalLength(const cameraID& id, const float& focalLength);
//#pragma endregion
//
//	//Metodos modelos
//#pragma region model
//	/*
//	* @brief anade un modelo a la escena.
//	*/
//	modelID addModel(const entityID& entityID, const std::string& modelName);
//	/*
//	* @brief Borra un modelo de la escena.
//	*/
//	void deleteModel(const modelID& id);
//	/*
//	* @brief Borra un modelo de la escena.
//	*/
//	void setSubmeshDiffuse(const modelID& id, const std::string& textureName, const int& submesh);
//	/*
//	* @brief Establecer tinte de material.
//	*/
//	void setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh);
//	/*
//	* @brief Establecer si el modelo es visible.
//	*/
//	void setModelVisible(const modelID& id, const bool& visible);
//#pragma endregion
//
//	//Metodos animaciones
//#pragma region animation
//	/*
//	* @brief Registrar animacion de esqueleto.
//	*/
//	animationID registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop);
//	/*
//	* @brief Crear animacion de transform.
//	*/
//	animationID createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration);
//	/*
//	* @brief Anadir keyframe a animacion de transform. Time pos en segundos.
//	*/
//	void addTransformKeyFrame(const animationID& animationID,
//		const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale);
//	/*
//	* @brief Anadir keyframe a animacion de transform con rotacion sencilla. Time pos en segundos.
//	*/
//	void addTransformKeyFrame(const animationID& animationID,
//		const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale);
//	/*
//	* @brief Establecer animacion activa.
//	*/
//	void setAnimEnabled(const animationID& animationID, const bool& active);
//	/*
//	* @brief Reanudar animacion a partir de cierto instante de tiempo.
//	*/
//	void setAnimTimePos(const animationID& animationID, const float& timePos);
//	/*
//	* @brief Ajusta la velocidad de la animacion.
//	*/
//	void setAnimSpeed(const animationID& animationID, const float& speed);
//	/*
//	* @brief Actualizar animacion.
//	*/
//	void updateAnimation(const animationID& animationID, const uint64_t& deltaTime);
//#pragma endregion
//
//	//Metodos luces
//#pragma region light
//	/*
//	* @brief Luz nueva. Se asigna un id por orden de creacion. Main Luz id 0 y anyadidas manualmente 1 en adelante.
//	*/
//	lightID addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity);
//	/*
//	* @brief Borrar luz por id. A las luces creadas posteriormente se les resta el id en 1.
//	*/
//	void deleteLight(const lightID& id);
//	/*
//	* @brief Establecer actividad de luz.
//	*/
//	void setLightActive(const lightID& id, const bool& active);
//	/*
//	* @brief Establecer el tipo de luz.
//	*/
//	void setLightType(const lightID& id, const int& type);
//	/*
//	* @brief Establecer el color de la luz.
//	*/
//	void setLightColor(const lightID& id, const core::Color& color);
//	/*
//	* @brief Establecer la intensidad de luz.
//	*/
//	void setLightIntensity(const lightID& id, const float& intensity);
//	/*
//	* @brief Establecer el cono de luz (angulo interno, angulo externo y suavidad de degradado).
//	*/
//	void setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff);
//#pragma endregion
//
//#pragma region ambientLight
//	/*
//	* @brief Cambiar luz ambiente de la escena.
//	*/
//	void setAmbientLight(const core::Color& color);
//#pragma endregion
//
//	//Metodos particulas
//#pragma region particle
//	//Metodos particulas
//	/*
//	* @brief Anadir generador de particulas.
//	*/
//	particleGenID addParticleGen(const entityID& entityID, const std::string& textureName);
//	/*
//	* @brief Borrar generador de particulas.
//	*/
//	void deleteParticleGen(const particleGenID& id);
//	/*
//	* @brief Establecer actividad de generador de particulas.
//	*/
//	void setParticleGenEnabled(const particleGenID& id, const bool& enabled);
//	/*
//	* @brief Establecer emision de generador de particulas.
//	*/
//	void setParticleGenEmitting(const particleGenID& id, const bool& emitting);
//	/*
//	* @brief Establecer cantidad total de particulas del generador.
//	*/
//	void setParticleGenQuota(const particleGenID& id, const float& quota);
//	/*
//	* @brief Establecer ratio de emision de generador de particulas.
//	*/
//	void setParticleGenEmissionRate(const particleGenID& id, const float& rate);
//	/*
//	* @brief Establecer tiempo de emision del generador de particulas.
//	*/
//	void setParticleGenDuration(const particleGenID& id, const float& duration);
//	/*
//	* @brief Establecer tiempo de vida de particulas del generador.
//	*/
//	void setParticleGenTimeToLive(const particleGenID& id, const float& time);
//	/*
//	* @brief Establecer velocidad de particulas del generador.
//	*/
//	void setParticleGenVelocity(const particleGenID& id, const float& velocity);
//	/*
//	* @brief Establecer velocidad minima de particulas del generador.
//	*/
//	void setParticleGenMinVelocity(const particleGenID& id, const float& velocity);
//	/*
//	* @brief Establecer velocidad maxima de particulas del generador.
//	*/
//	void setParticleGenMaxVelocity(const particleGenID& id, const float& velocity);
//	/*
//	* @brief Establecer direccion de particulas del generador.
//	*/
//	void setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction);
//	/*
//	* @brief Establecer angulo de dipsersion de particulas del generador.
//	*/
//	void setParticleGenAngle(const particleGenID& id, const float& angle);
//	/*
//	* @brief Establecer ancho de particulas del generador.
//	*/
//	void setParticleGenPartWidth(const particleGenID& id, const float& width);
//	/*
//	* @brief Establecer alto de particulas del generador.
//	*/
//	void setParticleGenPartHeight(const particleGenID& id, const float& height);
//	/*
//	* @brief Establecer color de particulas del generador.
//	*/
//	void setParticleGenPartColor(const particleGenID& id, const core::Color& color);
//#pragma endregion
//#pragma region skydome
//	/*
//	* @brief Establecer skydome.
//	*/
//	void setSkydome(const std::string& textureName, const float& curvature, const float& tiling, const float& distance, const bool& drawFirst);
//	/*
//	* @brief Quitar skydome
//	*/
//	void setSkydomeNull();
//#pragma endregion
//};
//
