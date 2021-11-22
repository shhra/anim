#version 440
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture_diffuse1;
uniform vec3 objectColor;
// uniform vec3 lightColor;

const float pi = 3.141592653589793238;
void main()
{
    vec3 lightColor =  1000. * vec3(0.5859, 0.4, 0.3);
    vec3 lightPos = 10.0 * vec3(1, 1, 0);
    // vec3 objectColor = vec3(0.125, 0.875, 0.250);
    float ambientStrength = 0.2;
    vec3 ambient = ambientStrength * lightColor / (4.0 * pi * length(lightPos) * length(lightPos));

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor / (4.0 * pi * length(lightPos) * length(lightPos));

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}
