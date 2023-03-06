#version 330 core
layout(location =0) in vec3 VertexPos;

layout(location =1) in vec3 InColor;

layout(location =2) in vec2 vertexUV;

layout(location =3) in vec3 vertexNormal;


out vec2 UV;
out vec3 FragColor;
out vec3 Normal;
out vec3 FragPos;

uniform vec3 color;
uniform mat4 mvp;
uniform mat4 model;

void main(){
	UV = vertexUV;
    FragPos= vec3(model * vec4(VertexPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * vertexNormal;
	gl_Position = mvp * vec4(VertexPos,1);	
	FragColor=color;
}