#version 330

uniform vec4 fillColor;

out vec4 outColor;


//creates the checkered pattern 
vec4 checker(vec4 fragCoord, int checkSize) {

	if (( int(fragCoord.x) % (2*checkSize) < checkSize && int(fragCoord.y) % (2*checkSize) >= checkSize) ||
                        (    int(fragCoord.x) % (2*checkSize) >= checkSize && int(fragCoord.y) % (2*checkSize) < checkSize) )
    {
    	//blue checker
        return fillColor;
    }
    
    else
    {
    	//black checker
        return vec4(0.0f,0.0f,0.0f,1.0f);
    }
}

void main()
{
	int checkSize = 10;
	outColor = checker(gl_FragCoord, checkSize);
}

