//
//  Model.h
//  cg-projects
//
//  Created by HUJI Computer Graphics course staff, 2013.
//

#ifndef __ex0__Model__
#define __ex0__Model__

#include <iostream>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#include "Ball.h"
#endif
#include <math.h>

#define MAX_NUM_BALLS 100
#define BALLS_PER_CALL 10

class Model {
	
	enum Coords {X=0, Y, Z, W, NUM_OF_COORDS};

	GLuint _vao, _vbo;

	//number of balls currently on the screen
	int _numOfBalls;

	Ball _balls[MAX_NUM_BALLS];


	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle: the fill color of the ball
	GLint _fillColorUV;
	
	// Uniform handle: the scale of the ball
	GLint _scaleUV;

	// Uniform handle: translation of the ball's center
	GLint _translationUV;

	// Uniform handle: the ball's absolute radius for lighting purposes
	GLint _ballRadiusUV;

	// Uniform handle: window size for lighting purposes
	GLint _windowSizeUV;

	//Data array
	float _fillColorArr[4*BALLS_PER_CALL];
	
	//Data array
	float _scaleArr[BALLS_PER_CALL*16];

	//Data array
	float _translationArr[4*BALLS_PER_CALL];

	//Data array
	float _ballRadiusArr[BALLS_PER_CALL];

	// View port frame:
	float _width, _height, _offsetX, _offsetY, _initWidth, _initHeight;

	int _verticesInPerimeter;

public:
	Model();

public:
	virtual ~Model();

public:
	void init();
	
public:
	void draw();
	
public:
	void move();

public:
	void resize(int width, int height);
	
public:
	void createBall(int x, int y);

private:

	//compute the largest possible initial radius for a new ball
	float computeLargestRadius(float newX, float newY);
	//generates the vertices to be drawn
	void generateCircleVertices(float* verticeArr, float center_location[NUM_OF_COORDS], float radius, int verticesInPerimeter);
	// find if we need to shrink any ball before collisions
	void calculateCollisions();
	// helper function to calculate distance between two ball centers
	float calculateDist(float x0, float y0, float x1, float y1);
	// decides if ball1 and ball 2 are colliding
	// distance shortage is the difference between the distance btw. the centers 
	// and the some of radii of the two balls
	bool isColliding(Ball& ball1, Ball& ball2, float& distanceShortage);

	//fill the arrays with data from a specific ball, before calling glDrawArrayInstanced
	void fillDataArrays(int i);

	//Set the uniform variables, executed straight before drawing
	void setUniformHandles (int numInstances);

};

#endif /* defined(__ex0__Model__) */
