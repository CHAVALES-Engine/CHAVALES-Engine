#pragma once
#include <memory>
#include <unordered_map>

class Scene;
class Timing;

using sceneID = uint32_t;
using scenePtr = std::shared_ptr<Scene>;
using scene = std::pair<sceneID, scenePtr>;

using stateMachine = std::unordered_map<sceneID, scenePtr>;

class StateMachine
{
public:
	StateMachine();
	~StateMachine();

	void gameLoop();

	void addScene(sceneID i, scenePtr s);
	void setScene(sceneID s);

	void deleteScene(sceneID s);

	inline sceneID getCurrentScnID() const {
		return _currentScene.first;
	}

	inline scenePtr getCurrentScnPtr() const {
		return _currentScene.second;
	}

	inline scene getCurrentScn() const {
		return _currentScene;
	}

private:
	stateMachine _stateMachine;
	scene _currentScene;
};

