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

#define MAX_NUM_BALLS 100

class Model {
	
	enum Coords {X=0, Y, Z, W, NUM_OF_COORDS};

	GLuint _vao, _vbo;

	int _numOfBalls;

	Ball _balls[MAX_NUM_BALLS];


	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
	// Uniform handle
	GLint _scaleUV;

	// Uniform handle
	GLint _translationUV;

	// View port frame:
	float _width, _height, _offsetX, _offsetY;

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
	void createRandomBall(int x, int y);


private:
	void generateCircleVertices(float* verticeArr, float center_location[NUM_OF_COORDS], float radius);

//private:
//	void checkOverlapAndShrink()

};

#endif /* defined(__ex0__Model__) */
