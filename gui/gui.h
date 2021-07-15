#ifndef GUI_H
#define GUI_H 1

#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>

class GUI
{
private:
    SDL_Window *window;
    int width;
    int height;
    SDL_Renderer *renderer;
    SDL_Event event;

public:
    int pixels_x;
    int pixels_y;
    int *display;
    GUI(int, int, int, int);
    void loop();
    void draw_point(int, int);
    void clear_screen();
    void render();
};

#endif