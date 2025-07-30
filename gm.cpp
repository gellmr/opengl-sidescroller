
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

#define SECTOR_WIDTH 10.0f

using namespace std;

const float GM::sectorwidth = SECTOR_WIDTH;
const float GM::sectorhalf = (SECTOR_WIDTH / 2.0f);
const glm::vec3 GM::sectorsOrigin = glm::vec3(-SECTOR_WIDTH, SECTOR_WIDTH, 0);

int GM::startSector = 4;
int GM::currentSector = 0;

vector<Mobject*> GM::world;

Ship* GM::playership;

GLFWwindow* GM::mwindow = NULL;

int GM::winWidth;
int GM::winHeight;




void GM::resetAll(){
    GM::playership->position = glm::vec3(0,0,0);
    GM::playership->velocity = glm::vec3(0,0,0);
    GM::playership->velrotate = 0;
    GM::playership->orientation = glm::vec3(1,0,0);

    //currentSector = startSector;
}




void GM::renderGrid(){
    glMatrixMode(GL_MODELVIEW); glLoadIdentity();

    glBegin(GL_LINES);
    glColor3f(1.f, 0.f, 0.f);

    const float dim = 0.4f;

    float maxval = static_cast< float >( INT_MAX );

    glColor3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.0f,  0.0f, 0.0f); glVertex3f(maxval,   0.0f, 0.0f); // positive X AXIS LINES

    glColor3f(dim, 0.0f, 0.0f);
    glVertex3f(0.0f,  0.0f, 0.0f); glVertex3f(-maxval,   0.0f, 0.0f); // negative X


    glColor3f(0.0f, 1.0f, 0.0f);
    glVertex3f(0.0f,  0.0f, 0.0f); glVertex3f(0.0f,   maxval, 0.0f); // positive Y AXIS LINES

    glColor3f(0.0f, dim, 0.0f);
    glVertex3f(0.0f,  0.0f, 0.0f); glVertex3f(0.0f,   -maxval, 0.0f); // negative Y

//    glColor3f(0.0f, 0.0f, 1.0f);
//    glVertex3f(0.0f,    0.0f, 0.0f); glVertex3f(0.0f,    0.0f, maxval);  // positive Z AXIS LINES
//
//    glColor3f(0.0f, 0.0f, dim);
//    glVertex3f(0.0f,    0.0f, 0.0f); glVertex3f(0.0f,    0.0f, -maxval); // negative Z

    glEnd();
}




void GM::loadSector(int sectorIdx){
    std::ifstream ifs;
    std::string myfilename = "./savedsectors/example " + std::string(".txt");
    ifs.open (myfilename.c_str(), std::ifstream::in);
    char c = ifs.get();
    while (ifs.good()) {
        std::cout << c;
        c = ifs.get();
    }
    std::cout << std::flush;
    ifs.close();
}




// see https://cplusplus.com/doc/tutorial/files/
int GM::saveSector(int sectorIdx){
    ofstream myfile;
    //myfile.open ("./savedsectors/example.txt");

    std::string myfilename = "./savedsectors/example " + std::string(".txt");
    myfile.open (myfilename.c_str());
    myfile << "Writing this to a file.\n";
    myfile.close();
    return 0;
}





void GM::renderWireframeRect(float xmin, float xmax, float ymin, float ymax){
    glBegin(GL_LINE_STRIP);
    glVertex2f(xmin, ymin); glVertex2f(xmax, ymin);
                            glVertex2f(xmax, ymax);
    glVertex2f(xmin, ymax);
    glVertex2f(xmin, ymin);
    glEnd();
}




void GM::renderRect(int s){

    s == GM::currentSector ? glColor3f(0.75f, 0.75f, 0.0f) : glColor3f(0.25f, 0.25f, 0.0f) ;

    float e = 0.0f;
    s == currentSector ? e = 0.003 : e = 0.0;

//    float halfwidth = sectorwidth * 0.5;
    renderWireframeRect(-sectorhalf + e, sectorhalf - e, -sectorhalf + e, sectorhalf - e);
}





void GM::renderSectors(){
    glMatrixMode(GL_MODELVIEW); glLoadIdentity(); // get ready to transform the drawing position

    //glColor3f(0.25f, 0.25f, 0.0f);

    //draw nine sectors, the center is the one we are in.
    int s = 0;

    glTranslatef(sectorsOrigin.x, sectorsOrigin.y, sectorsOrigin.z);
    //glTranslatef(-sectorwidth, sectorwidth, 0.0);
    glPushMatrix();
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    glPopMatrix();
    glTranslatef(0.0, -sectorwidth, 0.0);
    glPushMatrix();
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    renderRect(s++); //glTranslatef(sectorwidth, 0.0, 0.0);

    // Render a square somewhere else on the z axis so we can see if perspective is working.
    /**/
    glColor3f(0.25f, 0.25f, 0.0f);
    float halfwidth = sectorwidth * 0.5;
    glPushMatrix();
        glTranslatef(0.0f, 0.0f, -halfwidth);   renderWireframeRect(-halfwidth, halfwidth, halfwidth, -halfwidth);
        glTranslatef(0.0f, 0.0f,  sectorwidth); renderWireframeRect(-halfwidth, halfwidth, halfwidth, -halfwidth);
    glPopMatrix();
    glTranslatef(sectorwidth, 0.0, 0.0);


    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    glPopMatrix();
    glTranslatef(0.0, -sectorwidth, 0.0);
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
    renderRect(s++); glTranslatef(sectorwidth, 0.0, 0.0);
}
