#version 330

uniform vec4 fillColor;
uniform float lightRadius;
uniform vec4 whitePoint;

out vec4 outColor;

float distance_2D(vec4 p1, vec4 p2)
{
	return sqrt((p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y));
}

void main()
{
	float intensity = (distance_2D(gl_FragCoord, whitePoint) > lightRadius) ? 0.5 : 1;
	outColor = intensity*fillColor;
}

