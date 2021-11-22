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
    vec3 lightColor =  vec3(1.0, 1.0, 1.0);
    vec3 lightPos = 3.0 * vec3(1, 1, 1);
    // vec3 objectColor = vec3(0.125, 0.875, 0.250);
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    vec3 result = (diffuse) * objectColor;
    FragColor = vec4(norm, 1.0);
}
