#pragma once

#define _CRT_SECURE_NO_DEPRECATE
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
#include <string>

#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4, glm::ivec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm::value_ptr

#include "stb_image.h"

#include "AntTweakBar.h"



using namespace std;

class Texture
{
public:
	
	GLuint id;

	bool has_tex;

	int slot;

	Texture();

	Texture(const char *filename);

	bool addImageFile(const char *filename);

	bool load_image_to_texture(const char* file_name, unsigned int& _tex);
	bool load_image_to_texture(const char* file_name);

	bool loadTexType(const aiMaterial *mat, aiTextureType type, string file_dir);
	
};

