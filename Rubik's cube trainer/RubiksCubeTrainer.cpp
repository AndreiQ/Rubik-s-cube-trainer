#include "RubiksCubeTrainer.h"
#define RED		1.0f, 0.0f, 0.0f
#define BLUE	0.0f, 0.0f, 1.0f
#define GREEN	0.0f, 1.0f, 0.0f
#define ORANGE	1.0f, 0.5f, 0.0f
#define YELLOW	1.0f, 1.0f, 0.0f
#define WHITE	1.0f, 1.0f, 1.0f

void RubiksCubeTrainer::keyboard_callback()
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		// left key is pressed: move our character
		camera.ProcessKeyboard(LEFT, deltaTime);
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		// left key is pressed: move our character
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
}

void RubiksCubeTrainer::mouse_callback(sf::Vector2i mousePosition)
{
	if (firstMouse)
	{
		lastX = mousePosition.x;
		lastY = mousePosition.y;
		firstMouse = false;
	}

	GLfloat xoffset = mousePosition.x - lastX;
	GLfloat yoffset = lastY - mousePosition.y;  // Reversed since y-coordinates go from bottom to left

	lastX = mousePosition.x;
	lastY = mousePosition.y;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void RubiksCubeTrainer::setupVAOandVBO(vector<GLfloat> vertices)
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), &vertices[0], GL_STATIC_DRAW);
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), nullptr);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0); // Unbind VAO
}

RubiksCubeTrainer::RubiksCubeTrainer(string name, GLuint screenWidth, GLuint screenHeight): running(true)
{
	this->name = name;
	this->screenHeight = screenHeight;
	this->screenWidth = screenWidth;
}

void RubiksCubeTrainer::processEvents()
{
	sf::Event event;
	while (window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			running = false;
			break;
		case sf::Event::Resized:
			glViewport(0, 0, event.size.width, event.size.height);
			break;
		default:
			break;
		}
	}
}
void RubiksCubeTrainer::update(sf::Vector2i mousePosition)
{

	keyboard_callback();
	mouse_callback(mousePosition);
}
void RubiksCubeTrainer::draw(Shader ourShader,vector<glm::vec3> &cubePositions)
{

	glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	ourShader.Use();
	glm::mat4 view = camera.GetViewMatrix();
	glm::mat4 projection = glm::perspective(45.0f, static_cast<float>(screenWidth) / static_cast<float>(screenHeight), 0.1f, 1000.0f);
	// Get the uniform locations
	GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
	GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
	GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
	// Pass the matrices to the shader
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glBindVertexArray(VAO);
	//in draw function
	for (GLuint i = 0; i < 27; i++)
	{
		// Calculate the model matrix for each object and pass it to shader before drawing
		glm::mat4 model = glm::translate(model, cubePositions[i]);
		//ROTATE AROUND POINT
		//	GLfloat angle = 3.0f * deltaTime;
		//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		//	model = glm::translate(model, glm::vec3(-1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}

	this->window.display();
}
void RubiksCubeTrainer::initWindow()
{
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	this->window.create(sf::VideoMode(screenWidth,screenHeight),name, sf::Style::Default, settings);
	this->window.setActive(true);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
}
void RubiksCubeTrainer::start()
{
	initWindow();
	Shader ourShader("Shaders/vertex.vs", "Shaders/fragment.frag");
	// Set up our vertex data (and buffer(s)) and attribute pointers
	vector<GLfloat> vertices{
		//size = 6*36
		//positions				//colors
		//						R    G    B
		//BACK
		-0.5f, -0.5f, -0.5f,	RED,
		0.5f, -0.5f, -0.5f,		RED,
		0.5f,  0.5f, -0.5f,		RED,
		0.5f,  0.5f, -0.5f,		RED,
		-0.5f,  0.5f, -0.5f,	RED,
		-0.5f, -0.5f, -0.5f,	RED,
		//36
		//FRONT
		0.5f,  0.5f,  0.5f,  ORANGE,
		0.5f,  0.5f, -0.5f,  ORANGE,
		0.5f, -0.5f, -0.5f,  ORANGE,
		0.5f, -0.5f, -0.5f,  ORANGE,
		0.5f, -0.5f,  0.5f,  ORANGE,
		0.5f,  0.5f,  0.5f,  ORANGE,
		//LEFT

		-0.5f, -0.5f,  0.5f,  GREEN,
		0.5f, -0.5f,  0.5f,  GREEN,
		0.5f,  0.5f,  0.5f,  GREEN,
		0.5f,  0.5f,  0.5f,  GREEN,
		-0.5f,  0.5f,  0.5f,  GREEN,
		-0.5f, -0.5f,  0.5f,  GREEN,//green
		//RIGHT

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,//blue
		//UP
		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,//yellow
		//DOWN
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f//white
	};
	vector<glm::vec3> cubePositions = {
		//FRONT
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f,0.0f),
		glm::vec3(0.0f, 1.0f,0.0f),
		glm::vec3(0.0f, 2.0f,0.0f),
		glm::vec3(1.0f, 1.0f,0.0f), //CENTER
		glm::vec3(1.0f, 2.0f,0.0f),
		glm::vec3(2.0f, 1.0f,0.0f),
		glm::vec3(2.0f, 2.0f,0.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(1.0f, 0.0f, -1.0f),
		glm::vec3(2.0f, 0.0f,-1.0f),
		glm::vec3(0.0f, 1.0f,-1.0f),
		glm::vec3(0.0f, 2.0f,-1.0f),
		glm::vec3(1.0f, 1.0f,-1.0f), //CENTER
		glm::vec3(1.0f, 2.0f,-1.0f),
		glm::vec3(2.0f, 1.0f,-1.0f),
		glm::vec3(2.0f, 2.0f,-1.0f),

		glm::vec3(0.0f, 0.0f, -2.0f),
		glm::vec3(1.0f, 0.0f, -2.0f),
		glm::vec3(2.0f, 0.0f,-2.0f),
		glm::vec3(0.0f, 1.0f,-2.0f),
		glm::vec3(0.0f, 2.0f,-2.0f),
		glm::vec3(1.0f, 1.0f,-2.0f), //CENTER
		glm::vec3(1.0f, 2.0f,-2.0f),
		glm::vec3(2.0f, 1.0f,-2.0f),
		glm::vec3(2.0f, 2.0f,-2.0f),

	};
	sf::Clock clock;

	setupVAOandVBO(vertices);
	while (running)
	{

		sf::Time elapsed = clock.restart();
		deltaTime = elapsed.asSeconds();
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
		processEvents();
		update(localPosition);
		draw(ourShader,cubePositions);
		glBindVertexArray(0);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

}
