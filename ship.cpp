
#include "ship.h"
#include "mobject.h"
#include "utils.h"
#include "controls.h"
#include "camera.h"
#include "gm.h"

#include <GL/gl.h>
#include <GLFW/glfw3.h>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>


#define RAD_TO_DEG(rad) (rad*180.0/M_PI)
#define DEG_TO_RAD(deg) (deg*M_PI/180.0)

#define VECDISPLAYOFFS 1.0f

#define SHIPDEBUG true

using namespace std;

static int leftJet = 0;
static int rightJet = 0;
static int mainJet = 0;
static int retroJet = 0;
static int brakes = 0;

static glm::vec3 playerInitMovement;


Ship::Ship (){
    position           = glm::vec3(0.15,   0.11,     0.0); // playerPosition
    velocity           = glm::vec3(0,      0,        0);   // playerDrift
    playerInitMovement = glm::vec3(0.01,   0.000,    0.0); // drifting upwards-right
    orientation        = glm::vec3(0.0,    0.0,      0.0); // orientation

    thrustjetpow = 0.002f;

    velrotate = 0.0f; // rotating in space

    Utils::vecDisplayOffset = 0.0f;
}


void Ship::key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if ((key == GLFW_KEY_ESCAPE || key == GLFW_KEY_Q) && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GL_TRUE);
    }

    if (key == GLFW_KEY_R && action == GLFW_PRESS){
        GM::resetAll();
    }

    leftJet = glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS ? 1 : 0;
    rightJet = glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ? 1 : 0;
    mainJet = glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS ? 1 : 0;
    retroJet = glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS ? 1 : 0;
    brakes = glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS ? 1 : 0;

    if (key == GLFW_KEY_O && action == GLFW_PRESS){
        Controls::ortho == 1 ? Controls::ortho = 0 : Controls::ortho = 1;
    }
}



void Ship::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT)
    {
        if (action == GLFW_PRESS)
        {
            Controls::mousedown = 1;
            cout << "click" << endl;
        }
        else if (action == GLFW_RELEASE)
        {
            Controls::mousedown = 0;
            cout << "release" << endl;
        }
    }
}


glm::vec3 Ship::GetImpulse(float direction)
{
    return GetImpulseU(direction) * thrustjetpow;
}

glm::vec3 Ship::GetImpulseU(float direction)
{
    float radians = DEG_TO_RAD(direction); // We need radians to calculate the unit vector for altering our velocity.
    float ux = cos(radians);
    float uy = sin(radians);
    return glm::vec3( ux, uy, 0);
}



void Ship::GLRender ()
{
    //cout << "render the ship." << endl;

    glPushMatrix();

    if (Controls::mousedown){
        cout << "...down   v:" << velocity.x << endl;
    }

    Controls::playertomouse = glm::vec3(
        Controls::worldmpos3d.x - position.x,
        Controls::worldmpos3d.y - position.y,
        Controls::worldmpos3d.z - position.z
    );

    // use turn jets to affect velrotate. Then apply braking and affect velrotate. Then integrate velrotate into orientation.

    // left and right jets let the player turn the ship. Current direction is stored as orientation, in degrees.

    float turnjetpower = 0.05f;   // turning jets. More powerful because turning is in degrees.
    float turnjetbrakepower = turnjetpower * 3.0f;
    float brakepow = thrustjetpow; // might want less powerful brakes later

    glm::vec3 zv = glm::vec3(0,0,0);
    glm::vec3 impulse  = glm::vec3(0,0,0);
    glm::vec3 impulseu = glm::vec3(0,0,0);

    float impulser = 0.0f;       // the amount to adjust our velrotate by

    float theta = 0.0f;
    float radians = DEG_TO_RAD(theta);

    float ux = 0.0f;
    float uy = 0.0f;

    float driftMagnitude = 0.0f;

    mainThrustEmitter->active = (brakes || mainJet || retroJet) ? true : false;

    // See what jets are firing (main, retro, brake) to calculate the update for our velocity.
    if (brakes)
    {
        // are we spinning?
        if (velrotate != 0){
            if(velrotate > 0){
                impulser = -turnjetbrakepower; // decrease rotation
            }else{
                impulser = turnjetbrakepower; // decrease rotation
            }
        }

        //velrotate < 0 ? impulser = turnjetpower : impulser = -turnjetpower;
        velrotate = velrotate + impulser; // increase or decrease the speed we are spinning.
        orientation.z = orientation.z + velrotate; // apply rotation.
        theta = orientation.z;

        // hold space to cancel drift until you become stationary
        driftMagnitude = velocity.length();

        // get the reverse vector
        glm::vec3 reverseDrift = glm::vec3(velocity.x * -1.0f, velocity.y * -1.0f, velocity.z * -1.0f);

        if (brakepow > driftMagnitude){ brakepow = driftMagnitude * 0.5; } // dont exceed velocity.

        reverseDrift = glm::normalize(reverseDrift); // get a unit vector.

        reverseDrift = reverseDrift * brakepow;

        velocity = velocity + reverseDrift; // apply brakes

        mainThrustEmitter->orientation = velocity; // update emitter
    }
    else
    {
        // not braking.

        // Are we using left or right jets?
        if (leftJet){ impulser += turnjetpower; } else if (rightJet){ impulser -= turnjetpower; }
        velrotate = velrotate + impulser; // increase or decrease the speed we are spinning.
        orientation.z = orientation.z + velrotate; // apply rotation.
        // finished updating orientation

        theta = orientation.z;      // orientation.z as degrees

        // apply thrust in direction
        if (mainJet)
        {
            velocity = velocity + GetImpulse(theta);
            mainThrustEmitter->orientation = GetImpulseU(theta) * -1.0f;
        }
        else if(retroJet)
        {
            velocity = velocity + GetImpulse(theta) * -1.0f;
            mainThrustEmitter->orientation = GetImpulseU(theta);
        }
    }

    odelta = theta; // save to mobject

    Camera::cameraDistance = Camera::cameraMinDistance + (glm::length(velocity) * Camera::cameraVDistanceFactor);
    Camera::cameraDistance = Camera::cameraMinDistance; // DEBUG ONLY

    position = position + velocity; // apply drift

    ///glTranslatef(position.x, position.y, position.z); // get ready to draw ship
    ///glRotatef(theta, 0, 0, 1); // get ready to draw the ship


    glPushMatrix();
    mv = glm::mat4(1.0);
    mv = glm::translate(mv, position);
    mv = glm::rotate(mv, glm::radians(odelta), orientation);
    float * t = Utils::GetMat4(mv); // uses new
    glMultMatrixf(t);
    glColor3f(1,1,1); Utils::renderOrigin(zv, 1.0f);

    // render the ship
    glColor3f(1,0,0); glBegin(GL_LINE_LOOP); glVertex3f(-0.5f,0.5f,0.0f); glVertex3f(1,0,0); glVertex3f(-0.5f,-0.5f,0.0f); glEnd();
    Mobject::GLRenderChildren();

    // Draw range circles around ship.
    glm::vec3 zer = glm::vec3(0,0,0);
    glm::vec2 cursorXY = glm::vec2(Controls::worldmpos3d.x, Controls::worldmpos3d.y);
    glm::vec2 circlePos = glm::vec2(position.x, position.y);
    //glm::vec2 circlePos = glm::vec2(zer.x, zer.y);
    float radiusClose = 5.0f;
    float radiusFar = 15.0f;
    int numSegs = 35;
    bool cursorCloseRange = Utils::PointWithinCircle(cursorXY, circlePos, radiusClose);
    bool cursorFarRange = Utils::PointWithinCircle(cursorXY, circlePos, radiusFar);
    glColor3f(0.4f, 0.f, 0.f);
    glColor3f(0.8f, 0.f, 0.f);
    if (cursorCloseRange){
        glLoadIdentity();
        Utils::DrawCircle(circlePos.x, circlePos.y, radiusClose, numSegs);
    }
    if (cursorFarRange){
        glLoadIdentity();
        Utils::DrawCircle(circlePos.x, circlePos.y, radiusFar, numSegs);
    }
    if (Controls::mousedown){
        cout << "...worldmpos:x" << Controls::worldmpos3d.x << " y" << Controls::worldmpos3d.y << " z" << Controls::worldmpos3d.z << "  radiusClose:" << radiusClose << endl;
    }

    delete[] t; // free up memory
    glPopMatrix();
    return;
}
