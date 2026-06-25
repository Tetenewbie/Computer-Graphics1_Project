#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;


uniform sampler2D diffuseTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main()
{
    // Ambient light
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse light
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    // angle btw the light and the surface
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // specular light
    float specularStrength = 0.6;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    //
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = specularStrength * spec * lightColor;

    //
    vec3 texColor = texture(diffuseTexture, TexCoords).rgb;

    // Multiply the texture color by combined lighting
    vec3 result = (ambient + diffuse + specular) * texColor;

    // FOG MATH
    float distance = length(viewPos - FragPos);
    float fogNear = 70.0; // Where fog starts
    float fogFar = 100.0;  // Where road becomes completely invisible
    float fogFactor = (fogFar - distance) / (fogFar - fogNear);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    
    // A light grayish-blue to match the lower part of your skybox
    vec3 fogColor = vec3(0.6f, 0.7f, 0.8f); 
    
    // Mix the original color with the fog color based on distance
    vec3 finalColor = mix(fogColor, result, fogFactor);

    FragColor = vec4(finalColor, 1.0);
    // FragColor = vec4(result, 1.0);

}
