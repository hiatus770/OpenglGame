#version 330 core
out vec4 FragColor;
in float dist; 

uniform vec4 color = vec4(1.0, 1.0, 1.0, 1.0); 

void main()
{ 
    // FragColor = color;
    // if (dist > 10){
    //     FragColor = mix(color, vec4(0.0, 0.0, 0.0, 1.0), dist/30);
    // } else {
    //     FragColor = color; 
    // }

    FragColor = mix(color, vec4(0.0, 0.0, 0.0, 1.0), (dist)/400);
}