#version 330

layout(location = 2) in vec4 position;
uniform vec4 translation[10];
uniform mat4 scale[10];

flat out int InstanceID;


void main()
{	
    gl_Position = scale[gl_InstanceID]*position + translation[gl_InstanceID];
	InstanceID = gl_InstanceID;
}
