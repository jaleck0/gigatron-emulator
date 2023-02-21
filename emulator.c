#include "input.h"
#include "sdldraw.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_image.h>
//#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH (960)
#define WINDOW_HEIGHT (540)

typedef struct 
{
    uint16_t PC;
    uint8_t IR, D, AC, X, Y, OUTPUT, undef;
} CpuState;

uint8_t ROM[1<<16][2];  //1 megabit = 128 kilobyte rom data
uint8_t RAM[1<<15];     //32 kilobyte of ram data
uint8_t Input = 0xff;   //input from port, normally high

int quitRequest = 0;
    
CpuState CpuCycle(const CpuState newState)
{
    CpuState oldState = newState;

    oldState.IR = ROM[newState.PC][0];
    oldState.D  = ROM[newState.PC][1];

    int ins = newState.IR >> 5;
    int mod = (newState.IR >> 2) & 7;
    int bus = newState.IR & 3;
    int W = (ins == 6);
    int J = (ins == 7);

    uint8_t lo = newState.D;
    uint8_t hi = 0;
    uint8_t* to = NULL;
    int incX = 0;

    if(!J)
        switch (mod)
        {
            #define E(p) (W?0:p)
            case 0: to = E(&oldState.AC); break;
            case 1: to = E(&oldState.AC); lo = newState.X; break;
            case 2: to = E(&oldState.AC); hi = newState.Y; break;
            case 3: to = E(&oldState.AC); lo = newState.X; hi = newState.Y; break;
            case 4: to = &oldState.X; break;
            case 5: to = &oldState.Y; break;
            case 6: to = E(&oldState.OUTPUT); break;
            case 7: to = E(&oldState.OUTPUT); lo = newState.X; hi = newState.Y; incX = 1; break;
        }
    uint16_t addres = (hi << 8) | lo;

    int B = newState.undef; //databus
    switch (bus)
    {
        case 0: B = newState.D; break;
        case 1: if (!W) B = RAM[addres & 0x7fff]; break;
        case 2: B = newState.AC; break;
        case 3: B = Input; break;
    }

    if (W)
    {
        RAM[addres & 0x7fff] = B;
    }

    uint8_t ALU; //arithic logic unit

    switch (ins)
    {
        case 0: ALU = B; break;
        case 1: ALU = newState.AC & B; break;
        case 2: ALU = newState.AC | B; break;
        case 3: ALU = newState.AC ^ B; break;
        case 4: ALU = newState.AC + B; break;
        case 5: ALU = newState.AC - B; break;
        case 6: ALU = newState.AC; break;
        case 7: ALU = -newState.AC; break;
    }

    if (to) *to = ALU;
    if (incX) oldState.X = newState.X + 1;

    oldState.PC = newState.PC + 1;

    if (J)
    {
        if (mod != 0)
        {
            int cond = (newState.AC >> 7) + 2*(newState.AC == 0);
            if (mod & (1 << cond))
                oldState.PC = (newState.PC & 0xff00) | B;
        } else 
            oldState.PC = (newState.Y << 8) | B;
    }
    return oldState;
}

void garble(uint8_t mem[], unsigned int length)
{
    for (unsigned int i = 0; i < length; i++)
    {
        mem[i] = rand();
    }
}



int main(int argc, char* argv[])
{
    CpuState currentState;
    srand(time(NULL));
    garble((void*)ROM, sizeof(ROM));
    garble((void*)RAM, sizeof(RAM));
    garble((void*)&currentState, sizeof(currentState));
    
    FILE* fp = fopen(argv[1], "rb");

    if (!fp)
    {
        fprintf(stderr, "Failed to open ROM-file\n");
        exit(EXIT_FAILURE);
    }

    fread(ROM, 1, sizeof(ROM), fp);
    fclose(fp);

    int vgaX = 0;
    int vgaY = 0;
    
    // attempt to initialize graphics and timer system
    if (SDL_Init(SDL_INIT_VIDEO|SDL_INIT_TIMER) != 0)
    {
        printf("error initializing SDL: %s\n", SDL_GetError());
        return 1;
    }

    SDL_Window* win = SDL_CreateWindow("Jalecko's Gigatron Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_WIDTH, WINDOW_HEIGHT,0);

    if (!win)
    {
        printf("error creating window: %s\n", SDL_GetError());
        SDL_Quit();
	    return 1;
    }

    // create a renderer, which sets up the graphics hardware
    Uint32 render_flags = SDL_RENDERER_ACCELERATED;
    SDL_Renderer* rend = SDL_CreateRenderer(win, -1, render_flags);
    if (!rend)
    {
        printf("error creating renderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(win);
        SDL_Quit();
        return 1;
    }

    SDL_RenderPresent(rend);

    long long t = -2;
    
    while (!quitRequest)
    {
        if (t < 0) currentState.PC = 0;

        CpuState newState = CpuCycle(currentState);

        int hSync = (newState.OUTPUT & 0x40) - (currentState.OUTPUT & 0x40);
        int vSync = (newState.OUTPUT & 0x80) - (currentState.OUTPUT & 0x80);

        if (vSync < 0 )
        {
            vgaY = -36;

            quitRequest = GetQuitRequest();
            Input = GetInput(Input);
            DrawGigatronExtendedIO(rend, Input);

            SDL_RenderPresent(rend);
        }

        if (vgaX++ < 172 && vgaX > 12)
        {
            uint8_t pixel = (currentState.OUTPUT & 63);
            SDL_SetRenderDrawColor(rend, (pixel & 0x03) * 0x55, ((pixel >> 2) & 0x03) * 0x55, ((pixel >> 4) & 0x03) * 0x55,255);
            SDL_RenderDrawLine(rend, 160 + vgaX*4 - 52, vgaY + 30, vgaX*4 + 164 - 52, vgaY + 30);
        } 

        if (hSync > 0)
        {
            vgaX = 0;
            vgaY++;
            newState.undef = rand() & 0xff;
        }
        currentState = newState;
        t++;
    }

    SDL_DestroyRenderer(rend);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
