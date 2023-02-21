#include "sdldraw.h"

void DrawRectFill(SDL_Renderer* rend, int x, int y, int w, int h)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
            
    SDL_RenderFillRect(rend, &rect);
}

void DrawByte(SDL_Renderer* rend, int x, int y, uint8_t byte)
{
    SDL_SetRenderDrawColor(rend, 127,0,0,255);
    DrawRectFill(rend, x, y, 64, 8);
    SDL_SetRenderDrawColor(rend, 255,0,0,255);
            
    uint8_t ledOut = byte;

    for(int r = 0; r < 8; r++)
    {
        if ((ledOut & 0x80) == 0x80)
        {
            DrawRectFill(rend, x+r*8, y, 8, 8);
        }
        ledOut <<= 1;
    }
    
}

void DrawGigatronExtendedIO(SDL_Renderer* rend, uint8_t input)
{
    //DrawByte(rend, 8, 64, currentState.X);
    //DrawByte(rend, 8, 72, currentState.Y);
    //DrawByte(rend, 8, 112, currentState.undef);
    DrawByte(rend, 8, 120, input);
}