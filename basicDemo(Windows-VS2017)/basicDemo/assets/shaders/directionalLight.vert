#version 330 core
// Atributte 0 of the vertex
layout (location = 0) in vec3 vertexPosition;
// Atributte 1 of the vertex
layout (location = 1) in vec3 vertexNormal;
// Attribute 2 of the vertex
layout (location = 2) in vec2 vertexUV;

out Data{
    vec3 vertexPos;
    vec3 normal;
}dataOut;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main()
{
    mat4 modelView = view * model;
    mat4 MVP = proj * modelView;
    mat4 normalMatrix = transpose(inverse(modelView));
    // World space vertex
    dataOut.vertexPos = vec3(model*vec4(vertexPosition,1.f));
   // World space normal
    dataOut.normal  = vec3( normalMatrix * vec4(vertexNormal,0.0) );
    //dataOut.normal = vertexNormal;


    gl_Position = MVP * vec4(vertexPosition, 1.0f);
}