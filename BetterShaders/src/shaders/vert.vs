#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 model; 
uniform mat4 view; 
uniform mat4 projection; 
uniform vec3 cameraPos; 

out float dist; 

void main()
{ 
    
    vec3 pos = vec3(projection * view * model * vec4(aPos.xyz, 1.0)); 
    // dist = sqrt((pos.x-cameraPos.x)*(pos.x-cameraPos.x) + (pos.y-cameraPos.y)*(pos.y-cameraPos.y) + (pos.z - cameraPos.z)*(pos.z - cameraPos.z));  
    dist = length(pos); 
    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}