#include "RigidBody.h"

using namespace std;

RigidBody :: RigidBody()
{
	position = orientation = velocity = ang_velocity = glm::vec3(0.0f);
	translation_mat = glm::mat4();
	rotation_mat = glm::mat3();
}

RigidBody :: RigidBody(Mesh _mesh)
{
	position = orientation = velocity = ang_velocity = glm::vec3(0.0f);
	translation_mat  = glm::mat4();
	rotation_mat = glm::mat3();
	mesh = _mesh;

}

void RigidBody :: load_mesh()
{
	mesh.load_mesh();
}

void RigidBody :: draw(GLuint spID)
{	
	mesh.draw(spID, model_mat);
}


void RigidBody :: addTBar(TwBar *bar)
{
	TwAddVarRW(bar, "Position", TW_TYPE_DIR3F, &position, "");
	TwAddVarRO(bar, "Orientation", TW_TYPE_DIR3F, &orientation, "");
	TwAddVarRW(bar, "Velocity", TW_TYPE_DIR3F, &velocity, "");
	TwAddVarRW(bar, "Angular Velocity", TW_TYPE_DIR3F, &ang_velocity, "");


}

void RigidBody :: update(float dt)
{
	updateTranslation(dt);
	updateRotation(dt);
	model_mat = translation_mat * glm::mat4(rotation_mat);
}



void RigidBody :: updateTranslation(float dt)
{
	position += dt * velocity;

	translation_mat = glm::translate(glm::mat4(), position);
}

void RigidBody :: updateRotation(float dt)
{
	glm::mat3 ang_vel_mat = makeAngVelMat(ang_velocity);

	rotation_mat += ang_vel_mat * rotation_mat * dt;	

	rotation_mat = glm::orthonormalize(rotation_mat);

	orientation = getOrientationFromRotMat(rotation_mat);
}




glm::mat3 RigidBody :: makeAngVelMat(glm::vec3 av_vec)
{
	glm::mat3 av;

	av[0][1] = -av_vec.z;
	av[0][2] = av_vec.y;

	av[1][0] = av_vec.z;
	av[1][2] = -av_vec.x;

	av[2][0] = -av_vec.y;
	av[2][1] = av_vec.x;

	return av;
}




glm::vec3 RigidBody :: getOrientationFromRotMat(glm::mat3 mat)
{
	float x, y, z;
	glm::vec3 vec;

	x = atan2(mat[2][1], mat[2][2]);
	y = atan2(-mat[2][0], sqrt(mat[2][1]*mat[2][1] + mat[2][2]*mat[2][2]));
	z = atan2(mat[1][0], mat[0][0]);

	x = glm::degrees(x);
	y = glm::degrees(y);
	z = glm::degrees(z);

	while(x < 0)
		x += 360;

	while(y < 0)
		y += 360;

	while(z < 0)
		z += 360;

	vec = glm::vec3(x, y, z);
	return vec;

}


