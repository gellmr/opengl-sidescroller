
#include <GLFW/glfw3.h>

#include "particle.h"
#include "particles.h"

#include<iostream>
#include<cstdlib>
using namespace std;

Particle::Particle ()
{
    lifebegin = (float) glfwGetTime();
    spritescale = 1.0f;
    spriteanim = 1.0f;
}

void Particle::InitVelocity(float lifesec, glm::vec3 pos, glm::vec3 initvel)
{
    lifeseconds = lifesec;
    position    = pos;
    velocity    = initvel;
    cout<<"velocity: " << velocity.x << " " << velocity.y << " " << velocity.z <<endl;
}

void Particle::GLRender()
{
    glPushMatrix();

    //cout << "render a particle." << endl;

    float timenow = glfwGetTime();

    if (timenow - lifebegin > lifeseconds){
        glPopMatrix();
        return; //terminate particle
    }

    position = position + velocity; // animate

    glTranslatef(position.x,position.y,position.z);

    spritescale = spritescale * spriteanim;
    glScalef(spritescale,spritescale,spritescale);

    glBegin(GL_QUADS); glColor3f(1.f, 1.f, 1.f);
    glVertex3f(0,     0,    0);
    glVertex3f(0.15,  0,    0);
    glVertex3f(0.15,  0.15, 0);
    glVertex3f(0,     0.15, 0);
    glEnd();

    glPopMatrix();
}

