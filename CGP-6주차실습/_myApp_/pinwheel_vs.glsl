#version 430 core                 
                                  
layout (location = 0) in vec3 pos;   
layout (location = 1) in vec3 color;
layout (location = 2) in vec3 colorChange;


uniform mat4 rotMat;
uniform mat4 transMat;
uniform mat4 viewMat;
uniform mat4 projMat;

out vec3 vsColor;

void main(void)                    
{
	gl_Position = projMat * viewMat * transMat * rotMat * vec4(pos.x, pos.y, pos.z, 1.0);

	vsColor = color + colorChange;
}															