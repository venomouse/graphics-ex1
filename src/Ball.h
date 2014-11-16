/*
 * ball.h
 *
 *  Created on: Nov 9, 2014
 *      Author: moshemandel
 */

#ifndef BALL_H_
#define BALL_H_

#include <math.h>

#define SPEED 0.03
#define INIT_RADIUS	0.1

class Ball
{
	float _x, _y, _dx, _dy, _radius, _scale, _gXscale, _gYscale;
	float _color[4];

public:
	Ball(float radius = INIT_RADIUS);
	Ball(float x, float y, float radius = INIT_RADIUS);
	virtual ~Ball();

	void init();
	void move();
	void shrink(float radiusDiff);
	void enlarge();
	void setGXYScale (float gXscale, float gYscale);

	float getDefaultRadius()	{	return _radius;}
	float getCurrRadius()		{	return _scale*_radius;}
	bool isFullSize()			{	return _scale == 1.0f;}

	float getX();
	float getY();
	float* getColor();

private:

};


#endif /* BALL_H_ */
