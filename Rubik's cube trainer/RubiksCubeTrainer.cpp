#include "RubiksCubeTrainer.h"

void RubiksCubeTrainer::do_movement()
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		// left key is pressed: move our character
		camera.ProcessKeyboard(FORWARD, deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		// left key is pressed: move our character
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	}
}

void RubiksCubeTrainer::mouse_callback(sf::Vector2i pos)

{
	if (firstMouse)
	{
		lastX = pos.x;
		lastY = pos.y;
		firstMouse = false;
	}

	GLfloat xoffset = pos.x - lastX;
	GLfloat yoffset = lastY - pos.y;  // Reversed since y-coordinates go from bottom to left

	lastX = pos.x;
	lastY = pos.y;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

RubiksCubeTrainer::RubiksCubeTrainer()
{
}
void RubiksCubeTrainer::initWindow() 
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	sf::Window window(sf::VideoMode(screenWidth, screenHeight), "OpenGL", sf::Style::Default, settings);
	window.setActive(true);
	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	glewInit();
	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);
	// Setup and compile our shaders
}
void RubiksCubeTrainer::start()
{
	initWindow();
	Shader ourShader("Shaders/vertex.vs", "Shaders/fragment.frag");

}
