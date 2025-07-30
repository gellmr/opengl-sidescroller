
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
#include "mobject.h"
#include "particles.h"
#include "ship.h"
#include "utils.h"
#include "gm.h"
#include "hud.h"

using namespace std;







static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    GM::playership->key_callback(window, key, scancode, action, mods);
}







int main(int argc, char** argv)
{
    std::cout << "Starting..." << std::flush;

    glfwSetErrorCallback(error_callback);

    glutInit(&argc, argv);

    if (!glfwInit()) // init glfw
        exit(EXIT_FAILURE);

    //window = glfwCreateWindow(1920, 1080, "Simple example", NULL, NULL);
    GM::mwindow = glfwCreateWindow(1000, 1000, "Simple example", NULL, NULL);

    Controls::window = GM::mwindow;

    glfwSetWindowPos(GM::mwindow, 250, 50); // move the window right and down the screen a bit so our terminal window has some space on LHS of it.

    if (!GM::mwindow)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(GM::mwindow); // context of our window the main context on the current thread

    glEnable(GL_DEPTH_TEST);

    glfwSetKeyCallback(GM::mwindow, key_callback);

    glfwSetScrollCallback(GM::mwindow, Camera::scroll_callback);



    // Load text from sector zero file. Use as text to screen.

    std::cout << std::flush;
    GM::loadSector(0);
    const unsigned char* hellotext = reinterpret_cast<const unsigned char *>( "HELLO" );

    // generate colors for skybox
    int c = 0;
    const int cols = 6;
    const int components = 3;
    float const skyboxcols[cols][components] = {
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() },
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() },
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() },
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() },
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() },
        { Utils::makeRandCol(), Utils::makeRandCol(), Utils::makeRandCol() }
    };


    Utils::vecDisplayOffset = 0.0f;

    GM::playership = new Ship;
    GM::playership->name = "playership";
    GM::playership->orientation = glm::vec3(1,0,0);
    GM::world.push_back(GM::playership); // Remember to delete all of the Mobject* objects once you are done with the vector.

    glm::vec3 zeroVec = glm::vec3(0,0,0);
    glm::vec3 defPos = GM::playership->position; // glm::vec3(-0.05f, 0.05f, 0);
    glm::vec3 defVel = zeroVec; // GM::playership->orientation; //glm::vec3(-0.01, 0, 0);

    Mobject* shipExhaust = new Particles(120, defPos, defVel, &GM::world);
    shipExhaust->name = "shipExhaust";
    shipExhaust->position = GM::playership->position;
    shipExhaust->velocity = zeroVec;
    shipExhaust->orientation = GM::playership->orientation * -1.0f;
    GM::playership->AddChild(shipExhaust);
    GM::playership->mainThrustEmitter = dynamic_cast<Particles*>(shipExhaust);



    glfwSetMouseButtonCallback(GM::mwindow, GM::playership->mouse_button_callback);

    Camera::playership = GM::playership;

    GM::resetAll(); // prob needs to be in a different class





    // The render loop - keeps on drawing images and handling user input, until the program has been told to stop.
    while (!glfwWindowShouldClose(GM::mwindow))
    {
        //std::cout << "time " << (float) glfwGetTime() << "\n";

        Utils::vecDisplayOffset = 0.0f; // used for displaying rows of text information on the screen.

        glfwGetFramebufferSize(GM::mwindow, &GM::winWidth, &GM::winHeight);
        Utils::ratio = GM::winWidth / (float) GM::winHeight;
        glViewport(0, 0, GM::winWidth, GM::winHeight); // The third and fourth parameter set the width and height of the rendering window in pixels, which we set equal to GLFW's window size.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        Camera::PointAtTarget();

        Controls::GetMouse();
        Controls::worldmpos3d = Controls::GetOGLPos(Controls::mousex, Controls::mousey);

        // Render objects in the world
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        GM::renderGrid();
        Utils::renderText(hellotext); // Render text last so it appears on top of everything else
        GM::renderSectors();
        // render all the world objects
        for(unsigned int i = 0; i < GM::world.size(); i++)
        {
            glMatrixMode(GL_MODELVIEW); glLoadIdentity();

            Mobject * m = GM::world[i];
            //cout << "Render mobject " << i << " " << m->name << endl ;

            m->GLRender(); // render the object and its children
        }
        cout << endl;
        
        // render a quad far behind everything so the z depth testing of cursor will give us decent values.
        glLoadIdentity(); glTranslatef(5, 5, -05);     glColor3f(0.3f, 0.3f, 0.3f); Utils::DrawXYQuad(1.0, 1.0);
        glLoadIdentity(); glTranslatef(5, 5, -19.5);   glColor3f(0.2f, 0.2f, 0.2f); Utils::DrawXYQuad(1.0, 1.0);
        glLoadIdentity();
        glTranslatef(0, -25, 0);
        Utils::DrawSkyBox( 40.0f, skyboxcols);
        //Utils::DrawSkyBox(160.0f, skyboxcols);
        //Utils::DrawSkyBox(320.0f, skyboxcols);

        // Done rendering world objects.
        // Now render debug vectors so they appear over the top of other things.
        if (Controls::mousedown)
        {
            glm::vec3 zv = glm::vec3{0,0,0};

//            glColor3f(1,1,1); // white
//            Utils::renderOrigin(zv, 1.0f);


            glColor3f(1,1,0); // yellow
            glLoadIdentity();
            Utils::renderDashVec3(Controls::playertomouse, 1.0f);
            Utils::translateTo(Controls::worldmpos3d);
            glColor3f(0,1,1); // cyan
            Utils::renderOrigin(zv, 1000.0f);

            // MIGHT NEED BULLET.
            // http://www.opengl-tutorial.org/miscellaneous/clicking-on-objects/picking-with-a-physics-library/

            glColor3f(1,0,1); // magenta
            glLoadIdentity();
            Utils::translateTo(GM::playership->position);
            Utils::renderOrigin(zv, 1.0f);

//            glTranslatef(
//                GM::playership->position.x,
//                GM::playership->position.y,
//                GM::playership->position.z
//            );
//            Utils::renderOrigin(zv, 1.0f);

            //Utils::renderOrigin(glm::vec3{0,0,0}, 1.0f);

            //Utils::renderOrigin(glm::vec3{0,0,0}, 1.0f);

            //glTranslatef(Controls::playertomouse.x, Controls::playertomouse.y, Controls::playertomouse.z);
            //glTranslatef(1,0,0);
            //Utils::renderOrigin(glm::vec3{0,0,0}, 1.0f);
        }


        // Render the HUD. This appears over the top of everything else.
        glMatrixMode(GL_MODELVIEW); glLoadIdentity();
        Hud::renderHUD();





        // all the rendering commands draw to the back buffer.
        // As soon as all the rendering commands are finished we swap the back buffer to the front buffer
        // so the image can be displayed without still being rendered to. This avoids seeing artefacts.
        glFlush ();
        glfwSwapBuffers(GM::mwindow);

        // checks if any events are triggered (like keyboard input or mouse movement events),
        // updates the window state, and calls the corresponding functions
        // which we can register via callback methods
        glfwPollEvents();
    }
    GM::saveSector(0);
    glfwDestroyWindow(GM::mwindow);

    // As soon as we exit the render loop we would like to properly
    // clean/delete all of GLFW's resources that were allocated
    glfwTerminate();

    exit(EXIT_SUCCESS);
}

