#ifndef SDLDRAW_H
#define SDLDRAW_H

    #include <SDL2/SDL.h>

    void DrawRectFill(SDL_Renderer* rend, int x, int y, int w, int h);
    void DrawByte(SDL_Renderer* rend, int x, int y, uint8_t byte);
    void DrawGigatronExtendedIO(SDL_Renderer* rend, uint8_t input);

#endif