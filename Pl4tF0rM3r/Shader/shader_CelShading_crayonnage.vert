#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aText;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 Color;

out vec3 ourColor;

out vec3 FragmentPos;
out vec3 Normal;

void main(){
    // note that we read the multiplication from right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0);
    FragmentPos= vec3(model * vec4(aPos, 1.0f));
    ourColor = color; // aNormal pour voir les couleur des normals
    Normal = mat3(transpose(inverse(model))) * aNormal;
}