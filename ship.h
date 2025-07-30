
#ifndef SHIP_H
#define SHIP_H

#include <GLFW/glfw3.h>

#include "mobject.h"
#include "particles.h"

using namespace std;

class Ship: public Mobject {

  public:

    Ship();

    float thrustjetpow;

    Particles* mainThrustEmitter;

    void GLRender();
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

    glm::vec3 GetImpulse(float direction);
    glm::vec3 GetImpulseU(float direction);
};

#endif
