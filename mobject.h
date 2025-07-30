
#ifndef MOBJECT_H
#define MOBJECT_H


#include <GL/gl.h>

#include<iostream>
#include <vector>

#include <glm/glm.hpp>

using namespace std;

// movable object like a ship, player, asteroid, crate, car. With position and velocity.
class Mobject {

  public:

    string name;

    glm::vec3 position;
    glm::vec3 velocity;
    glm::vec3 orientation;
    glm::mat4 mv;


    float odelta;
    float velrotate;

    Mobject* parent;

    vector<Mobject*> children; // dynamic length array of pointers to my children.

    Mobject();

    void AddChild(Mobject* child);

    glm::vec3 GetAbsPos();

    virtual void GLRender();
    virtual void GLRenderChildren();
};



#endif
