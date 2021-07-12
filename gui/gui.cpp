#include "gui.h"
#include "stdlib.h"

GUI::GUI(int w, int h, int pix_x, int pix_y)
{
    this->width = w;
    this->height = h;
    this->pixels_x = pix_x;
    this->pixels_y = pix_y;
    this->window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, w - w % pix_x, h - h % pix_y, 0);
    this->renderer = SDL_CreateRenderer(this->window, -1, SDL_RENDERER_ACCELERATED);
    this->display = (int *)calloc(pix_x * pix_y, sizeof(int));
    for (int i = 0; i < pix_x * pix_y; i++)
    {
        this->display[i] = 0;
    }
}

void GUI::loop()
{
    int i = 0;
    while (1)
    {
        this->display[i] = 1;
        render();
        if (i < (pixels_x * pixels_y)-1)
        {
            this->display[i++] = 0;
        }
        while (SDL_PollEvent(&(this->event)))
        {
            switch (this->event.type)
            {
            case SDL_QUIT:
                exit(0);
            }
        }
        SDL_Delay(1000 / 60);
    }
}

void GUI::render()
{
    this->clear_screen();
    for (int i = 0; i < this->pixels_x; i++)
    {
        for (int j = 0; j < this->pixels_y; j++)
        {
            if (this->display[i + j * pixels_x])
            {
                this->draw_point(i, j);
            }
        }
    }
    SDL_RenderPresent(this->renderer);
}

void GUI::draw_point(int x, int y)
{
    SDL_SetRenderDrawColor(this->renderer, 255, 255, 255, 255);
    int cell_width = this->width / this->pixels_x;
    int cell_height = this->height / this->pixels_y;
    SDL_Rect a = {x * cell_width, y * cell_height, cell_width, cell_height};
    SDL_RenderFillRect(this->renderer, &a);
    SDL_SetRenderDrawColor(this->renderer, 0, 0, 0, 255);
}

void GUI::clear_screen()
{
    SDL_RenderClear(this->renderer);
}