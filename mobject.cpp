#include <GL/gl.h>
#include <GLFW/glfw3.h>

// OpenGL mathematics libraries used for transformation
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include<iostream>
#include <vector>

#include "mobject.h"
#include "utils.h"

using namespace std;


Mobject::Mobject ()
{
    glm::vec3 position = glm::vec3(0,0,0);
    glm::vec3 velocity = glm::vec3(0,0,0);
    glm::vec3 orientation = glm::vec3(0,0,0);

    float odelta = 0; // this is w for our change in orientation

    glm::mat4 mv = glm::mat4(1.0f); // perform transformations on this matrix and then apply when we call render.

    velrotate = 0.0f;

    parent = NULL;
}

// traverse up chain of objects and add position vectors together.
glm::vec3 Mobject::GetAbsPos(){

    glm::vec3 sum = position;

    Mobject* myParent = parent;

    if(myParent){
        sum = sum + myParent->GetAbsPos();
    }
    return sum;
}

// Add a child object
// Pass a reference to a newly created mobject. Stored internally as pointer.
void Mobject::AddChild(Mobject* child)
{
    child->parent = this;
    children.push_back(child);
}

void Mobject::GLRender()
{
    //cout << "render the Mobject." << endl;
    glPushMatrix();

    // Matrix transforms are applied in the reverse order to gl calls.
    mv = glm::mat4(1.0);
    mv = glm::translate(mv, position);
    mv = glm::rotate(mv, glm::radians(odelta), orientation);
    float * t = Utils::GetMat4(mv); // uses new
    glMultMatrixf(t); // apply transformations
    
    glColor3f(1,1,1); glm::vec3 zv = glm::vec3{0,0,0}; Utils::renderOrigin(zv, 1.0f); // render an origin
    
    Mobject::GLRenderChildren();

    delete[] t; // free up memory
    glPopMatrix();
    return;
}

void Mobject::GLRenderChildren()
{
    // render all child objects
    for(unsigned int i = 0; i < children.size(); i++)
    {
        glPushMatrix();
        Mobject * m = children[i]; m->GLRender();
        glPopMatrix();
    }
}


//void Mobject::GLR(){
//    glTranslatef(position.x, position.y, position.z); // get ready to draw ship
//
//    const float t[16] = {
//        1,            0,          0,          0,
//        position.x,   1,          0,          0,
//        position.y,   0,          1,          0,
//        position.z,   0,          0,          1
//    };
//    glMultMatrixf( t );
//}
