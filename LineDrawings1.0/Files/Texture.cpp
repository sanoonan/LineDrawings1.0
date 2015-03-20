#include "Texture.h"

using namespace std;

Texture :: Texture()
{
	id = 0;
	has_tex = false;
	slot = 0;

	glGenTextures(1, &id);
	glBindTexture(GL_TEXTURE_2D, id);
	
}

Texture :: Texture(const char *filename)
{
	id = 0;

	glGenTextures(1, &id);
	load_image_to_texture(filename);
	glBindTexture(GL_TEXTURE_2D, id);
}

bool Texture :: load_image_to_texture(const char* file_name, unsigned int& _tex)
{

	has_tex = true;
	int n;
	int force_channels = 4;
	int _ix, _iy;

	unsigned char* _img = stbi_load(file_name, &_ix, &_iy, &n, force_channels);

	if(!_img)
	{
		fprintf(stderr, "ERROR: could not load image %s. Check file type and path\n", file_name);
		return false;
	}

	// FLIP UP-SIDE DIDDLY-DOWN
	// make upside-down copy for GL
	unsigned char* imagePtr = &_img[0];
	int half_height_in_pixels = _iy/2;
	int height_in_pixels = _iy;

	//assuming RGBA for 4 components per pixel
	int num_colour_components = 4;

	//assuming each colour component is an unsigned char
	int width_in_chars = _ix * num_colour_components;

	unsigned char* top = NULL;
	unsigned char* bottom = NULL;
	unsigned char temp = 0;

	for(int h=0; h<half_height_in_pixels; h++)
	{
		top = imagePtr + h * width_in_chars;
		bottom = imagePtr + (height_in_pixels - h - 1) * width_in_chars;

		for(int w=0; w<width_in_chars; w++)
		{
			//swap the chars around
			temp = *top;
			*top = *bottom;
			*bottom = temp;
			++top;
			++bottom;
		}
	}

	//copy into an OpenGL texture
	

//	if(!sss)
//	{

		glActiveTexture(GL_TEXTURE0 + slot);
//		sss = true;
//	}
//	else
//		glActiveTexture(GL_TEXTURE1);
	
	glBindTexture(GL_TEXTURE_2D, _tex);


	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _ix, _iy, 0, GL_RGBA, GL_UNSIGNED_BYTE, _img);

	

	/*
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 4);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	*/
	

	glGenerateMipmap(GL_TEXTURE_2D);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

}


bool Texture :: addImageFile(const char *filename)
{
	bool ans = load_image_to_texture(filename);
	glBindTexture(GL_TEXTURE_2D, id);
	return ans;
}


bool Texture :: load_image_to_texture(const char* file_name)
{
	return load_image_to_texture(file_name, id);
}

bool Texture :: loadTexType(const aiMaterial *mat, aiTextureType type, string file_dir)
{
	if(type == aiTextureType_NORMALS)
	{
//		id = 1;
		slot = 1;
	}

	int num_textures = mat->GetTextureCount(type);

	
	if(num_textures > 0)
	{
		aiString tex_filename;
		mat->GetTexture(type, 0, &tex_filename);
		string dir = file_dir;
		dir = dir.substr(0, dir.find_last_of('/'));
		dir += "/";
		dir += tex_filename.C_Str();
		const char* s_tex = dir.c_str();
		return addImageFile(s_tex);
	}
	return false;
}