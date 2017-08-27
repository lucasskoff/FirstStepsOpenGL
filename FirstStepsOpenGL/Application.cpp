#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "shaderProgram.h"

#include <iostream>
#include <windows.h>

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";
const char *fragmentShaderRightSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 1.0f, 0.2f, 1.0f);\n"
"}\n\0";

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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
	Shader ourShader("vertexShader.vs", "fragmentShader.fs");

	//First Shader Program
	int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	int fragmentShaderOrange = glCreateShader(GL_FRAGMENT_SHADER);
	int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	int shaderProgramOrange = glCreateProgram();
	int shaderProgramYellow = glCreateProgram();
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Attach shader to the corresponding program.
	glShaderSource(fragmentShaderOrange, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShaderOrange);

	glShaderSource(fragmentShaderYellow, 1, &fragmentShaderRightSource, NULL);
	glCompileShader(fragmentShaderYellow);
	
	//Attach fragment and vertex shaders to the shader program.
	glAttachShader(shaderProgramOrange, vertexShader);
	glAttachShader(shaderProgramOrange, fragmentShaderOrange);
	glLinkProgram(shaderProgramOrange);

	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	//Remove vertex and fragment shaders as they are no longer necessary.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShaderOrange);
	glDeleteShader(fragmentShaderYellow);


	//Vertex Data / Vertex Attributes
	//---------------------------------------------------------------------------
	float vertices[4][12] = 
	{ //Top Left Square
	 { -0.1f,  0.9f, 0.0f,  // top right
	 -0.9f,  0.9f, 0.0f,  // top left
	 -0.9f,  0.1f, 0.0f,   // bottom left
	 -0.1f,  0.1f, 0.0f  // bottom right
	 },
		//Top Right Square
	 {  0.9f,  0.9f, 0.0f,  // top right
		0.1f,  0.9f, 0.0f,  // top left
		0.1f,  0.1f, 0.0f,  // bottom left
		0.9f,  0.1f, 0.0f   // bottom right
	 },
		//Bottom Right Square
	 { 0.9f, -0.1f, 0.0f,  // top right
		0.1f, -0.1f, 0.0f,  // top left
		0.1f, -0.9f, 0.0f,  // bottom left
		0.9f, -0.9f, 0.0f   // bottom right
	 },
		//Bottom Left Square
	 { -0.1f, -0.1f, 0.0f,  // top right
		-0.9f, -0.1f, 0.0f,  // top left
		-0.9f, -0.9f, 0.0f,  // bottom left
		-0.1f, -0.9f, 0.0f   // bottom right
	 }
	};

	unsigned int indices[] = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
	};  

	unsigned int VBOs[4], VAOs[4], EBOs[4];
	glGenVertexArrays(4, VAOs);
	glGenBuffers(4, VBOs);
	glGenBuffers(4, EBOs);

	int i = 0;
	for (int i = 0; i < 4; i++) {
		glBindVertexArray(VAOs[i]);
		glBindBuffer(GL_ARRAY_BUFFER, VBOs[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[i]), vertices[i], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOs[i]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	}
	int OorY = 0;
	//Uncomment to display vertices in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	//Render Loop
	//---------------------------------------------------------------------------
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		//render
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		for (i = 0; i < 4; i++) {
			if (OorY == i) {
				ourShader.use();
			}
			else {
				glUseProgram(shaderProgramOrange);
			}
			glBindVertexArray(VAOs[i]);
			//glDrawArrays(GL_TRIANGLES, 0, 6);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		OorY = (OorY + 1) % 4;

		//glfw: swap buffers and obtain all IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
		Sleep(500);
	}

	//Clean Up
	//---------------------------------------------------------------------------
	//glfw terminate to clear all allocated glfw resources.
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	//glDeleteBuffers(1, &EBO);
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