
#include <GLFW/glfw3.h>

//#include <glm/gtc/quaternion.hpp>

#include "particle.h"
#include "particles.h"
#include "utils.h"

#include<iostream>
#include<cstdlib>
using namespace std;

Particles::Particles (int maxNumParticles, glm::vec3 pos, glm::vec3 defVelocity, vector<Mobject*>* worldObj) {

    maxParticles = maxNumParticles;

    particleCount = 0;

    position = glm::vec3(pos.x,pos.y,pos.z); // position of the emitter

    defaultVelocity = glm::vec3(defVelocity.x, defVelocity.y, defVelocity.z);

    world = worldObj;
}

void Particles::GLRender()
{
    //cout << "render the emitter." << endl;

    glPushMatrix();

    glColor3f(0.3f, 0.7f, 0.6f);  Utils::renderVec3(orientation, 2.0f); // Debug vec
    glTranslatef(position.x, position.y, position.z);

    if (active){
        glColor3f(0.3f, 0.7f, 0.0f);  Utils::renderVec3(orientation, 2.0f); // Debug vec
        // Render a cone
        glm::vec3 zv = glm::vec3(0,0,0);
        float coneAng = 20.0f;
        float coneScale = 1.0f;
        Utils::DrawCone(zv, orientation, coneAng, coneScale);
    }

    float timenow = (float) glfwGetTime();

    // see if its time to create a new particle

    if (active && particleCount < maxParticles && timenow - lastgen > genrate){
        Gen();
    }

    //Particle * particle = particles;

    glPopMatrix();
}

void Particles::Gen()
{
    // generate a particle.

    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-
    // https://forum.openframeworks.cc/t/converting-from-rotate-to-glm-rotate/29213

    float timenow = (float) glfwGetTime();

    lastgen = timenow;

    Particle* p = new Particle;

    p->lifebegin = timenow;

    p->lifeseconds = defaultLifeSec;

    p->parent = this;

    glm::vec3 zeroVec = glm::vec3(0,0,0);

    p->position      = GetAbsPos();

    glm::vec3 ou = orientation / glm::length(orientation); // unit vector
    p->orientation   = ou;
    p->velocity      = ou * 0.2f;
    //p->velocity      = zeroVec;

    p->name = "particle";

    //particles[particleCount] = p;

    particleCount++;

    Mobject* ptr = dynamic_cast<Mobject*>(p);

    (*world).push_back(ptr); // Add particle to world.
}


// rotate given vector using supplied angles x y z in degrees
glm::vec3 Particles::RotateVec3(glm::vec3 vec, glm::vec4 rotation)
{
    // http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#how-do-i-create-a-quaternion-in-c-

    glm::quat myQuat = glm::quat( // RotationAngle is in radians
        glm::radians(rotation.x),    // x = RotationAxis.x * sin(RotationAngle / 2)
        glm::radians(rotation.y),    // y = RotationAxis.y * sin(RotationAngle / 2)
        glm::radians(rotation.z),    // z = RotationAxis.z * sin(RotationAngle / 2)
        rotation.w                   // cos(RotationAngle / 2)   zero means no rotation
    );

    // https://gamedev.stackexchange.com/questions/94061/rotating-an-object-with-quaternion
    //glm::quat rot = glm::angleAxis(glm::radians(90.f), glm::vec3(0.f, 1.f, 0.f));

    //cout<< "vec: " << vec.x << " " << vec.y << " " << vec.z <<endl;

    vec = myQuat * vec; // apply the transformation to the vector.

    //cout<< "vec: " << vec.x << " " << vec.y << " " << vec.z <<endl;
    //cout<<endl;

    return vec;
}


