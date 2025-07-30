
#ifndef CONTROLS_H
#define CONTROLS_H

#include <GLFW/glfw3.h>

using namespace std;

class Controls {

  public:

    static GLFWwindow* window;

    static double mousex;
    static double mousey;

    static int ortho;
    static int mousedown;

    static glm::vec3 worldmpos3d;
    static glm::vec3 worldmpos2d; // unprojected world mouse position x y z vector

    static glm::vec3 playertomouse;

    static glm::vec3 camtarget;

    static void GetMouse();

    static glm::vec3 GetOGLPos(double x, double y);

    //static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
};

#endif
