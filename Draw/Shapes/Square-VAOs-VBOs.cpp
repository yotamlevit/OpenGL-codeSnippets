#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


// Const
#define WINDOW_HIGHT 800
#define WINDOW_WIDTH 800
#define WINDOW_TITLE "Golf World"
#define BOTTOM_LEFT_CORD 0,0
#define TOP_RIGHT_CORD WINDOW_WIDTH, WINDOW_HIGHT
#define SCREEN_BACKGROUND_COLOR 0.07f, 0.13f, 0.17f, 1.0f
//#define SCREEN_BACKGROUND_COLOR 0.00f, 0.2f, 0.2f, 1.0f
#define SCREEN_SECONDERY_COLOR 0.00f, 0.2f, 0.2f, 1.0f


// Vertex Shader source code
const char* vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";
//Fragment Shader source code
const char* fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(0.8f, 0.3f, 0.02f, 1.0f);\n"
"}\n\0";


int main(void)
{
	GLFWwindow* window;
	float red = 255;
	float green = 0;
	float blue = 0;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	// Windows Settings
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_ANY_PROFILE);

	/* Create a windowed mode window and its OpenGL context */
	// Input: hight, width, Window name, is full screen, not importent
	window = glfwCreateWindow(WINDOW_HIGHT, WINDOW_WIDTH, WINDOW_TITLE, NULL, NULL);
	// Error check if the window fails to create
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	//Load GLAD so it configures OpenGL
	gladLoadGL();

	// Tells openGL what area to render
	// Input: Bottom left corner coordinates, Top right corner coordinates
	glViewport(BOTTOM_LEFT_CORD, TOP_RIGHT_CORD);

	// Create vetex shader Object and get refrence
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// Attach Vertex Shader source to the vertex shader Object
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	//Compile the vertex Shader into machine code
	glCompileShader(vertexShader);
	
	// Create Fragment Shader Object and get its reference
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// Attach Fragment Shader source to the Fragment Shader Object
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// Compile the Vertex Shader into machine code
	glCompileShader(fragmentShader);

	// Create Shader Program Object and get its reference
	GLuint shaderProgram = glCreateProgram();
	// Attach the Vertex and Fragment Shaders to the Shader Program
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// Wrap-up/Link all the shaders together into the Shader Program
	glLinkProgram(shaderProgram);

	// Delete the now useless Vertex and Fragment Shader objects
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// Create a 6 points of 2 triangles to create one square - Vertices coordinates
	GLfloat vertices[] = 
	{ 
		-0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		-0.5f, 0.5f * float(sqrt(3)) / 3, 0.0f, // Top left corner
		-0.5f, 0.5f * float(sqrt(3)) / 3, 0.0f, // Top left corner
		0.5f, -0.5f * float(sqrt(3)) / 3, 0.0f, // Lower right corner
		0.5f, 0.5f * float(sqrt(3)) / 3, 0.0f // Top right corner
	}; 


	// Create reference containers for the Vartex Array Object and the Vertex Buffer Object
	GLuint VAOs[1] ,VBOs[1];
	
	// Generate the VAO and VBO with only 1 object each
	glGenVertexArrays(1, VAOs);
	glGenBuffers(1, VBOs);

	// Make the VAO the current Vertex Array Object by binding it
	glBindVertexArray(VAOs[0]);

	// Bind the VBO specifying it's a GL_ARRAY_BUFFER
	glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
	// Introduce the vertices into the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Configure the Vertex Attribute so that OpenGL knows how to read the VBO
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// Enable the Vertex Attribute so that OpenGL knows to use it
	glEnableVertexAttribArray(0);

	// Bind both the VBO and VAO to 0 so that we don't accidentally modify the VAO and VBO we created
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Specifies the rgb color in the color buffer to applay new color on the screen
		glClearColor(SCREEN_BACKGROUND_COLOR);

		// Cleans the back buffer (color) and assign the new color into it
		// GL_COLOR_BUFFER_BIT indecates the color buffer is enabled for color writing
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(shaderProgram);
		glBindVertexArray(VAOs[0]);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now we have the back buffer with the color we want and the
		// front buffer with the defult color.
		// In order to refrese the color we want to swap between the buffers
		// (back buffer and from buffer) and see the new wanted color

		// Swap the back buffer with the front buffer
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete all the objects we've created
	glDeleteVertexArrays(1, VAOs);
	glDeleteBuffers(1, VBOs);
	glDeleteProgram(shaderProgram);


	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Termianate GLFW before ending the program
	glfwTerminate();
	return 0;
}