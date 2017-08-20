#pragma once
#include <string>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
using namespace std;

class RubiksCubeTrainer
{
private:
	void initWindow(string, int, int);

	void do_movement();
	void mouse_callback(sf::Vector2i pos);

public:
	RubiksCubeTrainer();
	void start();
};

