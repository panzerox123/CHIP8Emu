#include "gui.h"
#include "stdlib.h"

GUI::GUI(int w, int h, int pix_x, int pix_y)
{
    this->width = w;
    this->height = h;
    this->pixels_x = pix_x;
    this->pixels_y = pix_y;
    this->window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w - w%pix_x, h-h%pix_y, SDL_WINDOW_FULLSCREEN);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    this->display = (int *) calloc(pix_x*pix_y, sizeof(int));
}

void GUI::init()
{
    this->clear_screen();
    this->draw_point(63,31);
    this->render();
    while (1)
    {
        while (SDL_PollEvent(&(this->event)))
        {
            switch (this->event.type)
            {
            case SDL_QUIT:
                exit(0);
            }
        }
    }
}

void GUI::render(){
    SDL_RenderPresent(this->renderer);
}

void GUI::draw_point(int x, int y){
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    int cell_width = this->width/this->pixels_x;
    int cell_height = this->height/this->pixels_y;

    SDL_Rect a = {x*cell_width, y*cell_height, cell_width, cell_height};
    SDL_RenderFillRect(this->renderer, &a);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

void GUI::clear_screen(){
    SDL_RenderClear(this->renderer);
}