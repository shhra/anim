#version 440
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in ivec4 joints;
layout (location = 3) in vec4 weights;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform mat4 inversebindPose[100];
uniform mat4 worldPose[100];
uniform bool is_skin;

void main()
{
    mat4 skin = (worldPose[joints.x] * inversebindPose[joints.x]) * weights.x;
    skin += (worldPose[joints.y] * inversebindPose[joints.y]) * weights.y;
    skin += (worldPose[joints.z] * inversebindPose[joints.z]) * weights.z;
    skin += (worldPose[joints.w] * inversebindPose[joints.w]) * weights.w;

    if (is_skin) {
        FragPos = vec3(model * skin * vec4(aPos, 1.0));
        Normal = vec3( skin * vec4(aNormal, 0.0));
    } else {
        FragPos = vec3(model * vec4(aPos, 1.0));
        Normal = aNormal;
//        Normal = transpose(inverse(mat3(model))) * aNormal;
    }
//    Normal = transpose(inverse(mat3(model))) * aNormal;
    gl_Position = projection * view * vec4(FragPos, 1.0);
}
