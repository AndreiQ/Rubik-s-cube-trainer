#include <iostream>
#include "RubiksCubeTrainer.h"
#include "Shader.h"
#include "Camera.h"
#include <glm/gtc/type_ptr.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>
using namespace std;

GLuint screenWidth = 1200, screenHeight = 700;
Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));
GLfloat lastX = 400, lastY = 700;
bool firstMouse = true;

GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;
int main()
{

	// Set up our vertex data (and buffer(s)) and attribute pointers
	GLfloat vertices[] = {
		//red:		1.0f, 0.0f, 0.0f
		//blue:		0.0f, 0.0f, 1.0f
		//green:	0.0f, 1.0f, 0.0f
		//orange:	1.0f, 0.5f, 0.0f
		//yellow:	1.0f, 1.0f, 0.0f
		//white:	1.0f, 1.0f, 1.0f  
		//positions				//colors
		//						R    G    B
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 0.0f,0.0f,
		-0.5f, -0.5f, -0.5f,  1.0f, 0.0f,0.0f,//red

		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
		0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 1.0f,0.0f,//green

		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,1.0f,//blue

		0.5f,  0.5f,  0.5f,  1.0f, 0.5f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.5f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.5f, 0.0f,//orange
		0.5f,  0.5f,  0.5f,  1.0f, 0.5f,0.0f,

		-0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 0.0f,//yellow

		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f//white
	};

	glm::vec3 cubePositions[] = {
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

	GLuint VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// Bind our Vertex Array Object first, then bind and set our buffers and pointers.
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0); // Unbind VAO
						  // Game loop
	bool running = true;
	sf::Clock clock;
	while (running) {
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				running = false;

			else if (event.type == sf::Event::Resized)
			{
				// adjust the viewport when the window is resized
				glViewport(0, 0, event.size.width, event.size.height);
			}
		}
		glClearColor(0.0f, 0.0f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



		sf::Vector2i localPosition = sf::Mouse::getPosition(window);
			// Set frame time
		sf::Time elapsed = clock.restart();
		deltaTime = elapsed.asSeconds();

		mouse_callback(localPosition);
			do_movement();			
			ourShader.Use();

			// Create camera transformation
			glm::mat4 view;
			view = camera.GetViewMatrix();
			glm::mat4 projection;
			projection = glm::perspective(45.0f, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
			// Get the uniform locations
			GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
			GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
			GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");
			// Pass the matrices to the shader
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));

			glBindVertexArray(VAO);

			for (GLuint i = 0; i < 27; i++)
			{
				// Calculate the model matrix for each object and pass it to shader before drawing
				glm::mat4 model;
				model = glm::translate(model, cubePositions[i]);
				//ROTATE AROUND POINT
				//	GLfloat angle = 3.0f * currentFrame;
				//	model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
				//	model = glm::translate(model, glm::vec3(-1.0f, 3.0f, 0.0f));
				glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

				glDrawArrays(GL_TRIANGLES, 0, 36);
			}

			window.display();
			glBindVertexArray(0);

		}
		// Properly de-allocate all resources once they've outlived their purpose
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);

	}

