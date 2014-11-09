/*
 * ball.h
 *
 *  Created on: Nov 9, 2014
 *      Author: moshemandel
 */

#ifndef BALL_H_
#define BALL_H_

#define SPEED 0.05
#define INIT_RADIUS	0.1

class Ball
{
	float _x, _y, _dx, _dy, _radius;
	float _color[3];

public:
	Ball(float radius = INIT_RADIUS);
	Ball(float x, float y, float radius = INIT_RADIUS);
	virtual ~Ball();

	void init();
	void move();

	void setRadius(float radius);
	float getRadius() {return _radius;}

	float getX();
	float getY();
	float* getColor();
private:

};


#endif /* BALL_H_ */
