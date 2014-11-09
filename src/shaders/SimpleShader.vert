#version 330

layout(location = 2) in vec4 position;
uniform vec4 translation;
uniform float scale;


void main()
{	
    gl_Position = scale*position + translation;
}
