
#include <GL/gl.h>
#include <GLFW/glfw3.h>

#include <cstdlib>
#include <iostream>
#include <fstream>

#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>

#include <stdlib.h>
#include <stdio.h>
#include <string>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>/// for M_PI

#include "camera.h"
#include "controls.h"
#include "utils.h"
#include "mobject.h"

#define CAMERA_MIN_Z_DIST 5.0f
#define CAMERA_V_DIST_FACTOR 13.0f

using namespace std;

float Camera::cameraDistance        = CAMERA_MIN_Z_DIST;
float Camera::cameraMinDistance     = CAMERA_MIN_Z_DIST;
float Camera::cameraVDistanceFactor = CAMERA_V_DIST_FACTOR;

Mobject* Camera::playership = NULL;

void Camera::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    Utils::fov -= (float)yoffset;
    if (Utils::fov < 1.0f)
        Utils::fov = 1.0f;
    if (Utils::fov > 45.0f)
        Utils::fov = 45.0f;
    std::cout << "fov " << Utils::fov << "\n";
}

// Need to get mouse input for flight control.
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-6-keyboard-and-mouse/#field-of-view
void Camera::PointAtTarget()
{
  glm::vec3 camUpVec = glm::vec3(0.0, 1.0, 0.0);
  glm::vec3 campos = glm::vec3(
      playership->position.x,
      playership->position.y,
      playership->position.z + cameraDistance
  );

  // calculate point P
  Controls::GetMouse();
  Controls::worldmpos3d = Controls::GetOGLPos(Controls::mousex, Controls::mousey);
  glm::vec3 camtomouse = Controls::worldmpos3d - campos;
  glm::vec3 camtoplane = campos - Controls::camtarget;
  glm::vec3 ahat = glm::normalize(camtoplane);
  float adotb = glm::dot(ahat, camtomouse);
  glm::vec3 fparallel = adotb * ahat;
  glm::vec3 fperp = camtomouse - fparallel;

  // debug
  glPushMatrix();
  glMatrixMode(GL_MODELVIEW); glLoadIdentity();
  //glTranslatef(Controls::camtarget.x, Controls::camtarget.y, Controls::camtarget.z);
  glTranslatef(campos.x, campos.y, campos.z);
  glColor3f(0.0f, 0.9f, 0.9f);
  //Utils::renderVec3(camtomouse, 1.0f);
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();


  if (Controls::ortho){
      Utils::OrthoMode();
  }else{
      Utils::PerspMode();
      Controls::camtarget = glm::vec3(playership->position.x, playership->position.y, playership->position.z);
      //Controls::camtarget = glm::vec3(Controls::worldmpos3d.x, Controls::worldmpos3d.y, Controls::worldmpos3d.z);
      gluLookAt(
          campos.x,               campos.y,               campos.z,
          Controls::camtarget.x,  Controls::camtarget.y,  Controls::camtarget.z,
          camUpVec.x,             camUpVec.y,             camUpVec.z
      );
  }
}
