#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <thread>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <engine.hpp>

GLuint VertexBuffer, VertexArray, ElementBuffer, ShaderProgram;

GLfloat vertices[] = {
		-0.5, 0.5, 0.5, 1.0, 0.0, 0.0,	// Front Top Left		- Red	- 0
		0.5,  0.5, 0.5, 0.0, 1.0, 0.0,	// Front Top Right		- Green	- 1
		0.5, -0.5, 0.5, 0.0, 0.0, 1.0,	// Front Bottom Right	- Blue	- 2
		-0.5,-0.5, 0.5, 0.0, 1.0, 1.0,	// Front Bottom Left	- Cyan	- 3
		-0.5, 0.5,-0.5, 1.0, 0.0, 1.0,	// Back Top Left		- Pink	- 4
		0.5,  0.5,-0.5, 1.0, 1.0, 0.0,	// Back Top Right		- Yellow- 5
		0.5, -0.5,-0.5, 0.1, 0.1, 0.1,	// Back Bottom Right	- White - 6
		-0.5,-0.5,-0.5, 1.0, 1.0, 1.0,	// Back Bottom Left		- Gray  - 7
};

GLuint elements[]{
		0,3,2,  //Front
		2,1,0,
		1,5,6,	//Right
		6,2,1,
		5,4,7,	//Left
		7,6,5,
		4,7,3,	//Back
		3,0,4,
		4,5,1,	//Top
		1,0,4,
		3,2,6,	//Bottom
		6,7,3,
};

float rotate_x = 1.75;
float rotate_y = 1.5;
float rotate_z = 1.25;
float rotate_speed = 50;

void changevars(){
	std::cout << "Please input a value for x rotation (default is " << rotate_x << " ):\n";
	std::cin >> rotate_x;
	std::cout << "Please input a value for y rotation (default is " << rotate_y << " ):\n";
	std::cin >> rotate_y;
	std::cout << "Please input a value for z rotation (default is " << rotate_z << " ):\n";
	std::cin >> rotate_z;
	std::cout << "Please input a value for rotation speed (default is " << rotate_speed << " ):\n";
	std::cin >> rotate_speed;
}

int main()
{
	CheckForGLErrors(__func__, __LINE__);

	//Create the window and the OpenGL context
	GLFWwindow* window = InitializeContext();

	//Create our vertex attribute array
	VertexArray = CreateVertexArray();
	CheckForGLErrors(__func__, __LINE__);

	//Compile and link the shaders
	ShaderProgram = LoadShaders();
	CheckForGLErrors(__func__, __LINE__);
	glUseProgram(ShaderProgram);
	CheckForGLErrors(__func__, __LINE__);

	//Create our vertex  buffer object using the vertices array
	VertexBuffer = InitializeVertexBuffer(vertices, sizeof(vertices));
	CheckForGLErrors(__func__, __LINE__);

	//Create our element buffer object using the elements array
	ElementBuffer = InitializeElementBuffer(elements, sizeof(elements));
	CheckForGLErrors(__func__, __LINE__);

	//Assign our array the appropriate structure
	InitializeVertexArray(ShaderProgram);
	CheckForGLErrors(__func__, __LINE__);


	// Get the location of the color uniform
	GLint uniTrans = glGetUniformLocation(ShaderProgram, "trans");
	while(!glfwWindowShouldClose(window))
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS){
			changevars();
		}
		// Clear the screen to black
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glm::mat4 trans;
		trans = glm::rotate(
				trans,
				(float)clock() / (float)CLOCKS_PER_SEC * rotate_speed,
				glm::vec3(rotate_x, rotate_y, rotate_z)
		);
		glUniformMatrix4fv(uniTrans, 1, GL_FALSE, glm::value_ptr(trans));

		glDrawElements(GL_TRIANGLES, sizeof(elements)/sizeof(GLuint), GL_UNSIGNED_INT, 0);

		CheckForGLErrors(__func__, __LINE__);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	CheckForGLErrors(__func__, __LINE__);

	glfwTerminate();
}
