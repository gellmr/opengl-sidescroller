
#ifndef UTILS_H
#define UTILS_H

#define VECDISPLAYOFFDIST 1.0f

#define CAM_Z_NEAR 0.01f
#define CAM_Z_FAR 200.0f

using namespace std;

class Utils {

  public:

    const static float vecDisplayOffdist = VECDISPLAYOFFDIST;

    static float vecDisplayOffset;

    static float ratio;

    static float fov;

    static float orthofactor;

    static void renderVec3(glm::vec3 vec, float scalar = 1.0f);
    static void renderDashVec3(glm::vec3 vec, float scalar = 1.0f);

    static void renderOrigin(glm::vec3 pos, float scalar);

    static void translateTo(glm::vec3 pos);

    static void renderText(const unsigned char* text);

    static void displayFloatVal(float myfloat, float xoff = 0.0f, float yoff = 0.0f, char prefix = ' ');

    static void displayVec3(glm::vec3 vec, char prefix = ' ');

    static void OrthoMode();
    static void OrthoModeLeft();

    static void PerspMode();

    static float* GetMat4(glm::mat4 pMat4);

    static float AngleBetweenVec3(glm::vec3 a, glm::vec3 b);

    static void DrawCircle(float cx, float cy, float r, int num_segments);

    static void DrawCone(glm::vec3 pos, glm::vec3 orient, float coneAngle, float scale);

    static void DrawXYQuad(float w, float h);

    static bool PointWithinCircle(glm::vec2 point, glm::vec2 circlePos, float circRadius);

    static void DrawSkyBox(float scale, const float colors[6][3]);

    static int getRandi(int lb, int ub);

    static float makeRandCol();

    static void glColorArgs(const float* vals);

    static glm::quat GetRotationQuat(glm::vec3 coordinateSys, glm::vec3 ou);

    static void GLRotateUsingQuat(glm::quat q);
};
#endif
