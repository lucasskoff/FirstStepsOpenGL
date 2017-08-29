#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaderProgram.h"

#include <iostream>
#include <windows.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
const int NUMBER_OF_SQUARES = 4;
int main()
{
	//GLFW
	//----------------------------------------------------------------------------
	//Initialize and configure glfw
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create glfw window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//GLAD
	//----------------------------------------------------------------------------
	//Load all OpenGL function pointers with GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//Shader
	//---------------------------------------------------------------------------
	//Use our shader program with the filenames of the vertex and fragment shaders.
	//Shader rainbowShader("vertexShader.vs", "fragmentShader.fs");
	//Shader orangeShader("basicVertexShader.vs", "orangeFragmentShader.fs");
	Shader fragmentShader("basicVertexShader.vs", "textureFragment.fs");

	//Vertex Data / Vertex Attributes
	//---------------------------------------------------------------------------
	
	float vertices[NUMBER_OF_SQUARES][128] =
	{ //Top Left Square	   //Colors			 //Textures  
	 { -0.1f,  0.9f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right     - red
	   -0.9f,  0.9f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // top left      - green
	   -0.9f,  0.1f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left   - blue
	   -0.1f,  0.1f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f  // bottom right  - white
	 },
		//Top Right Square
	 {  0.9f,  0.9f, 0.0f,  0.0f, 1.0f, 0.0f,  // top right    - green
		0.1f,  0.9f, 0.0f,  1.0f, 0.0f, 0.0f,  // top left     - red
		0.1f,  0.1f, 0.0f,  1.0f, 1.0f, 1.0f,  // bottom left  - white
		0.9f,  0.1f, 0.0f,  0.0f, 0.0f, 1.0f   // bottom right - blue
	 },
		//Bottom Right Square
	 {  0.9f, -0.1f, 0.0f,  0.0f, 0.0f, 1.0f,  // top right    - blue
		0.1f, -0.1f, 0.0f,  1.0f, 1.0f, 1.0f,  // top left	   - white
		0.1f, -0.9f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom left  - red
		0.9f, -0.9f, 0.0f,  0.0f, 1.0f, 0.0f   // bottom right - green
	 },
		//Bottom Left Square
	 {  -0.1f, -0.1f, 0.0f,  1.0f, 1.0f, 1.0f, // top right    - white
		-0.9f, -0.1f, 0.0f,  0.0f, 0.0f, 1.0f, // top left     - blue
		-0.9f, -0.9f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom left  - green
		-0.1f, -0.9f, 0.0f,  1.0f, 0.0f, 0.0f  // bottom right - red
	 }
	};

	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};

	unsigned int indices2[] = {
		0, 1, 2,
		0, 3, 2
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]), vertices[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	//vertex
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//texture
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	/*
	unsigned int VBOs[NUMBER_OF_SQUARES], VAOs[NUMBER_OF_SQUARES], EBOs[NUMBER_OF_SQUARES];
	glGenVertexArrays(NUMBER_OF_SQUARES, VAOs);
	glGenBuffers(NUMBER_OF_SQUARES, VBOs);
	glGenBuffers(NUMBER_OF_SQUARES, EBOs);

	int i = 0;
	for (int i = 0; i < NUMBER_OF_SQUARES; i++) {
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
		if (i % 2 == 0) { //fixes fragmentation issue related to indices
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		}
		else
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices2), indices2, GL_STATIC_DRAW);
		}
	}
	*/
	//Texture
	//---------------------------------------------------------------------------
	//generate and bind the texture pointer
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	//set the shape and repeating pattern for the texture
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	int width, height, nrChannels;
	//use stb_image.h to load the designated image as a texture
	unsigned char *data = stbi_load(".\\resources\\texture\\container.jpg", &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
	}
	stbi_image_free(data);

	//Uncomment to display vertices in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	int OorY = 0;
	//Render Loop
	//---------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);
		//start each frame by clearing
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		//render multiple squares
		/*for (i = 0; i < 4; i++) {
			if (OorY == i) {
				rainbowShader.use();
			}
			else {
				orangeShader.use();
			}
			//rainbowShader.use();
			//rainbowShader.setFloat("someUniform", 1.0f);
			glBindVertexArray(VAOs[i]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		OorY = (OorY + 1) % 4;
		*/

		// bind Texture
		glBindTexture(GL_TEXTURE_2D, texture);

		// render container
		fragmentShader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		//glfw: swap buffers and obtain all IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
		//Sleep(500);
	}

	//Clean Up
	//---------------------------------------------------------------------------
	//glfw terminate to clear all allocated glfw resources.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	/*
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteBuffers(1, EBOs);
	*/
	glfwTerminate();
	return 0;
}

//Process all user input
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

//Called each time user resizes window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}