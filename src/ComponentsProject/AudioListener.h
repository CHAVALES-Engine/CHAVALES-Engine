#pragma once
#include <Component.h>
#include "Vector3.h"

class Transform;
class Engine;
class AudioListener : public core::Component
{
private:
	Transform* _transform;
	Engine* _eng;
	core::Vector3<> _lastPos;

public:
	AudioListener();
	~AudioListener() override;

	bool init(const Properties& p) override;
	virtual void ready() override;
	virtual void update(uint64_t deltaTime) override;
};

