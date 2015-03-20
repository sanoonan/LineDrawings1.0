#include "Mesh.h"

using namespace std;

Mesh :: Mesh()
{
	filename = "";
	vao = 0;
	point_count = 0;
}

Mesh :: Mesh(const char* _filename)
{
	filename = _filename;
	vao = 0;
	point_count = 0;
}

bool Mesh :: load_mesh (std::vector<glm::vec3> &v)
{
	
	v = getVertices();
	
  return load_mesh();
}



bool Mesh :: load_mesh ()
{
	

	
	const aiScene* scene = aiImportFile (filename, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs); // TRIANGLES!

  
	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		fprintf (stderr, "ERROR: reading mesh %s\n", filename);
		return false;
	}
	printf ("  %i animations\n", scene->mNumAnimations);
	printf ("  %i cameras\n", scene->mNumCameras);
	printf ("  %i lights\n", scene->mNumLights);
	printf ("  %i materials\n", scene->mNumMaterials);
	printf ("  %i meshes\n", scene->mNumMeshes);
	printf ("  %i textures\n", scene->mNumTextures);
  
  
//	load_material(scene);
	material.load_material(scene, filename);
	assign_vao(scene);

 
	aiReleaseImport (scene);
	printf("mesh loaded\n");
  
	return true;
}

//loads assimp mesh data into the vao
void Mesh :: assign_vao(const aiScene* scene)
{
	

	 const aiMesh* mesh = scene->mMeshes[0]; 


	 printf("   %i vertices in mesh[%i]\n", mesh->mNumVertices, 0);

	  point_count = mesh->mNumVertices;


	  glGenVertexArrays(1, &vao);
	  glBindVertexArray(vao);

		GLfloat* points = NULL;
		GLfloat* normals = NULL;
		GLfloat* texcoords = NULL;
		GLfloat *tangents = NULL;
		GLfloat *bittangents = NULL;


		GLint* bone_ids_lbs = NULL;
		GLfloat* bone_weights = NULL;

		//get vertex positions
		  if (mesh->HasPositions ())
		  {

			points = (GLfloat*)malloc (point_count * 3 * sizeof (GLfloat));

			for (int i=0; i< point_count; i++)
			{
				const aiVector3D* vp = &(mesh->mVertices[i]);
				points[i*3] = (GLfloat)vp->x;
				points[i*3+1] = (GLfloat)vp->y;
				points[i*3+2] = (GLfloat)vp->z;
			}


		  }

		  //get normals
		  if (mesh->HasNormals ()) {
			normals = (GLfloat*)malloc(point_count * 3 * sizeof (GLfloat));

			for (int i=0; i<point_count; i++)
			{
				const aiVector3D* vn = &(mesh->mNormals[i]);
				normals[i*3] = (GLfloat)vn->x;
				normals[i*3+1] = (GLfloat)vn->y;
				normals[i*3+2] = (GLfloat)vn->z;
			}
		  }

		  //get texture coordinates
		  if (mesh->HasTextureCoords (0))
		  {
			texcoords = (GLfloat*)malloc(point_count * 2 * sizeof (GLfloat));

			for (int i=0; i<point_count; i++)
			{
				const aiVector3D* vt = &(mesh->mTextureCoords[0][i]);
				texcoords[i*2] = (GLfloat)vt->x;
				texcoords[i*2+1] = (GLfloat)vt->y;
		
			}
		  }

		  //get tangents
		  if (mesh->HasTangentsAndBitangents ())
		  {
			tangents = (GLfloat*)malloc(point_count * 3 * sizeof (GLfloat));

			for (int i=0; i<point_count; i++)
			{
				const aiVector3D* vtan = &(mesh->mTangents[i]);
				tangents[i*2] = (GLfloat)vtan->x;
				tangents[i*2+1] = (GLfloat)vtan->y;
				tangents[i*3+2] = (GLfloat)vtan->z;
			}

			bittangents = (GLfloat*)malloc(point_count * 3 * sizeof (GLfloat));

			for (int i=0; i<point_count; i++)
			{
				const aiVector3D* vbtan = &(mesh->mBitangents[i]);
				bittangents[i*2] = (GLfloat)vbtan->x;
				bittangents[i*2+1] = (GLfloat)vbtan->y;
				bittangents[i*3+2] = (GLfloat)vbtan->z;
			}

		  }




	
		  // copy mesh data in VBOs


		  //load in vertex positions
		  if(mesh->HasPositions())
		  {
			  GLuint vbo;
			  glGenBuffers (1, &vbo);
			  glBindBuffer(GL_ARRAY_BUFFER, vbo);
			  glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), points, GL_STATIC_DRAW);

			  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			  glEnableVertexAttribArray(0);
			  free (points);
		  }

		  //load in normals
		  if(mesh->HasNormals())
		  {
			  GLuint vbo;
			  glGenBuffers (1, &vbo);
			  glBindBuffer(GL_ARRAY_BUFFER, vbo);
			  glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), normals, GL_STATIC_DRAW);

			  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			  glEnableVertexAttribArray(1);
			  free (normals);
		  }

		  //load in texture coordinates
		  if(mesh->HasTextureCoords(0))
		  {
			  GLuint vbo;
			  glGenBuffers (1, &vbo);
			  glBindBuffer(GL_ARRAY_BUFFER, vbo);
			  glBufferData(GL_ARRAY_BUFFER, 2 * point_count * sizeof(GLfloat), texcoords, GL_STATIC_DRAW);

			  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);
			  glEnableVertexAttribArray(2);
			  free (texcoords);
		  }

		   //load in tangents
		  if(mesh->HasTangentsAndBitangents())
		  {
			  GLuint vbo;
			  glGenBuffers (1, &vbo);
			  glBindBuffer(GL_ARRAY_BUFFER, vbo);
			  glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), tangents, GL_STATIC_DRAW);

			  glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			  glEnableVertexAttribArray(3);
			  free (tangents);
		  }
		   //load in tangents
		  if(mesh->HasTangentsAndBitangents())
		  {
			  GLuint vbo;
			  glGenBuffers (1, &vbo);
			  glBindBuffer(GL_ARRAY_BUFFER, vbo);
			  glBufferData(GL_ARRAY_BUFFER, 3 * point_count * sizeof(GLfloat), bittangents, GL_STATIC_DRAW);

			  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
			  glEnableVertexAttribArray(4);
			  free (bittangents);
		  }


}

void Mesh :: draw(GLuint spID, glm::mat4 model_mat)
{


	int first_tex_location = glGetUniformLocation(spID, "diffuse_tex");
	glUniform1i(first_tex_location, 0);

	int second_tex_location = glGetUniformLocation(spID, "normal_tex");
	glUniform1i(second_tex_location, 1);

	if(material.diffuse_tex.has_tex)
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, material.diffuse_tex.id);
	}
	if(material.normal_tex.has_tex)
	{
		

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, material.normal_tex.id);

	//	glBindSampler(1, tex_location);
	}
//	GLint first_tex_location = glGetUniformLocation(spID, "diffuse_tex");
//	glUniform1i(first_tex_location, GL_TEXTURE0);

//	GLint second_tex_location = glGetUniformLocation(spID, "normal_tex");
//	glUniform1i(second_tex_location, 1);

//	int cube_tex_location = glGetUniformLocation(spID, "CubeMapTex");
//	glUniform1i(cube_tex_location, 0);

	

	int ka_location = glGetUniformLocation (spID, "ka");
	int kd_location = glGetUniformLocation (spID, "kd");
	int ks_location = glGetUniformLocation (spID, "ks");
	int shine_location = glGetUniformLocation (spID, "spec_ex");
	int beckmann_shine_location = glGetUniformLocation (spID, "beckmann_m");
	int refraction_location = glGetUniformLocation (spID, "refraction");
	int kcool_location = glGetUniformLocation (spID, "kcool");
	int kwarm_location = glGetUniformLocation (spID, "kwarm");
	int texbool_location = glGetUniformLocation (spID, "has_texture");
	int reflect_location = glGetUniformLocation (spID, "reflect_factor");
	int eta_location = glGetUniformLocation (spID, "eta");

	int roughness_location = glGetUniformLocation (spID, "roughness");
	int normalbool_location = glGetUniformLocation (spID, "has_normalmap");

	glUniform3fv(ka_location, 1, glm::value_ptr(material.ambient));
	glUniform3fv(kd_location, 1, glm::value_ptr(material.diffuse));
	glUniform3fv(ks_location, 1, glm::value_ptr(material.specular));
	glUniform1fv(shine_location, 1, &material.shininess);
	glUniform1fv(beckmann_shine_location, 1, &material.beckmann_m);
	glUniform1fv(refraction_location, 1, &material.refraction);
	glUniform3fv(kcool_location, 1, glm::value_ptr(material.cool));
	glUniform3fv(kwarm_location, 1, glm::value_ptr(material.warm));
	glUniform1fv(reflect_location, 1, &material.reflection);
	glUniform3fv(eta_location, 1, glm::value_ptr(material.eta));

	glUniform1fv(roughness_location, 1, &material.beckmann_m);

	int int_bool;
	int_bool = material.diffuse_tex.has_tex;
	glUniform1i(texbool_location, int_bool);

	int_bool = material.normal_tex.has_tex;
	glUniform1i(normalbool_location, int_bool);

	int matrix_location = glGetUniformLocation (spID, "model");
	glUniformMatrix4fv(matrix_location, 1, GL_FALSE, glm::value_ptr(model_mat));
	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, point_count);
}

std::vector<glm::vec3> Mesh :: getVertices()
{
	const aiScene* scene = aiImportFile (filename, aiProcess_Triangulate);

	const aiMesh* mesh = scene->mMeshes[0]; 

	int vertex_count = mesh->mNumVertices;
	std::vector<glm::vec3> v (vertex_count);

	//get vertex positions
	if (mesh->HasPositions ())
	{

		for (int i=0; i< vertex_count; i++)
		{
			const aiVector3D* vp = &(mesh->mVertices[i]);
			v[i].x = (GLfloat)vp->x;
			v[i].y = (GLfloat)vp->y;
			v[i].z = (GLfloat)vp->z;
		}
	}
	return v;
}


void Mesh :: addTBar(TwBar *bar)
{
	TwAddVarRW(bar, vao+"ETA", TW_TYPE_COLOR3F, &material.eta, "label=ETA ");
	TwAddVarRW(bar, vao+"Reflectiveness", TW_TYPE_FLOAT, &material.reflection, " label=Reflectiveness step=0.05 ");
	TwAddVarRW(bar, vao+"Ambient", TW_TYPE_COLOR3F, &material.ambient, "label=Ambient");
	TwAddVarRW(bar, vao+"Diffuse", TW_TYPE_COLOR3F, &material.diffuse, "label=Diffuse");
	TwAddVarRW(bar, vao+"Speculular", TW_TYPE_COLOR3F, &material.specular, "label=Specular");
	TwAddVarRW(bar, vao+"Shininess", TW_TYPE_FLOAT, &material.shininess, "label=Shininess");
	TwAddVarRW(bar, vao+"Roughness", TW_TYPE_FLOAT, &material.beckmann_m, " label=Roughness step=0.05 ");
	TwAddVarRW(bar, vao+"Refraction", TW_TYPE_FLOAT, &material.refraction, " label=Refraction step=0.05 ");
	TwAddVarRW(bar, vao+"Cool", TW_TYPE_COLOR3F, &material.cool, "label=Cool");
	TwAddVarRW(bar, vao+"Warm", TW_TYPE_COLOR3F, &material.warm, "label=Warm");
}
