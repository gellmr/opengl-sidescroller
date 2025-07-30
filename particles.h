
#ifndef PARTICLES_H
#define PARTICLES_H

#include <GL/gl.h>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mobject.h"

//#define DEFAULT_LIFESEC 1.5f

const static float defaultLifeSec = 1.5f;



class Particles: public Mobject {
  public:
    int particleCount = 0; // how many particles we have
    int maxParticles = 120; // the max number we can have

    bool active = false; // must be true for us to emit particles

    //glm::vec3 position; // position of the emitter

    glm::vec3 defaultVelocity; // default velocity of the particles we will create.

    //Particle* particles[120]; // array of pointers to particles created using new

    //Particle* particle;

    float genrate = 0.12f; // seconds between each particle generation.
    float lastgen = 0.0f;  // the time that we last created a particle.

    vector<Mobject*>* world;

    Particles(int maxNumParticles, glm::vec3 pos, glm::vec3 defVelocity, vector<Mobject*>* worldObj);
    ~Particles(){};

    void GLRender();

    void Gen();

    // rotate given vector using supplied euler angles with x y z components
    static glm::vec3 RotateVec3(glm::vec3 vec, glm::vec4 rotation);

    static float getRand(float lowerbound, float upperbound);
};


#endif
