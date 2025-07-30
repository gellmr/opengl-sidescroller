
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

#include "hud.h"
#include "gm.h"
#include "utils.h"
#include "controls.h"

using namespace std;

void Hud::renderHUD()
{

    int width = 0;
    int height = 0;

    glfwGetFramebufferSize(GM::mwindow, &width, &height);

    Utils::ratio = width / (float) height;

    float halfwidth = width / 2.0f;
    float halfheight = height / 2.0f;

    double mx = Controls::mousex;
    double my = Controls::mousey;

    Controls::GetMouse();
    Controls::worldmpos3d = Controls::GetOGLPos(mx, my);




    Utils::OrthoModeLeft(); // Render in screen coordinates. This gives us 0,0 in top left corner.

    Controls::GetMouse();
    Controls::worldmpos2d = Controls::GetOGLPos(mx, my);

    halfwidth = Utils::ratio * Utils::orthofactor;
    halfheight = Utils::orthofactor;

    //render mouse quad
    // std::cout << "mx " << mx << " my " << my << "\n";
    //std::cout << "worldpos: " << Controls::worldmpos2d.x << " " << Controls::worldmpos2d.y << " " << Controls::worldmpos2d.z << "\n";
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glTranslatef(Controls::worldmpos2d.x, Controls::worldmpos2d.y, Controls::worldmpos2d.z);
    glColor3f(1.f, 1.f, 1.f);
    Utils::DrawXYQuad(0.15, 0.15);

    // Render mouse text
    glTranslatef(0, 0, -0.05f); // come towards camera a bit.
    char charVal[50];
    sprintf(charVal, "mouse %0.3f %0.3f %0.3f", Controls::worldmpos3d.x, Controls::worldmpos3d.y, Controls::worldmpos3d.z);
    Utils::renderText( reinterpret_cast<const unsigned char *>(charVal));




    Utils::OrthoMode(); // Go back to centered coordinate system

    // render oort map
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();
    glTranslatef(
        -1.0f * (halfwidth  - 0.05),
                 halfheight - 0.05,
                 0
    );
    float oortmapWidth = 8.0f;
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0,0,0);      glVertex3f(oortmapWidth,0,0);
    glVertex3f(oortmapWidth,-oortmapWidth,0); glVertex3f(0, -oortmapWidth,0);
    glEnd();
    glTranslatef(oortmapWidth / 2.0f, -oortmapWidth / 2.0f, 0);
    Utils::DrawCircle(0, 0, oortmapWidth/2.0f, 20);
    glColor3f(1.0f, 0.0f, 0.0f);
//    glRotatef(45,0,1,0);
//    DrawCircle(0, 0, oortmapWidth/2.0f, 20);

}

