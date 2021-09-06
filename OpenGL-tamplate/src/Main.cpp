#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>


#include"shaderClass.h"
#include"EBO.h"
#include"VAO.h"
#include"VBO.h"

// Const
#define WINDOW_HIGHT 800
#define WINDOW_WIDTH 800
#define WINDOW_TITLE "Golf World"
#define BOTTOM_LEFT_CORD 0,0
#define TOP_RIGHT_CORD WINDOW_WIDTH, WINDOW_HIGHT
#define SCREEN_BACKGROUND_COLOR 0.07f, 0.13f, 0.17f, 1.0f
//#define SCREEN_BACKGROUND_COLOR 0.00f, 0.2f, 0.2f, 1.0f
#define SCREEN_SECONDERY_COLOR 0.00f, 0.2f, 0.2f, 1.0f


// Vertices coordinates
GLfloat vertices[] =
{
	//					COORDINATES			/	COLORS				 //
	-0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f,  // Lower left corner
	 0.5f, -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f,  // Lower right corner
	 0.0f,  0.5f * float(sqrt(3)) * 2 / 3, 0.0f, 1.0f, 0.6f, 0.32f,  // Upper corner
	-0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Inner left
	 0.25f, 0.5f * float(sqrt(3)) / 6,     0.0f, 0.9f, 0.45f, 0.17f, // Inner right
	 0.0f, -0.5f * float(sqrt(3)) / 3,     0.0f, 0.8f, 0.3f, 0.02f,  // Inner down
};

// Indices for vertices order
GLuint indices[] =
{
	0, 3, 5, // Lower left triangle
	3, 2, 4, // Lower right triangle
	5, 4, 1 // Upper triangle
};



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

	// Generates Shader object using shaders defualt.vert and default.frag
	Shader shaderProgram("default.vert", "default.frag");
	// Generates Vertex Array Object and binds it
	VAO VAO1;
	VAO1.Bind();

	// Generates Vertex Buffer Object and links it to vertices
	VBO VBO1(vertices, sizeof(vertices));
	// Generates Element Buffer Object and links it to indices
	EBO EBO1(indices, sizeof(indices));

	// Links VBO attributes such as coordinates and colors to VAO
	VAO1.LinkAttrib(VBO1, 0, 3, GL_FLOAT, 6 * sizeof(float), (void*)0);
	VAO1.LinkAttrib(VBO1, 1, 3, GL_FLOAT, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	// Unbind all to prevent accidentally modifying them
	VAO1.Unbind();
	VBO1.Unbind();
	EBO1.Unbind();

	// Gets ID of uniform called ""scale
	GLuint uniID = glGetUniformLocation(shaderProgram.ID, "scale");

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		// Specifies the rgb color in the color buffer to applay new color on the screen
		glClearColor(SCREEN_BACKGROUND_COLOR);

		// Cleans the back buffer (color) and assign the new color into it
		// GL_COLOR_BUFFER_BIT indecates the color buffer is enabled for color writing
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Tell OpenGL which Shader Program we want to use
		shaderProgram.Activate();
		// Assigns a valur to the uniform; NOTE: Must alwats be done after activatin the Shader Program
		glUniform1f(uniID, 0.0f);
		// Bind the VAO so OpenGL knows to use it
		VAO1.Bind();

		glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);

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
	VAO1.Delete();
	VBO1.Delete();
	EBO1.Delete();
	shaderProgram.Delete();

	// Delete window before ending the program
	glfwDestroyWindow(window);
	// Termianate GLFW before ending the program
	glfwTerminate();
	return 0;
}