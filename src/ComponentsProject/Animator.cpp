#include "Animator.h"

#include "Entity.h"
#include "Engine.h"

Animator::Animator()
{

}

Animator::~Animator()
{

}

bool Animator::init(const Properties& p)
{
	Engine::instance()->addAnimator(getEntity()->getEntityID(), _modelID);
	return true;
}

void Animator::update(uint64_t deltaTime)
{
	for (auto& animation : _animations)
	{
		if (animation.second.active)
		{
			Engine::instance()->updateAnimation(animation.second.id, deltaTime);
		}
	}
}

bool Animator::registerSkeletonAnim(const std::string& animationName, const bool& loop)
{
	animationID id = Engine::instance()->registerSkeletonAnim(_modelID, animationName, loop);
	if (id != -1)
	{
		_animations.emplace(animationName, Animation(id, skeletal));
		return true;
	}
	return false;
}

void Animator::createTransformAnimation(const std::string& animationName, const bool& loop, const float& totalDuration)
{
	animationID id = Engine::instance()->createTransformAnimation(getEntity()->getEntityID(), animationName, loop, totalDuration);
	_animations.emplace(animationName, Animation(id, transform));
}

void Animator::addTransformKeyFrame(const std::string& animationName, const float& timePos, const core::Vector3<float>& pos, const core::Quaternion<float>& rot, const core::Vector3<float>& scale)
{
	auto aux = _animations.find(animationName);
	if (aux != _animations.end())
	{
		Engine::instance()->addTransformKeyFrame(aux->second.id, timePos, pos, rot, scale);
	}
}

void Animator::addTransformKeyFrame(const std::string& animationName, const float& timePos, const core::Vector3<float>& pos, const float& rot, const Axis& axis, const core::Vector3<float>& scale)
{
	auto aux = _animations.find(animationName);
	if (aux != _animations.end() && aux->second.type == transform)
	{
		int axisToInt;
		switch (axis)
		{
		case X:
			axisToInt = 0;
			break;
		case Y:
			axisToInt = 1;
			break;
		case Z:
			axisToInt = 2;
			break;
		}
		Engine::instance()->addTransformKeyFrame(aux->second.id, timePos, pos, rot, axisToInt, scale);
	}
}

void Animator::setAnimEnabled(const std::string& animationName, const bool& active)
{
	auto aux = _animations.find(animationName);
	if (aux != _animations.end())
	{
		Engine::instance()->setAnimEnabled(aux->second.id, active);
	}
}

void Animator::setAnimTimePos(const std::string& animationName, const float& timePos)
{
	auto aux = _animations.find(animationName);
	if (aux != _animations.end())
	{
		Engine::instance()->setAnimEnabled(aux->second.id, timePos);
	}
}


