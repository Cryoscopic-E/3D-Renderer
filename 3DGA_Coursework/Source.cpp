// 3D Graphics and Animation - Main Template
// Visual Studio 2019
// Last Changed 01/10/2019

#pragma comment(linker, "/NODEFAULTLIB:MSVCRT")
#define GLM_ENABLE_EXPERIMENTAL
//#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <map>
using namespace std;

#include <GL/glew.h>			// Add library to extend OpenGL to newest version
#include <GLFW/glfw3.h>			// Add library to launch a window
#include <GLM/glm.hpp>			// Add helper maths library
#include <GLM/gtx/transform.hpp>

//#include <stb_image.h>			// Add library to load images for textures

//#include "Mesh.h"				// Simplest mesh holder and OBJ loader - can update more - from https://github.com/BennyQBD/ModernOpenGLTutorial
#include "Camera.h"
#include "Model.h"
#include "Texture.h"
#include "Light.h"

// MAIN FUNCTIONS
void setupRender();
void startup();
void update(GLfloat currentTime);
void render(GLfloat currentTime);
void endProgram();

// HELPER FUNCTIONS OPENGL
void hintsGLFW();
void errorCallbackGLFW(int error, const char* description);
void debugGL();
static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam);

// CALLBACK FUNCTIONS FOR WINDOW
void onResizeCallback(GLFWwindow* window, int w, int h);
void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
void onMouseMoveCallback(GLFWwindow* window, double x, double y);
void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset);
void onIconifyCallback(GLFWwindow* window, int iconified);

// VARIABLES
GLFWwindow*		window;											// Keep track of the window
int				windowWidth = 640;				
int				windowHeight = 480;
bool			running = true;									// Are we still running?
bool			pause = false;
bool			keyStatus[1024];
GLfloat			deltaTime = 0.0f;
GLfloat			lastTime = 0.0f;

Object* root;
Camera mainCamera(windowWidth, windowHeight, 45.0f, 0.1f, 1000.0f);


/*DICTIONARIES*/
std::map<string, Mesh*> meshesDictionary;
std::map<string, Texture*> texturesdictionary;
std::map<string, Shader*> shaderesDictionary;
std::map<string, Material*> materialsDictionary;
std::map<string, Model*> modelsDictionary;

DirectionalLight* dirLight;
std::vector<PointLight*> pointLights;
std::vector<SpotLight*> spotLigthts;

int main()
{
	if (!glfwInit()) {							// Checking for GLFW
		cout << "Could not initialise GLFW...";
		return 0;
	}

	glfwSetErrorCallback(errorCallbackGLFW);	// Setup a function to catch and display all GLFW errors.

	hintsGLFW();								// Setup glfw with various hints.		

												// Start a window using GLFW
	string title = "My OpenGL Application";

	// Fullscreen
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
	// windowWidth = mode->width; windowHeight = mode->height; //fullscreen
	// window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), glfwGetPrimaryMonitor(), NULL); // fullscreen

	// Window
	window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), NULL, NULL);
	if (!window) {								// Window or OpenGL context creation failed
		cout << "Could not initialise GLFW...";
		endProgram();
		return 0;
	}

	glfwMakeContextCurrent(window);				// making the OpenGL context current

												// Start GLEW (note: always initialise GLEW after creating your window context.)
	glewExperimental = GL_TRUE;					// hack: catching them all - forcing newest debug callback (glDebugMessageCallback)
	GLenum errGLEW = glewInit();
	if (GLEW_OK != errGLEW) {					// Problems starting GLEW?
		cout << "Could not initialise GLEW...";
		endProgram();
		return 0;
	}

	debugGL();									// Setup callback to catch openGL errors.	

												// Setup all the message loop callbacks.
	glfwSetWindowIconifyCallback(window, onIconifyCallback);
	glfwSetWindowSizeCallback(window, onResizeCallback);		// Set callback for resize
	glfwSetKeyCallback(window, onKeyCallback);					// Set Callback for keys
	glfwSetMouseButtonCallback(window, onMouseButtonCallback);	// Set callback for mouse click
	glfwSetCursorPosCallback(window, onMouseMoveCallback);		// Set callback for mouse move
	glfwSetScrollCallback(window, onMouseWheelCallback);		// Set callback for mouse wheel.
																//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);	// Set mouse cursor. Fullscreen
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);	// Set mouse cursor FPS fullscreen.

	setupRender();								// setup some render variables.
	startup();									// Setup all necessary information for startup (aka. load texture, shaders, models, etc).

	do {										// run until the window is closed
		GLfloat currentTime = (GLfloat)glfwGetTime();		// retrieve timelapse
		deltaTime = currentTime - lastTime;		// Calculate delta time
		lastTime = currentTime;					// Save for next frame calculations.
		glfwPollEvents();						// poll callbacks
		if (!pause)
		{
			update(deltaTime);					// update (physics, animation, structures, etc)
			render(deltaTime);					// call render function.
			glfwSwapBuffers(window);				// swap buffers (avoid flickering and tearing)

			running &= (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_RELEASE);	// exit if escape key pressed
			running &= (glfwWindowShouldClose(window) != GL_TRUE);
		}
	} while (running);

	endProgram();			// Close and clean everything up...

	cout << "\nPress any key to continue...\n";
	cin.ignore(); cin.get(); // delay closing console to read debugging errors.

	return 0;
}

void errorCallbackGLFW(int error, const char* description) {
	cout << "Error GLFW: " << description << "\n";
}

void hintsGLFW() {
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);			// Create context in debug mode - for debug message callback
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
}

void endProgram() {

	for (std::map<std::string, Mesh*>::iterator iter = meshesDictionary.begin(); iter != meshesDictionary.end(); iter++)
	{
		delete iter->second;
	}
	for (std::map<std::string, Texture*>::iterator iter = texturesdictionary.begin(); iter != texturesdictionary.end(); iter++)
	{
		delete iter->second;
	}
	for (std::map<std::string, Shader*>::iterator iter = shaderesDictionary.begin(); iter != shaderesDictionary.end(); iter++)
	{
		delete iter->second;
	}
	for (std::map<std::string, Material*>::iterator iter = materialsDictionary.begin(); iter != materialsDictionary.end(); iter++)
	{
		delete iter->second;
	}
	for (std::map<std::string, Model*>::iterator iter = modelsDictionary.begin(); iter != modelsDictionary.end(); iter++)
	{
		delete iter->second;
	}
	delete root;

	delete dirLight;

	for (int i = 0; i < pointLights.size(); i++)
	{
		delete pointLights[i];
	}

	for (int i = 0; i < spotLigthts.size(); i++)
	{
		delete spotLigthts[i];
	}
	glfwMakeContextCurrent(window);		// destroys window handler
	glfwTerminate();	// destroys all windows and releases resources.
}

void setupRender() {
	glfwSwapInterval(1);	// Ony render when synced (V SYNC)

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 2);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
}

void startup() {
	//	0. CREATE LIGHTS
	dirLight = new DirectionalLight(glm::vec3(-0.2f, -1.0f, -0.3f), glm::vec3(0.05f, 0.05f, 0.05f), glm::vec3(0.2f, 0.2f, 0.2f), glm::vec3(0.6f, 0.6f, 0.6f));
	
	pointLights.push_back(new PointLight(glm::vec3(-0.5f, 1.0f, 0.3f), glm::vec3(1.0f, 0.6f, 0.0f), glm::vec3(1.0f, 0.6f, 0.0f), glm::vec3(1.0f, 0.6f, 0.0f),1.0f,0.09f,0.032f));
	pointLights.push_back(new PointLight(glm::vec3(-1.0f, 1.0f, 0.3f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f));

	spotLigthts.push_back(new SpotLight(glm::vec3(0.0f, 2.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f),1.0f,0.09f,0.032f,glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	spotLigthts.push_back(new SpotLight(glm::vec3(0.0f, 2.0f, 1.5f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	spotLigthts.push_back(new SpotLight(glm::vec3(0.0f, 2.0f, 2.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	//	1. LOAD MESHES
	Mesh *mesh = new Mesh();
	mesh->LoadModel("./Models/road.obj");
	meshesDictionary.insert(std::pair<string,Mesh*>("road", mesh));

	//	2. LOAD TEXTURES
	TextureOptions::TextureSettings settings(TextureOptions::FilterType::LINEAR, TextureOptions::WrappingType::CLAMP, 0, 0);
	Texture* diffuse = new Texture("./Models/road.png", settings);
	texturesdictionary.insert(std::pair<string, Texture*>("road", diffuse));

	//	3. CREATE SHADERS
	Shader* shader = new Shader("vs_model.vs", "fs_model.fs");
	shaderesDictionary.insert(std::pair<string, Shader*>("default", shader));

	//	4. CREATE MATERIAL
	Material *mat = new Material(
		323.0f,
		diffuse,
		diffuse,
		shader);

	mat->SetDirectionalLight(dirLight);
	mat->SetPointLights(pointLights);
	mat->SetSpotLights(spotLigthts);

	materialsDictionary.insert(std::pair<string, Material*>("roadMaterial", mat));

	// 4. CREATE MODELS
	Model* road = new Model(new Renderer(mesh, mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("roadModel", road));
	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//	5. INIT OBJECTS PARAMETERS
	mainCamera.transform->SetPosition(glm::vec3(0.0f, 3.0f, 5.0f));
	mainCamera.transform->Rotate(Transform::UP, -90.0);
	mainCamera.transform->Rotate(Transform::LEFT, -45.0);
	modelsDictionary["roadModel"]->transform->SetScale(glm::vec3(0.5f, 0.5f, 0.5f));
}

void update(GLfloat delta) {
	if (keyStatus[GLFW_KEY_LEFT])			modelsDictionary["roadModel"]->transform->Rotate(Transform::UP,-1.0f);
	if (keyStatus[GLFW_KEY_RIGHT])			modelsDictionary["roadModel"]->transform->Rotate(Transform::UP,+1.0f);
	if (keyStatus[GLFW_KEY_UP])				modelsDictionary["roadModel"]->transform->Translate(modelsDictionary["roadModel"]->transform->GetFront() * 5.0f * deltaTime);
	

	//CAMERA /TODO INPUT SYSTEM
	if (keyStatus[GLFW_KEY_W])	mainCamera.transform->Translate(mainCamera.transform->GetFront() * 5.0f * deltaTime);
	if (keyStatus[GLFW_KEY_S])	mainCamera.transform->Translate(mainCamera.transform->GetFront() * -5.0f * deltaTime);
	if (keyStatus[GLFW_KEY_A])	mainCamera.transform->Translate(mainCamera.transform->GetRight() * -5.0f * deltaTime);
	if (keyStatus[GLFW_KEY_D])	mainCamera.transform->Translate(mainCamera.transform->GetRight() * 5.0f * deltaTime);
}

void render(GLfloat delta) {

	glViewport(0, 0, windowWidth, windowHeight);
	mainCamera.transform->CalculateModelMatrix();
	mainCamera.transform->UpdateLocalVectors();
	// Clear colour buffer
	glm::vec4 backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f); 
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; glClearBufferfv(GL_DEPTH, 0, &one);

	// Enable blend
	glEnable(GL_BLEND); 
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	/*DRAW ALL MODELS*/
	for (std::map<std::string, Model*>::iterator iter = modelsDictionary.begin(); iter != modelsDictionary.end(); iter++)
	{
		iter->second->Render();
	}
}

void onIconifyCallback(GLFWwindow* window, int iconified)
{
	if (iconified)
	{
		pause = true;
		glfwIconifyWindow(window);
	}
	else
	{
		pause = false;
		glfwRestoreWindow(window);
	}
}

void onResizeCallback(GLFWwindow* window, int w, int h) {
	if (w > 0 && h > 0)
	{
		windowWidth = w;
		windowHeight = h;
		mainCamera.UpdateView(w, h);
	}
}

void onKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_PRESS) keyStatus[key] = true;
	else if (action == GLFW_RELEASE) keyStatus[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

void onMouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

}

void onMouseMoveCallback(GLFWwindow* window, double x, double y) {
	int mouseX = static_cast<int>(x);
	int mouseY = static_cast<int>(y);
}

static void onMouseWheelCallback(GLFWwindow* window, double xoffset, double yoffset) {
	int yoffsetInt = static_cast<int>(yoffset);
}

void debugGL() {
	//Output some debugging information
	cout << "VENDOR: " << (char *)glGetString(GL_VENDOR) << endl;
	cout << "VERSION: " << (char *)glGetString(GL_VERSION) << endl;
	cout << "RENDERER: " << (char *)glGetString(GL_RENDERER) << endl;

	// Enable Opengl Debug
	//glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback((GLDEBUGPROC)openGLDebugCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, NULL, true);
}

static void APIENTRY openGLDebugCallback(GLenum source,
	GLenum type,
	GLuint id,
	GLenum severity,
	GLsizei length,
	const GLchar* message,
	const GLvoid* userParam) {

	cout << "---------------------opengl-callback------------" << endl;
	cout << "Message: " << message << endl;
	cout << "type: ";
	switch (type) {
	case GL_DEBUG_TYPE_ERROR:
		cout << "ERROR";
		break;
	case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
		cout << "DEPRECATED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
		cout << "UNDEFINED_BEHAVIOR";
		break;
	case GL_DEBUG_TYPE_PORTABILITY:
		cout << "PORTABILITY";
		break;
	case GL_DEBUG_TYPE_PERFORMANCE:
		cout << "PERFORMANCE";
		break;
	case GL_DEBUG_TYPE_OTHER:
		cout << "OTHER";
		break;
	}
	cout << " --- ";

	cout << "id: " << id << " --- ";
	cout << "severity: ";
	switch (severity) {
	case GL_DEBUG_SEVERITY_LOW:
		cout << "LOW";
		break;
	case GL_DEBUG_SEVERITY_MEDIUM:
		cout << "MEDIUM";
		break;
	case GL_DEBUG_SEVERITY_HIGH:
		cout << "HIGH";
		break;
	case GL_DEBUG_SEVERITY_NOTIFICATION:
		cout << "NOTIFICATION";
	}
	cout << endl;
	cout << "-----------------------------------------" << endl;
}
