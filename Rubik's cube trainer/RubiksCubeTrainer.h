#pragma once
#include <string>
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <GL/glew.h>
#include <SFML/Graphics.hpp>
using namespace std;

class RubiksCubeTrainer
{
	GLfloat lastX = 400, lastY = 700;
	bool firstMouse = true;
	GLfloat deltaTime = 0.0f;
	GLuint VBO, VAO;
	Camera camera;
	sf::Window window;
	sf::ContextSettings settings;
	string name;
	GLuint screenWidth;
	GLuint screenHeight;
	bool running;
	void processEvents();
	void update(sf::Vector2i mousePosition);
	void draw(Shader shader, vector<glm::vec3> &cubePositions);
	void initWindow();
	void keyboard_callback();
	void mouse_callback(sf::Vector2i mousePosition);
	void setupVAOandVBO(vector<GLfloat> vertices);

public:
	RubiksCubeTrainer(string name, GLuint screenWidth, GLuint screenHeight);
	
	void start();
};

