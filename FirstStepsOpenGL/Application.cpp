#include <glad/glad.h> 
#include <GLFW/glfw3.h>

#include <iostream>
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

//Simple vertex shader program
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"layout (location = 1) in vec3 aColor;\n"
"out vec3 ourColor;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos, 1.0);\n"
"	ourColor = aColor;\n"
"}\0";

//Simple fragment shader program
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"in vec3 ourColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(ourColor, 1.0f);\n"
"}\n\0";

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
	//Create a vertex shader object and give it a unique id.
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Compile our program above and bind it to an object with our id.
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	glCompileShader(vertexShader);

	//Test our shader compilation
	int  success;
	char infoLog[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create a fragment shader object and give it a unique id.
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Compile our shader program and bind it an object with our id.
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	//Test our fragement shader.
	if (!success) {
		glGetProgramInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Create an overall shader program and assign it a unique id.
	unsigned int shaderProgram = glCreateProgram();

	//Bind both our vertex shader and fragment shader to our new shader program.
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	//Link our overall shader program to the larger graphics program.
	glLinkProgram(shaderProgram);

	//Test our overall shader program.
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	//Delete vertext & fragment, as they are now linked so we no longer need them.
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	//Vertex Data / Vertext Attributes
	//---------------------------------------------------------------------------
	//An array of points for our 2 triangles. Only valid points are between -1 and 1.
	float vertices[] = {
		// positions         // colors
		0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	//EBO CODE
	//---------------
	//Vertices for two triangles to form a rectangle using EBO
	/*float vertices[] = {
	-0.5f,  0.0f, 0.0f, //Top of left triangle
	0.5f,  0.0f, 0.0f,  //top of right triangle
	-1.0f, -0.5f, 0.0f, //Bottom of left triangle
	1.0f, -0.5f, 0.0f,
	};

	unsigned int indices[] = {
	1, 2, 0,   // first triangle
	0, 3, 4    // second triangle
	};*/

	//Create a vertex buffer and vertext array. Give it a unique id.
	unsigned int VBO, VAO, EBO;
	glGenBuffers(1, &VBO);
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &EBO);

	//Bind vertex array first.
	glBindVertexArray(VAO);

	//Bind and set vertex buffers.
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//EBO CODE
	//---------------
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Tell OpenGL how to interpret vertex data when rendering.
	//position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	//color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

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

		//Activate shader
		glUseProgram(shaderProgram);

		//Update uniform color
		float timeValue = glfwGetTime();
		float greenValue = sin(timeValue) / 2.0f + 0.5f;
		int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// draw our triangle
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//EBO CODE
		//---------------
		//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//glBindVertexArray(0); //No need to unbind each time as we only draw one triangle

		//swap buffers and obtain all IO events
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//Clean Up
	//---------------------------------------------------------------------------
	//glfw terminate to clear all allocated glfw resources.
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
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