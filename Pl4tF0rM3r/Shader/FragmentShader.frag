#version 330 core

in vec3 FragColor;
in vec2 UV;
in vec3 Normal;
in vec3 FragPos;


out vec4 color;

uniform sampler2D myTextureSampler;
uniform bool isTexture;

uniform bool isCellShading;

uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(){

	//ambient
	float ambiantFactor = 0.1f;
	vec3 ambient = lightColor * ambiantFactor;

	//diffuse
	vec3 normal = normalize(Normal);
	vec3 lightDir = normalize(lightPos-FragPos);
	float diff = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = lightColor * diff;

	//specular
	float specularFactor = 3.f;
	float shininess = 128.0f;
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 halfDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfDir), 0.0f), shininess);
	vec3 specular = lightColor * specularFactor * spec;

	vec3 result = ambient;

	//cellshading
	float intensity=0.f;
	if(isCellShading){
		intensity = 0.6 * diff + 0.4 * spec;
		if (intensity > 0.8) {
			intensity = 1.0;
		}
		else if (intensity > 0.60) {
			intensity = 0.7;
		}
		else if (intensity > 0.5) {
			intensity = ((int(gl_FragCoord.x)%3==1 || int(gl_FragCoord.x)%3==2) && (int(gl_FragCoord.y)%3==1 || int(gl_FragCoord.y)%3==2)?0.15:0.5);
		}
		else if (intensity > 0.25){
			intensity = ((int(gl_FragCoord.x)%3==1 || int(gl_FragCoord.x)%3==2) && (int(gl_FragCoord.y)%3==1 || int(gl_FragCoord.y)%3==2)?0.1:0.35);
		}
		else {
			intensity = ((int(gl_FragCoord.x)%3==1 || int(gl_FragCoord.x)%3==2) && (int(gl_FragCoord.y)%3==1 || int(gl_FragCoord.y)%3==2)?0.05:0.2);
		}

		float discontinuite = length(fwidth(normal));
		result = (1- discontinuite)+ambient+ intensity;
	}else{
		//result
		result = ambient + diffuse + specular;
	}


	if(isTexture){
		vec4 texel = texture(myTextureSampler, UV);
		color = vec4(result, 1.0f) * texel;
	}else{
		color = vec4(result, 1.0f) * vec4(FragColor, 1.0f);
	}
}