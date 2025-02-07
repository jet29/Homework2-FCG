#version 330 core
out vec4 fragColor;

struct LightColor{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};


struct DirectionalLightProperties {
   vec3 direction;
   LightColor color;
};

in Data{
    vec3 vertexPos;
    vec3 normal;
}dataIn;


uniform DirectionalLightProperties light;

uniform vec3 viewPos;
uniform float shininess;

uniform sampler2D mySampler;

void main()
{
    vec3 normal=normalize(dataIn.normal);
    vec3 ambient=light.color.ambient;
   
    vec3 lightContribution=ambient;

    // Direction to the light (Directional Light)
    vec3 lightDir=normalize(-light.direction);

    // Lambert cos(angle(Normal, Light))
    float diff=max(dot(normal,lightDir),0.f);
    vec3 diffuse=diff*light.color.diffuse;
    lightContribution=lightContribution+diffuse;

    if(diff>0.f){
        // Vector from the vertex to the camera
        vec3 viewDir=normalize(viewPos-dataIn.vertexPos);
        // Blinn-Phong
        vec3 halfwayDir=normalize(lightDir+viewDir);
        float spec=pow(max(dot(normal,halfwayDir),0.f),shininess);
        vec3 specular=light.color.specular*spec;
        lightContribution=lightContribution+specular;
    }

    fragColor=vec4(lightContribution, 1.f);
}