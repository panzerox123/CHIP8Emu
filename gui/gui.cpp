#include "gui.h"
#include "stdlib.h"

GUI::GUI(int w, int h)
{
    this->width = w;
    this->height = h;
    this->window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w, h, 0);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    this->display = (int *) calloc(w*h, sizeof(int));
}

void GUI::init()
{
    this->clear_screen();
    this->draw_point(0,0);
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
    SDL_Rect a = {x,y, 40,40};
    SDL_RenderFillRect(this->renderer, &a);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

void GUI::clear_screen(){
    SDL_RenderClear(this->renderer);
}