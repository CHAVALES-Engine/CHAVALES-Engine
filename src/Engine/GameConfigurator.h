#pragma once
#include <string>
using namespace std;

class GameConfigurator {
	string sceneName;
	string root;

public:
	GameConfigurator(string s, string r) : sceneName(s), root(r) {};

	string getSceneName() { return sceneName; };
	string getRoot() { return root; };
};