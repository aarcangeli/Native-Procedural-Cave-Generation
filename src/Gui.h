#ifndef GUI_H
#define GUI_H

#include <SDL2/SDL.h>
#include <vector>
#include "LandmassGenerator.h"

struct nk_context;
struct nk_font_atlas;

const int SIDEBAR_WIDTH = 250;

class Gui {
    bool isFirstEditor = true;
    nk_context *ctx;
    nk_font_atlas *atlas;
    int width, height;
    bool isGrabbingGuiLeft = false;
    bool isGrabbingLeft = false, isGrabbingMiddle = false, isGrabbingRight = false;
    LandmassParams defaultValues;
    LandmassParams lastValues;

public:
    Gui(SDL_Window *window);

    // wrappers of nk_*
    void inputBegin();
    int inputhandleEvent(SDL_Event &event);
    void inputEnd();
    void render();

    // custom editors
    void editor(const char *string, LandmassParams &params);

    void resize(int width, int height);
};

#endif //GUI_H
