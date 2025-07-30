
#ifndef PARTICLE_H
#define PARTICLE_H

#include <GL/gl.h>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mobject.h"


class Particle: public Mobject {
  public:

    float lifebegin;     // time the particle was created
    float lifeseconds;   // how long the particle can live

    float spritescale; // 1.0f for default scale.
    float spriteanim;
    Particle();

    void InitVelocity(float lifesec, glm::vec3 pos, glm::vec3 initvel);

    void GLRender();
};

#endif
