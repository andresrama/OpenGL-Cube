#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <glm/glm.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include <thread>
#include <iostream>

#define GLEW_STATIC
#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


void PrintGLError(GLenum error){
	switch(error){
	case GL_NO_ERROR:
		break;

	case GL_INVALID_ENUM:
		std::cout << "GL_INVALID_ENUM \n";
		break;

	case GL_INVALID_OPERATION:
		std::cout << "GL_INVALID_OPERATION \n";
		break;

	case GL_INVALID_FRAMEBUFFER_OPERATION:
		std::cout << "GL_INVALID_FRAMEBUFFER_OPERATION \n";
		break;

	case GL_OUT_OF_MEMORY:
		std::cout << "GL_OUT_OF_MEMORY \n";
		break;

	case GL_STACK_UNDERFLOW:
		std::cout << "GL_STACK_UNDERFLOW \n";
		break;

	case GL_STACK_OVERFLOW:
		std::cout << "GL_STACK_OVERFLOW \n";
		break;

	case GL_INVALID_VALUE:
		std::cout << "GL_INVALID_VALUE:" << error << "\n";
		break;

	default:
		std::cout << "UNKOWN ERROR CODE: " << error << "\n";
		break;
	}
}

void CheckForGLErrors(std::string function, int line){
	GLenum error = glGetError();
	if(error != GL_NO_ERROR ){
		std::cout << "OpenGL error in " << function
				<< " (at line "<< line << ")" << ": \n";
		PrintGLError(error);
	}
}

GLFWwindow* InitializeContext(){
	CheckForGLErrors(__func__, __LINE__);
	//Because or else everything else breaks
	glfwInit();
	CheckForGLErrors(__func__, __LINE__);

	//Settings for window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	CheckForGLErrors(__func__, __LINE__);

	//Because I don't want to write code to resize
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	CheckForGLErrors(__func__, __LINE__);

	//The actual creation of the window
	GLFWwindow* window = glfwCreateWindow(800, 600, "CUBE", nullptr, nullptr); // Windowed
	CheckForGLErrors(__func__, __LINE__);

	//Tell glfw to use the window
	glfwMakeContextCurrent(window);
	CheckForGLErrors(__func__, __LINE__);


	//Use GLEW
	glewExperimental = GL_TRUE;
	GLenum err=glewInit();
	if(err!=GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		std::cout<<"glewInit failed, aborting."<<std::endl;
		exit(-1);
	}
	CheckForGLErrors(__func__, __LINE__);

	glEnable(GL_DEPTH_TEST);

	return window;
}

GLuint InitializeElementBuffer(GLuint elements[], size_t size){
	GLuint ebo;
	glGenBuffers(1, &ebo);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, elements, GL_STATIC_DRAW);

	return ebo;
}

GLuint InitializeVertexBuffer(GLfloat vertices[], size_t size){
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);

	return vbo;
}

GLuint CreateVertexArray(){
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	return vao;
}

void InitializeVertexArray(GLuint ShaderProgram){
	GLint posAttrib = glGetAttribLocation(ShaderProgram, "position");
	CheckForGLErrors(__func__, __LINE__);
	glEnableVertexAttribArray(posAttrib);
	CheckForGLErrors(__func__, __LINE__);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE,
	                       6*sizeof(GLfloat), 0);
	CheckForGLErrors(__func__, __LINE__);


	GLint colAttrib = glGetAttribLocation(ShaderProgram, "color");
	CheckForGLErrors(__func__, __LINE__);
	glEnableVertexAttribArray(colAttrib);
	CheckForGLErrors(__func__, __LINE__);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE,
	                       6*sizeof(GLfloat), (void*)(3*sizeof(GLfloat)));
	CheckForGLErrors(__func__, __LINE__);

}

// Shader sources
const GLchar* VertexSource =
		"#version 150 core\n"
		"in vec3 position;"
		"in vec3 color;"
		"out vec3 Color;"
		"uniform mat4 trans;"
		"void main() {"
		"	Color = color;"
		"   gl_Position = trans * vec4(position, 1.0);"
		"}";
const GLchar* FragmentSource =
    "#version 150 core\n"
	"in vec3 Color;"
    "out vec4 outColor;"
    "void main() {"
    "   outColor = vec4(Color, 1.0);"
    "}";


GLuint LoadShaders(){
	// Create and compile the vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &VertexSource, NULL);
	glCompileShader(vertexShader);

	// Create and compile the fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &FragmentSource, NULL);
	glCompileShader(fragmentShader);

	// Link the vertex and fragment shader into a shader program
	GLuint shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);

	return shaderProgram;
}



#endif
