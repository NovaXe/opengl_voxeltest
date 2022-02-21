#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 objectColor;

uniform sampler2D objectTexture;

void main()
{
    float ambientStrength = 0.2f;
    float specularStrength = 0.5f;
    vec3 ambient = ambientStrength * lightColor;
    
    

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(reflectDir, viewDir), 0.0), 32);

    vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = texture(objectTexture, TexCoord) * vec4 (result, 1.0);


    //FragColor = vec4 (result, 1.0);

}