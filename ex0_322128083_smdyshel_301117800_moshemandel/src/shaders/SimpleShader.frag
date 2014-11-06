#version 330

#define CHECKER_SIZE  10
uniform vec4 fillColor;

out vec4 outColor;


//creates the checkered pattern 
vec4 checker(vec4 fragCoord) {

   	if (( int(fragCoord.x) % (2*CHECKER_SIZE) < CHECKER_SIZE && int(fragCoord.y) % (2*CHECKER_SIZE) >= CHECKER_SIZE) ||
                        (    int(fragCoord.x) % (2*CHECKER_SIZE) >= CHECKER_SIZE && int(fragCoord.y) % (2*CHECKER_SIZE) < CHECKER_SIZE) )
    {
    	//colored checker
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
	outColor = checker(gl_FragCoord);
}

