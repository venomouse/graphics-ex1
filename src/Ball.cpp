/*
 * Ball.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: moshemandel
 */


#include "Ball.h"
#include <cstdlib>

Ball::Ball(float radius)
{
	_x = 0;
	_y = 0;
	init();
}

Ball::Ball(float x, float y, float radius)
{
	_x = x;
	_y = y;
	_radius = radius;
	init();
}

Ball::~Ball()
{

}

void Ball::init()
{
	_dx = SPEED * (rand() % 10)/10.0;
	_dy = SPEED * (rand() % 10)/10.0;
	_color[0] = (rand() % 10)/10.0;
	_color[1] = (rand() % 10)/10.0;
	_color[2] = (rand() % 10)/10.0;
}

void Ball::move()
{
	if ((_x + _radius > 1.0) || (_x - _radius < -1.0))
		{
			_dx *= -1;
		}
		if ((_y - _radius < -1.0) || (_y + _radius > 1.0))
		{
			_dy *= -1;
		}
		_x += _dx;
		_y += _dy;
}

void Ball::setRadius(float radius)
{
	_radius = radius;
}

float Ball::getX()
{
	return _x;
}

float Ball::getY()
{
	return _y;
}

float* Ball::getColor()
{
	return _color;
}


