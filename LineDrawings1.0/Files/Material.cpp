#include "Material.h"

using namespace std;

Material :: Material()
{
	ambient = diffuse = specular = cool = warm = glm::vec3(0.0f);
	shininess = beckmann_m = refraction = 0.0f;
	eta = glm::vec3(0.0f);
}


void Material :: load_material(const aiScene* scene, const char *filename)
{
	//get material
	if (scene->mNumMaterials > 0)
	{
		const aiMaterial* a_mat = scene->mMaterials[0];


		diffuse_tex.loadTexType(a_mat, aiTextureType_DIFFUSE, filename);
		normal_tex.loadTexType(a_mat, aiTextureType_NORMALS, filename);
		
	
		
		aiColor4D a_diffuse;
		aiGetMaterialColor(a_mat, AI_MATKEY_COLOR_DIFFUSE, &a_diffuse);

		aiColor4D a_specular;
		aiGetMaterialColor(a_mat, AI_MATKEY_COLOR_SPECULAR, &a_specular);

		float a_shininess = 0.0f;
		aiGetMaterialFloat(a_mat, AI_MATKEY_SHININESS, &a_shininess);


		if(!diffuse_tex.has_tex)
			diffuse = glm::vec3(a_diffuse.r, a_diffuse.g, a_diffuse.b);
		else
			diffuse = glm::vec3(1.0f);


		specular = glm::vec3(a_specular.r, a_specular.g, a_specular.b);
		shininess = a_shininess;

		beckmann_m = 0.3f;
		refraction = 0.9f;

		eta = glm::vec3(0.8f, 0.75f, 0.7f);

		reflection = 0.5f;

		ambient = (diffuse) * (0.1f);

		cool = glm::vec3(0.0, 0.0, 1.0);
		warm = glm::vec3(1.0, 1.0, 0.4);
	}
}

