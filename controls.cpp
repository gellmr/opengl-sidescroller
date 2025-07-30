
#include <GLFW/glfw3.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include<cstdlib>

#include "controls.h"

using namespace std;

GLFWwindow* Controls::window = NULL;

double Controls::mousex = 0.0d;
double Controls::mousey = 0.0d;

int Controls::ortho = 0;
int Controls::mousedown = 0;

glm::vec3 Controls::camtarget = glm::vec3(0,0,0);
glm::vec3 Controls::worldmpos2d = glm::vec3(0,0,0);
glm::vec3 Controls::worldmpos3d = glm::vec3(0,0,0);

glm::vec3 Controls::playertomouse = glm::vec3(0,0,0);

void Controls::GetMouse()
{
    GLFWwindow* w = window;
    if (window){
        glfwGetCursorPos(w, &Controls::mousex, &Controls::mousey);
    }
}


glm::vec3 Controls::GetOGLPos(double x, double y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;

    glReadPixels( int(x), int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ ); // does this give us the z depth of our mouse click?

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    glm::vec3 unprojectedWorldMousePos = glm::vec3(posX, posY, posZ);

    return unprojectedWorldMousePos;
}

