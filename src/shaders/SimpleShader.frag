#version 330

uniform vec2 windowSize;

uniform vec4 fillColor[10];
uniform float ballRadius[10];
uniform vec4 translation[10];

flat in int InstanceID;
out vec4 outColor;

void main()
{
	vec4 fillColorFrag = fillColor[InstanceID];
	float ballRadiusFrag = ballRadius[InstanceID];
	vec2 offset = vec2(translation[InstanceID].x, translation[InstanceID].y);
	
	float gradientAlpha = 0.8;
	vec4 lightColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	vec2 lightPosition = vec2(1.1f, 1.5f);
	
	vec2 position;
	position.x = 2*(gl_FragCoord.x - windowSize.x/2)/windowSize.x; //w.r.t. to window
	position.y = 2*(gl_FragCoord.y - windowSize.y/2)/windowSize.y; //w.r.t. to window
	
	position = position - offset.xy; //w.r.t. ball center
	
	lightPosition.xy = lightPosition.xy - offset.xy; //w.r.t. ball center
	
	float lightDist = sqrt(lightPosition.x*lightPosition.x + lightPosition.y*lightPosition.y);
	vec2 lightDir = lightPosition/lightDist; //normalized direction vector
	vec2 gradientCenter = lightDir*0.7*ballRadiusFrag; 
	vec2 positionToGradientCenter = position - gradientCenter; //position w.r.t. gradient center
	float distFromGradientCenter = sqrt(positionToGradientCenter.x*positionToGradientCenter.x 
											+ positionToGradientCenter.y*positionToGradientCenter.y);

	float p = distFromGradientCenter/ballRadiusFrag;
	outColor = (1-gradientAlpha)*fillColorFrag + gradientAlpha*(p*fillColorFrag +(1-p)*lightColor);
	

	
}

