#version 330 core

// Input data from your 3D models (.obj files)
layout (location = 0) in vec3 aPos;       // Vertex Position
layout (location = 1) in vec2 aTexCoords; // Texture Coordinates (for images)
layout (location = 2) in vec3 aNormal;    // Vertex Normal (for lighting)


out vec3 FragPos;  
out vec3 Normal;
out vec2 TexCoords;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normalMatrix; // Your custom 3x3 normal matrix!

void main()
{
    //Calculate where the pixel is in the 3D world
    // model matrix = umView * umWorld * umRotation
    FragPos = vec3(model * vec4(aPos, 1.0));
    
    // normal for 
    Normal = normalMatrix * aNormal;  
    
    // 
    TexCoords = aTexCoords;
    
    
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
