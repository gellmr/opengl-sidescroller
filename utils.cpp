
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <GL/freeglut.h>


#include <stdio.h>
#include <math.h>
#include <cstdlib>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define RAD_TO_DEG(rad) (rad*180.0/M_PI)
#define DEG_TO_RAD(deg) (deg*M_PI/180.0)

#include <cmath>/// for M_PI

#include "utils.h"

float Utils::vecDisplayOffset;

float Utils::ratio = 0;

float Utils::fov = 37.0f;

float Utils::orthofactor = 30.0f;

void Utils::OrthoMode(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(
        -Utils::ratio * orthofactor,  // left
         Utils::ratio * orthofactor,  // right
         -1.f  * orthofactor,  // bottom
          1.f  * orthofactor,  // top
          1.f, // near
         -1.f  // far
    );
}

void Utils::OrthoModeLeft(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    float width = Utils::ratio * orthofactor;
    float height = orthofactor;

    glOrtho(
         0 ,      // left
         width ,  // right
         0 - height,         // bottom
         height - height,    // top
           1.f, // near
          -1.f  // far
    );
}

void Utils::PerspMode(){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(fov * 3, Utils::ratio, CAM_Z_NEAR, CAM_Z_FAR);
}


void Utils::renderVec3(glm::vec3 vec, float scalar){
    glPushMatrix();
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    if(scalar == 1.0f){
        glVertex3f(vec.x, vec.y, vec.z);
    }else{
        vec = glm::normalize(vec);
        glVertex3f(
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar
        );
    }
    glEnd();
    glPopMatrix();
}

void Utils::renderDashVec3(glm::vec3 vec, float scalar)
{
    glPushAttrib(GL_ENABLE_BIT);  // glPushAttrib is done to return everything to normal after drawing

    glLineStipple(1, 0x00FF);
    glEnable(GL_LINE_STIPPLE);
    glBegin(GL_LINES);
    glVertex3f(0, 0, 0);
    if(scalar == 1.0f){
        glVertex3f(vec.x, vec.y, vec.z);
    }else{
        vec = glm::normalize(vec);
        glVertex3f(
            vec.x * scalar,
            vec.y * scalar,
            vec.z * scalar
        );
    }
    glEnd();
    glPopAttrib();
}

void Utils::renderOrigin(glm::vec3 pos, float scalar)
{
    float unit = 0.5f;

    glPushMatrix();
    glTranslatef(pos.x, pos.y, pos.z); glScalef(scalar, scalar, scalar); //glColor3f(1.0f, 1.0f, 1.0f);

    glBegin(GL_LINES);
    glVertex3f(pos.x - unit, pos.y,        pos.z);         glVertex3f(pos.x + unit, pos.y,        pos.z);
    glVertex3f(pos.x,        pos.y - unit, pos.z);         glVertex3f(pos.x,        pos.y + unit, pos.z);
    glVertex3f(pos.x,        pos.y,        pos.z - unit);  glVertex3f(pos.x,        pos.y,        pos.z + unit);
    glEnd();

    glPopMatrix();
}

void Utils::translateTo(glm::vec3 pos){
    glTranslatef(
        pos.x,
        pos.y,
        pos.z
    );
}


void Utils::renderText(const unsigned char* text){
    glPushMatrix();
    glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

    glRasterPos2f(0,  0);     // center

    glutBitmapString(GLUT_BITMAP_9_BY_15, text);
    glPopMatrix();
}

void Utils::displayFloatVal(float myfloat, float xoff, float yoff, char prefix)
{
    glPushMatrix(); glTranslatef(xoff, yoff, 0);

    char charVal[20];
    sprintf(charVal, "%c %f", prefix, myfloat);
    const unsigned char* mytext = reinterpret_cast<const unsigned char *>(charVal);

    renderText(mytext);
    glPopMatrix();
}


void Utils::displayVec3(glm::vec3 vec, char prefix){

    displayFloatVal(vec.x,  0.0f,   vecDisplayOffset, prefix);
    displayFloatVal(vec.y,  5.0f,   vecDisplayOffset);
    displayFloatVal(vec.z, 10.0f,   vecDisplayOffset);

    vecDisplayOffset -= Utils::vecDisplayOffdist;
}

float* Utils::GetMat4(glm::mat4 mat)
{
    float *a = new float[16];
    const float *vals = (const float*)glm::value_ptr(mat);
    for (int i = 0; i < 16; ++i){
        a[i] = vals[i];
    }
    return a;
}

float Utils::AngleBetweenVec3(glm::vec3 a, glm::vec3 b)
{
    float aDotB = glm::dot(a, b);
    float magA = a.length();
    float magB = b.length();
    float degrees = (
        aDotB
        /
        (magA * magB)
    );
    float radians = DEG_TO_RAD(degrees);
    float theta = acosf(radians);
    return theta;
}

void Utils::DrawCircle(float cx, float cy, float r, int num_segments) {
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++)   {
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float x = r * cosf(theta);//calculate the x component
        float y = r * sinf(theta);//calculate the y component
        glVertex3f(x + cx, y + cy, 0);//output vertex
    }
    glEnd();
}

void Utils::DrawCone(glm::vec3 pos, glm::vec3 orient, float coneAngle, float scale){

    //glColor3f(1,1,0);                // DEBUG ONLY
    //glm::vec3 v1 = glm::vec3(1,0,0); // DEBUG ONLY
    //Utils::renderVec3(v1, scale);    // DEBUG ONLY

    glPushMatrix();

    // calculate stuff before we transform
    float halfPeakRadians = DEG_TO_RAD( coneAngle / 2.0f ); // divide cone in half so we can work with right triangle. Get the peak angle in radians.
    float x = cosf(halfPeakRadians); // x is how broad our cone is.
    float y = sinf(halfPeakRadians); // y is how tall our cone is.

    glm::vec3 coordinateSys = glm::vec3(1,0,0);              // A
    glm::vec3 ou = glm::normalize(orient);                   // B
    glm::quat q = Utils::GetRotationQuat(coordinateSys, ou); // A cross B
    float odelta = q.w;

//    float orientAngle = RAD_TO_DEG( AngleBetweenVec3(coordinateSys, orient) );
//    glRotatef(orientAngle, 0, 0, 1);
    //Utils::GLRotateUsingQuat(q);

    // transform
    glm::mat4 mv = glm::mat4(1.0);
    mv = glm::translate(mv, pos);
    mv = glm::rotate(mv, glm::radians(odelta), ou);
    float * t = Utils::GetMat4(mv); // uses new
    glMultMatrixf(t); // apply transformations

    // Draw the cone
    x = x * scale;
    y = y * scale;
    glBegin(GL_LINES);
        glVertex3f(0,  0, 0);
        glVertex3f(x,  y, 0);
        glVertex3f(x, -y, 0);
        glVertex3f(0,  0, 0);
    glEnd();
    glBegin(GL_LINES);
        glVertex3f(x,  0,  y);
        glVertex3f(0,  0,  0);
        glVertex3f(x,  0, -y);
        glVertex3f(0,  0,  0);
    glEnd();
    // The circle part of the cone
    glTranslatef(x, 0, 0);
    int num_segments = 20;
    float r = float(y);
    glBegin(GL_LINE_LOOP);
    for (int ii = 0; ii < num_segments; ii++){
        float theta = 2.0f * 3.1415926f * float(ii) / float(num_segments);//get the current angle
        float zz = r * cosf(theta);//calculate the z component
        float yy = r * sinf(theta);//calculate the y component
        glVertex3f(0, yy, zz);//output vertex
    }
    glEnd();

    delete[] t; // free up memory
    glPopMatrix();
}

// CoordinateSys is a vector like 1,0,0 that represents our reference coordinate system.
// ou is a unit vector telling the orientation of a vector in that coordinate system.
// If these two vectors are A and B, this function uses cross product A x B to find the
// axis of rotation between the two vectors (using Right hand rule) and the amount of rotation
// between the angles (in radians) as w. We then return the quaternion q(x,y,z,w)
// which can be used to perform a rotation in the coordinate space of A, to rotate to B.
glm::quat Utils::GetRotationQuat(glm::vec3 coordinateSys, glm::vec3 ou)
{
    // USEFUL LINKS
    // https://danceswithcode.net/engineeringnotes/quaternions/quaternions.html#:~:text=The%20quaternion%20q%20%3D%20(1%2C,It%20represents%20no%20rotation.
    // https://www.geeksforgeeks.org/angle-between-two-vectors-formula/
    // https://stackoverflow.com/questions/1171849/finding-quaternion-representing-the-rotation-from-one-vector-to-another

    // special cases
    // (1) angle between the vectors is zero degrees. ( vectors are parallel )
    // (2) user passes zero length vector as argument. just return no rotation.
    glm::quat noRotation = glm::quat(1,0,0,0); // return this if we have a special case.
    if (coordinateSys.length() == 0 || ou.length() == 0){
        return noRotation;
    }

    // Ready to calculate
    glm::quat q;
    glm::vec3 a = glm::cross(coordinateSys, ou);
    q.x = a.x;
    q.y = a.y;
    q.z = a.z;
    q.w = sqrt( pow(coordinateSys.length(), 2.0f) * pow(ou.length(), 2.0f) ) + glm::dot(coordinateSys, ou);
    glm::normalize(q);
    return q;
}

bool Utils::PointWithinCircle(glm::vec2 point, glm::vec2 circlePos, float radius)
{
    float x = point.x;
    float y = point.y;
    float center_x = circlePos.x;
    float center_y = circlePos.y;
    //  In general, x and y must satisfy (x - center_x)² + (y - center_y)² < radius².
    //     Please note that points that satisfy the above equation with < replaced by == are considered the points
    //     on the circle, and the points that satisfy the above equation with < replaced by > are considered the
    //     outside the circle
    return pow((x - center_x), 2.0f) + pow((y - center_y), 2.0f) < pow(radius, 2.0f);
}

// Given rotation quaternion q, apply a glRotatef to perform the rotation.
void Utils::GLRotateUsingQuat(glm::quat q)
{
    const glm::mat4 m = glm::mat4_cast(q);
    glMultMatrixf(glm::value_ptr(m));
}


void Utils::DrawXYQuad(float w, float h)
{
    float wide = w / 2.0f;
    float high = h / 2.0f;
    glBegin(GL_QUADS);
        glVertex3f(-wide,  high, 0);
        glVertex3f( wide,  high, 0);
        glVertex3f( wide, -high, 0);
        glVertex3f(-wide, -high, 0);
    glEnd();
}


void Utils::DrawSkyBox(float scale, const float colors[6][3])
{

    float hwide = 30.0f;
    float hhigh = 30.0f;

    glPushMatrix();
    //glLoadIdentity();

    glScalef(scale,scale,scale);

    glColor3f(0, 0.05f, 0.2f);

    glBegin(GL_QUADS);

    int c = 0;


    // top
    glColorArgs(colors[c++]);
    glNormal3f(0.0f, 1.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // behind cam
    glColorArgs(colors[c++]);
    glNormal3f(0.0f, 0.0f, 1.0f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);

    // far wall
    glColorArgs(colors[c++]);
    glNormal3f(0.0f, 0.0f, -1.0f);
    glVertex3f(0.5f, -0.5f,  -0.5f);
    glVertex3f(0.5f, 0.5f,   -0.5f);
    glVertex3f(-0.5f, 0.5f,  -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // right
    glColorArgs(colors[c++]);
    glNormal3f(1.0f, 0.0f, 0.0f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);

    // left
    glColorArgs(colors[c++]);
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    // bottom
    glColorArgs(colors[c++]);
    glNormal3f(0.0f, -1.0f, 0.0f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);

    glEnd();
    glPopMatrix();
}



float Utils::makeRandCol()
{
//    int ub = 255;
    int ub = 64;
    return Utils::getRandi(1, ub) / 255.0f;
}

void Utils::glColorArgs(const float* vals)
{
    glColor3f( vals[0], vals[1], vals[2] );
}

// calling getRand(0,10) will return a number between 0-10
int Utils::getRandi(int lb, int ub)
{
    return lb + (std::rand() % (ub - lb + 1)); // https://stackoverflow.com/questions/7560114/random-number-c-in-some-range
}

