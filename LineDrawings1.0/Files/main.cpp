#define _CRT_SECURE_NO_DEPRECATE
#define _SCL_SECURE_NO_WARNINGS

//Some Windows Headers (For Time, IO, etc.)
#include <windows.h>
#include <mmsystem.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>


// Assimp includes

#include <assimp/cimport.h> // C importer
#include <assimp/scene.h> // collects data
#include <assimp/postprocess.h> // various extra operations
#include <stdio.h>
#include <math.h>
#include <vector> // STL dynamic memory.
#include <stdlib.h>
#include "assert.h"

// Header includes

#include "Mesh.h"
#include "Camera.h"
#include "RigidBody.h"
#include "RigidBodyManager.h"
#include "Shaders.h"
#include "ShaderManager.h"
#include "FBO.h"

#include "AntTweakBar.h"

using namespace std;

int width = 1200;
int height = 900;


#pragma region OLD SHADERS

#define V_SHADER_PHONG "../Shaders/phongVertexShader.txt"
#define F_SHADER_PHONG "../Shaders/phongFragmentShader.txt"
#define V_SHADER_BLINNPHONG "../Shaders/blinnPhongVertexShader.txt"
#define F_SHADER_BLINNPHONG "../Shaders/blinnPhongFragmentShader.txt"
#define V_SHADER_GOURAD "../Shaders/gouradVertexShader.txt"
#define F_SHADER_GOURAD "../Shaders/gouradFragmentShader.txt"
#define V_SHADER_TOON "../Shaders/toonVertexShader.txt"
#define F_SHADER_TOON "../Shaders/toonFragmentShader.txt"
#define V_SHADER_GOOCH "../Shaders/goochVertexShader.txt"
#define F_SHADER_GOOCH "../Shaders/goochFragmentShader.txt"
#define V_SHADER_COOKTORRANCE "../Shaders/cookTorranceVertexShader.txt"
#define F_SHADER_COOKTORRANCE "../Shaders/cookTorranceFragmentShader.txt"
#define V_SHADER_BECKMANN "../Shaders/beckmannVertexShader.txt"
#define F_SHADER_BECKMANN "../Shaders/beckmannFragmentShader.txt"

#define V_SHADER_LINE "../Shaders/lineVertexShader.txt"
#define F_SHADER_LINE "../Shaders/lineFragmentShader.txt"

#define VS_LAMBERT "../Shaders/lambertianVS.txt"
#define FS_LAMBERT "../Shaders/lambertianFS.txt"

#define VS_ORENNAYAR "../Shaders/orenNayarVS.txt"
#define FS_ORENNAYAR "../Shaders/orenNayarFS.txt"

#define VS_JOURNEYDIFFUSE "../Shaders/journeyDiffuseVS.txt"
#define FS_JOURNEYDIFFUSE "../Shaders/journeyDiffuseFS.txt"

#define VS_NORMALMAP "../Shaders/normalMapVS.txt"
#define FS_NORMALMAP "../Shaders/normalMapFS.txt"

#pragma endregion

#define VS_TOON "../Shaders/LineDrawingShaders/diffuseToonVS.txt"
#define FS_TOON "../Shaders/LineDrawingShaders/diffuseToonFS.txt"

#define VS_TONEDIFF "../Shaders/LineDrawingShaders/toneDiffuseVS.txt"
#define FS_TONEDIFF "../Shaders/LineDrawingShaders/toneDiffuseFS.txt"

#define VS_TONESPEC "../Shaders/LineDrawingShaders/toneSpecVS.txt"
#define FS_TONESPEC "../Shaders/LineDrawingShaders/toneSpecFS.txt"

#pragma region FRAME SHADERS
#define VS_DRAWFRAME "../Shaders/LineDrawingShaders/drawFrameVS.txt"
#define FS_DRAWFRAME "../Shaders/LineDrawingShaders/drawFrameFS.txt"
#pragma endregion

#define MESH_SPHERE "Meshes/sphere.dae"
#define MESH_SUZANNE "Meshes/suzanne.dae"

int oldTime = 0;

Camera camera(glm::vec3(0.0f, 0.0f, 8.0f));

glm::vec4 light_pos(-5.0f, 0.0f, 5.0f, 1.0f); 
glm::vec3 la(1.0f, 1.0f, 1.0f); 
glm::vec3 ld(1.0f, 1.0f, 1.0f); 
glm::vec3 ls(1.0f, 1.0f, 1.0f); 

float light_intensity = 0.9f;

RigidBodyManager bodies;

glm::mat4 proj_mat, view_mat;


ShaderManager shaders;
ShaderManager frame_shaders;
Shaders *current_shader;

Shaders *frame_shader;

bool use_frame_shader = false;

FBO frame_buffer;

float line_width = 0.005f;


Shaders text_shader("Text", V_SHADER_LINE, F_SHADER_LINE);

glm::vec3 background_colour(0.5f, 0.5f, 0.5f);
float toon_threshold = 0.25f;
float toon_shade = 0.5f;

void addShaders()
{
	shaders.addShader("Tone Specular", VS_TONESPEC, FS_TONESPEC);
	shaders.addShader("Tone Diffuse", VS_TONEDIFF, FS_TONEDIFF);
	shaders.addShader("Toon", VS_TOON, FS_TOON);
	shaders.addShader("Lambertian", VS_LAMBERT, FS_LAMBERT);
//	shaders.addShader("Oren-Nayar", VS_ORENNAYAR, FS_ORENNAYAR);
//	shaders.addShader("Journey Diffuse", VS_JOURNEYDIFFUSE, FS_JOURNEYDIFFUSE);
	
	frame_shaders.addShader("Draw Frame", VS_DRAWFRAME, FS_DRAWFRAME);
	frame_shader = &frame_shaders.shaders[0];
}

void addBodies()
{

	bodies.addRigidBody(MESH_SUZANNE);
	bodies.addRigidBody(MESH_SPHERE);

	bodies.bodies[0].position = glm::vec3(-2.0f, 0.0f, 0.0f);
	bodies.bodies[1].position = glm::vec3(2.0f, -0.3f, 0.0f);

	bodies.load_mesh();


}


#pragma region TWEAK BAR STUFF


void init_tweak()
{
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(width, height);

	TwBar *bar;
	bar = TwNewBar("Line Drawing");

	shaders.addTBar(bar);

	TwAddVarRW(bar, "Frame Shader?", TW_TYPE_BOOLCPP, &use_frame_shader, "");

	camera.addTBar(bar);

	TwAddVarRW(bar, "Light Position", TW_TYPE_DIR3F, &light_pos, "");

	TwAddVarRW(bar, "Light Intensity", TW_TYPE_FLOAT, &light_intensity, "step = 0.05");

	TwAddVarRW(bar, "Background Colour", TW_TYPE_COLOR3F, &background_colour, "");
	TwAddVarRW(bar, "Toon Threshold", TW_TYPE_FLOAT, &toon_threshold, "");
	TwAddVarRW(bar, "Toon Darkness", TW_TYPE_FLOAT, &toon_shade, "step = 0.05");
	TwAddVarRW(bar, "Line Width", TW_TYPE_FLOAT, &line_width, "step = 0.001");
	/*
	TwAddVarRW(bar, "Light Ambient", TW_TYPE_COLOR3F, &la, "");
	TwAddVarRW(bar, "Light Diffuse", TW_TYPE_COLOR3F, &ld, "");
	TwAddVarRW(bar, "Light Specular", TW_TYPE_COLOR3F, &ls, "");
	*/

	
//	bodies.bodies[0].mesh.addTBar(bar);

	
}

void draw_tweak()
{
	TwDraw();
}



#pragma endregion 


void init()
{
	addShaders();
	shaders.compileShaders();

	text_shader.CompileShaders();

	frame_shaders.compileShaders();
	

	current_shader = &shaders.current_shader;

	addBodies();

	init_tweak();

	frame_buffer.setup(width, height);
}

void drawScene()
{
	// tell GL to only draw onto a pixel if the shape is closer to the viewer
	glEnable (GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc (GL_LESS); // depth-testing interprets a smaller value as "closer"
	glClearColor (background_colour.x, background_colour.y, background_colour.z, 1.0f);

	if((current_shader->name == "Tone Diffuse")||(current_shader->name == "Tone Specular"))
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);



	glUseProgram(current_shader->id);

	int light_pos_location = glGetUniformLocation (current_shader->id, "LightPosition");
	glUniform4fv(light_pos_location, 1, glm::value_ptr(light_pos));

	int la_location = glGetUniformLocation (current_shader->id, "La");
	int ld_location = glGetUniformLocation (current_shader->id, "Ld");
	int ls_location = glGetUniformLocation (current_shader->id, "Ls");

	glUniform3fv(la_location, 1, glm::value_ptr(la));
	glUniform3fv(ld_location, 1, glm::value_ptr(ld));
	glUniform3fv(ls_location, 1, glm::value_ptr(ls));



	proj_mat = glm::perspective(45.0f, (float)width/(float)height, 0.1f, 200.0f);
	view_mat = camera.getRotationMat();

	current_shader->setViewProj(view_mat, proj_mat);
	current_shader->setBackground(background_colour);
	current_shader->setToonValues(toon_threshold, toon_shade);

	
	bodies.draw(current_shader->id);



	
}



void display()
{
	if(use_frame_shader)
	{
		frame_buffer.bind();
		drawScene();
		frame_buffer.drawFrame(frame_shader->id);

		frame_shader->setOffset(line_width/2);
	}
	else
		drawScene();
	

	glUseProgram(text_shader.id);

	int text_colour_location = glGetUniformLocation (text_shader.id, "colour");
	glUniform3fv(text_colour_location, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, 0.0f)));


	string cs_string = current_shader->name + " Shader";
	unsigned char* cs_char = new unsigned char[cs_string.size()+1]();
	copy(cs_string.begin(), cs_string.end(), cs_char);
	glWindowPos2d(130 + 400, 100);
	glutBitmapString(GLUT_BITMAP_HELVETICA_18, cs_char);
	

	

	
	draw_tweak();

	glutSwapBuffers();
}

void updateScene()
{
	int time = glutGet(GLUT_ELAPSED_TIME);
	int delta_time = time-oldTime;
	oldTime = time;

	//time since last frame in seconds
	double elapsed_seconds = (double)delta_time/1000;


	bodies.update(elapsed_seconds);

	ld = glm::vec3(light_intensity);


	glutPostRedisplay();
}





int main(int argc, char** argv)
{

	// Set up the window
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB);
    glutInitWindowSize(width, height);
    glutCreateWindow("Line Drawing");

	// Tell glut where the display function is
	glutDisplayFunc(display);
	glutIdleFunc(updateScene);


	glutMouseFunc((GLUTmousebuttonfun)TwEventMouseButtonGLUT);
	glutMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT); // same as MouseMotion
	glutKeyboardFunc((GLUTkeyboardfun)TwEventKeyboardGLUT);
	glutSpecialFunc((GLUTspecialfun)TwEventSpecialGLUT);
 
	// send the ''glutGetModifers'' function pointer to AntTweakBar
	TwGLUTModifiersFunc(glutGetModifiers);
 


	// A call to glewInit() must be done after glut is initialized!
    GLenum res = glewInit();
	// Check for any errors
    if (res != GLEW_OK) {
      fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
      return 1;
    }
	// Set up your objects and shaders
	init();
	// Begin infinite event loop
	glutMainLoop();
    return 0;
}