
#ifndef GM_H
#define GM_H

#include <GLFW/glfw3.h>

#include "ship.h"

#define SECTOR_WIDTH 10.0f

using namespace std;

class GM {

  public:

    static const float sectorwidth;
    static const float sectorhalf;
    static const glm::vec3 sectorsOrigin;

    static int startSector;
    static int currentSector;

    static vector<Mobject*> world;

    static Ship* playership;

    static GLFWwindow* mwindow;
    
    static int winWidth;
    static int winHeight;

    static void resetAll();

    static void renderGrid();

    static void loadSector(int sectorIdx);

    static int saveSector(int sectorIdx);

    static void renderWireframeRect(float xmin, float xmax, float ymin, float ymax);

    static void renderRect(int s);

    static void renderSectors();

};

#endif
