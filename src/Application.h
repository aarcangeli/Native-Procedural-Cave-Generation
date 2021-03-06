#ifndef APPLICATION_H
#define APPLICATION_H

#include "config.h"
#include <SDL.h>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ResourceLoader.h"
#include "ApplicationRenderer.h"
#include "utils/CameraHandler.h"
#include "utils/PerlinNoise.h"
#include "Mesh.h"
#include "Shader.h"
#include "Gui.h"

class Application {
public:
    Application();

    int runLoop();

    ResourceLoader *getLoader();
    bool wireframe = false;

    const glm::mat4 getViewMatrix() const {
        return glm::make_mat4(camera.getViewMatrix());
    }

private:
    Gui gui;
    CameraHandler camera;
    SDL_Window *window;
    ResourceLoader loader;

    LandmassParams params;
    ApplicationRenderer renderer;
    LandmassGenerator generator;
    ChunkData chunk;

    void initGL();
    void updateState();

    int lastUpdate = 0;
    int lastDigest = 0;
    int lastFps = 0;
    int count = 0;

    void resetDefaultParams();
};


#endif //APPLICATION_H
