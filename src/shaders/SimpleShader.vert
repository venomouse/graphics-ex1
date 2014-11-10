#version 330

layout(location = 2) in vec4 position;
uniform vec4 translation;
uniform mat4 scale;


void main()
{	
    gl_Position = scale*position + translation;
}
