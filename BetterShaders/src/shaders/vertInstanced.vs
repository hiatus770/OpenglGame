
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aOffset;

out vec3 fColor;

uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;
uniform vec4 aColor; 

void main()
{
    gl_Position = projection * view * model * vec4(aPos + aOffset, 1.0);
    fColor = {1.0f, 1.0f, 0.0f};
}