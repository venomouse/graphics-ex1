//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define SHADERS_DIR                 "shaders/"
#define DEGREES_IN_CIRCLE           360


Model::Model() :
_vao(0), _vbo(0)
{

}

Model::~Model()
{
	if (_vao != 0)
		glDeleteVertexArrays(1, &_vao);
	if (_vbo != 0)
		glDeleteBuffers(1, &_vbo);
}


void Model::init()
{
	programManager::sharedInstance()
	.createProgram("default",
				   SHADERS_DIR "SimpleShader.vert",
				   SHADERS_DIR "SimpleShader.frag");

	GLuint program = programManager::sharedInstance().programWithID("default");
		
	// Obtain uniform variable handles:
	_fillColorUV  = glGetUniformLocation(program, "fillColor");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		// For this example we create a single triangle:
		float center[] = {
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		
		float vertices[(DEGREES_IN_CIRCLE+2)*NUM_OF_COORDS];

		float radius = 0.75;

		generateCircleVertices(vertices, center, radius);


		// Create and bind the object's Vertex Array Object:
		glGenVertexArrays(1, &_vao);
		glBindVertexArray(_vao);
		
		// Create and load vertex data into a Vertex Buffer Object:
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		
		// Tells OpenGL that there is vertex data in this buffer object and what form that vertex data takes:

		// Obtain attribute handles:
		_posAttrib = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(_posAttrib);
		glVertexAttribPointer(_posAttrib, // attribute handle
							  4,          // number of scalars per vertex
							  GL_FLOAT,   // scalar type
							  GL_FALSE,
							  0,
							  0);
		
		// Unbind vertex array:
		glBindVertexArray(0);
	}
}

void Model::generateCircleVertices(float* verticeArr, float center_location[NUM_OF_COORDS], float radius)
{
	//initializing the center
	for (int coordNum = X; coordNum < NUM_OF_COORDS; coordNum++)
	{
		verticeArr[coordNum] = center_location[coordNum];
	}

	//calculating the vertices on the circle perimeter
	for (int numTriangle = 1; numTriangle < DEGREES_IN_CIRCLE +2; numTriangle++)
	{
		for (int coordNum = X; coordNum < NUM_OF_COORDS; coordNum++)
		{
			verticeArr[numTriangle*NUM_OF_COORDS + coordNum] = center_location[coordNum];
		}

		verticeArr[numTriangle*NUM_OF_COORDS + X] += radius*cos(numTriangle*M_PI/180.0f);
		verticeArr[numTriangle*NUM_OF_COORDS + Y] += radius*sin(numTriangle*M_PI/180.0f);
	}



}

void Model::draw()
{
	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL;   // Also try using GL_FILL and GL_POINT
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Set uniform variable with RGB values:
	float red = 0.3f; float green = 0.5f; float blue = 0.7f;
	glUniform4f(_fillColorUV, red, green, blue, 1.0);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	
	size_t numberOfVertices = DEGREES_IN_CIRCLE+2;
	glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	
	// Unbind the Vertex Array object
	glBindVertexArray(0);
	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}
