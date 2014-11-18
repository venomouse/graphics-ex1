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

	float _x, _y, //center location
		_dx, _dy, //speed
		_radius, //radius at creation
		_scale, // relative to initial size
		_gXscale, _gYscale;
	float _color[4];

public:
	Ball(float radius = INIT_RADIUS);
	Ball(float x, float y, float radius = INIT_RADIUS);
	virtual ~Ball();

	void init();
	//update the ball's center position and speed
	void move();
	//shrink the ball in case of collision
	void shrink(float radiusDiff);
	//enlarge the shrinked ball at a steady rate
	void enlarge();

	void setGXYScale (float gXscale, float gYscale);


	//Getters
	float getDefaultRadius()	{	return _radius;}
	float getCurrRadius()		{	return _scale*_radius;}
	bool isFullSize()			{	return _scale == 1.0f;}

	float getX()				{	return _x;} 
	float getY()				{	return _y;}
	float* getColor()			{	return _color;}			

private:

};


#endif /* BALL_H_ */
