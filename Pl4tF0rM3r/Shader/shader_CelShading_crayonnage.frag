#version 330 core

out vec4 FragColor;
in vec3 ourColor;
in vec3 Normal;

uniform vec3 viewPos;

in vec3 FragmentPos;

void main(){

    vec3 lightPos = vec3(2.0f, 1.5f, 2.0f);
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f); // color light
    vec3 norm = normalize(Normal);

    // ambient
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // diffuse
    vec3 lightDir = normalize(lightPos - FragmentPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

//    // specular
    vec3 viewDir = normalize(viewPos - FragmentPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 5);
    float intensity = 0.6 * diff + 0.4 * spec;


    if (intensity > 0.8) {
        intensity = 1.2;
    }
    else if (intensity > 0.60) {
        intensity = 0.8;
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

    float discontinuite = length(fwidth(norm));
    vec3 result = ((1- discontinuite)+ambient+ intensity);
    FragColor = vec4(result, 1.0f);

}