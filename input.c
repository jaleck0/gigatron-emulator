#include "input.h"
#include <SDL2/SDL.h>

static uint8_t Inputp = 0xff;
static uint8_t shiftPressed = 0;
static uint8_t ctrlPressed = 0;
static int quitRequested = 0;
InputDevice inputDev = GT_CONTROLLER; 



uint8_t GetControllerInput()
{
    SDL_Event event;
    uint8_t inputByte = Inputp;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quitRequested = 1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                inputByte &= 247;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                inputByte &= 253;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                inputByte &= 251;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                inputByte &= 254;
                break;
            case SDL_SCANCODE_Z:
            case SDL_SCANCODE_RETURN:
                inputByte &= 127;
                break;
            case SDL_SCANCODE_X:
            case SDL_SCANCODE_RSHIFT:
                inputByte &= 191;
                break;
            case SDL_SCANCODE_TAB:
                inputByte &= 223;
                break;
            case SDL_SCANCODE_ESCAPE:
                inputByte &= 239;
                break;
            case SDL_SCANCODE_F2:
                inputDev = GT_KEYBOARD;
                printf("%s\n","keyboard");
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_W:
            case SDL_SCANCODE_UP:
                inputByte |= 8;
                break;
            case SDL_SCANCODE_A:
            case SDL_SCANCODE_LEFT:
                inputByte |= 2;
                break;
            case SDL_SCANCODE_S:
            case SDL_SCANCODE_DOWN:
                inputByte |= 4;
                break;
            case SDL_SCANCODE_D:
            case SDL_SCANCODE_RIGHT:
                inputByte |= 1;
                break;
            case SDL_SCANCODE_Z:
            case SDL_SCANCODE_RETURN:
                inputByte |= 128;
                break;
            case SDL_SCANCODE_X:
            case SDL_SCANCODE_RSHIFT:
                inputByte |= 64;
                break;
            case SDL_SCANCODE_TAB:
                inputByte |= 32;
                break;
            case SDL_SCANCODE_ESCAPE:
                inputByte |= 16;
                break;
            default:
                inputByte = 255;
                break;
            }
            break;
        }
    }
    return inputByte;
}

uint8_t GetKeyboardInput()
{
    SDL_Event event;
    uint8_t inputByte = Inputp;

    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            quitRequested = 1;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_UP:
                    inputByte = 255;
                    inputByte &= 247;
                    break;
                case SDL_SCANCODE_LEFT:
                    inputByte = 255;
                    inputByte &= 253;
                    break;
                case SDL_SCANCODE_DOWN:
                    inputByte = 255;
                    inputByte &= 251;
                    break;
                case SDL_SCANCODE_RIGHT:
                    inputByte = 255;
                    inputByte &= 254;
                    break;
                case SDL_SCANCODE_RETURN:
                    inputByte = '\n';
                    break;
                case SDL_SCANCODE_BACKSPACE:
                    inputByte = 127;
                    break;
                case SDL_SCANCODE_LSHIFT:
                case SDL_SCANCODE_RSHIFT:
                    shiftPressed = 1;
                    break;
                case SDL_SCANCODE_LCTRL:
                case SDL_SCANCODE_RCTRL:
                    ctrlPressed = 1;
                    break;
                case SDL_SCANCODE_TAB:
                    inputByte = 255;
                    inputByte &= 223;
                    break;
                case SDL_SCANCODE_ESCAPE:
                    inputByte &= 239;
                    break;
                case SDL_SCANCODE_F1:
                    inputDev = GT_CONTROLLER;
                    printf("%s\n","controller");
                    break;
                default:
                    inputByte = SDL_GetKeyFromScancode(event.key.keysym.scancode);
                
                    if (shiftPressed)
                    {
                        switch(inputByte)
                        {    
                            case '1':
                                inputByte = '!';
                            break;
                            case '2':
                                inputByte = '@';
                            break;
                            case '3':
                                inputByte = '#';
                            break;
                            case '4':
                                inputByte = '$';
                            break;
                            case '5':
                                inputByte = '%';
                            break;
                            case '6':
                                inputByte = '^';
                            break;
                            case '7':
                                inputByte = '&';
                            break;
                            case '8':
                                inputByte = '*';
                            break;
                            case '9':
                                inputByte = '(';
                            break;
                            case '0':
                                inputByte = ')';
                            break;
                            case '-':
                                inputByte = '_';
                            break;
                            case '=':
                                inputByte = '+';
                            break;
                            case ';':
                                inputByte = ':';
                            break;
                            case '[':
                                inputByte = '{';
                            break;
                            case ']':
                                inputByte = '}';
                            break;
                            case '.':
                                inputByte = '>';
                            break;
                            case ',':
                                inputByte = '<';
                            break;
                            case 92:
                                inputByte = '|';
                            break;
                            case 39:
                                inputByte = '"';
                            break;
                            case '/':
                                inputByte = '?';
                            break;
                            case 96:
                                inputByte = '~';
                            break;
                            default:
                                inputByte &= ~(0x20*shiftPressed);
                            break;
                        }
                    }
                    if (ctrlPressed)
                    {
                        inputByte &= ~(0x60*ctrlPressed);
                    }
                break;
            }
            break;
        case SDL_KEYUP:
            switch (event.key.keysym.scancode)
            {
                case SDL_SCANCODE_LSHIFT:
                case SDL_SCANCODE_RSHIFT:
                    shiftPressed = 0;
                    break;
                case SDL_SCANCODE_LCTRL:
                case SDL_SCANCODE_RCTRL:
                    ctrlPressed = 0;
                    break;
                default:
                    inputByte = 0;  
                    break;
            }
            break;
        }
    }
    return inputByte;
}

uint8_t GetInput(uint8_t input)
{
    Inputp = input;
    switch(inputDev)
    {
        case GT_CONTROLLER:
            return GetControllerInput();
        break;
        case GT_KEYBOARD:
            return GetKeyboardInput();
        break; 
    }
    return 0;
}

int GetQuitRequest()
{
    return quitRequested;
}