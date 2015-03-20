#include "ShaderManager.h"

using namespace std;

ShaderManager :: ShaderManager()
{
	num_shaders = 0;
	current_shader_num = 0;

}

ShaderManager :: ShaderManager(std::vector<Shaders> shader_array)
{
	num_shaders = shader_array.size();
	shaders = shader_array;
}



void ShaderManager :: addShader(Shaders shader)
{
	num_shaders++;
	shaders.push_back(shader);
	
	
	current_shader = shaders[current_shader_num];
}
	

void ShaderManager :: addShader(string name, const char* vertex, const char* fragment)
{
	Shaders shader(name, vertex, fragment);

	addShader(shader);
}



void ShaderManager :: compileShaders()
{
	for(int i=0; i<num_shaders; i++)
		shaders[i].CompileShaders();

	current_shader = shaders[current_shader_num];
}

	
void ShaderManager :: nextShader()
{
	current_shader_num++;

	if(current_shader_num >= num_shaders)
		current_shader_num = 0;

	current_shader = shaders[current_shader_num];

}


void TW_CALL nextShaderCB(void *clientData)
{
	ShaderManager* temp = static_cast<ShaderManager *>(clientData);
	temp->nextShader();
	*static_cast<ShaderManager *>(clientData) = *temp;
}

void ShaderManager :: addTBar(TwBar *bar)
{

	TwAddVarRO(bar, "Shader Name", TW_TYPE_STDSTRING, &current_shader.name, "");

	TwAddButton(bar, "Change Shader", nextShaderCB,  this, "");
}
