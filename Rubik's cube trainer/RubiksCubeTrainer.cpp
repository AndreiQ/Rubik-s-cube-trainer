#include "RubiksCubeTrainer.h"
#define redColor	1.0f, 0.0f, 0.0f
#define blueColor	0.0f, 0.0f, 1.0f
#define greenColor	0.0f, 1.0f, 0.0f
#define orangeColor	1.0f, 0.5f, 0.0f
#define yellowColor	1.0f, 1.0f, 0.0f
#define whiteColor	1.0f, 1.0f, 1.0f
#define RED_SQUARE		redColor, redColor, redColor, redColor, redColor, redColor
#define BLUE_SQUARE		blueColor,blueColor,blueColor,blueColor,blueColor,blueColor
#define GREEN_SQUARE	greenColor,greenColor,greenColor,greenColor, greenColor,greenColor
#define ORANGE_SQUARE	orangeColor,orangeColor,orangeColor,orangeColor,orangeColor,orangeColor
#define YELLOW_SQUARE	yellowColor,yellowColor,yellowColor,yellowColor,yellowColor,yellowColor
#define WHITE_SQUARE	whiteColor,whiteColor,whiteColor,whiteColor,whiteColor,whiteColor
#define BACK_SQUARE		-0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f,		0.5f, 0.5f, -0.5f,		0.5f, 0.5f, -0.5f,		-0.5f, 0.5f, -0.5f,		-0.5f, -0.5f, -0.5f
#define FRONT_SQUARE	-0.5f, -0.5f, 0.5f,		0.5f, -0.5f, 0.5f,		0.5f, 0.5f, 0.5f,		0.5f, 0.5f, 0.5f,		-0.5f, 0.5f, 0.5f,		-0.5f, -0.5f, 0.5f
#define RIGHT_SQUARE     0.5f, 0.5f, 0.5f,		0.5f, 0.5f, -0.5f,		0.5f, -0.5f, -0.5f,		0.5f, -0.5f, -0.5f,		 0.5f, -0.5f, 0.5f,		 0.5f, 0.5f, 0.5f
#define LEFT_SQUARE     -0.5f, 0.5f, 0.5f,	   -0.5f, 0.5f, -0.5f,	   -0.5f, -0.5f, -0.5f,	   -0.5f, -0.5f, -0.5f,	    -0.5f, -0.5f, 0.5f,     -0.5f, 0.5f, 0.5f
#define DOWN_SQUARE     -0.5f, -0.5f, -0.5f,	0.5f, -0.5f, -0.5f,		0.5f, -0.5f, 0.5f,		0.5f, -0.5f, 0.5f,		-0.5f, -0.5f, 0.5f,		-0.5f, -0.5f, -0.5f
#define UP_SQUARE		-0.5f, 0.5f, -0.5f,     0.5f, 0.5f, -0.5f,      0.5f, 0.5f, 0.5f,       0.5f, 0.5f, 0.5f,       -0.5f, 0.5f, 0.5f,      -0.5f, 0.5f, -0.5f

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
	GLfloat yoffset = lastY - mousePosition.y; // Reversed since y-coordinates go from bottom to left

	lastX = mousePosition.x;
	lastY = mousePosition.y;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void RubiksCubeTrainer::setupVAOandVBO(vector<vector<GLfloat>> blocks)
{
	glGenVertexArrays(1, VAO);
	glGenBuffers(1, VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	for (int i = 0; i < 1; i++)
	{
		glBindVertexArray(VAO[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, blocks[i].size() * sizeof(GLfloat), &blocks[i][0], GL_STATIC_DRAW);
		// Position attribute
		//18 vertices, 18 color.  //square 1 
		//18 vertices, 18 color  //square 2

		
		//3v 3c , 3v 3c 
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
		glEnableVertexAttribArray(0);
		// color attribute
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), reinterpret_cast<GLvoid*>(18 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
	}

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

void RubiksCubeTrainer::draw(Shader ourShader, vector<glm::vec3>& cubePositions)
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
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, value_ptr(view));
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, value_ptr(projection));

	//in draw function
	for (GLuint i = 0; i < 3; i++)
	{
		glBindVertexArray(VAO[i]);
		// Calculate the model matrix for each object and pass it to shader before drawing
		//aici se pare ca nu pot sa atribui direct
		glm::mat4 model;
		model = translate(model, cubePositions[i]);
		//ROTATE AROUND POINT
		//	GLfloat angle = 3.0f * deltaTime;
		//model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
		//	model = glm::translate(model, glm::vec3(-1.0f, 3.0f, 0.0f));
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, value_ptr(model));
		glDrawArrays(GL_TRIANGLES, 0, 72);
	}

	this->window.display();

	glBindVertexArray(0);
}

void RubiksCubeTrainer::initWindow()
{
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	this->window.create(sf::VideoMode(screenWidth, screenHeight), name, sf::Style::Default, settings);
	this->window.setActive(true);
	glewExperimental = GL_TRUE;
	glewInit();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEBUG_OUTPUT);
	window.setMouseCursorVisible(false);
}

void RubiksCubeTrainer::start()
{
	initWindow();
	Shader ourShader("Shaders/vertex.vs", "Shaders/fragment.frag");
	// Set up our vertex data (and buffer(s)) and attribute pointers
	vector<GLfloat> block1{
		BACK_SQUARE, BLUE_SQUARE,
		
		RIGHT_SQUARE, ORANGE_SQUARE,
		
		FRONT_SQUARE, GREEN_SQUARE,
		
		LEFT_SQUARE, YELLOW_SQUARE,

		UP_SQUARE,WHITE_SQUARE,

		DOWN_SQUARE,RED_SQUARE
	}; vector<GLfloat> block2{
		BACK_SQUARE, BLUE_SQUARE,

		RIGHT_SQUARE, ORANGE_SQUARE,

		FRONT_SQUARE, GREEN_SQUARE,

		LEFT_SQUARE, YELLOW_SQUARE,

		UP_SQUARE,WHITE_SQUARE,

		DOWN_SQUARE,RED_SQUARE
	};
	vector<GLfloat> block3{
		BACK_SQUARE, BLUE_SQUARE,

		RIGHT_SQUARE, ORANGE_SQUARE,

		FRONT_SQUARE, GREEN_SQUARE,

		LEFT_SQUARE, YELLOW_SQUARE,

		UP_SQUARE,WHITE_SQUARE,

		DOWN_SQUARE,RED_SQUARE
	};

	vector<vector<GLfloat>> blocks;
	blocks.push_back(block1);
	//blocks.push_back(block2);
	//blocks.push_back(block3);

	vector<glm::vec3> cubePositions = {
		//FRONT
		glm::vec3(0.0f, 0.0f, 0.0f),
	 	glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(2.0f, 0.0f, 0.0f)
		//glm::vec3(0.0f, 1.0f, 0.0f)
		/*
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
		*/
	};
	sf::Clock clock;

	setupVAOandVBO(blocks);
	while (running)
	{
		sf::Time elapsed = clock.restart();
		deltaTime = elapsed.asSeconds();
		sf::Vector2i localPosition = sf::Mouse::getPosition(window);

		processEvents();
		update(localPosition);
		draw(ourShader, cubePositions);
	}
	glDeleteVertexArrays(3, VAO);
	glDeleteBuffers(3, VBO);
}
