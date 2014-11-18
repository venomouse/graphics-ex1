/*
 * Ball.cpp
 *
 *  Created on: Nov 9, 2014
 *      Author: moshemandel
 */


#include "Ball.h"
#include <cstdlib>
#include <algorithm>

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
	_dx = 2*SPEED * (rand() % 10)/10.0 - SPEED;
	_dy = 2*SPEED * (rand() % 10)/10.0 - SPEED;
	_color[0] = (rand() % 255)/255.0;
	_color[1] = (rand() % 255)/255.0;
	_color[2] = (rand() % 255)/255.0;
	_scale = 1.0f;
	_gXscale = 1.0f;
	_gYscale = 1.0f;
}

void Ball::move()
{
	if ((_x + _radius/_gXscale > 1.0) || (_x - _radius/_gXscale < -1.0))
	{
		_dx *= -1;
	}
	if ((_y - _radius/_gYscale < -1.0) || (_y + _radius/_gYscale > 1.0))
	{
		_dy *= -1;
	}
	_x += _dx/_gXscale;
	_y += _dy/_gYscale;
}


void Ball::shrink(float radiusDiff)
{
	float newRadius = _scale*_radius - radiusDiff;
	_scale = newRadius/_radius;
}

void Ball::enlarge()
{
	_scale = std::min(1.0f, _scale+0.05f);
}

void Ball::setGXYScale (float gXscale, float gYscale)
{
	_gXscale = gXscale;
	_gYscale = gYscale;
}

