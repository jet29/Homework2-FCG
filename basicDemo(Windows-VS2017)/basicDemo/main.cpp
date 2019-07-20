#include <glad/glad.h> // Glad has to be include before glfw
#include <GLFW/glfw3.h>
#include <map>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <stb_image.h>
#include "UserInterface.h"
#include "Shader.h"
#include "Model.h"
#include "Light.h"

const int NUM_POINTLIGHT = 2;

// set up vertex data (and buffer(s)) and configure vertex attributes
	// ------------------------------------------------------------------
float planeVertices[] = {
	// positions            // normals         // texcoords
	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

	 25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
	-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
	 25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 25.0f
};

float cubeVertices[] = {
	// back face
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
	 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
	-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
	-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
	// front face
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
	-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
	-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
	// left face
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
	-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
	// right face
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
	 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
	 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
	 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
	// bottom face
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
	 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
	-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
	-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
	// top face
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
	 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
	-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
	-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        
};

float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};

float transparentVertices[] = {
	// positions         // texture Coords (swapped y coordinates because texture is flipped upside down)
	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

	0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
	1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
	1.0f,  0.5f,  0.0f,  1.0f,  0.0f
};

// transparent window locations
vector<glm::vec3> windows
{
	glm::vec3(-1.5f, 10.0f, -1.48f),
	glm::vec3(1.5f, 10.0f, 1.51f),
	glm::vec3(0.0f, 10.0f, 2.7f),
	glm::vec3(-0.3f, 0.0f, -2.3f),
	glm::vec3(0.5f, 0.0f, -0.6f)
};

enum TypeEnviromentMapping {
	reflection,
	refraction
};

struct LightColor {
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct Attenuation {
	float constant;
	float linear;
	float quadratic;
};

struct PointLightProperties {
	glm::vec3 position;
	LightColor color;
	Attenuation attenuation;
};


struct DirectionalLightProperties {
	glm::vec3 direction;
	LightColor color;
};

struct SpotLightProperties {
	glm::vec3 position;
	glm::vec3 direction;
	LightColor color;
	float cutOff;
	float outerCutOff;
	Attenuation attenuation;
};


struct Data {
	glm::vec3 vertexPos;
	glm::vec3 normal;
};

//Models dependent of his Material
vector<Model *> modelsBlinnPhong;
vector<Model *> modelsOrenNayar;
vector<Model *> modelsCookTorrance;
vector<Model *> modelsNormalMapping;
vector<Model *> modelsReflection;
vector<Model *> modelsRefraction;
vector<Model *> modelsTransparent;
vector<Model *> modelsShadow;

//Skybox cubemap
Model *modelSkyBox;

//Lighting Things
Light *lightSources[2];
PointLightProperties pointLights[2];
DirectionalLightProperties directionalLight;
SpotLightProperties spotLight;
float shininess = 32;
float roughness = .3;
float intensity = 1;
float reflectance = 0.8;
float heightScale = 0.1;
float refractiveIndex = 1.52;

float currentTime = 0;
float lastTime = 0;

//Frame Buffer Object for depth map
unsigned int depthMapFBO;
//Texture for depth map
unsigned int depthMap;

//Shadow Map Size
unsigned int SHADOW_WIDTH = 1024;
unsigned int SHADOW_HEIGHT = 1024;

// Window current width
unsigned int windowWidth = 800;
// Window current height
unsigned int windowHeight = 600;
// Window title
const char *windowTitle = "Tarea2";
// Window pointer
GLFWwindow *window;
// User Interface
CUserInterface * userInterface;
// Right button is currently pressed
bool rightButtonPressed = false;

// Shader for regular models
Shader *shaderBlinnPhong;
Shader *shaderOrenNayar;
Shader *shaderCookTorrance;
Shader *shaderNormalMapping;
Shader *shaderSkyBox;
Shader *shaderReflection;
Shader *shaderRefraction;
Shader *shaderBlending;
Shader *shaderDepth;
Shader *shaderShadowMap;
Shader *debugDepthQuad;
// Shader for lights
Shader *shaderLights;

// Index (GPU) of the textures used
unsigned int houseTextureID;
unsigned int planeTextureID;
unsigned int brickTextureID;
unsigned int brickNormalTextureID;
unsigned int brickDepthTextureID;
unsigned int windowTextureID;
unsigned int containerTextureID;
unsigned int toyBoxTextureID;
unsigned int toyBoxNormalTextureID;
unsigned int toyBoxDepthTextureID;

//cube map texture id
unsigned int cubemapTextureID;

// Camera Start Position
glm::vec3 position = glm::vec3( 0, 0, 5);
// horizontal angle : toward -Z
float horizontalAngle = -3.14;
// vertical angle : 0, look at the horizon
float verticalAngle = 0.0f;
// Initial Field of View
float initialFoV = 45.0f;

float speed = 3.0f; // 3 units / second
float mouseSpeed = 0.005f;

// Direction of the Camera
glm::vec3 direction;
// Up vector for the Camera
glm::vec3 up;


//Toggle Active Lights
bool isActiveSpotLight = true;
bool isActiveDirLight = true;
bool isActivePointLight1 = true;
bool isActivePointLight2 = true;
bool isActiveParallaxMapping = true;


/**
 * Handles the window resize
 * @param{GLFWwindow} window pointer
 * @param{int} new width of the window
 * @param{int} new height of the window
 * */
void resize(GLFWwindow *window, int width, int height)
{
    windowWidth = width;
    windowHeight = height;
    // Sets the OpenGL viewport size and position
    glViewport(0, 0, windowWidth, windowHeight);
	userInterface->reshape(windowWidth,windowHeight);
}

void mouseButton(GLFWwindow* window, int button, int action, int mods)
{
	if (TwEventMouseButtonGLFW(button, action))
		return;
}

void cursorPos(GLFWwindow* window, double x, double y)
{
	if (TwEventMousePosGLFW(int(x), int(y)))
		return;
}

void charInput(GLFWwindow* window, unsigned int scanChar)
{
	if (TwEventCharGLFW(scanChar, GLFW_PRESS))
		return;
}


/**
 * Initialize the glfw library
 * @returns{bool} true if everything goes ok
 * */
bool initWindow()
{
	// Initialize glfw
	glfwInit();
	// Sets the Opegl context to Opengl 3.0
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Creates the window
	window = glfwCreateWindow(windowWidth, windowHeight, windowTitle, NULL, NULL);

	// The window couldn't be created
	if (!window)
	{
		std::cout << "Failed to create the glfw window" << std::endl;
		glfwTerminate(); // Stops the glfw program
		return false;
	}

	// Creates the glfwContext, this has to be made before calling initGlad()
	glfwMakeContextCurrent(window);

	// Window resize callback
	glfwSetFramebufferSizeCallback(window, resize);

	// Other callbacks
	glfwSetMouseButtonCallback(window, mouseButton);
	glfwSetCursorPosCallback(window, cursorPos);
	glfwSetCharCallback(window, charInput);


    return true;
}
/**
 * Initialize the glad library
 * @returns{bool} true if everything goes ok
 * */
bool initGlad()
{
    // Initialize glad
    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    // If something went wrong during the glad initialization
    if (!status)
    {
        std::cout << status << std::endl;
        std::cout << "Failed to initialize GLAD" << std::endl;
        return false;
    }
    return true;
}


bool initUserInterface()
{

	if (!TwInit(TW_OPENGL_CORE, NULL))
		return false;


	TwWindowSize(windowWidth, windowHeight);
	userInterface = CUserInterface::Instance();

	return true;
}

void UpdateUserInterface() {

	//Material Parameters
	shininess = userInterface->getShininess();
	roughness = userInterface->getRoughness();
	reflectance = userInterface->getReflectance();
	intensity = userInterface->getIntensity();
	refractiveIndex = userInterface->getRefractiveIndex();

	//POINT LIGHT 1
	pointLights[0].position = userInterface->getPointLight1Translation();
	
	float* ambientPointLight1 = userInterface->getPointLight1AmbientColor();
	float* diffusePointLight1 = userInterface->getPointLight1DiffuseColor();
	float* specularPointLight1 = userInterface->getPointLight1SpecularColor();

	pointLights[0].color.ambient = glm::vec3(ambientPointLight1[0], ambientPointLight1[1], ambientPointLight1[2]);
	pointLights[0].color.diffuse = glm::vec3(diffusePointLight1[0], diffusePointLight1[1], diffusePointLight1[2]);
	pointLights[0].color.specular = glm::vec3(specularPointLight1[0], specularPointLight1[1], specularPointLight1[2]);

	pointLights[0].attenuation.constant  = userInterface->getPointLight1AttConsColor();
	pointLights[0].attenuation.linear = userInterface->getPointLight1AttLiColor();
	pointLights[0].attenuation.quadratic = userInterface->getPointLight1AttCuaColor();

	isActivePointLight1 = userInterface->getIsActivePointLight1();

	//POINT LIGHT 2
	pointLights[1].position = userInterface->getPointLight2Translation();
	
	float* ambientPointLight2 = userInterface->getPointLight2AmbientColor();
	float* diffusePointLight2 = userInterface->getPointLight2DiffuseColor();
	float* specularPointLight2 = userInterface->getPointLight2SpecularColor();

	pointLights[1].color.ambient = glm::vec3(ambientPointLight2[0], ambientPointLight2[1], ambientPointLight2[2]);
	pointLights[1].color.diffuse = glm::vec3(diffusePointLight2[0], diffusePointLight2[1], diffusePointLight2[2]);
	pointLights[1].color.specular = glm::vec3(specularPointLight2[0], specularPointLight2[1], specularPointLight2[2]);

	pointLights[1].attenuation.constant = userInterface->getPointLight2AttConsColor();
	pointLights[1].attenuation.linear = userInterface->getPointLight2AttLiColor();
	pointLights[1].attenuation.quadratic = userInterface->getPointLight2AttCuaColor();

	isActivePointLight2 = userInterface->getIsActivePointLight2();

	//DIRECTIONAL LIGHT (SUN)
	float* lightDirection = userInterface->getLightDirection();
	directionalLight.direction = glm::vec3(lightDirection[0], lightDirection[1], lightDirection[2]);

	float* ambientDirLight = userInterface->getDirLightAmbientColor();
	float* diffuseDirLight = userInterface->getDirLightDiffuseColor();
	float* specularDirLight = userInterface->getDirLightSpecularColor();

	directionalLight.color.ambient = glm::vec3(ambientDirLight[0], ambientDirLight[1], ambientDirLight[2]);
	directionalLight.color.diffuse = glm::vec3(diffuseDirLight[0], diffuseDirLight[1], diffuseDirLight[2]);
	directionalLight.color.specular = glm::vec3(specularDirLight[0], specularDirLight[1], specularDirLight[2]);

	isActiveDirLight = userInterface->getIsActiveDirLight();

	//SPOT LIGHT
	spotLight.outerCutOff = userInterface->getOuterCutOff();
	spotLight.cutOff = userInterface->getInnerCutOff();

	spotLight.attenuation.constant = userInterface->getSpotLightAttConsColor();
	spotLight.attenuation.linear = userInterface->getSpotLightAttLiColor();
	spotLight.attenuation.quadratic = userInterface->getSpotLightAttCuaColor();

	float* ambientSpotLight = userInterface->getSpotLightAmbientColor();
	float* diffuseSpotLight = userInterface->getSpotLightDiffuseColor();
	float* specularSpotLight = userInterface->getSpotLightSpecularColor();

	spotLight.color.ambient = glm::vec3(ambientSpotLight[0], ambientSpotLight[1], ambientSpotLight[2]);
	spotLight.color.diffuse = glm::vec3(diffuseSpotLight[0], diffuseSpotLight[1], diffuseSpotLight[2]);
	spotLight.color.specular = glm::vec3(specularSpotLight[0], specularSpotLight[1], specularSpotLight[2]);

	isActiveSpotLight = userInterface->getIsActiveSpotLight();

	heightScale = userInterface->getHeightScale();
	isActiveParallaxMapping = userInterface->getIsActiveParallaxMapping();
}


/**
 * Initialize the opengl context
 * */
void initGL()
{
    // Enables the z-buffer test
    glEnable(GL_DEPTH_TEST);
	// Enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Sets the ViewPort
    glViewport(0, 0, windowWidth, windowHeight);
    // Sets the clear color
    glClearColor(0.3f, 0.3f, 0.3f, 1.0f);

}

/**
 * Loads a texture into the GPU
 * @param{const char} path of the texture file
 * @returns{unsigned int} GPU texture index
 * */
unsigned int loadTexture(const char *path)
{
    unsigned int id;
    // Creates the texture on GPU
    glGenTextures(1, &id);
    // Loads the texture
    int textureWidth, textureHeight, numberOfChannels;
    // Flips the texture when loads it because in opengl the texture coordinates are flipped
    stbi_set_flip_vertically_on_load(true);
    // Loads the texture file data
    unsigned char *data = stbi_load(path, &textureWidth, &textureHeight, &numberOfChannels, 0);
    if (data)
    {
        // Gets the texture channel format
        GLenum format;
        switch (numberOfChannels)
        {
        case 1:
            format = GL_RED;
            break;
        case 3:
            format = GL_RGB;
            break;
        case 4:
            format = GL_RGBA;
            break;
        }

        // Binds the texture
        glBindTexture(GL_TEXTURE_2D, id);
        // Creates the texture
        glTexImage2D(GL_TEXTURE_2D, 0, format, textureWidth, textureHeight, 0, format, GL_UNSIGNED_BYTE, data);
        // Creates the texture mipmaps
        glGenerateMipmap(GL_TEXTURE_2D);

        // Set the filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        std::cout << "ERROR:: Unable to load texture " << path << std::endl;
        glDeleteTextures(1, &id);
    }
    // We dont need the data texture anymore because is loaded on the GPU
    stbi_image_free(data);

    return id;
}

/**
* load the cubemap
**/
unsigned int loadCubemap(vector<std::string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	// Flips the texture when loads it because in opengl the texture coordinates are flipped
	stbi_set_flip_vertically_on_load(false);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}


/**
 * Initialize everything
 * @returns{bool} true if everything goes ok
 * */
bool init()
{
    // Initialize the window, and the glad components
    if (!initWindow() || !initGlad() || !initUserInterface() )
        return false;

    // Initialize the opengl context
    initGL();

    // Loads the shader
	shaderLights = new Shader("assets/shaders/basic.vert", "assets/shaders/basic.frag");
	shaderBlinnPhong = new Shader("assets/shaders/lightningBlingPhong.vert", "assets/shaders/lightningBlingPhong.frag");
	shaderOrenNayar = new Shader("assets/shaders/lightningOrenNayar.vert", "assets/shaders/lightningOrenNayar.frag");
	shaderCookTorrance = new Shader("assets/shaders/lightningCookTorrance.vert", "assets/shaders/lightningCookTorrance.frag");
	shaderNormalMapping = new Shader("assets/shaders/normalMapping.vert", "assets/shaders/normalMapping.frag");
	shaderSkyBox = new Shader("assets/shaders/cubeMap.vert", "assets/shaders/cubeMap.frag");
	shaderReflection = new Shader("assets/shaders/reflection.vert", "assets/shaders/reflection.frag");
	shaderRefraction = new Shader("assets/shaders/refraction.vert", "assets/shaders/refraction.frag");
	shaderBlending = new Shader("assets/shaders/blending.vert", "assets/shaders/blending.frag");
	shaderDepth = new Shader("assets/shaders/shadowMapDepth.vert", "assets/shaders/shadowMapDepth.frag");
	shaderShadowMap = new Shader("assets/shaders/shadowMap.vert", "assets/shaders/shadowMap.frag");
	debugDepthQuad = new Shader("assets/shaders/debugDepth.vert", "assets/shaders/debugDepth.frag");

	#pragma region loadTextures
	// Loads the texture into the GPU
	houseTextureID = loadTexture("assets/textures/cottage_diffuse.png");
	planeTextureID = loadTexture("assets/textures/plane_diffuse.jpg");
	brickTextureID = loadTexture("assets/textures/bricks2.jpg");
	brickNormalTextureID = loadTexture("assets/textures/bricks2_normal.jpg");
	brickDepthTextureID = loadTexture("assets/textures/bricks2_disp.jpg");
	windowTextureID = loadTexture("assets/textures/window.png");
	containerTextureID = loadTexture("assets/textures/container2.png");
	toyBoxTextureID = loadTexture("assets/textures/toy_box_diffuse.png");
	toyBoxNormalTextureID = loadTexture("assets/textures/toy_box_normal.png");
	toyBoxDepthTextureID = loadTexture("assets/textures/toy_box_disp.png");

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



	// Loads the cubemap
	vector<std::string> faces =
	{
		"assets/textures/cubemap2/right.jpg",
		"assets/textures/cubemap2/left.jpg",
		"assets/textures/cubemap2/top.jpg",
		"assets/textures/cubemap2/bottom.jpg",
		"assets/textures/cubemap2/front.jpg",
		"assets/textures/cubemap2/back.jpg"
	};

	cubemapTextureID = loadCubemap(faces);
	#pragma endregion


	//POSITION TESTING MODELS
	glm::vec3 modelPosition1 = glm::vec3(0, 0, -30); //BLINN-PHONG
	glm::vec3 modelPosition2 = glm::vec3(0, 0, 0); //OREN-NAYAR
	glm::vec3 modelPosition3 = glm::vec3(0, 0, 30); //COOK-TORRANCE

	glm::vec3 planePosition = glm::vec3(0, 0, 0);


	string pathCube = "./assets/models/cube.obj";
	string pathHouse = "./assets/models/cottage/cottage.obj";
	string pathPlane = "./assets/models/plane.obj";

	#pragma region LoadModels

	//CREATING THE HOUSE
	Model *model1 = new Model();
	if (model1->LoadObj(pathHouse.c_str())) {
		model1->BuildGeometry();

		modelsBlinnPhong.push_back(model1);
	}

	model1->setPosition(glm::vec3(0, 0, -30));
	model1->setMaterial(blinnPhong);
	model1->setTextureID(houseTextureID);

	//CREATING THE PLANE
	Model *model4 = new Model();
	if (model4->LoadObj(pathPlane.c_str())) {
		model4->BuildGeometry();

		modelsShadow.push_back(model4);
	}

	model4->setPosition(glm::vec3(0, -1, 0));
	model4->setMaterial(blinnPhong);
	model4->setTextureID(planeTextureID);
	
	//Creating Model Skybox
	modelSkyBox = new Model();
	if (!modelSkyBox->LoadWithArray1(skyboxVertices, sizeof(skyboxVertices))) {
		cout << "couldn't load the cube map :(" << endl;
	}

	//Creating Plane for Parallax Mapping
	Model *planeNP = new Model();
	if (planeNP->LoadPlane()) {

		modelsNormalMapping.push_back(planeNP);
	}

	planeNP->setPosition(glm::vec3(0, 5, 0));
	planeNP->setMaterial(blinnPhong);
	planeNP->setTextureID(brickTextureID);
	planeNP->setNormalMapTextureID(brickNormalTextureID);
	planeNP->setParallaxMapTextureID(brickDepthTextureID);

	//Creating Plane for Parallax Mapping
	Model *planeNP2 = new Model();
	if (planeNP2->LoadPlane()) {

		modelsNormalMapping.push_back(planeNP2);
	}

	planeNP2->setPosition(glm::vec3(-10, 5, 0));
	planeNP2->setMaterial(blinnPhong);
	planeNP2->setTextureID(toyBoxTextureID);
	planeNP2->setNormalMapTextureID(toyBoxNormalTextureID);
	planeNP2->setParallaxMapTextureID(toyBoxDepthTextureID);


	//Creating reflection models
	Model *reflectionModel = new Model();
	if (reflectionModel->LoadObj(pathCube.c_str())) {
		reflectionModel->BuildGeometry();

		modelsReflection.push_back(reflectionModel);
	}

	reflectionModel->setPosition(glm::vec3(3, 10, 25));

	//Creating refraction models
	Model *refractionModel = new Model();
	if (refractionModel->LoadObj(pathCube.c_str())) {
		refractionModel->BuildGeometry();
		modelsRefraction.push_back(refractionModel);
	}


	refractionModel->setPosition(glm::vec3(-3, 10, 25));

	//CREATING THE LIGHTS MODELS
	for (int i = 0; i < 2; i++) {

		lightSources[i] = new Light();
		if (lightSources[i]->LoadObj(pathCube.c_str())) {
			lightSources[i]->BuildGeometry();
		}
	}

	userInterface->setPointLight1Translation(glm::vec3(-3, 5, 5));
	userInterface->setPointLight2Translation(glm::vec3(-71.0591, 68.8646, -8.3236));


	Model *modelWindow1 = new Model();
	if (modelWindow1->LoadWithArray2(transparentVertices, (sizeof(transparentVertices) / sizeof(*transparentVertices)) )) {
		modelsTransparent.push_back(modelWindow1);
	}
	else {
		cout << "couldn't load the first window :(" << endl;
	}

	modelWindow1->setTextureID(windowTextureID);
	modelWindow1->setPosition(windows[0]);

	Model *modelWindow2 = new Model();
	if (modelWindow2->LoadWithArray2(transparentVertices, (sizeof(transparentVertices) / sizeof(*transparentVertices)))) {
		modelsTransparent.push_back(modelWindow2);
	}
	else {
		cout << "couldn't load the first window :(" << endl;
	}

	modelWindow2->setTextureID(windowTextureID);
	modelWindow2->setPosition(windows[1]);

	Model *modelWindow3 = new Model();
	if (modelWindow3->LoadWithArray2(transparentVertices, (sizeof(transparentVertices) / sizeof(*transparentVertices)))) {
		modelsTransparent.push_back(modelWindow3);
	}
	else {
		cout << "couldn't load the first window :(" << endl;
	}

	modelWindow3->setTextureID(windowTextureID);
	modelWindow3->setPosition(windows[1]);

	//MODELS TO BE AFFECTED WITH SHADOWS
	Model *modelCubeShadow = new Model();

	if (modelCubeShadow->LoadWithArray3(cubeVertices, (sizeof(cubeVertices) / sizeof(*cubeVertices)) )) {
		modelsShadow.push_back(modelCubeShadow);
	}
	else
	{
		cout << "Could not load the cube shadow 1 :(" << endl;
	}
	
	modelCubeShadow->setPosition(glm::vec3(-20, 5, 0));
	modelCubeShadow->setTextureID(containerTextureID);

	Model *modelCubeShadow2 = new Model();

	if (modelCubeShadow2->LoadWithArray3(cubeVertices, (sizeof(cubeVertices) / sizeof(*cubeVertices)))) {
		modelsShadow.push_back(modelCubeShadow2);
	}
	else
	{
		cout << "Could not load the cube shadow 1 :(" << endl;
	}

	modelCubeShadow2->setPosition(glm::vec3(-25, 7, 0));
	modelCubeShadow2->setTextureID(containerTextureID);


	#pragma endregion

    return true;
}
/**
 * Process the keyboard input
 * There are ways of implementing this function through callbacks provide by
 * the GLFW API, check the GLFW documentation to find more
 * @param{GLFWwindow} window pointer
 * */
void processKeyboardInput(GLFWwindow *window)
{
    // Checks if the escape key is pressed
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        // Tells glfw to close the window as soon as possible
        glfwSetWindowShouldClose(window, true);

    // Checks if the r key is pressed
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        // Reloads the shader
        delete shaderBlinnPhong;
		delete shaderOrenNayar;
		delete shaderCookTorrance;
		delete shaderNormalMapping;
		delete shaderSkyBox;
		delete shaderReflection;
		delete shaderRefraction;
		delete shaderBlending;
		delete shaderDepth;
		delete shaderShadowMap;
		delete debugDepthQuad;

		shaderBlinnPhong = new Shader("assets/shaders/lightningBlingPhong.vert", "assets/shaders/lightningBlingPhong.frag");
		shaderOrenNayar = new Shader("assets/shaders/lightningOrenNayar.vert", "assets/shaders/lightningOrenNayar.frag");
		shaderCookTorrance = new Shader("assets/shaders/lightningCookTorrance.vert", "assets/shaders/lightningCookTorrance.frag");
		shaderNormalMapping = new Shader("assets/shaders/normalMapping.vert", "assets/shaders/normalMapping.frag");
		shaderSkyBox = new Shader("assets/shaders/cubeMap.vert", "assets/shaders/cubeMap.frag");
		shaderReflection = new Shader("assets/shaders/reflection.vert", "assets/shaders/reflection.frag");
		shaderRefraction = new Shader("assets/shaders/refraction.vert", "assets/shaders/refraction.frag");
		shaderBlending = new Shader("assets/shaders/blending.vert", "assets/shaders/blending.frag");
		shaderDepth = new Shader("assets/shaders/shadowMapDepth.vert", "assets/shaders/shadowMapDepth.frag");
		shaderShadowMap = new Shader("assets/shaders/shadowMap.vert", "assets/shaders/shadowMap.frag");
		debugDepthQuad = new Shader("assets/shaders/debugDepth.vert", "assets/shaders/debugDepth.frag");

    }

	// Check is the right click of the mouse is pressed
	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS){
		rightButtonPressed = true;
	}
	else {
		rightButtonPressed = false;
	}


	lastTime = currentTime;
	currentTime = glfwGetTime();


	float deltaTime = currentTime - lastTime;
	
	if (rightButtonPressed) {
		
		// Get mouse position
		double xpos, ypos;
		glfwGetCursorPos(window, &xpos, &ypos);

		// Reset mouse position for next frame
		glfwSetCursorPos(window, windowWidth / 2, windowHeight / 2);



		// Compute new orientation
		horizontalAngle += mouseSpeed /* * 1 */ * float(windowWidth / 2 - xpos);
		verticalAngle += mouseSpeed /* * deltaTime */ * float(windowHeight / 2 - ypos);

		// Direction : Spherical coordinates to Cartesian coordinates conversion
		direction = glm::vec3(
			cos(verticalAngle) * sin(horizontalAngle),
			sin(verticalAngle),
			cos(verticalAngle) * cos(horizontalAngle)
		);

		// Right vector
		glm::vec3 right = glm::vec3(
			sin(horizontalAngle - 3.14f / 2.0f),
			0,
			cos(horizontalAngle - 3.14f / 2.0f)
		);

		up = glm::cross(right, direction);

		// Move forward
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
			position += direction * deltaTime * speed * 4.0f;
		}
		// Move backward
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
			position -= direction * deltaTime * speed * 4.0f;
		}
		// Strafe right
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			position += right * deltaTime * speed * 4.0f;
		}
		// Strafe left
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			position -= right * deltaTime * speed * 4.0f;
		}

	}
}


void RenderModelsMaterial(vector<Model *> materialModels, glm::mat4 view, glm::mat4 projection) {

	if (materialModels.size() <= 0) return;

	Shader *shaderMaterial;
	MaterialType materialType;

	switch (materialModels[0]->getMaterial() )
	{
		case 0:
			shaderMaterial = shaderBlinnPhong;
			materialType = blinnPhong;
			break;

		case 1:
			shaderMaterial = shaderOrenNayar;
			materialType = orenNayar;
			break;

		case 2:
			shaderMaterial = shaderCookTorrance;
			materialType = cookTorrance;
			break;
		case 3:
			shaderMaterial = shaderNormalMapping;
			materialType = blinnPhong;
		default:
			shaderMaterial = shaderBlinnPhong;
			materialType = blinnPhong;
			break;
	}

	shaderMaterial->use();

	shaderMaterial->setMat4("view", view);
	shaderMaterial->setMat4("proj", projection);

	//dirLight
	shaderMaterial->setVec3("dirLight.direction", directionalLight.direction);
	shaderMaterial->setVec3("dirLight.color.ambient", directionalLight.color.ambient);
	shaderMaterial->setVec3("dirLight.color.diffuse", directionalLight.color.diffuse);
	shaderMaterial->setVec3("dirLight.color.specular", directionalLight.color.specular);
	shaderMaterial->setBool("isActiveDirLight", isActiveDirLight);

	//spotLight
	shaderMaterial->setVec3("spotLight.position", position);
	shaderMaterial->setVec3("spotLight.direction", direction);
	shaderMaterial->setVec3("spotLight.color.ambient", spotLight.color.ambient);
	shaderMaterial->setVec3("spotLight.color.diffuse", spotLight.color.diffuse);
	shaderMaterial->setVec3("spotLight.color.specular", spotLight.color.specular);
	shaderMaterial->setFloat("spotLight.cutOff", spotLight.cutOff);
	shaderMaterial->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
	shaderMaterial->setFloat("spotLight.attenuation.constant", spotLight.attenuation.constant);
	shaderMaterial->setFloat("spotLight.attenuation.linear", spotLight.attenuation.linear);
	shaderMaterial->setFloat("spotLight.attenuation.quadratic", spotLight.attenuation.quadratic);
	shaderMaterial->setBool("isActiveSpotLight", isActiveSpotLight);


	//pointLights
	for (int i = 0; i < 2; i++) {

		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".position", pointLights[i].position);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.ambient", pointLights[i].color.ambient);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.diffuse", pointLights[i].color.diffuse);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.specular", pointLights[i].color.specular);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.constant", pointLights[i].attenuation.constant);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.linear", pointLights[i].attenuation.linear);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.quadratic", pointLights[i].attenuation.quadratic);
	}

	shaderMaterial->setBool("isActivePointLight1", isActivePointLight1);
	shaderMaterial->setBool("isActivePointLight2", isActivePointLight2);



	if (materialType == blinnPhong) {
		//BLINN PHONG PARAMETERS
		shaderMaterial->setFloat("shininess", shininess);
	}else if (materialType == orenNayar) {
		//OREN NAYAR PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity);
	}
	else if (materialType == cookTorrance) {
		//COOK TORRANCE PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity);
		shaderMaterial->setFloat("reflectance", reflectance);
	}
	
	//GENERAL PARAMETERS
	shaderMaterial->setVec3("viewPos", position);

	//DRAW THE MODELS
	for (int i = 0; i < materialModels.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getTextureID() );

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderMaterial->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}
}

void RenderNormalMapping(vector<Model *> materialModels, glm::mat4 view, glm::mat4 projection) {

	if (materialModels.size() <= 0) return;

	Shader *shaderMaterial;
	MaterialType materialType;

	shaderMaterial = shaderNormalMapping;
	materialType = blinnPhong;

	shaderMaterial->use();

	shaderMaterial->setMat4("view", view);
	shaderMaterial->setMat4("proj", projection);

	//dirLight
	shaderMaterial->setVec3("dirLight.direction", directionalLight.direction);
	shaderMaterial->setVec3("dirLight.color.ambient", directionalLight.color.ambient);
	shaderMaterial->setVec3("dirLight.color.diffuse", directionalLight.color.diffuse);
	shaderMaterial->setVec3("dirLight.color.specular", directionalLight.color.specular);
	shaderMaterial->setBool("isActiveDirLight", isActiveDirLight);

	/*

	//spotLight

	shaderMaterial->setVec3("spotLight.position", position);
	shaderMaterial->setVec3("spotLight.direction", direction);
	shaderMaterial->setVec3("spotLight.color.ambient", spotLight.color.ambient);
	shaderMaterial->setVec3("spotLight.color.diffuse", spotLight.color.diffuse);
	shaderMaterial->setVec3("spotLight.color.specular", spotLight.color.specular);
	shaderMaterial->setFloat("spotLight.cutOff", spotLight.cutOff);
	shaderMaterial->setFloat("spotLight.outerCutOff", spotLight.outerCutOff);
	shaderMaterial->setFloat("spotLight.attenuation.constant", spotLight.attenuation.constant);
	shaderMaterial->setFloat("spotLight.attenuation.linear", spotLight.attenuation.linear);
	shaderMaterial->setFloat("spotLight.attenuation.quadratic", spotLight.attenuation.quadratic);
	shaderMaterial->setBool("isActiveSpotLight", isActiveSpotLight);


	//pointLights
	for (int i = 0; i < 2; i++) {

		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".position", pointLights[i].position);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.ambient", pointLights[i].color.ambient);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.diffuse", pointLights[i].color.diffuse);
		shaderMaterial->setVec3("pointLights[" + to_string(i) + "]" + ".color.specular", pointLights[i].color.specular);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.constant", pointLights[i].attenuation.constant);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.linear", pointLights[i].attenuation.linear);
		shaderMaterial->setFloat("pointLights[" + to_string(i) + "]" + ".attenuation.quadratic", pointLights[i].attenuation.quadratic);
	}

	shaderMaterial->setBool("isActivePointLight1", isActivePointLight1);
	shaderMaterial->setBool("isActivePointLight2", isActivePointLight2);

	*/

	if (materialType == blinnPhong) {
		//BLINN PHONG PARAMETERS
		shaderMaterial->setFloat("shininess", shininess);
	}
	else if (materialType == orenNayar) {
		//OREN NAYAR PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity);
	}
	else if (materialType == cookTorrance) {
		//COOK TORRANCE PARAMETERS
		shaderMaterial->setFloat("roughness", roughness);
		shaderMaterial->setFloat("intensity", intensity); 
		shaderMaterial->setFloat("reflectance", reflectance);
	}

	shaderMaterial->setBool("isActiveParallaxMapping", isActiveParallaxMapping);

	//GENERAL PARAMETERS
	shaderMaterial->setVec3("viewPos", position);
	shaderMaterial->setVec3("lightPos", pointLights[0].position);

	shaderMaterial->setInt("diffuseMap", 0);
	shaderMaterial->setInt("normalMap", 1);
	shaderMaterial->setInt("depthMap", 2);
	shaderMaterial->setFloat("heightScale", heightScale);

	//DRAW THE MODELS
	for (int i = 0; i < materialModels.size(); i++) {

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getTextureID());
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getNormalMapTextureID());
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getParallaxMapTextureID());

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);
		modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));

		shaderMaterial->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}
}

void RenderSkyBox(Model * model, glm::mat4 view, glm::mat4 projection) {


	glDepthMask(GL_FALSE);
	shaderSkyBox->use();

	glm::mat4 viewSkyBox = glm::mat4(glm::mat3(view));

	shaderSkyBox->setMat4("view", viewSkyBox);
	shaderSkyBox->setMat4("proj", projection);

	glBindVertexArray(modelSkyBox->GetVAO());
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glDepthMask(GL_TRUE);

}

void RenderEnvMappingModel(vector<Model *> materialModels, glm::mat4 view, glm::mat4 projection, TypeEnviromentMapping type) {

	if (materialModels.size() <= 0) return;

	Shader *shaderMaterial;
	MaterialType materialType;

	if (type == reflection) {
		shaderMaterial = shaderReflection;
	}
	else {
		shaderMaterial = shaderRefraction;
	}
	
	materialType = blinnPhong;

	shaderMaterial->use();

	if (type == refraction) shaderMaterial->setFloat("refractiveIndex", refractiveIndex);

	shaderMaterial->setMat4("view", view);
	shaderMaterial->setMat4("proj", projection);
	shaderMaterial->setVec3("cameraPos", position);

	for (int i = 0; i < materialModels.size(); i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderMaterial->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextureID);
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}
}

void RenderTransparentModels(vector<Model *> materialModels, glm::mat4 view, glm::mat4 projection) {


	Shader *shaderMaterial = shaderBlending;
	shaderBlending->use();

	shaderBlending->setMat4("view", view);
	shaderBlending->setMat4("projection", projection);
	//shaderBlending->setInt("texture1", windowTextureID);

	//Sort Windows objects
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < /*windows.size()*/ 3; i++)
	{
		float distance = glm::length(position - windows[i]);
		sorted[distance] = windows[i];
	}

	// windows (from furthest to nearest)

	glBindVertexArray(materialModels[0]->GetVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, windowTextureID);

	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		model = glm::scale(model, glm::vec3(3, 3, 3));
		model = glm::rotate(model, 90.0f, it->second);
		shaderBlending->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
}

//This function is only used for shadow step rendering
void RenderDepthMap(vector<Model *> materialModels) {


	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);

	//RENDER OBJECTS 

	for (int i = 1; i < materialModels.size(); i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		

		modelMatrix = glm::translate(modelMatrix, modelPosition);
		
		/*
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.5f, 0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		*/

		shaderDepth->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}


	//RENDER WINDOWS 

	//Sort Windows objects
	std::map<float, glm::vec3> sorted;
	for (unsigned int i = 0; i < /*windows.size()*/ 3; i++)
	{
		float distance = glm::length(position - windows[i]);
		sorted[distance] = windows[i];
	}

	// windows (from furthest to nearest)

	glBindVertexArray(modelsTransparent[0]->GetVAO());
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, windowTextureID);

	for (std::map<float, glm::vec3>::reverse_iterator it = sorted.rbegin(); it != sorted.rend(); ++it)
	{
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, it->second);
		model = glm::scale(model, glm::vec3(3, 3, 3));
		model = glm::rotate(model, 90.0f, it->second);
		shaderDepth->setMat4("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	//RENDER HOUSE
	for (int i = 0; i < modelsBlinnPhong.size(); i++) {

		glBindTexture(GL_TEXTURE_2D, modelsBlinnPhong[i]->getTextureID());

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = modelsBlinnPhong[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderDepth->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsBlinnPhong[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsBlinnPhong[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}


	//RENDER POINT LIGHT (not the one used to make shadow)
	glm::mat4 modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, pointLights[0].position);

	shaderDepth->setMat4("model", modelMatrix);

	glBindVertexArray(lightSources[0]->GetVAO());
	glDrawArrays(GL_TRIANGLES, 0, lightSources[0]->GetNumTriangles() * 3);
	glBindVertexArray(0);


	//RENDER REFLECTION AND REFRACTION
	for (int i = 0; i < modelsReflection.size(); i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = modelsReflection[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderDepth->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsReflection[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsReflection[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}


	for (int i = 0; i < modelsRefraction.size(); i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = modelsRefraction[i]->getPosition();
		modelMatrix = glm::translate(modelMatrix, modelPosition);

		shaderDepth->setMat4("model", modelMatrix);

		// Binds the vertex array to be drawn
		glBindVertexArray(modelsRefraction[i]->GetVAO());
		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, modelsRefraction[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void RenderShadowMap(vector<Model *> materialModels, glm::mat4 view, glm::mat4 projection) {

	shaderShadowMap->setMat4("view", view);
	shaderShadowMap->setMat4("projection", projection);

	shaderShadowMap->setInt("shadowMap", 0);
	shaderShadowMap->setInt("diffuseTexture", 1);

	//MAKING shaderShadowMap->setInt("shadowMap", textureID) IS WRONG!

	//dirLight
	shaderShadowMap->setVec3("dirLight.direction", directionalLight.direction);
	shaderShadowMap->setVec3("dirLight.color.ambient", directionalLight.color.ambient);
	shaderShadowMap->setVec3("dirLight.color.diffuse", directionalLight.color.diffuse);
	shaderShadowMap->setVec3("dirLight.color.specular", directionalLight.color.specular);
	shaderShadowMap->setFloat("shininess", shininess);


	for (int i = 0; i < materialModels.size(); i++) {

		glm::mat4 modelMatrix = glm::mat4(1.0f);
		glm::vec3 modelPosition = materialModels[i]->getPosition();
		
		modelMatrix = glm::translate(modelMatrix, modelPosition);
		
		/*
		modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, 1.5f, 0.0));
		modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f));
		*/
		

		shaderShadowMap->setMat4("model", modelMatrix);

	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, materialModels[i]->getTextureID());


		// Binds the vertex array to be drawn
		glBindVertexArray(materialModels[i]->GetVAO());

		// Renders the triangle gemotry
		glDrawArrays(GL_TRIANGLES, 0, materialModels[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}


}

unsigned int quadVAO = 0;
unsigned int quadVBO;

void renderQuad() {

	// renderQuad() renders a 1x1 XY quad in NDC
	// -----------------------------------------
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}

	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	
}

/**
 * Render Function
 * */
void render()
{

	// 1. render depth of scene to texture (from light's perspective)
	// --------------------------------------------------------------
	shaderDepth->use();
	//PREPARE LIGHT DEPTH MAP
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	glm::mat4 lightProjection, lightView;
	glm::mat4 lightSpaceMatrix;
	float near_plane = 1.0f, far_plane = 100.0f;
	//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
	lightProjection = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, near_plane, far_plane);

	float distanceLight = -30.0f;
	glm::vec3 lightPos = directionalLight.direction * distanceLight;

	lightView = glm::lookAt(lightPos, glm::vec3(0,0,0), glm::vec3(0,1,0) );

	lightSpaceMatrix = lightProjection * lightView;
	// render scene from light's point of view
	shaderDepth->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	RenderDepthMap(modelsShadow);

	
	// 2. render scene as normal using the generated depth/shadow map  
	// --------------------------------------------------------------
	glViewport(0, 0, windowWidth, windowHeight);
    // Clears the color and depth buffers from the frame buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)windowWidth / (float)windowHeight, .5f, 1000.0f);
	//glm::mat4 projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

	glm::mat4 view = glm::lookAt(
		position, // Camera is at (4,3,3), in world space
		position + direction, // and looks at the origin
		up  // Head is up (set to 0,-1,0 to look upside-down) 
	);


	glm::mat4 modelMatrix = glm::mat4(1.0f); //model matrix: an identity matrix (model will be at the origin)



	//Rendering the skybox
	RenderSkyBox(modelSkyBox,view, projection);

	//Render models 
	RenderModelsMaterial(modelsBlinnPhong, view, projection);
	//RenderModelsMaterial(modelsOrenNayar, view, projection);
	//RenderModelsMaterial(modelsCookTorrance, view, projection);

	//Render Reflection Models
	RenderEnvMappingModel(modelsReflection, view, projection, reflection);
	//Render Refraction Models
	RenderEnvMappingModel(modelsRefraction, view, projection, refraction);
	
	//Render models with normal mapping and parallax mapping
	RenderNormalMapping(modelsNormalMapping, view, projection);

	//Render objects affected by shadow

	shaderShadowMap->use();

	shaderShadowMap->setVec3("viewPos", position);
	shaderShadowMap->setVec3("lightPos", pointLights[1].position);
	shaderShadowMap->setMat4("lightSpaceMatrix", lightSpaceMatrix);

	RenderShadowMap(modelsShadow, view, projection);

	
	//DRAW THE LIGHTNINGS
	shaderLights->use();
	for (int i = 0; i < 2; i++) {

		modelMatrix = glm::mat4(1.0f);
		modelMatrix = glm::translate(modelMatrix, pointLights[i].position);
		glm::mat4 mvp = projection * view * modelMatrix; //Remember, matrix multimplication is the other way around

		shaderLights->setMat4("MVP", mvp);
		shaderLights->setVec3("colorIn", glm::vec3(0, 0, 1));

		glBindVertexArray(lightSources[i]->GetVAO());
		glDrawArrays(GL_TRIANGLES, 0, lightSources[i]->GetNumTriangles() * 3);
		glBindVertexArray(0);
	}

	//Render Transparent objects
	RenderTransparentModels(modelsTransparent, view, projection);

	// render Depth map to quad for visual debugging
	// ---------------------------------------------
	debugDepthQuad->use();
	debugDepthQuad->setFloat("near_plane", 0.5f);
	debugDepthQuad->setFloat("far_plane", 1000.0f);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depthMap);

	//renderQuad();

	TwDraw();
	// Swap the buffer
	glfwSwapBuffers(window);

}
/**
* App main loop
* */
void update()
{
    // Loop until something tells the window, that it has to be closed
    while (!glfwWindowShouldClose(window))
    {
        // Checks for keyboard inputs
        processKeyboardInput(window);

        // Renders everything
        render();

		//Get the data from the user interface
		UpdateUserInterface();
		
        // Check and call events
        glfwPollEvents();
    }
}
/**
 * App starting point
 * @param{int} number of arguments
 * @param{char const *[]} running arguments
 * @returns{int} app exit code
 * */
int main(int argc, char const *argv[])
{
	// Initialize all the app components
    if (!init())
    {
        // Something went wrong
        std::cin.ignore();
        return -1;
    }

    std::cout << "=====================================================" << std::endl
              << "        Press Escape to close the program            " << std::endl
              << "=====================================================" << std::endl;

    // Starts the app main loop
    update();

    // Deletes the texture from the gpu
    glDeleteTextures(1, &houseTextureID);
	glDeleteTextures(1, &planeTextureID);


	//FREE SPACE FROM GPU

#pragma region FreeSpaceModels

	for (auto x : modelsBlinnPhong) {

		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}

	for (auto x : modelsOrenNayar) {
		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}


	for (auto x : modelsCookTorrance) {

		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}

	for (auto x : modelsNormalMapping){
		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}

	for (auto x : modelsReflection) {
		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}

	for (auto x : modelsRefraction) {
		unsigned int VBO = x->GetVBO();
		unsigned int VAO = x->GetVAO();

		GLuint normalBuffer = x->GetNormalBuffer();
		GLuint uvBuffer = x->GetUvBuffer();

		// Deletes the vertex array from the GPU
		glDeleteVertexArrays(1, &VAO);
		// Deletes the vertex object from the GPU
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &normalBuffer);
		glDeleteBuffers(1, &uvBuffer);
	}
#pragma endregion

	


    // Destroy the shader
    delete shaderBlinnPhong;
	delete shaderOrenNayar;
	delete shaderCookTorrance;
	delete shaderNormalMapping;
	// Destroy the shader lights
	delete shaderLights;

    // Stops the glfw program
    glfwTerminate();

    return 0;
}
