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
#endif

class Model {
	
	enum Coords {X=0, Y, Z, W, NUM_OF_COORDS};
//	enum Triangle_Coords {FIRST_VERTICE_X, FIRST_VERTICE_Y,


	GLuint _vao, _vbo;

	// Attribute handle:
	GLint _posAttrib;
	
	// Uniform handle:
	GLint _fillColorUV;
	
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
	void resize(int width, int height);
	
private:
	void generateCircleVertices(float* verticeArr, float center_location[NUM_OF_COORDS], float radius);

};

#endif /* defined(__ex0__Model__) */
