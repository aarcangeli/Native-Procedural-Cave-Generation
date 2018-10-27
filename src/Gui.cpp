#include "Gui.h"
#include "iostream"

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sdl_gl2.h"

Gui::Gui(SDL_Window *window) {
    ctx = nk_sdl_init(window);
    nk_sdl_font_stash_begin(&atlas);
    nk_sdl_font_stash_end();
}

void Gui::inputBegin() {
    nk_input_begin(ctx);
}

int Gui::inputhandleEvent(SDL_Event &event) {
    // workaround mouse disappear
    ctx->input.mouse.grab = 0;
    ctx->input.mouse.ungrab = 0;
    ctx->input.mouse.grabbed = 0;

    if (event.type == SDL_MOUSEMOTION) {
        mouseX = event.motion.x;
        mouseY = event.motion.y;
    }

    // mark input as grabbed just if mouse is inside a window
    return nk_sdl_handle_event(&event) && isInsideAWindow;
}

void Gui::inputEnd() {
    nk_input_end(ctx);
    isInsideAWindow = false;
}

void Gui::render() {
    nk_sdl_render(NK_ANTI_ALIASING_ON);
}

void Gui::resize(int _width, int _height) {
    width = _width;
    height = _height;
}

void Gui::editor(const char *string, NoiseParams &params) {
    static nk_colorf bg;

    int windowWidth = SIDEBAR_WIDTH;
    int windowHeight = height;
    struct nk_rect bounds = nk_rect(width - windowWidth, 0, windowWidth, windowHeight);
    static NoiseParams defaultValues = params;

    if (nk_begin(ctx, "Noise", bounds, NK_WINDOW_BORDER | NK_WINDOW_MOVABLE)) {
        nk_layout_row_dynamic(ctx, 30, 1);
        nk_label(ctx, "View:", NK_TEXT_LEFT);
        nk_radio_label(ctx, "Real Time", &params.realtime);
        nk_label(ctx, "Mode:", NK_TEXT_LEFT);
        if (nk_option_label(ctx, "Noise Map", params.mode == DRAW_MODE_NOISE)) params.mode = DRAW_MODE_NOISE;
        if (nk_option_label(ctx, "Colour Map", params.mode == DRAW_MODE_COLOURS)) params.mode = DRAW_MODE_COLOURS;

        nk_label(ctx, "Noise:", NK_TEXT_LEFT);
        nk_property_int(ctx, "Octaves:", 1, &params.octaves, 10, 1, 1);
        nk_property_float(ctx, "Persistence:", 0, &params.persistence, 1, 0.01, 0.01);
        nk_property_float(ctx, "Lacunarity:", 0, &params.lacunarity, 10, 0.05, 0.05);
        nk_layout_row_dynamic(ctx, 30, 3);
        nk_label(ctx, "Offset:", NK_TEXT_LEFT);
        nk_property_float(ctx, "X:", -100, &params.offsetX, 100, 0.1, 0.1);
        nk_property_float(ctx, "Y:", -100, &params.offsetY, 100, 0.1, 0.1);
        nk_layout_row_dynamic(ctx, 30, 1);
        if (nk_button_label(ctx, "Defaults")) {
            params.persistence = defaultValues.persistence;
            params.lacunarity = defaultValues.lacunarity;
            params.offsetX = defaultValues.offsetX;
            params.offsetY = defaultValues.offsetY;
        }

        if (!params.realtime) {
            if (nk_button_label(ctx, "Generate")) params.refreshRequested = 1;
        }
    }

    struct nk_rect rect = nk_window_get_bounds(ctx);
    if (mouseX >= rect.x &&
        mouseY >= rect.y &&
        mouseX < rect.x + rect.w &&
        mouseY < rect.y + rect.h) {
        isInsideAWindow = true;
    }

    nk_end(ctx);
}