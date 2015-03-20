#include "RigidBodyManager.h"


RigidBodyManager :: RigidBodyManager()
{
	num = 0;
}


void RigidBodyManager :: addRigidBody(RigidBody body)
{
	bodies.push_back(body);
	num++;
}



void RigidBodyManager :: addRigidBody(Mesh mesh)
{
	RigidBody body(mesh);
	addRigidBody(body);
}

void RigidBodyManager :: addRigidBody(const char* filename)
{
	RigidBody body(filename);
	addRigidBody(body);
}


void RigidBodyManager :: addTBar(TwBar *bar)
{
	for(int i=0; i<num; i++)
		bodies[i].addTBar(bar);
}

void RigidBodyManager :: load_mesh()
{
	for(int i=0; i<num; i++)
		bodies[i].load_mesh();

}


void RigidBodyManager :: draw(GLuint spID)
{
	for(int i=0; i<num; i++)
		bodies[i].draw(spID);
}



void RigidBodyManager :: update(float dt)
{
	for(int i=0; i<num; i++)
		bodies[i].update(dt);
}

