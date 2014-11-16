//
//  Model.cpp
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#include "ShaderIO.h"
#include "Model.h"

#include <assert.h>
#include <cstring>
#include <algorithm>
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
#define EPSILON						0.001f

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
		
	_fillColorUV  = glGetUniformLocation(program, "fillColor");
	_translationUV = glGetUniformLocation(program, "translation");
	_scaleUV = glGetUniformLocation(program, "scale");
	_ballRadiusUV = glGetUniformLocation(program, "ballRadius");
	_windowSizeUV = glGetUniformLocation(program, "windowSize");
	
	_initWidth = glutGet(GLUT_WINDOW_WIDTH);
	_initHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// Initialize vertices buffer and transfer it to OpenGL
	{
		//the circle should be in the middle of the window		
		float center[] = {
			0.0f, 0.0f, 0.0f, 1.0f,
		};
		
		//----------------------------------NEW CODE---------------------------------//
		float vertices[(VERTICES_IN_PERIMETER+2)*NUM_OF_COORDS];
		float radius = DEFAULT_RADIUS;

		//fill the vertices array for triangle_fan object
		generateCircleVertices(vertices, center, radius);

		//fill in the first ball
		_balls[0] = Ball(0,0);
		_numOfBalls++;
		//--------------------------------END NEW CODE-------------------------------//

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
    float x1 = ball1.getX()*_width/_initWidth;
    float y1 = ball1.getY()*_height/_initHeight;
    float x2 = ball2.getX()*_width/_initWidth;
    float y2 = ball2.getY()*_height/_initHeight;
    float dist = calculateDist(x1, y1, x2, y2);
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
	
	//-------------------------------------------NEW CODE------------------------------------//
	size_t numberOfVertices = VERTICES_IN_PERIMETER+2;

	glUniform2f(_windowSizeUV,_width, _height );	

	for (int i = 0; i < _numOfBalls; i++)
	{
		fillDataArrays(i);

		if (((i+1) % BALLS_PER_CALL == 0) || i == _numOfBalls - 1 )
		{
			//int numInstances = (i == _numOfBalls - 1) ? _numOfBalls % BALLS_PER_CALL : BALLS_PER_CALL;
			int numInstances = ((i+1) % _numOfBalls == 0) ? BALLS_PER_CALL : (i+1) % _numOfBalls;
	     	setUniformHandles(numInstances);
			glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, numberOfVertices, numInstances);
		}
	}
	//----------------------------------------END NEW CODE------------------------------------//

	// Unbind the Vertex Array object
	glBindVertexArray(0);
	
	// Cleanup, not strictly necessary
	glUseProgram(0);
}

void Model::fillDataArrays(int i)
{
	int ind = i % BALLS_PER_CALL;
	_fillColorArr[4*ind] = _balls[i].getColor()[0];
	_fillColorArr[4*ind + 1] =  _balls[i].getColor()[1];
	_fillColorArr[4*ind + 2] =  _balls[i].getColor()[2];
	_fillColorArr[4*ind + 3] = 1.0;

	float scale = _balls[i].getCurrRadius()/DEFAULT_RADIUS;

	float gScaleX = _width/_initWidth;
	float gScaleY = _height/_initHeight;

	float scaleMatrix[] = {scale/gScaleX, 0.0, 0.0, 0.0,
			0.0, scale/gScaleY, 0.0, 0.0,
			0.0, 0.0, 1.0, 0.0,
			0.0, 0.0, 0.0, 1.0};

	memcpy(_scaleArr + 16*ind, scaleMatrix, sizeof(scaleMatrix));

	_translationArr[4*ind] = _balls[i].getX();
	_translationArr[4*ind+1] = _balls[i].getY();
	_translationArr[4*ind+2] = 0.0;
	_translationArr[4*ind+3] = 0.0;

	_ballRadiusArr[ind] = _balls[i].getCurrRadius();

}

void Model::setUniformHandles (int numInstances)
{
	glUniform4fv(_fillColorUV, numInstances, _fillColorArr);
	glUniform4fv(_translationUV, numInstances, _translationArr);
	glUniformMatrix4fv(_scaleUV, numInstances, false, _scaleArr);
	glUniform1fv(_ballRadiusUV, numInstances, _ballRadiusArr);

}

void Model::move()
{
	float gXScale = _width/_initWidth;
	float gYScale = _height/_initHeight;
	for (int i = 0; i < _numOfBalls; i++)
	{
		_balls[i].setGXYScale(gXScale, gYScale);
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

void Model::createBall(int x, int y)
{
	float normalizedX = ((float)x - 0.5*_width)/(_width*0.5);
	float normalizedY = ( 0.5*_height - (float)y)/(_height*0.5);

	//calculating the largest possible radius relative to other balls
	double largestRadius = computeLargestRadius(normalizedX, normalizedY);

	//we clicked the mouse inside the existing ball
	if (largestRadius < 0)
	{
		//according to the school solution, if this is the case, a new ball
		//is not created
		return;
	}

	//calculating the larges possible radius relative to the walls
	double radius = std::min(DEFAULT_RADIUS, 1.0-(float)fabs(normalizedX));
	radius = std::min(radius, 1.0 - fabs(normalizedY) );

	radius = std::min(radius, largestRadius);

	_balls[_numOfBalls] = Ball(normalizedX,normalizedY, radius);
	_numOfBalls++;
}

float Model::computeLargestRadius(float newX, float newY)
{
	float maxRadius = DEFAULT_RADIUS;
	for (int i = 0; i < _numOfBalls; i++)
	{
		float dist = calculateDist(newX, newY, _balls[i].getX(), _balls[i].getY());
		//we clicked inside the ball or too close to it
		if (dist - _balls[i].getCurrRadius() < MIN_RADIUS)
		{
			return -1.0;
		}
		else
		{
			maxRadius = std::min(maxRadius, dist - _balls[i].getCurrRadius()- EPSILON);
		}
	}

	return maxRadius;
}

