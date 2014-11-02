#version 330

uniform vec4 fillColor;

out vec4 outColor;

vec4 checker(vec4 FragCoord, int checkSize) {
	if (( int(gl_FragCoord.x) % (2*checkSize) < checkSize && int(gl_FragCoord.y) % (2*checkSize) >= checkSize) ||
                        (    int(gl_FragCoord.x) % (2*checkSize) >= checkSize && int(gl_FragCoord.y) % (2*checkSize) < checkSize) )
    {
        return fillColor;
    }
    else
    {
        return fillColor* 0;
    }
}

void main()
{
	int checkSize = 10;
	outColor = checker(gl_FragCoord, checkSize);
}

