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

bool camControlPressed = false;
bool camControl = false;
int lastmousex = 320;
int lastmousey = 240;

GLfloat			deltaTime = 0.0f;
GLfloat			lastTime = 0.0f;

Object* root;
Camera mainCamera(windowWidth, windowHeight, 45.0f, 0.1f, 1000.0f);


// frambuffer var
unsigned int qVAO, qVBO, framebuffer, textureColBuffer, renderBuffer;

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
	dirLight = new DirectionalLight(glm::vec3(-.2f, -.4f, -0.3f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.1f, 0.1f, 0.1f));
	
	pointLights.push_back(new PointLight(glm::vec3(-13.5f, 1.0f, -15.3f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0f, 0.6f, 0.0f), glm::vec3(1.0f, 0.6f, 0.0f),1.0f,0.09f,0.032f));
	pointLights.push_back(new PointLight(glm::vec3(-13.0f, 1.0f, 1.3f), glm::vec3(0.3f, 0.3f, 0.3f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 0.0f, 0.0f), 1.0f, 0.09f, 0.032f));

	spotLigthts.push_back(new SpotLight(glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f),1.0f,0.09f,0.032f,glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	spotLigthts.push_back(new SpotLight(glm::vec3(0.0f, 1.0f, 20.7f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	spotLigthts.push_back(new SpotLight(glm::vec3(0.0f, 1.0f, 0.8f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.8f, 0.8f, 0.8f), glm::vec3(0.8f, 0.8f, 0.8f), 1.0f, 0.09f, 0.032f, glm::cos(glm::radians(14.0f)), glm::cos(glm::radians(11.0f))));
	//	1. LOAD MESHES
	Mesh *roadMesh = new Mesh();
	roadMesh->LoadModel("./Models/road.obj");
	meshesDictionary.insert(std::pair<string,Mesh*>("road", roadMesh));

	Mesh* carMesh = new Mesh();
	carMesh->LoadModel("./Models/car.obj");
	meshesDictionary.insert(std::pair<string, Mesh*>("car", carMesh));

	Mesh* poleMesh = new Mesh();
	poleMesh->LoadModel("./Models/lamp.obj");
	meshesDictionary.insert(std::pair<string, Mesh*>("pole", poleMesh));

	Mesh* buildingMesh = new Mesh();
	buildingMesh->LoadModel("./Models/building.obj");
	meshesDictionary.insert(std::pair<string, Mesh*>("building", buildingMesh));

	Mesh* pinetreeMesh = new Mesh();
	pinetreeMesh->LoadModel("./Models/tree2.obj");
	meshesDictionary.insert(std::pair<string, Mesh*>("building", pinetreeMesh));

	Mesh* smalltreeMesh = new Mesh();
	smalltreeMesh->LoadModel("./Models/tree1.obj");
	meshesDictionary.insert(std::pair<string, Mesh*>("building", smalltreeMesh));

	//	2. LOAD TEXTURES
	TextureOptions::TextureSettings settings(TextureOptions::FilterType::LINEAR, TextureOptions::WrappingType::CLAMP, 0, 0);
	Texture* diffuse = new Texture("./Models/road.png", settings);
	Texture* cardiffuse = new Texture("./Models/car.png", settings);
	Texture* polediffuse = new Texture("./Models/Lamp.png", settings);
	Texture* buildingdiffuse = new Texture("./Models/building.png", settings);
	Texture* tree1diffuse = new Texture("./Models/Tree_1.png", settings);
	Texture* tree2diffuse = new Texture("./Models/Tree_2.png", settings);

	texturesdictionary.insert(std::pair<string, Texture*>("road", diffuse));
	texturesdictionary.insert(std::pair<string, Texture*>("car", cardiffuse));
	texturesdictionary.insert(std::pair<string, Texture*>("pole", polediffuse));
	texturesdictionary.insert(std::pair<string, Texture*>("building", buildingdiffuse));
	texturesdictionary.insert(std::pair<string, Texture*>("tree1", tree1diffuse));
	texturesdictionary.insert(std::pair<string, Texture*>("tree2", tree2diffuse));

	//	3. CREATE SHADERS
	Shader* shader = new Shader("vs_model.vs", "fs_model.fs");
	shaderesDictionary.insert(std::pair<string, Shader*>("default", shader));

	//	4. CREATE MATERIAL
	Material *mat = new Material(
		10.0f,
		diffuse,
		diffuse,
		shader);

	Material* carMat = new Material(
		400.0f,
		cardiffuse,
		cardiffuse,
		shader);

	Material* poleMat = new Material(
		200.0f,
		polediffuse,
		polediffuse,
		shader);
	
	Material* buildingMat = new Material(
		50.0f,
		buildingdiffuse,
		buildingdiffuse,
		shader);

	Material* tree1Mat = new Material(
		50.0f,
		tree1diffuse,
		tree1diffuse,
		shader);

	Material* tree2Mat = new Material(
		50.0f,
		tree2diffuse,
		tree2diffuse,
		shader);

	materialsDictionary.insert(std::pair<string, Material*>("roadMaterial", mat));
	materialsDictionary.insert(std::pair<string, Material*>("carMaterial", carMat));
	materialsDictionary.insert(std::pair<string, Material*>("poleMat", poleMat));
	materialsDictionary.insert(std::pair<string, Material*>("buildingMat", buildingMat));
	materialsDictionary.insert(std::pair<string, Material*>("tree1Mat", tree1Mat));
	materialsDictionary.insert(std::pair<string, Material*>("tree2Mat", tree2Mat));

	for (std::map<std::string, Material*>::iterator iter = materialsDictionary.begin(); iter != materialsDictionary.end(); iter++)
	{
		iter->second->SetDirectionalLight(dirLight);
		iter->second->SetPointLights(pointLights);
		iter->second->SetSpotLights(spotLigthts);
	}


	// 4. CREATE MODELS
	Model* road = new Model(new Renderer(roadMesh, mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("roadModel", road));
	Model* carModel = new Model(new Renderer(carMesh, carMat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("carModel", carModel));
	Model* poleModel = new Model(new Renderer(poleMesh, poleMat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("poleModel", poleModel));
	Model* poleModel2 = new Model(new Renderer(poleMesh, poleMat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("poleModel2", poleModel2));
	Model* poleModel3 = new Model(new Renderer(poleMesh, poleMat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("poleModel3", poleModel3));
	Model* buildingModel = new Model(new Renderer(buildingMesh, buildingMat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("buildingModel", buildingModel));
	Model* pineModel1 = new Model(new Renderer(pinetreeMesh, tree2Mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("pm1", pineModel1));
	Model* pineModel2 = new Model(new Renderer(pinetreeMesh, tree2Mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("pm2", pineModel2));
	Model* smalltreeModel1 = new Model(new Renderer(smalltreeMesh, tree1Mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("st1", smalltreeModel1));
	Model* smalltreeModel2 = new Model(new Renderer(smalltreeMesh, tree1Mat), &mainCamera);
	modelsDictionary.insert(std::pair<string, Model*>("st2", smalltreeModel2));
	// A few optimizations.
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	//	5. INIT OBJECTS PARAMETERS
	mainCamera.transform->SetPosition(glm::vec3(0.0f, 25.0f, -10.0f));
	mainCamera.transform->Rotate(Transform::UP, -90.0);
	mainCamera.transform->Rotate(Transform::LEFT, -45.0);
	modelsDictionary["roadModel"]->transform->SetScale(glm::vec3(3.0f, 1.0f, 3.0f));
	modelsDictionary["carModel"]->transform->SetScale(glm::vec3(0.6f, 0.6f, 0.6f));
	modelsDictionary["carModel"]->transform->SetPosition(glm::vec3(20.0f, 0.0f, 0.0f));

	modelsDictionary["poleModel"]->transform->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
	modelsDictionary["poleModel"]->transform->SetPosition(glm::vec3(8.5f, 0.0f, 0.0f));
	modelsDictionary["poleModel"]->transform->Rotate(Transform::UP,-90);

	modelsDictionary["poleModel2"]->transform->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
	modelsDictionary["poleModel2"]->transform->SetPosition(glm::vec3(8.5f, 0.0f, 15.0f));
	modelsDictionary["poleModel2"]->transform->Rotate(Transform::UP, -90);

	modelsDictionary["poleModel3"]->transform->SetScale(glm::vec3(5.0f, 5.0f, 5.0f));
	modelsDictionary["poleModel3"]->transform->SetPosition(glm::vec3(8.5f, 0.0f, -15.0f));
	modelsDictionary["poleModel3"]->transform->Rotate(Transform::UP, -90);


	modelsDictionary["buildingModel"]->transform->Scale(glm::vec3(3.0f, 3.0, 3.0f));
	modelsDictionary["buildingModel"]->transform->Translate(glm::vec3(0.0f,0.0, -3.0f));

	modelsDictionary["pm1"]->transform->Scale(glm::vec3(2.0f, 2.0, 2.0f));
	modelsDictionary["pm1"]->transform->Translate(glm::vec3(-13.0f, 0.0, -14.0f));

	modelsDictionary["pm2"]->transform->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	modelsDictionary["pm2"]->transform->Translate(glm::vec3(-10.0f, 0.0, -20.0f));

	modelsDictionary["st1"]->transform->Scale(glm::vec3(2.0f, 2.0f, 2.0f));
	modelsDictionary["st1"]->transform->Translate(glm::vec3(-12.0f, 0.0, -22.0f));

	modelsDictionary["st2"]->transform->Scale(glm::vec3(2.0f, 2.0, 2.0f));
	modelsDictionary["st2"]->transform->Translate(glm::vec3(-9.0f, 0.0, -16.0f));
}

void update(GLfloat delta) {
	if (keyStatus[GLFW_KEY_LEFT])			modelsDictionary["carModel"]->transform->Rotate(modelsDictionary["carModel"]->transform->GetUp(),+1.0f);
	if (keyStatus[GLFW_KEY_RIGHT])			modelsDictionary["carModel"]->transform->Rotate(modelsDictionary["carModel"]->transform->GetUp(),-1.0f);
	if (keyStatus[GLFW_KEY_UP])				modelsDictionary["carModel"]->transform->Translate(modelsDictionary["carModel"]->transform->GetFront() * 5.0f * deltaTime);
	if (keyStatus[GLFW_KEY_DOWN])			modelsDictionary["carModel"]->transform->Translate(modelsDictionary["carModel"]->transform->GetFront() * -5.0f * deltaTime);
	

	//CAMERA
	if (keyStatus[GLFW_KEY_C] && !camControlPressed) {
		camControlPressed = true;
	}
	if (camControlPressed && !keyStatus[GLFW_KEY_C])
	{
		camControlPressed = false;
		if (camControl) 
			camControl = false;
		
		else 
			camControl = true;
	}
	if (camControl) {
		if (keyStatus[GLFW_KEY_W])	mainCamera.transform->Translate(mainCamera.transform->GetFront() * 10.0f * deltaTime);
		if (keyStatus[GLFW_KEY_S])	mainCamera.transform->Translate(mainCamera.transform->GetFront() * -10.0f * deltaTime);
		if (keyStatus[GLFW_KEY_A])	mainCamera.transform->Translate(mainCamera.transform->GetRight() * -10.0f * deltaTime);
		if (keyStatus[GLFW_KEY_D])	mainCamera.transform->Translate(mainCamera.transform->GetRight() * 10.0f * deltaTime);
		if (keyStatus[GLFW_KEY_Q])	mainCamera.transform->Rotate(Transform::UP, -1.0f);
		if (keyStatus[GLFW_KEY_E])	mainCamera.transform->Rotate(Transform::UP, 1.0f);
	}
	}
	


void render(GLfloat delta) {
	
	glViewport(0, 0, windowWidth, windowHeight);

	mainCamera.transform->CalculateModelMatrix();
	mainCamera.transform->UpdateLocalVectors();
	// Clear colour buffer
	glm::vec4 backgroundColor = glm::vec4(0.2f, 0.2f, 0.2f, 1.0f);
	glClearBufferfv(GL_COLOR, 0, &backgroundColor[0]);

	// Clear deep buffer
	static const GLfloat one = 1.0f; 
	glClearBufferfv(GL_DEPTH, 0, &one);

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

	int xOff = mouseX - lastmousex;
	int yOff = mouseY - lastmousey;
	lastmousex = mouseX;
	lastmousey = mouseY;

	if (camControl)
	{
		mainCamera.transform->Rotate(Transform::UP, (float)xOff);
		mainCamera.transform->Rotate(Transform::LEFT, (float)yOff);
	}
	

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
