
#ifndef CAMERA_H
#define CAMERA_H

#include <GLFW/glfw3.h>

#include "mobject.h"

using namespace std;

class Camera {

  public:
    static float cameraDistance;
    static float cameraMinDistance;
    static float cameraVDistanceFactor;

    static Mobject* playership;

    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
    static void PointAtTarget();

};

#endif
