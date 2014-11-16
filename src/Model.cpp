//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <assert.h>
#include <GL/glew.h>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glm/gtc/matrix_transform.hpp"

#define SHADERS_DIR                 "shaders/" // "C:\\Users\\Maria\\Documents\\Visual Studio 2010\\Projects\\graphics-ex1\\src\\shaders\\"
#define DEGREES_IN_CIRCLE           360
#define VERTICES_IN_PERIMETER		DEGREES_IN_CIRCLE*1 //must be a factor of number of degrees in a full circle
//TODO not to do this twice!
#define DEFAULT_RADIUS              0.1
#define MIN_RADIUS					0.01
#define EPSILON						0.001

const float lightSourceX = 1.5f;
const float lightSourceY = 1.5f;

Model::Model() :
_vao(0), _vbo(0), _numOfBalls(0)
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
	_translationUV = glGetUniformLocation(program, "translation");
	_scaleUV = glGetUniformLocation(program, "scale");
	_whiteSpotUV = glGetUniformLocation(program, "whitePoint");
	_lightRadiusUV = glGetUniformLocation(program, "lightRadius");

	// Initialize vertices buffer and transfer it to OpenGL
	{
		//the circle should be in the middle of the window		
		float center[] = {
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		
		float vertices[(VERTICES_IN_PERIMETER+2)*NUM_OF_COORDS];

		float radius = DEFAULT_RADIUS;
		//fill the vertices array for triangle_fan object
		generateCircleVertices(vertices, center, radius);

		_balls[0] = Ball(0,0);
		_numOfBalls++;

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
	for (int numTriangle = 1; numTriangle < VERTICES_IN_PERIMETER +2; numTriangle++)
	{
		for (int coordNum = X; coordNum < NUM_OF_COORDS; coordNum++)
		{
			verticeArr[numTriangle*NUM_OF_COORDS + coordNum] = center_location[coordNum];
		}
		//we offset the X and Y of each point to the needed direction from the circle center
		float deg = (numTriangle-1)*DEGREES_IN_CIRCLE/VERTICES_IN_PERIMETER;
		verticeArr[numTriangle*NUM_OF_COORDS + X] += radius*cos(deg*M_PI/180.0f);
		verticeArr[numTriangle*NUM_OF_COORDS + Y] += radius*sin(deg*M_PI/180.0f);
	}

}

void Model::calculateCollisions()
{
	float shortageDist = 0.0f;
	for (int i = 0; i < _numOfBalls; i++)
	{
		_balls[i].enlarge();
		for (int j = 0; j < i; j++) 
		{
			if (isColliding(_balls[i], _balls[j], shortageDist))
			{
				assert(shortageDist > 0);
				if (_balls[i].getCurrRadius() < MIN_RADIUS) 
				{
					_balls[j].shrink(shortageDist+EPSILON);
				}
				else if (_balls[j].getCurrRadius() < MIN_RADIUS)
				{
					_balls[i].shrink(shortageDist+EPSILON);
				}
				else
				{
					_balls[i].shrink(shortageDist*0.5 + EPSILON);
					_balls[j].shrink(shortageDist*0.5 + EPSILON);
				}
			}
		}
	}
}

float Model::calculateDist(float x0, float y0, float x1, float y1)
{
	return sqrt((x0 - x1) * (x0 - x1) + (y0 - y1) * (y0 - y1) );
}

bool Model::isColliding(Ball& ball1, Ball& ball2, float& distanceShortage)
{
	float r1 = ball1.getCurrRadius();
	float r2 = ball2.getCurrRadius();
	float dist = calculateDist(ball1.getX(), ball1.getY(), ball2.getX(), ball2.getY());
	distanceShortage = r1+r2 - dist;
	return (dist < r1 + r2);
}

void Model::draw()
{
	calculateCollisions();

	// Set the program to be used in subsequent lines:
	GLuint program = programManager::sharedInstance().programWithID("default");
	glUseProgram(program);

	GLenum polygonMode = GL_FILL;
	glPolygonMode(GL_FRONT_AND_BACK, polygonMode);

	// Draw using the state stored in the Vertex Array object:
	glBindVertexArray(_vao);
	
	size_t numberOfVertices = VERTICES_IN_PERIMETER+2;
	
	for (int i = 0; i < _numOfBalls; i++)
	{
		float scale = _balls[i].getCurrRadius()/DEFAULT_RADIUS;
		float whitePointX = 0 , whitePointY = 0;
		_balls[i].calculateWhitePoint(lightSourceX, lightSourceY, whitePointX, whitePointY);
		whitePointX = whitePointX*0.5*_width + 0.5*_width;
		whitePointY = - whitePointY*0.5*_height + 0.5*_height;

		//for testing purposes
		float imageX = _balls[i].getX()*0.5*_width + 0.5*_width;
		float imageY = - _balls[i].getY()*0.5*_height + 0.5*_height;
		float imageRadius = _balls[i].getCurrRadius()*_width*0.5;

		float scaleMatrix[] = {scale, 0.0, 0.0, 0.0,
		                     0.0, scale, 0.0, 0.0,
		                     0.0, 0.0, 1.0, 0.0,
		                     0.0, 0.0, 0.0, 1.0};
		//TODO remove
//		std::cout << " Ball " << i << " scale " << scale << " radius "  << _balls[i].getRadius() << std::endl;
		glUniform4f(_fillColorUV, _balls[i].getColor()[0], _balls[i].getColor()[1], _balls[i].getColor()[2], 1.0);
		glUniform4f(_translationUV, _balls[i].getX(), _balls[i].getY(),0.0, 0.0);
		glUniformMatrix4fv(_scaleUV, 1, false, scaleMatrix);
		glUniform4f(_whiteSpotUV, whitePointX, whitePointY, 0.0, 0.0);
		glUniform1f(_lightRadiusUV, _balls[i].getCurrRadius()*0.5*_width/2);

		glDrawArrays(GL_TRIANGLE_FAN, 0, numberOfVertices);
	}

	// Unbind the Vertex Array object
	glBindVertexArray(0);
	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::move()
{
	for (int i = 0; i < _numOfBalls; i++)
	{
		_balls[i].move();
	}
}

void Model::resize(int width, int height)
{
    _width	= width;
    _height = height;
    _offsetX = 0;
    _offsetY = 0;
}

void Model::createRandomBall(int x, int y)
{
	float normalizedX = ((float)x - 0.5*_width)/(_width*0.5);
	float normalizedY = ( 0.5*_height - (float)y)/(_height*0.5);

	double radius = std::min(DEFAULT_RADIUS, 1.0-(float)fabs(normalizedX));
	radius = std::min(radius, 1.0 - fabs(normalizedY) );

	_balls[_numOfBalls] = Ball(normalizedX,normalizedY, radius);
	_numOfBalls++;
}

