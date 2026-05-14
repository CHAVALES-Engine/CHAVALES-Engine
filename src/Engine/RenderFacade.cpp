//#include "RenderFacade.h"
//#include "RenderModule.h"
//
//RenderFacade::RenderFacade(RenderModule* render): _render(render)
//{}
//
//transformID RenderFacade::addTransform(const entityID& entityID, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
//{
//	return _render->addNode(entityID, pos, rot, scale, true);
//}
//
//void RenderFacade::setTransformPosition(const transformID& id, const core::Vector3<float>& pos)
//{
//	_render->setNodePosition(id, pos);
//}
//
//void RenderFacade::setTransformRotation(const transformID& id, const core::Quaternion<float>& rot)
//{
//	_render->setNodeRotation(id, rot);
//}
//
//void RenderFacade::setTransformScale(const transformID& id, const core::Vector3<float>& scale)
//{
//	_render->setNodeScale(id, scale);
//}
//
//cameraID RenderFacade::addCamera(const entityID& entityID, const float& FOVy, const float& nearClipDistance, const float& farClipDistance, const float& focalLength, const core::Color& bgColor)
//{
//	return _render->addCamera(entityID, FOVy, nearClipDistance, farClipDistance, focalLength, bgColor);
//}
//
//void RenderFacade::deleteCamera(const cameraID& id)
//{
//	_render->deleteCamera(id);
//}
//
//void RenderFacade::setAsActiveCamera(const cameraID& id)
//{
//	_render->setAsActiveCamera(id);
//}
//
//void RenderFacade::setCameraFOVy(const cameraID& id, const float& FOVy)
//{
//	_render->setCameraFOVy(id, FOVy);
//}
//
//void RenderFacade::setCameraNearClipDistance(const cameraID& id, const float& nearClipDistance)
//{
//	_render->setCameraNearClipDistance(id, nearClipDistance);
//}
//
//void RenderFacade::setCameraFarClipDistance(const cameraID& id, const float& farClipDistance)
//{
//	_render->setCameraFarClipDistance(id, farClipDistance);
//}
//
//void RenderFacade::setCameraFocalLength(const cameraID& id, const float& focalLength)
//{
//	_render->setCameraFocalLength(id, focalLength);
//}
//
//modelID RenderFacade::addModel(const entityID& entityID, const std::string& modelName)
//{
//	auto model = _resourcesModule->getAssetSourceFolder(modelName);
//	return _render->addModel(entityID, model.second, model.first);
//}
//
//void RenderFacade::deleteModel(const modelID& id)
//{
//	_render->deleteModel(id);
//}
//
//void RenderFacade::setSubmeshDiffuse(const modelID& id, const std::string& textureName, const int& submesh)
//{
//	auto texture = getAssetSourceFolder(textureName);
//	_render->setDiffuse(id, submesh, texture.second, texture.first);
//}
//
//void RenderFacade::setSubmeshTint(const modelID& id, const core::Color& tint, const int& submesh)
//{
//	_render->setTint(id, submesh, tint);
//}
//
//void RenderFacade::setModelVisible(const modelID& id, const bool& visible)
//{
//	_render->setModelVisible(id, visible);
//}
//
//animationID RenderFacade::registerSkeletonAnim(const modelID& modelID, const std::string& animationName, const bool& loop)
//{
//	return _render->registerSkeletonAnim(modelID, animationName, loop);
//}
//
//animationID RenderFacade::createTransformAnimation(const entityID& entityID, const std::string& animationName, const bool& loop, const float& totalDuration)
//{
//	return _render->createTransformAnimation(entityID, animationName, loop, totalDuration);
//}
//
//void RenderFacade::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
//{
//	_render->addTransformKeyFrame(animationID, timePos, pos, rot, scale);
//}
//
//void RenderFacade::addTransformKeyFrame(const animationID& animationID, const float& timePos, const core::Vector3<float>& pos, const float& rot, const int& axis, const core::Vector3<float>& scale)
//{
//	_render->addTransformKeyFrame(animationID, timePos, pos, rot, axis, scale);
//}
//
//void RenderFacade::setAnimEnabled(const animationID& animationID, const bool& active)
//{
//	_render->setAnimEnabled(animationID, active);
//}
//
//void RenderFacade::setAnimTimePos(const animationID& animationID, const float& timePos)
//{
//	_render->setAnimTimePos(animationID, timePos);
//}
//
//void RenderFacade::setAnimSpeed(const animationID& animationID, const float& speed)
//{
//	_render->setAnimSpeed(animationID, speed);
//}
//
//void RenderFacade::updateAnimation(const animationID& animationID, const uint64_t& deltaTime)
//{
//	_render->updateAnimation(animationID, deltaTime);
//}
//
//lightID RenderFacade::addLight(const entityID& entityID, const int& type, const core::Color& color, const float& intensity)
//{
//	return _render->addLight(entityID, type, color, intensity);
//}
//
//void RenderFacade::deleteLight(const lightID& id)
//{
//	_render->deleteLight(id);
//}
//
//void RenderFacade::setLightActive(const lightID& id, const bool& active)
//{
//	_render->setLightActive(id, active);
//}
//
//void RenderFacade::setLightType(const lightID& id, const int& type)
//{
//	_render->setLightType(id, type);
//}
//
//void RenderFacade::setLightColor(const lightID& id, const core::Color& color)
//{
//	_render->setLightColor(id, color);
//}
//
//void RenderFacade::setLightIntensity(const lightID& id, const float& intensity)
//{
//	_render->setLightIntensity(id, intensity);
//}
//
//void RenderFacade::setLightSpotRange(const lightID& id, const float& inner, const float& outer, const float& falloff)
//{
//	_render->setLightSpotRange(id, inner, outer, falloff);
//}
//
//void RenderFacade::setAmbientLight(const core::Color& color)
//{
//	_render->setAmbientLight(color);
//}
//
//particleGenID RenderFacade::addParticleGen(const entityID& entityID, const std::string& textureName)
//{
//	auto particle = getAssetSourceFolder(textureName);
//	return _render->addParticleGen(entityID, particle.second, particle.first);
//}
//
//void RenderFacade::deleteParticleGen(const particleGenID& id)
//{
//	_render->deleteParticleGen(id);
//}
//
//void RenderFacade::setParticleGenEnabled(const particleGenID& id, const bool& enabled)
//{
//	_render->setParticleGenEnabled(id, enabled);
//}
//
//void RenderFacade::setParticleGenEmitting(const particleGenID& id, const bool& emitting)
//{
//	_render->setParticleGenEmitting(id, emitting);
//}
//
//void RenderFacade::setParticleGenQuota(const particleGenID& id, const float& quota)
//{
//	_render->setParticleGenQuota(id, quota);
//}
//
//void RenderFacade::setParticleGenEmissionRate(const particleGenID& id, const float& rate)
//{
//	_render->setParticleGenEmissionRate(id, rate);
//}
//
//void RenderFacade::setParticleGenDuration(const particleGenID& id, const float& duration)
//{
//	_render->setParticleGenDuration(id, duration);
//}
//
//void RenderFacade::setParticleGenTimeToLive(const particleGenID& id, const float& time)
//{
//	_render->setParticleGenTimeToLive(id, time);
//}
//
//void RenderFacade::setParticleGenVelocity(const particleGenID& id, const float& velocity)
//{
//	_render->setParticleGenVelocity(id, velocity);
//}
//
//void RenderFacade::setParticleGenMinVelocity(const particleGenID& id, const float& velocity)
//{
//	_render->setParticleGenMinVelocity(id, velocity);
//}
//
//void RenderFacade::setParticleGenMaxVelocity(const particleGenID& id, const float& velocity)
//{
//	_render->setParticleGenMaxVelocity(id, velocity);
//}
//
//void RenderFacade::setParticleGenDirection(const particleGenID& id, const core::Vector3<float>& direction)
//{
//	_render->setParticleGenDirection(id, direction);
//}
//
//void RenderFacade::setParticleGenAngle(const particleGenID& id, const float& angle)
//{
//	_render->setParticleGenAngle(id, angle);
//}
//
//void RenderFacade::setParticleGenPartWidth(const particleGenID& id, const float& width)
//{
//	_render->setParticleGenPartWidth(id, width);
//}
//
//void RenderFacade::setParticleGenPartHeight(const particleGenID& id, const float& height)
//{
//	_render->setParticleGenPartHeight(id, height);
//}
//
//void RenderFacade::setParticleGenPartColor(const particleGenID& id, const core::Color& color)
//{
//	_render->setParticleGenPartColor(id, color);
//}
//
//void RenderFacade::setSkydome(const std::string& textureName, const float& curvature, const float& tiling, const float& distance, const bool& drawFirst)
//{
//	auto skydome = getAssetSourceFolder(textureName);
//	_render->setSkydome(skydome.second, skydome.first, curvature, tiling, distance, drawFirst);
//}
//
//void RenderFacade::setSkydomeNull()
//{
//	_render->setSkydomeNull();
//}
